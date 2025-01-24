#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "malloc.h"
#include "ikigai_characters.h"
#include "data/ikigai_characters.h"
#include "event_data.h"
#include "sprite.h"
#include "decompress.h"
#include "field_weather.h"
#include "random.h"
#include "script_menu.h"
#include "string_util.h"
#include "event_object_movement.h"
#include "script_movement.h"
#include "script.h"
#include "global.fieldmap.h"
#include "fpmath.h"
#include "battle_util.h"
#include "field_message_box.h"

static s32 ClampedOpinionDelta(s32 opinionCurrent, s32 opinionDelta);
static uq4_12_t GetGymTypeEffectiveness(u16 species, bool32 speciesAtk);

static const struct OamData sCharacteristicDialogue_Oam = {
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 0,
};

static const struct SpriteTemplate sDialogueIconSprite_SpriteTemplate = {
    .tileTag = TAG_CHARACTER_DIALOGUE_ICON,
    .paletteTag = TAG_CHARACTER_DIALOGUE_ICON,
    .oam = &sCharacteristicDialogue_Oam,
    .callback = SpriteCallbackDummy,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable,
};

static s32 ClampedOpinionDelta(s32 opinionCurrent, s32 opinionDelta)
{
    if (opinionCurrent > 127 - opinionDelta)
        return 127 - opinionCurrent;
    else if (opinionCurrent < -128 - opinionDelta)
        return -128 - opinionCurrent;
    else
        return opinionDelta;
}

u32 IkigaiCharacter_GetPlayerAttitude(void)
{
    s32 opinionKindness = IkigaiCharacter_GetAverageKindness();
    s32 opinionStrength = IkigaiCharacter_GetAverageStrength();

    if (opinionKindness > OPINION_NEUTRAL_BUFFER)
    {
        if (opinionStrength > OPINION_NEUTRAL_BUFFER)
            return ATTITUDE_INSPIRED;

        if (opinionStrength < - OPINION_NEUTRAL_BUFFER)
            return ATTITUDE_HUMBLE;
    }

    if (opinionKindness < - OPINION_NEUTRAL_BUFFER)
    {
        if (opinionStrength > OPINION_NEUTRAL_BUFFER)
            return ATTITUDE_DOMINANT;

        if (opinionStrength < - OPINION_NEUTRAL_BUFFER)
            return ATTITUDE_CYNICAL;
    }

    return ATTITUDE_NEUTRAL;
}

void IkigaiCharacter_SetDefaultOpinion(u32 character)
{
    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return;
    
    gSaveBlock3Ptr->characters.opinionKindness[character] = gIkigaiCharactersInfo[character].baseOpinionKindness;
    gSaveBlock3Ptr->characters.opinionStrength[character] = gIkigaiCharactersInfo[character].baseOpinionStrength;
}

void IkigaiCharacter_SetAllCharacterDefaultOpinion(void)
{
    u32 character;

    for (character = CHARACTER_DEFAULT + 1; character < MAIN_CHARACTER_COUNT; character++)
    {
        IkigaiCharacter_SetDefaultOpinion(character);
    }
}

s32 IkigaiCharacter_GetKindness(u32 character)
{
    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return 0;
     
    s32 kindnessCharacter = gSaveBlock3Ptr->characters.opinionKindness[character];
    s32 kindnessAdded = IkigaiCharacter_GetOpinionBonus(character, OPINION_TYPE_KINDNESS);

    return kindnessCharacter + ClampedOpinionDelta(kindnessCharacter, kindnessAdded);
}

s32 IkigaiCharacter_GetStrength(u32 character)
{
    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return 0;
     
    s32 strengthCharacter = gSaveBlock3Ptr->characters.opinionStrength[character];
    s32 strengthAdded = IkigaiCharacter_GetOpinionBonus(character, OPINION_TYPE_STRENGTH);

    return strengthCharacter + ClampedOpinionDelta(strengthCharacter, strengthAdded);
}

