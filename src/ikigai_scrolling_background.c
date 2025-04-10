#include "ikigai_scrolling_background.h"
#include "global.h"
#include "main.h"
#include "bg.h"
#include "constants/pokemon.h"

const u32 IkigaiScrollingBgTiles[] = INCBIN_U32("graphics/ikigai_scrolling_background/scroll_tiles.4bpp.lz");
const u32 IkigaiScrollingBgTilemap_PalOne[] = INCBIN_U32("graphics/ikigai_scrolling_background/scroll_tiles_pal_one.bin.lz");
const u32 IkigaiScrollingBgTilemap_PalTwo[] = INCBIN_U32("graphics/ikigai_scrolling_background/scroll_tiles_pal_two.bin.lz");
const u32 IkigaiScrollingBgTilemap_PalEleven[] = INCBIN_U32("graphics/ikigai_scrolling_background/scroll_tiles_pal_eleven.bin.lz");
const u16 IkigaiScrollingBgPal_Default[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/default.gbapal");

static const u16 IkigaiScrollingBgPal_Tornadus[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/tornadus.gbapal");
static const u16 IkigaiScrollingBgPal_Thundurus[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/thundurus.gbapal");
static const u16 IkigaiScrollingBgPal_Landorus[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/landorus.gbapal");
static const u16 IkigaiScrollingBgPal_Enamorus[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/enamorus.gbapal");

static const u16 IkigaiUIPal_Default[] = INCBIN_U16("graphics/ikigai_scrolling_background/ui_tiles_palettes/default.gbapal");
static const u16 IkigaiUIPal_Tornadus[] = INCBIN_U16("graphics/ikigai_scrolling_background/ui_tiles_palettes/tornadus.gbapal");
static const u16 IkigaiUIPal_Thundurus[] = INCBIN_U16("graphics/ikigai_scrolling_background/ui_tiles_palettes/thundurus.gbapal");
static const u16 IkigaiUIPal_Landorus[] = INCBIN_U16("graphics/ikigai_scrolling_background/ui_tiles_palettes/landorus.gbapal");
static const u16 IkigaiUIPal_Enamorus[] = INCBIN_U16("graphics/ikigai_scrolling_background/ui_tiles_palettes/enamorus.gbapal");

#define SCROLLING_SPEED         64

void StartIkigaiScrollingBackground(u8 background)
{
    ChangeBgX(background, SCROLLING_SPEED, BG_COORD_SUB);
    ChangeBgY(background, SCROLLING_SPEED, BG_COORD_ADD);
}

const u16 *ReturnScrollingBackgroundPalette(void)
{
    switch (gSaveBlock2Ptr->optionsInterfaceColor)
    {
        case IKIGAI_INTERFACE_GREEN:
            return IkigaiScrollingBgPal_Tornadus;
        
        case IKIGAI_INTERFACE_BLUE:
            return IkigaiScrollingBgPal_Thundurus;
        
        case IKIGAI_INTERFACE_ORANGE:
            return IkigaiScrollingBgPal_Landorus;
        
        case IKIGAI_INTERFACE_PINK:
            return IkigaiScrollingBgPal_Enamorus;

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
            return IkigaiScrollingBgPal_Default;
    }
}

const u16 *ReturnMenuUIPalette(void)
{
    switch (gSaveBlock2Ptr->optionsInterfaceColor)
    {
        case IKIGAI_INTERFACE_GREEN:
            return IkigaiUIPal_Tornadus;
        
        case IKIGAI_INTERFACE_BLUE:
            return IkigaiUIPal_Thundurus;
        
        case IKIGAI_INTERFACE_ORANGE:
            return IkigaiUIPal_Landorus;
        
        case IKIGAI_INTERFACE_PINK:
            return IkigaiUIPal_Enamorus;
            
        case IKIGAI_INTERFACE_GYM_TYPE_COLOUR:
        default:
            return ReturnMenuUIGymPalette();
    }
}

const u16 *ReturnMenuUIGymPalette(void)
{
    switch (gSaveBlock2Ptr->ikigaiGymType)
    {
        case TYPE_NONE:
        default:
            return IkigaiUIPal_Default;
    }
}

const u16 *ReturnScrollingBackgroundSeasonPalette(u8 season)
{
    switch (season)
    {
        case 1:
            return IkigaiScrollingBgPal_Enamorus;

        case 2:
            return IkigaiScrollingBgPal_Tornadus;
        
        case 3:
            return IkigaiScrollingBgPal_Landorus;
        
        case 4:
            return IkigaiScrollingBgPal_Thundurus;

        case 0:
        default:
            return IkigaiScrollingBgPal_Default;
    }
}