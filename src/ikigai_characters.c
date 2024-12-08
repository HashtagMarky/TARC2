#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "ikigai_characters.h"
#include "data/ikigai_characters.h"

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