s32 IkigaiCharacter_GetKindness_Special(u32 character)
{
    if (character <= MAIN_CHARACTER_COUNT && character == CHARACTER_COUNT_TOTAL)
    {
        return 0;
    }

    s32 opinionKindness = IkigaiCharacter_GetAverageKindness();
    s32 kindnessAdded = IkigaiCharacter_GetOpinionBonus(character, OPINION_TYPE_KINDNESS);

    return opinionKindness + ClampedOpinionDelta(opinionKindness, kindnessAdded);
}

s32 IkigaiCharacter_GetStrength_Special(u32 character)
{
    if (character <= MAIN_CHARACTER_COUNT && character == CHARACTER_COUNT_TOTAL)
    {
        return 0;
    }

    s32 opinionStrength = IkigaiCharacter_GetAverageStrength();
    s32 strengthAdded = IkigaiCharacter_GetOpinionBonus(character, OPINION_TYPE_STRENGTH);

    return opinionStrength + ClampedOpinionDelta(opinionStrength, strengthAdded);
}

s32 IkigaiCharacter_GetOpinionBonus(u32 character, u32 opinionType)
{
    s32 opinionBonus = 0;

    opinionBonus += IkigaiCharacterOpinionBonus_Relationship(character, opinionType);
    opinionBonus += IkigaiCharacterOpinionBonus_PartnerPokemon(character, opinionType);
    opinionBonus += IkigaiCharacterOpinionBonus_StarterPokemon(character, opinionType);

    return opinionBonus;
}

s32 IkigaiCharacter_GetAverageKindness(void)
{
    s32 opinionKindness = 0;
    u32 character;

    for (character = CHARACTER_DEFAULT + 1; character < MAIN_CHARACTER_COUNT; character++)
    {
        opinionKindness += IkigaiCharacter_GetKindness(character);
    }

    return (opinionKindness / (MAIN_CHARACTER_COUNT - 1));
}

s32 IkigaiCharacter_GetAverageStrength(void)
{
    s32 opinionStrength = 0;
    u32 character;

    for (character = CHARACTER_DEFAULT + 1; character < MAIN_CHARACTER_COUNT; character++)
    {
        opinionStrength += IkigaiCharacter_GetStrength(character);
    }

    return (opinionStrength / (MAIN_CHARACTER_COUNT - 1));
}

s32 IkigaiCharacter_GetSetConversedFlag(u32 character, bool32 setFlag)
{
    u32 index, bit, mask;
    s32 retVal = 0;

    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return retVal;

    index = character / 8;
    bit = character % 8;
    mask = 1 << bit;

    if (setFlag)
        gSaveBlock3Ptr->characters.conversed[index] |= mask;
    else
        retVal = ((gSaveBlock3Ptr->characters.conversed[index] & mask) != 0);

    return retVal;
}

void IkigaiCharacter_ClearConversedFlags(void)
{
    memset(gSaveBlock3Ptr->characters.conversed, 0, sizeof(gSaveBlock3Ptr->characters.conversed));
}

void IkigaiCharacter_SetRomanticFlag(u32 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;
    
    FlagSet(gIkigaiCharactersInfo[character].flagRomantic);
}

void IkigaiCharacter_ToggleRomanticFlag(u32 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    FlagToggle(gIkigaiCharactersInfo[character].flagRomantic);
}

void IkigaiCharacter_ClearRomanticFlag(u32 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    FlagClear(gIkigaiCharactersInfo[character].flagRomantic);
}

bool32 IkigaiCharacter_GetRomanticFlag(u32 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return FALSE;

    return FlagGet(gIkigaiCharactersInfo[character].flagRomantic);
}

void IkigaiCharacter_SetRomanticFlag_Exclusive(u32 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    u32 i;

    for (i = CHARACTER_DEFAULT + 1; i < CHARACTER_COUNT_TOTAL; i++)
    {
        if (i == character)
            IkigaiCharacter_SetRomanticFlag(i);
        else
            IkigaiCharacter_ClearRomanticFlag(i);
    }
}

void IkigaiCharacter_ClearRomanticFlag_Amicable(u32 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    FlagClear(gIkigaiCharactersInfo[character].flagRomantic);
}

