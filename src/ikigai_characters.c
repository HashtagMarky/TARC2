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

static const u32 sCharacteristicIcon_Neutral[] = INCBIN_U32("graphics/dialogue_icons/neutral.4bpp.lz");
static const u16 sCharacteristicPal_Neutral[] = INCBIN_U16("graphics/dialogue_icons/neutral.gbapal");
static const u32 sCharacteristicIcon_Inspired[] = INCBIN_U32("graphics/dialogue_icons/inspired.4bpp.lz");
static const u16 sCharacteristicPal_Inspired[] = INCBIN_U16("graphics/dialogue_icons/inspired.gbapal");
static const u32 sCharacteristicIcon_Humble[] = INCBIN_U32("graphics/dialogue_icons/humble.4bpp.lz");
static const u16 sCharacteristicPal_Humble[] = INCBIN_U16("graphics/dialogue_icons/humble.gbapal");
static const u32 sCharacteristicIcon_Dominant[] = INCBIN_U32("graphics/dialogue_icons/dominant.4bpp.lz");
static const u16 sCharacteristicPal_Dominant[] = INCBIN_U16("graphics/dialogue_icons/dominant.gbapal");
static const u32 sCharacteristicIcon_Cynical[] = INCBIN_U32("graphics/dialogue_icons/cynical.4bpp.lz");
static const u16 sCharacteristicPal_Cynical[] = INCBIN_U16("graphics/dialogue_icons/cynical.gbapal");

static const struct OamData sCharacteristic_Oam = {
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 0,
};

static const struct SpriteTemplate sDialogueIconSprite_SpriteTemplate = {
    .tileTag = TAG_CHARACTER_DIALOGUE_ICON,
    .paletteTag = TAG_CHARACTER_DIALOGUE_ICON,
    .oam = &sCharacteristic_Oam,
    .callback = SpriteCallbackDummy,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable,
};

struct DialogueCharacteristics
{
    const u8 *name;
    s8 kindnessEffect;
    s8 strengthEffect;
    const u32 *iconImage;
    const u16 *iconPal;
};

static const struct DialogueCharacteristics sDialogueCharacteristics[ATTITUDE_COUNT] =
{
    [ATTITUDE_NEUTRAL] =
    {
        .name = COMPOUND_STRING("Neutral"),
        .kindnessEffect = 0,
        .strengthEffect = 0,
        .iconImage = sCharacteristicIcon_Neutral,
        .iconPal = sCharacteristicPal_Neutral
    },
    [ATTITUDE_INSPIRED] =
    {
        .name = COMPOUND_STRING("Inspired"),
        .kindnessEffect = 1,
        .strengthEffect = 1,
        .iconImage = sCharacteristicIcon_Inspired,
        .iconPal = sCharacteristicPal_Inspired
    },
    [ATTITUDE_HUMBLE] =
    {
        .name = COMPOUND_STRING("Humble"),
        .kindnessEffect = 1,
        .strengthEffect = -1,
        .iconImage = sCharacteristicIcon_Humble,
        .iconPal = sCharacteristicPal_Humble
    },
    [ATTITUDE_DOMINANT] =
    {
        .name = COMPOUND_STRING("Dominant"),
        .kindnessEffect = -1,
        .strengthEffect = 1,
        .iconImage = sCharacteristicIcon_Dominant,
        .iconPal = sCharacteristicPal_Dominant
    },
    [ATTITUDE_CYNICAL] =
    {
        .name = COMPOUND_STRING("Cynical"),
        .kindnessEffect = -1,
        .strengthEffect = -1,
        .iconImage = sCharacteristicIcon_Cynical,
        .iconPal = sCharacteristicPal_Cynical
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

    return (opinionKindness / MAIN_CHARACTER_COUNT - 1);

}

s8 IkigaiCharacter_GetAverageStrength(void)
{
    s32 opinionStrength = 0;
    u8 character;

    for (character = CHARACTER_DEFAULT + 1; character < MAIN_CHARACTER_COUNT; character++)
    {
        opinionStrength += gSaveBlock3Ptr->characters.opinionStrength[character];
    }

    return (opinionStrength / MAIN_CHARACTER_COUNT - 1);

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

void IkigaiCharacter_HandleDialogue(void)
{
    u8 character = ReturnIkigaiCharacter_ObjectEventGraphicsId(gObjectEvents[gSelectedObjectEvent].graphicsId);

    if (character == CHARACTER_DEFAULT || character >= MAIN_CHARACTER_COUNT)
        return;

    if (gSpecialVar_Result == MULTI_B_PRESSED && !IkigaiCharacter_GetSetConversedFlag(character, FALSE))
    {
        IkigaiCharacter_OpinionDecay(character);
        return;
    }

    if (gSpecialVar_Result >= NELEMS(sDialogueCharacteristics))
        gSpecialVar_Result = ATTITUDE_NEUTRAL;

    if (!IkigaiCharacter_GetSetConversedFlag(character, FALSE))
    {
        s8 opinionKindness = sDialogueCharacteristics[gSpecialVar_Result].kindnessEffect;
        s8 opinionStrength = sDialogueCharacteristics[gSpecialVar_Result].strengthEffect;

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

u8 CreateDialogueIconSprite(u8 characteristicIndex)
{
    u8 spriteId;

    struct CompressedSpriteSheet sheet = { .size = 0x1000, .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpritePalette pal = { .tag = TAG_CHARACTER_DIALOGUE_ICON };
    struct SpriteTemplate *spriteTemplate;

    if (characteristicIndex >= ATTITUDE_COUNT)
        return SPRITE_NONE;

    sheet.data = sDialogueCharacteristics[characteristicIndex].iconImage;
    pal.data = sDialogueCharacteristics[characteristicIndex].iconPal;

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
