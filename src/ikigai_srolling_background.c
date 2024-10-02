#include "ikigai_scrolling_background.h"
#include "global.h"
#include "main.h"
#include "bg.h"
#include "constants/pokemon.h"

const u32 sScrollBgTiles[] = INCBIN_U32("graphics/ikigai_scrolling_background/scroll_tiles.4bpp.lz");
const u32 sScrollBgTilemap[] = INCBIN_U32("graphics/ikigai_scrolling_background/scroll_tiles.bin.lz");

static const u16 sScrollBgPalette[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_tiles.gbapal");
static const u16 sScrollBgPaletteGreen[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_tiles_green.gbapal");
static const u16 sScrollBgPaletteBlue[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_tiles_blue.gbapal");
static const u16 sScrollBgPaletteOrange[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_tiles_orange.gbapal");
static const u16 sScrollBgPalettePink[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_tiles_pink.gbapal");

#define SCROLLING_SPEED         64
#define SCROLL_X_DIRECTION      1
#define SCROLL_Y_DIRECTION      -1
// Positive Direction is RIGHT/DOWN

void StartScrollingBackground(u8 background, u8 op)
{
    ChangeBgX(background, SCROLLING_SPEED * SCROLL_X_DIRECTION, op);
    ChangeBgY(background, SCROLLING_SPEED * SCROLL_Y_DIRECTION, op);
}

const u16 *ReturnScrollingBackgroundPalette(void)
{
    switch (gSaveBlock2Ptr->optionsInterfaceColor)
    {
        case IKIGAI_INTERFACE_GREEN:
            return sScrollBgPaletteGreen;
        
        case IKIGAI_INTERFACE_BLUE:
            return sScrollBgPaletteBlue;
        
        case IKIGAI_INTERFACE_ORANGE:
            return sScrollBgPaletteOrange;
        
        case IKIGAI_INTERFACE_PINK:
            return sScrollBgPalettePink;

        case IKIGAI_INTERFACE_GYM_TYPE_COLOUR:
        default:
            return ReturnScrollingBackgroundGymPalette();
    }
}

const u16 *ReturnScrollingBackgroundGymPalette(void)
{
    switch (gSaveBlock2Ptr->ikigaiGymType)
    {
        case TYPE_NONE:
        default:
            return sScrollBgPalette;
    }
}