void IkigaiCharacter_ClearRomanticFlag_Hostile(u32 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    FlagClear(gIkigaiCharactersInfo[character].flagRomantic);
    if (character > CHARACTER_DEFAULT && character < MAIN_CHARACTER_COUNT)
        gSaveBlock3Ptr->characters.opinionKindness[character] = - OPINION_NEUTRAL_BUFFER;
}

u32 IkigaiCharacter_CheckRelationships(void)
{
    if (IkigaiCharacter_IsPlayerSingleOrMonogamous())
        return ReturnIkigaiCharacter_RomanceFlag_Exclusive();
        
    else
        return CHARACTER_COUNT_TOTAL;
}

bool32 IkigaiCharacter_IsPlayerSingleOrMonogamous(void)
{
    u32 character;
    u32 relationships = RELATIONSHIP_SINGLE;

    for (character = CHARACTER_DEFAULT + 1; character < CHARACTER_COUNT_TOTAL; character++)
    {
        if (gIkigaiCharactersInfo[character].flagRomantic != 0 
            && FlagGet(gIkigaiCharactersInfo[character].flagRomantic))
        {
            relationships++;
            if (relationships > RELATIONSHIP_MONOGOMOUS)
                return FALSE;
        }
    }

    return TRUE;
}

void ScrCmd_IkigaiCharacter_SetRomanticFlag(void)
{
    IkigaiCharacter_SetRomanticFlag(
        ReturnIkigaiCharacter_SelectedObject()
    );

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
}

void ScrCmd_IkigaiCharacter_ToggleRomanticFlag(void)
{
    IkigaiCharacter_ToggleRomanticFlag(
        ReturnIkigaiCharacter_SelectedObject()
    );

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
}

void ScrCmd_IkigaiCharacter_ClearRomanticFlag(void)
{
    IkigaiCharacter_ClearRomanticFlag(
        ReturnIkigaiCharacter_SelectedObject()
    );

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
}

void ScrCmd_IkigaiCharacter_GetRomanticFlag(void)
{
    gSpecialVar_Result = IkigaiCharacter_GetRomanticFlag(
        ReturnIkigaiCharacter_SelectedObject()
    );

    Script_RequestEffects(SCREFF_V1);
}

void ScrCmd_IkigaiCharacter_SetRomanticFlag_Exclusive(void)
{
    IkigaiCharacter_SetRomanticFlag_Exclusive(
        ReturnIkigaiCharacter_SelectedObject()
    );

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
}

void ScrCmd_IkigaiCharacter_ClearRomanticFlag_Amicable(void)
{
    IkigaiCharacter_ClearRomanticFlag_Amicable(
        ReturnIkigaiCharacter_SelectedObject()
    );

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
}

void ScrCmd_IkigaiCharacter_ClearRomanticFlag_Hostile(void)
{
    IkigaiCharacter_ClearRomanticFlag_Hostile(
        ReturnIkigaiCharacter_SelectedObject()
    );

    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);
}

void ScrCmd_IkigaiCharacter_CheckRelationships(void)
{
    gSpecialVar_Result = IkigaiCharacter_CheckRelationships();

    Script_RequestEffects(SCREFF_V1);
}

void ScrCmd_IkigaiCharacter_BurfferName(void)
{
    StringCopy(gStringVar1, gIkigaiCharactersInfo[gSpecialVar_Result].name);

    Script_RequestEffects(SCREFF_V1);
}

bool32 IkigaiCharacter_ReturnOpinionDecay(u32 character)
{
    if (character > CHARACTER_COUNT_TOTAL)
        return FALSE; 

    if ((Random() % 100) < gIkigaiCharactersInfo[character].opinionDecayChance)
        return TRUE;
    else
        return FALSE;
}

