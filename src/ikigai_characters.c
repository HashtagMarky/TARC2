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
static const u32 sCharacteristicDialogueIcon_Talk[] = INCBIN_U32("graphics/dialogue_icons/talk.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Talk[] = INCBIN_U16("graphics/dialogue_icons/talk.gbapal");
static const u32 sCharacteristicDialogueIcon_Opinion[] = INCBIN_U32("graphics/dialogue_icons/opinion.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Opinion[] = INCBIN_U16("graphics/dialogue_icons/opinion.gbapal");
static const u32 sCharacteristicDialogueIcon_Romance[] = INCBIN_U32("graphics/dialogue_icons/romance.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Romance[] = INCBIN_U16("graphics/dialogue_icons/romance.gbapal");
static const u32 sCharacteristicDialogueIcon_Quest[] = INCBIN_U32("graphics/dialogue_icons/quest.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Quest[] = INCBIN_U16("graphics/dialogue_icons/quest.gbapal");
static const u32 sCharacteristicDialogueIcon_Battle[] = INCBIN_U32("graphics/dialogue_icons/battle.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Battle[] = INCBIN_U16("graphics/dialogue_icons/battle.gbapal");
static const u32 sCharacteristicDialogueIcon_Gift[] = INCBIN_U32("graphics/dialogue_icons/gift.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Gift[] = INCBIN_U16("graphics/dialogue_icons/gift.gbapal");
static const u32 sCharacteristicDialogueIcon_Goodbye[] = INCBIN_U32("graphics/dialogue_icons/goodbye.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Goodbye[] = INCBIN_U16("graphics/dialogue_icons/goodbye.gbapal");


static const u32 sCharacteristicDialogueIcon_Neutral[] = INCBIN_U32("graphics/dialogue_icons/neutral.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Neutral[] = INCBIN_U16("graphics/dialogue_icons/neutral.gbapal");
static const u32 sCharacteristicDialogueIcon_Inspired[] = INCBIN_U32("graphics/dialogue_icons/inspired.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Inspired[] = INCBIN_U16("graphics/dialogue_icons/inspired.gbapal");
static const u32 sCharacteristicDialogueIcon_Humble[] = INCBIN_U32("graphics/dialogue_icons/humble.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Humble[] = INCBIN_U16("graphics/dialogue_icons/humble.gbapal");
static const u32 sCharacteristicDialogueIcon_Dominant[] = INCBIN_U32("graphics/dialogue_icons/dominant.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Dominant[] = INCBIN_U16("graphics/dialogue_icons/dominant.gbapal");
static const u32 sCharacteristicDialogueIcon_Cynical[] = INCBIN_U32("graphics/dialogue_icons/cynical.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Cynical[] = INCBIN_U16("graphics/dialogue_icons/cynical.gbapal");

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

struct DialogueOptions
{
    const u8 *name;
    s8 kindnessEffect;
    s8 strengthEffect;
    const u32 *iconImage;
    const u16 *iconPal;
};

static const struct DialogueOptions sDialogueOptions[DIALOGUE_OPTION_COUNT] =
{
    [DIALOGUE_OPTION_TALK] =
    {
        .iconImage = sCharacteristicDialogueIcon_Talk,
        .iconPal = sCharacteristicDialoguePal_Talk
    },
    [DIALOGUE_OPTION_OPINION] =
    {
        .iconImage = sCharacteristicDialogueIcon_Opinion,
        .iconPal = sCharacteristicDialoguePal_Opinion
    },
    [DIALOGUE_OPTION_ROMANCE] =
    {
        .iconImage = sCharacteristicDialogueIcon_Romance,
        .iconPal = sCharacteristicDialoguePal_Romance
    },
    [DIALOGUE_OPTION_QUESTS] =
    {
        .iconImage = sCharacteristicDialogueIcon_Quest,
        .iconPal = sCharacteristicDialoguePal_Quest
    },
    [DIALOGUE_OPTION_BATTLE] =
    {
        .iconImage = sCharacteristicDialogueIcon_Battle,
        .iconPal = sCharacteristicDialoguePal_Battle
    },
    [DIALOGUE_OPTION_GIFT] =
    {
        .iconImage = sCharacteristicDialogueIcon_Gift,
        .iconPal = sCharacteristicDialoguePal_Gift
    },
    [DIALOGUE_OPTION_GOODBYE] =
    {
        .iconImage = sCharacteristicDialogueIcon_Goodbye,
        .iconPal = sCharacteristicDialoguePal_Goodbye
    },
};

static const struct DialogueOptions sDialogueAttitudes[ATTITUDE_COUNT] =
{
    [ATTITUDE_NEUTRAL] =
    {
        .name = COMPOUND_STRING("Neutral"),
        .kindnessEffect = 0,
        .strengthEffect = 0,
        .iconImage = sCharacteristicDialogueIcon_Neutral,
        .iconPal = sCharacteristicDialoguePal_Neutral
    },
    [ATTITUDE_INSPIRED] =
    {
        .name = COMPOUND_STRING("Inspired"),
        .kindnessEffect = 1,
        .strengthEffect = 1,
        .iconImage = sCharacteristicDialogueIcon_Inspired,
        .iconPal = sCharacteristicDialoguePal_Inspired
    },
    [ATTITUDE_HUMBLE] =
    {
        .name = COMPOUND_STRING("Humble"),
        .kindnessEffect = 1,
        .strengthEffect = -1,
        .iconImage = sCharacteristicDialogueIcon_Humble,
        .iconPal = sCharacteristicDialoguePal_Humble
    },
    [ATTITUDE_DOMINANT] =
    {
        .name = COMPOUND_STRING("Dominant"),
        .kindnessEffect = -1,
        .strengthEffect = 1,
        .iconImage = sCharacteristicDialogueIcon_Dominant,
        .iconPal = sCharacteristicDialoguePal_Dominant
    },
    [ATTITUDE_CYNICAL] =
    {
        .name = COMPOUND_STRING("Cynical"),
        .kindnessEffect = -1,
        .strengthEffect = -1,
        .iconImage = sCharacteristicDialogueIcon_Cynical,
        .iconPal = sCharacteristicDialoguePal_Cynical
    },
};

u8 IkigaiCharacter_GetPlayerAttitude(void)
{
    s8 opinionKindness = IkigaiCharacter_GetAverageKindness();
    s8 opinionStrength = IkigaiCharacter_GetAverageStrength();

    if (opinionKindness > ATTITUDE_NEUTRAL_BUFFER)
    {
        if (opinionStrength > ATTITUDE_NEUTRAL_BUFFER)
            return ATTITUDE_INSPIRED;

        if (opinionStrength < - ATTITUDE_NEUTRAL_BUFFER)
            return ATTITUDE_HUMBLE;
    }

    if (opinionKindness < - ATTITUDE_NEUTRAL_BUFFER)
    {
        if (opinionStrength > ATTITUDE_NEUTRAL_BUFFER)
            return ATTITUDE_DOMINANT;

        if (opinionStrength < - ATTITUDE_NEUTRAL_BUFFER)
            return ATTITUDE_CYNICAL;
    }

    return ATTITUDE_NEUTRAL;
}

void IkigaiCharacter_SetDefaultOpinion(u8 character)
{
    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return;
    
    gSaveBlock3Ptr->characters.opinionKindness[character] = gIkigaiCharactersInfo[character].baseOpinionKindness;
    gSaveBlock3Ptr->characters.opinionStrength[character] = gIkigaiCharactersInfo[character].baseOpinionStrength;
}

void IkigaiCharacter_SetAllCharacterDefaultOpinion(void)
{
    u8 character;

    for (character = CHARACTER_DEFAULT + 1; character < MAIN_CHARACTER_COUNT; character++)
    {
        IkigaiCharacter_SetDefaultOpinion(character);
    }
}

s8 IkigaiCharacter_GetAverageKindness(void)
{
    s32 opinionKindness = 0;
    u8 character;

    for (character = CHARACTER_DEFAULT + 1; character < MAIN_CHARACTER_COUNT; character++)
    {
        opinionKindness += gSaveBlock3Ptr->characters.opinionKindness[character];
    }

    return (opinionKindness / (MAIN_CHARACTER_COUNT - 1));

}

s8 IkigaiCharacter_GetAverageStrength(void)
{
    s32 opinionStrength = 0;
    u8 character;

    for (character = CHARACTER_DEFAULT + 1; character < MAIN_CHARACTER_COUNT; character++)
    {
        opinionStrength += gSaveBlock3Ptr->characters.opinionStrength[character];
    }

    return (opinionStrength / (MAIN_CHARACTER_COUNT - 1));

}

s8 IkigaiCharacter_GetSetConversedFlag(u8 character, bool8 setFlag)
{
    u32 index, bit, mask;
    s8 retVal = 0;

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

void IkigaiCharacter_SetRomanticFlag(u8 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;
    
    FlagSet(gIkigaiCharactersInfo[character].flagRomantic);
}

void IkigaiCharacter_ToggleRomanticFlag(u8 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    FlagToggle(gIkigaiCharactersInfo[character].flagRomantic);
}

void IkigaiCharacter_ClearRomanticFlag(u8 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    FlagClear(gIkigaiCharactersInfo[character].flagRomantic);
}

bool8 IkigaiCharacter_GetRomanticFlag(u8 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return FALSE;

    return FlagGet(gIkigaiCharactersInfo[character].flagRomantic);
}

void IkigaiCharacter_SetRomanticFlag_Exclusive(u8 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    u8 i;

    for (i = CHARACTER_DEFAULT + 1; i < CHARACTER_COUNT_TOTAL; i++)
    {
        if (i == character)
            IkigaiCharacter_SetRomanticFlag(i);
        else
            IkigaiCharacter_ClearRomanticFlag(i);
    }
}

void IkigaiCharacter_ClearRomanticFlag_Amicable(u8 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    FlagClear(gIkigaiCharactersInfo[character].flagRomantic);
}

void IkigaiCharacter_ClearRomanticFlag_Hostile(u8 character)
{
    if (gIkigaiCharactersInfo[character].flagRomantic == 0)
        return;

    FlagClear(gIkigaiCharactersInfo[character].flagRomantic);
    if (character > CHARACTER_DEFAULT && character < MAIN_CHARACTER_COUNT)
        gSaveBlock3Ptr->characters.opinionKindness[character] = - ATTITUDE_NEUTRAL_BUFFER;
}

u8 IkigaiCharacter_CheckRelationships(void)
{
    if (IkigaiCharacter_IsPlayerSingleOrMonogamous())
        return ReturnIkigaiCharacter_RomanceFlag_Exclusive();
        
    else
        return CHARACTER_COUNT_TOTAL;
}

bool8 IkigaiCharacter_IsPlayerSingleOrMonogamous(void)
{
    u8 character;
    u8 relationships = RELATIONSHIP_SINGLE;

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
}

void ScrCmd_IkigaiCharacter_ToggleRomanticFlag(void)
{
    IkigaiCharacter_ToggleRomanticFlag(
        ReturnIkigaiCharacter_SelectedObject()
    );
}

void ScrCmd_IkigaiCharacter_ClearRomanticFlag(void)
{
    IkigaiCharacter_ClearRomanticFlag(
        ReturnIkigaiCharacter_SelectedObject()
    );
}

void ScrCmd_IkigaiCharacter_GetRomanticFlag(void)
{
    gSpecialVar_Result = IkigaiCharacter_GetRomanticFlag(
        ReturnIkigaiCharacter_SelectedObject()
    );
}

void ScrCmd_IkigaiCharacter_SetRomanticFlag_Exclusive(void)
{
    IkigaiCharacter_SetRomanticFlag_Exclusive(
        ReturnIkigaiCharacter_SelectedObject()
    );
}

void ScrCmd_IkigaiCharacter_ClearRomanticFlag_Amicable(void)
{
    IkigaiCharacter_ClearRomanticFlag_Amicable(
        ReturnIkigaiCharacter_SelectedObject()
    );
}

void ScrCmd_IkigaiCharacter_ClearRomanticFlag_Hostile(void)
{
    IkigaiCharacter_ClearRomanticFlag_Hostile(
        ReturnIkigaiCharacter_SelectedObject()
    );
}

void ScrCmd_IkigaiCharacter_CheckRelationships(void)
{
    gSpecialVar_Result = IkigaiCharacter_CheckRelationships();
}

void ScrCmd_IkigaiCharacter_BurfferName(void)
{
    StringCopy(gStringVar1, gIkigaiCharactersInfo[gSpecialVar_Result].name);
}

bool8 IkigaiCharacter_ReturnOpinionDecay(u8 character)
{
    if (character > CHARACTER_COUNT_TOTAL)
        return FALSE; 

    if ((Random() % 100) < gIkigaiCharactersInfo[character].opinionDecayChance)
        return TRUE;
    else
        return FALSE;
}

void IkigaiCharacter_OpinionDecay(u8 character)
{
    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return;
    
    if (IkigaiCharacter_ReturnOpinionDecay(character))
    {
        if (gSaveBlock3Ptr->characters.opinionKindness[character] > ATTITUDE_NEUTRAL_BUFFER)
            gSaveBlock3Ptr->characters.opinionKindness[character]--;
        else if (gSaveBlock3Ptr->characters.opinionKindness[character] < ATTITUDE_NEUTRAL_BUFFER)
            gSaveBlock3Ptr->characters.opinionKindness[character]++;
    }

    if (IkigaiCharacter_ReturnOpinionDecay(character))
    {
        if (gSaveBlock3Ptr->characters.opinionStrength[character] > ATTITUDE_NEUTRAL_BUFFER)
            gSaveBlock3Ptr->characters.opinionStrength[character]--;
        else if (gSaveBlock3Ptr->characters.opinionStrength[character] < ATTITUDE_NEUTRAL_BUFFER)
            gSaveBlock3Ptr->characters.opinionStrength[character]++;
    }
}

void IkigaiCharacter_AllOpinionDecay_NonConverse(void)
{
    u8 character;

    for (character = CHARACTER_DEFAULT + 1; character < MAIN_CHARACTER_COUNT; character++)
    {
        if (!IkigaiCharacter_GetSetConversedFlag(character, FALSE))
            IkigaiCharacter_OpinionDecay(character);
    }
}

void IkigaiCharacter_HandleDialogue_Attitudes(void)
{
    u8 character = ReturnIkigaiCharacter_SelectedObject();

    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return;

    if (gSpecialVar_Result == MULTI_B_PRESSED && !IkigaiCharacter_GetSetConversedFlag(character, FALSE))
    {
        IkigaiCharacter_OpinionDecay(character);
        return;
    }

    if (gSpecialVar_Result >= NELEMS(sDialogueAttitudes))
        gSpecialVar_Result = ATTITUDE_NEUTRAL;

    if (!IkigaiCharacter_GetSetConversedFlag(character, FALSE))
    {
        s8 opinionKindness = sDialogueAttitudes[gSpecialVar_Result].kindnessEffect;
        s8 opinionStrength = sDialogueAttitudes[gSpecialVar_Result].strengthEffect;

        if (gSpecialVar_Result == gIkigaiCharactersInfo[character].personality)
        {
            u8 opinionMultiplier = Random() % 2;

            if (opinionKindness < 0)
                opinionKindness *= (opinionMultiplier - 1);
            else
                opinionKindness *= (opinionMultiplier + 1);

            if (opinionStrength < 0)
                opinionStrength *= (opinionMultiplier - 1);
            else
                opinionStrength *= (opinionMultiplier + 1);
        }
 
        gSaveBlock3Ptr->characters.opinionKindness[character] += opinionKindness;
        gSaveBlock3Ptr->characters.opinionStrength[character] += opinionStrength;
        IkigaiCharacter_GetSetConversedFlag(character, TRUE);
    }
}

void IkigaiCharacter_CharacterOpinions(void)
{
    u8 character = ReturnIkigaiCharacter_SelectedObject();
    u8 string[3];
    s8 opinionKindness = gSaveBlock3Ptr->characters.opinionKindness[character];
    s8 opinionStrength = gSaveBlock3Ptr->characters.opinionStrength[character];

    if (character > MAIN_CHARACTER_COUNT)
    {
        opinionKindness = IkigaiCharacter_GetAverageKindness();
        opinionStrength = IkigaiCharacter_GetAverageStrength();
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
    if (character > MAIN_CHARACTER_COUNT)
    {
        StringAppend(gStringVar1, COMPOUND_STRING("\pI am a special character, I get my\nopinion from the PokéSphere."));
    }
}

u8 ReturnIkigaiCharacter_ObjectEventGraphicsId(u16 graphicsId)
{
    u8 character;

    for (character = CHARACTER_DEFAULT + 1; character < CHARACTER_COUNT_TOTAL; character++)
    {
        if (graphicsId == gIkigaiCharactersInfo[character].overworldGraphicsId)
            return character;
    }

    return CHARACTER_DEFAULT;
}

u8 ReturnIkigaiCharacter_SelectedObject(void)
{
    return ReturnIkigaiCharacter_ObjectEventGraphicsId(gObjectEvents[gSelectedObjectEvent].graphicsId);
}

void ScrCmd_ReturnIkigaiCharacter_SelectedObject(void)
{
    gSpecialVar_Result = ReturnIkigaiCharacter_SelectedObject();
}

u8 ReturnIkigaiCharacter_RomanceFlag_Exclusive(void)
{
    u8 character;

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

u8 CreateDialogueOptionIconSprite(u8 dialogueIndex)
{
    u8 spriteId;

    struct CompressedSpriteSheet sheet = { .size = 0x1000, .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpritePalette pal = { .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpriteTemplate *spriteTemplate;

    if (dialogueIndex >= DIALOGUE_OPTION_COUNT)
        dialogueIndex = DIALOGUE_OPTION_TALK;

    sheet.data = sDialogueOptions[dialogueIndex].iconImage;
    pal.data = sDialogueOptions[dialogueIndex].iconPal;

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

u8 CreateAttitudeIconSprite(u8 attitudeIndex)
{
    u8 spriteId;

    struct CompressedSpriteSheet sheet = { .size = 0x1000, .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpritePalette pal = { .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpriteTemplate *spriteTemplate;

    if (attitudeIndex >= ATTITUDE_COUNT)
        attitudeIndex = ATTITUDE_NEUTRAL;

    sheet.data = sDialogueAttitudes[attitudeIndex].iconImage;
    pal.data = sDialogueAttitudes[attitudeIndex].iconPal;

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