#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "event_data.h"
#include "ikigai_characters.h"
#include "data/ikigai_characters.h"

struct DialogueCharacteristics
{
    const u8 *name;
    s8 kindnessEffect;
    s8 strengthEffect;
};

static const struct DialogueCharacteristics sDialogueCharacteristics[CHARACTERISTIC_COUNT] =
{
    [CHARACTERISTIC_NEUTRAL] =
    {
        .name = COMPOUND_STRING("Neutral"),
        .kindnessEffect = 0,
        .strengthEffect = 0,
    },
    [CHARACTERISTIC_INSPIRING] =
    {
        .name = COMPOUND_STRING("Inspiring"),
        .kindnessEffect = 1,
        .strengthEffect = 1,
    },
    [CHARACTERISTIC_HUMBLE] =
    {
        .name = COMPOUND_STRING("Humble"),
        .kindnessEffect = 1,
        .strengthEffect = -1,
    },
    [CHARACTERISTIC_DOMINATING] =
    {
        .name = COMPOUND_STRING("Dominating"),
        .kindnessEffect = -1,
        .strengthEffect = 1,
    },
    [CHARACTERISTIC_CYNICAL] =
    {
        .name = COMPOUND_STRING("Cynical"),
        .kindnessEffect = -1,
        .strengthEffect = -1,
    },
};

u8 ReturnCharacterFromObjectGraphicsId(u16 graphicsId)
{
    u8 i;

    for (i = 0; i < CHARACTER_COUNT; i++)
    {
        if (graphicsId == gIkigaiCharactersInfo[i].overworldGraphicsId)
            return i;
    }

    return CHARACTER_DEFAULT;
}

s8 GetSetConversedFlag(u8 character, bool8 setFlag)
{
    u32 index, bit, mask;
    s8 retVal = 0;

    if (character >= CHARACTER_COUNT)
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

void ClearConversedDailyFlags(void)
{
    memset(gSaveBlock3Ptr->characters.conversed, 0, sizeof(gSaveBlock3Ptr->characters.conversed));
}

void IkigaiCharacter_HandleDialogue(void)
{
    u8 character = ReturnCharacterFromObjectGraphicsId(gObjectEvents[gSelectedObjectEvent].graphicsId);

    if (character == CHARACTER_DEFAULT)
        return;

    if (GetSetConversedFlag(character, FALSE))
        GetSetConversedFlag(character, TRUE);

    if (gSpecialVar_Result >= NELEMS(sDialogueCharacteristics))
        gSpecialVar_Result = CHARACTERISTIC_NEUTRAL;

    gSaveBlock3Ptr->characters.opinionKindness[character] += sDialogueCharacteristics[gSpecialVar_Result].kindnessEffect;
    gSaveBlock3Ptr->characters.opinionStrength[character] += sDialogueCharacteristics[gSpecialVar_Result].strengthEffect;
}