void IkigaiCharacter_OpinionDecay(u32 character)
{
    s32 opinionKindness = gSaveBlock3Ptr->characters.opinionKindness[character];
    s32 opinionStrength = gSaveBlock3Ptr->characters.opinionStrength[character];

    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return;
    
    if (IkigaiCharacter_ReturnOpinionDecay(character))
    {
        if (opinionKindness > OPINION_NEUTRAL_BUFFER)
            gSaveBlock3Ptr->characters.opinionKindness[character] -= ClampedOpinionDelta(opinionKindness, -1);
        else if (opinionKindness < OPINION_NEUTRAL_BUFFER)
            gSaveBlock3Ptr->characters.opinionKindness[character] += ClampedOpinionDelta(opinionKindness, 1);
    }

    if (IkigaiCharacter_ReturnOpinionDecay(character))
    {
        if (opinionStrength > OPINION_NEUTRAL_BUFFER)
            gSaveBlock3Ptr->characters.opinionStrength[character] -= ClampedOpinionDelta(opinionStrength, -1);
        else if (opinionStrength < OPINION_NEUTRAL_BUFFER)
            gSaveBlock3Ptr->characters.opinionStrength[character] += ClampedOpinionDelta(opinionStrength, 1);
    }
}

void IkigaiCharacter_AllOpinionDecay_NonConverse(void)
{
    u32 character;

    for (character = CHARACTER_DEFAULT + 1; character < MAIN_CHARACTER_COUNT; character++)
    {
        if (!IkigaiCharacter_GetSetConversedFlag(character, FALSE))
            IkigaiCharacter_OpinionDecay(character);
    }
}

void IkigaiCharacter_HandleDialogue_Attitudes(void)
{
    u32 character = ReturnIkigaiCharacter_SelectedObject();

    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return;

    if (gSpecialVar_Result == MULTI_B_PRESSED && !IkigaiCharacter_GetSetConversedFlag(character, FALSE))
    {
        IkigaiCharacter_OpinionDecay(character);
        return;
    }

    if (gSpecialVar_Result >= NELEMS(gDialogueAttitudes))
        gSpecialVar_Result = ATTITUDE_NEUTRAL;

    if (!IkigaiCharacter_GetSetConversedFlag(character, FALSE))
    {
        s32 opinionKindness = gDialogueAttitudes[gSpecialVar_Result].kindnessEffect;
        s32 opinionStrength = gDialogueAttitudes[gSpecialVar_Result].strengthEffect;

        if (gSpecialVar_Result == gIkigaiCharactersInfo[character].personality)
        {
            u32 opinionMultiplier = Random() % 2;

            if (opinionKindness < 0)
                opinionKindness *= (opinionMultiplier - 1);
            else
                opinionKindness *= (opinionMultiplier + 1);

            if (opinionStrength < 0)
                opinionStrength *= (opinionMultiplier - 1);
            else
                opinionStrength *= (opinionMultiplier + 1);
        }
 
        gSaveBlock3Ptr->characters.opinionKindness[character] += ClampedOpinionDelta(gSaveBlock3Ptr->characters.opinionKindness[character], opinionKindness);
        gSaveBlock3Ptr->characters.opinionStrength[character] += ClampedOpinionDelta(gSaveBlock3Ptr->characters.opinionStrength[character], opinionStrength);
        IkigaiCharacter_GetSetConversedFlag(character, TRUE);
    }
}

