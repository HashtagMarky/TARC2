#include "global.h"
#include "test/test.h"
#include "text.h"

#include "ikigai_characters.h"
#include "constants/ikigai_characters.h"

TEST("Character Descriptions Fit in PokéSphere")
{
    u32 i;
    const u32 fontId = FONT_SHORT_NARROWER, widthPx = 136 - 10;
    u32 character = CHARACTER_FIRST;
    for (i = CHARACTER_FIRST; i < CHARACTER_COUNT_TOTAL; i++)
    {
        if (gIkigaiCharactersInfo[i].highlights == NULL)
            continue;
        
        PARAMETRIZE_LABEL("%S", gIkigaiCharactersInfo[i].highlights) { character = i; }
    }
    EXPECT_LE(GetStringWidth(fontId, gIkigaiCharactersInfo[character].highlights, 0), widthPx);
}