void DEBUG_IkigaiCharacter_CharacterOpinions(void)
{
    u32 character = ReturnIkigaiCharacter_SelectedObject();
    s32 opinionKindness = IkigaiCharacter_GetKindness(character);
    s32 opinionStrength = IkigaiCharacter_GetStrength(character);
    u8 string[3];

    if (character > MAIN_CHARACTER_COUNT)
    {
        s8 addedKindess, addedStrength;

        opinionKindness = IkigaiCharacter_GetAverageKindness();
        addedKindess = IkigaiCharacter_GetOpinionBonus(character, OPINION_TYPE_KINDNESS);
        opinionKindness += ClampedOpinionDelta(opinionKindness, addedKindess);

        opinionStrength = IkigaiCharacter_GetAverageStrength();
        addedStrength = IkigaiCharacter_GetOpinionBonus(character, OPINION_TYPE_STRENGTH);
        opinionStrength += ClampedOpinionDelta(opinionStrength, addedStrength);
    }

    StringCopy(gStringVar1, gIkigaiCharactersInfo[character].name);
    StringAppend(gStringVar1, COMPOUND_STRING("'s Kindness Opinion: "));
    if (opinionKindness < 0)
    {
        opinionKindness *= -1;
        StringAppend(gStringVar1, COMPOUND_STRING("-"));
    }
    ConvertIntToDecimalStringN(string, opinionKindness, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(gStringVar1, string);
    StringAppend(gStringVar1, COMPOUND_STRING("\n"));
    StringAppend(gStringVar1, gIkigaiCharactersInfo[character].name);
    StringAppend(gStringVar1, COMPOUND_STRING("'s Strength Opinion: "));
    if (opinionStrength < 0)
    {
        opinionStrength *= -1;
        StringAppend(gStringVar1, COMPOUND_STRING("-"));
    }
    ConvertIntToDecimalStringN(string, opinionStrength, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(gStringVar1, string);
    StringCopy(gStringVar2, COMPOUND_STRING("\pIncludes Kindness Bonus of: "));
    ConvertIntToDecimalStringN(string, IkigaiCharacter_GetOpinionBonus(character, OPINION_TYPE_KINDNESS), STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(gStringVar2, string);
    StringAppend(gStringVar2, COMPOUND_STRING(",\nIncludes Strength Bonus of: "));
    ConvertIntToDecimalStringN(string, IkigaiCharacter_GetOpinionBonus(character, OPINION_TYPE_STRENGTH), STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(gStringVar2, string);
    StringAppend(gStringVar2, COMPOUND_STRING("."));
    if (character > MAIN_CHARACTER_COUNT)
    {
        StringAppend(gStringVar2, COMPOUND_STRING("\pI am a special character, I get my\nopinion from the PokéSphere."));
    }
}

void SetDefaultPlayerNickname(void)
{
    StringCopy(gSaveBlock3Ptr->characters.playerNickname, gSaveBlock2Ptr->playerName);
}

bool32 IkigaiCharacter_NicknameInsteadOfName(u32 character)
{
    if (StringCompare(gSaveBlock3Ptr->characters.playerNickname, gSaveBlock2Ptr->playerName)
        && (IkigaiCharacter_GetRomanticFlag(character) || IkigaiCharacter_GetKindness(character) > OPINION_NEUTRAL_BUFFER * 2))
    {
        return TRUE;
    }

    return FALSE;
}

void IkigaiCharacter_MovementEmote(u8 localId, const u8 *movement)
{
    gSpecialVar_0x8000 = FALSE;
    if (movement == NULL)
        return;

    gObjectEvents[GetObjectEventIdByLocalId(localId)].directionOverwrite = DIR_NONE;
    ScriptMovement_StartObjectMovementScript(
        localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, movement
    );
    SetMovingNpcId(gObjectEvents[gSelectedObjectEvent].localId);
    gSpecialVar_0x8000 = TRUE;
}

u32 IkigaiCharacter_GetPoseFromAttitude(u32 attitude)
{
    u32 pose;

    if (attitude == ATTITUDE_NEUTRAL)
        pose = Random() % 4;
    else
        pose = attitude - 1;

    return pose;
}

const u8 *IkigaiCharacter_GetMovementFromAttitude(u32 character, u32 attitude)
{
    if (attitude >= ATTITUDE_COUNT)
        return NULL;

    return gIkigaiCharactersInfo[character].poses[IkigaiCharacter_GetPoseFromAttitude(attitude)].movement;
}

void IkigaiCharacter_DefaultEmote(void)
{
    u32 character = ReturnIkigaiCharacter_ObjectEventGraphicsId(gObjectEvents[gSelectedObjectEvent].graphicsId);
    
    IkigaiCharacter_MovementEmote(
        gObjectEvents[gSelectedObjectEvent].localId,
        IkigaiCharacter_GetMovementFromAttitude(character, gIkigaiCharactersInfo[character].personality)
    );

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
}

void IkigaiCharacter_ResponseEmote(void)
{
    u32 character = ReturnIkigaiCharacter_ObjectEventGraphicsId(gObjectEvents[gSelectedObjectEvent].graphicsId);
    
    IkigaiCharacter_MovementEmote(
        gObjectEvents[gSelectedObjectEvent].localId,
        IkigaiCharacter_GetMovementFromAttitude(character, gSpecialVar_Result)
    );

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
}

u32 ReturnIkigaiCharacter_ObjectEventGraphicsId(u16 graphicsId)
{
    u32 character;

    for (character = CHARACTER_DEFAULT + 1; character < CHARACTER_COUNT_TOTAL; character++)
    {
        if (graphicsId == gIkigaiCharactersInfo[character].overworldGraphicsId)
            return character;
    }

    return CHARACTER_DEFAULT;
}

u32 ReturnIkigaiCharacter_SelectedObject(void)
{
    return ReturnIkigaiCharacter_ObjectEventGraphicsId(gObjectEvents[gSelectedObjectEvent].graphicsId);
}

void ScrCmd_ReturnIkigaiCharacter_SelectedObject(void)
{
    gSpecialVar_Result = ReturnIkigaiCharacter_SelectedObject();

    Script_RequestEffects(SCREFF_V1);
}

u32 ReturnIkigaiCharacter_RomanceFlag_Exclusive(void)
{
    u32 character;

    for (character = CHARACTER_DEFAULT + 1; character < CHARACTER_COUNT_TOTAL; character++)
    {
        if (gIkigaiCharactersInfo[character].flagRomantic != 0 
            && FlagGet(gIkigaiCharactersInfo[character].flagRomantic))
        {
            return character;
        }
    }

    return CHARACTER_DEFAULT;
}

s32 IkigaiCharacterOpinionBonus_Relationship(u32 character, bool32 opinionType)
{
    u32 relationshipCharacter, relationshipAffinity, relationshipBonus, i;

    relationshipBonus = 0;

    for (i = 0; i < MAX_RELATIONSHIPS; i++)
    {
        relationshipCharacter = gIkigaiCharactersInfo[character].relationships[i].characterId;
        relationshipAffinity = gIkigaiCharactersInfo[character].relationships[i].affinity;

        if (relationshipCharacter == CHARACTER_DEFAULT || relationshipCharacter >= MAIN_CHARACTER_COUNT)
        {}

        else if (opinionType == OPINION_TYPE_KINDNESS &&
            gSaveBlock3Ptr->characters.opinionKindness[relationshipCharacter]>=
            relationshipAffinity * RELATIONSHIP_AFFINITY_MULTIPLIER)
        {
            relationshipBonus += relationshipAffinity;
        }

        else if (opinionType == OPINION_TYPE_STRENGTH &&
            gSaveBlock3Ptr->characters.opinionStrength[relationshipCharacter] >=
            relationshipAffinity * RELATIONSHIP_AFFINITY_MULTIPLIER)
        {
            relationshipBonus += relationshipAffinity;
        }
    }

    return relationshipBonus;
}

s32 IkigaiCharacterOpinionBonus_PartnerPokemon(u32 character, bool32 opinionType)
{
    u32 species = gIkigaiCharactersInfo[character].partnerPokemon;
    u32 bonusPartner = 0;

    if (gSaveBlock2Ptr->ikigaiGymType == TYPE_NONE)
    {
        return bonusPartner;
    }

    if (opinionType == OPINION_TYPE_KINDNESS)
    {
        if (gSpeciesInfo[species].types[0] == gSaveBlock2Ptr->ikigaiGymType)
            bonusPartner += OPINION_VERY_POSITIVE;
        else if (gSpeciesInfo[species].types[1] == gSaveBlock2Ptr->ikigaiGymType)
            bonusPartner += OPINION_POSITIVE;
    }
    else if (opinionType == OPINION_TYPE_STRENGTH)
    {
        uq4_12_t typeEffectiveness = GetGymTypeEffectiveness(species, TRUE);

        if (typeEffectiveness == 4.0)
            bonusPartner += OPINION_VERY_POSITIVE;
        else if (typeEffectiveness == 2.0)
            bonusPartner += OPINION_POSITIVE;
    }

    return bonusPartner;
}

s32 IkigaiCharacterOpinionBonus_StarterPokemon(u32 character, bool32 opinionType)
{
    u32 starter = VarGet(VAR_STARTER_MON);
    u32 species = gIkigaiCharactersInfo[character].partnerPokemon;
    u32 bonusStarter = 0;

    if (gSaveBlock2Ptr->ikigaiGymType == TYPE_NONE)
    {
        return bonusStarter;
    }

    if (opinionType == OPINION_TYPE_KINDNESS)
    {
        if (gSpeciesInfo[starter].bodyColor == gIkigaiCharactersInfo[character].favouriteColour)
            bonusStarter += OPINION_POSITIVE;
            
        if (gSpeciesInfo[starter].types[0] == gIkigaiCharactersInfo[character].favouriteType)
            bonusStarter += OPINION_POSITIVE;
        else if (gSpeciesInfo[starter].types[1] == gIkigaiCharactersInfo[character].favouriteType)
            bonusStarter += OPINION_POSITIVE;
    }
    else if (opinionType == OPINION_TYPE_STRENGTH)
    {
        uq4_12_t typeEffectiveness = GetGymTypeEffectiveness(species, FALSE);

        if (typeEffectiveness == 0.25)
            bonusStarter += OPINION_VERY_POSITIVE;
        else if (typeEffectiveness == 0.50)
            bonusStarter += OPINION_POSITIVE;
    }

    return bonusStarter;
}

// static s32 IkigaiCharacterOpinionBonus_Player(u32 character, bool32 opinionType)
// {
//     u32 bonusPlayerAttitude = 0;

//     if (opinionType == OPINION_TYPE_KINDNESS)
//     {
//         Attitude Matching Personality
//     }
//     else if (opinionType == OPINION_TYPE_STRENGTH)
//     {
//         Gym Wins
//     }

//     return bonusPlayerAttitude;
// }

// static s32 IkigaiCharacterOpinionBonus_Player(u32 character, bool32 opinionType)
// {
//     if (IkigaiCharacter_GetRomanticFlag)
// }

static uq4_12_t GetGymTypeEffectiveness(u16 species, bool32 speciesAtk)
{
    uq4_12_t modifier[3] = { UQ_4_12(1.0), UQ_4_12(1.0), UQ_4_12(1.0) };
    u8 typeIkigaiGym = gSaveBlock2Ptr->ikigaiGymType;
    u8 typeSpeciesPrimary = gSpeciesInfo[species].types[0];
    u8 typeSpeciesSecondary = gSpeciesInfo[species].types[1];

    if (typeIkigaiGym == TYPE_NONE
    ||  typeIkigaiGym == TYPE_MYSTERY
    ||  typeIkigaiGym == TYPE_STELLAR
    ||  typeIkigaiGym >= NUMBER_OF_MON_TYPES)
        return modifier[0];

    if (typeSpeciesPrimary == TYPE_NONE
    ||  typeSpeciesPrimary == TYPE_MYSTERY
    ||  typeSpeciesPrimary == TYPE_STELLAR
    ||  typeSpeciesPrimary >= NUMBER_OF_MON_TYPES)
        return modifier[0];

    if (typeSpeciesSecondary == TYPE_NONE
    ||  typeSpeciesSecondary == TYPE_MYSTERY
    ||  typeSpeciesSecondary == TYPE_STELLAR
    ||  typeSpeciesSecondary >= NUMBER_OF_MON_TYPES)
        return modifier[0];

    if (speciesAtk == TRUE)
    {
        modifier[1] = GetTypeModifier(typeIkigaiGym, typeSpeciesPrimary);
        modifier[2] = (typeSpeciesSecondary != typeSpeciesPrimary) ? GetTypeModifier(typeIkigaiGym, typeSpeciesSecondary) : UQ_4_12(1.0);
    }
    else
    {
        modifier[1] = GetTypeModifier(typeSpeciesPrimary, typeIkigaiGym);
        modifier[2] = (typeSpeciesSecondary != typeSpeciesPrimary) ? GetTypeModifier(typeSpeciesSecondary, typeIkigaiGym) : UQ_4_12(1.0);
    }

    modifier[0] = uq4_12_multiply(modifier[1], modifier[2]);
    return modifier[0];
}
#define CHARACTER_NAME_TEXT_COLOUR FALSE
u8 IkigaiCharacter_ReturnMessageBoxPersonalityPalette(u32 character)
{
    u32 textColour = 2;

    switch (gIkigaiCharactersInfo[character].personality)
    {
    case ATTITUDE_CYNICAL:
        textColour = 9;
        break;
        
    case ATTITUDE_DOMINANT:
        textColour = 4;
        break;
        
    case ATTITUDE_HUMBLE:
        textColour = 6;
        break;
        
    case ATTITUDE_INSPIRED:
        textColour = 8;
        break;
    }

    if (gSpeakerName == gIkigaiCharactersInfo[CHARACTER_DEFAULT].name)
        textColour = 2;

    if (CHARACTER_NAME_TEXT_COLOUR)
        return textColour;
    else
        return 2;
}

u8 CreateDialogueOptionIconSprite(u32 dialogueIndex)
{
    u8 spriteId;

    struct CompressedSpriteSheet sheet = { .size = 0x1000, .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpritePalette pal = { .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpriteTemplate *spriteTemplate;

    if (dialogueIndex >= DIALOGUE_OPTION_COUNT)
        dialogueIndex = DIALOGUE_OPTION_TALK;

    sheet.data = gDialogueOptions[dialogueIndex].iconImage;
    pal.data = gDialogueOptions[dialogueIndex].iconPal;

    LoadCompressedSpriteSheet(&sheet);
    LoadSpritePalette(&pal);

    spriteTemplate = Alloc(sizeof(*spriteTemplate));
    if (spriteTemplate == NULL)
        return SPRITE_NONE;

    CpuCopy16(&sDialogueIconSprite_SpriteTemplate, spriteTemplate, sizeof(*spriteTemplate));
    spriteTemplate->tileTag = TAG_CHARACTER_DIALOGUE_ICON;
    spriteTemplate->paletteTag = TAG_CHARACTER_DIALOGUE_ICON;

    spriteId = CreateSprite(spriteTemplate, 0, 0, 0);

    Free(spriteTemplate);

    if (spriteId == SPRITE_NONE)
        return SPRITE_NONE;

    PreservePaletteInWeather(gSprites[spriteId].oam.paletteNum + 0x10);

    return spriteId;
}

u8 CreateAttitudeIconSprite(u32 attitudeIndex)
{
    u8 spriteId;

    struct CompressedSpriteSheet sheet = { .size = 0x1000, .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpritePalette pal = { .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpriteTemplate *spriteTemplate;

    if (attitudeIndex >= ATTITUDE_COUNT)
        attitudeIndex = ATTITUDE_NEUTRAL;

    sheet.data = gDialogueAttitudes[attitudeIndex].iconImage;
    pal.data = gDialogueAttitudes[attitudeIndex].iconPal;

    LoadCompressedSpriteSheet(&sheet);
    LoadSpritePalette(&pal);

    spriteTemplate = Alloc(sizeof(*spriteTemplate));
    if (spriteTemplate == NULL)
        return SPRITE_NONE;

    CpuCopy16(&sDialogueIconSprite_SpriteTemplate, spriteTemplate, sizeof(*spriteTemplate));
    spriteTemplate->tileTag = TAG_CHARACTER_DIALOGUE_ICON;
    spriteTemplate->paletteTag = TAG_CHARACTER_DIALOGUE_ICON;

    spriteId = CreateSprite(spriteTemplate, 0, 0, 0);

    Free(spriteTemplate);

    if (spriteId == SPRITE_NONE)
        return SPRITE_NONE;

    PreservePaletteInWeather(gSprites[spriteId].oam.paletteNum + 0x10);

    return spriteId;
}