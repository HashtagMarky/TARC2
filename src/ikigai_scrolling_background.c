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

static const u16 IkigaiScrollingBgPal_Normal[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/normal.gbapal");
static const u16 IkigaiScrollingBgPal_Fighting[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/fighting.gbapal");
static const u16 IkigaiScrollingBgPal_Flying[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/flying.gbapal");
static const u16 IkigaiScrollingBgPal_Poison[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/poison.gbapal");
static const u16 IkigaiScrollingBgPal_Ground[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/ground.gbapal");
static const u16 IkigaiScrollingBgPal_Rock[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/rock.gbapal");
static const u16 IkigaiScrollingBgPal_Bug[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/bug.gbapal");
static const u16 IkigaiScrollingBgPal_Ghost[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/ghost.gbapal");
static const u16 IkigaiScrollingBgPal_Steel[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/steel.gbapal");
static const u16 IkigaiScrollingBgPal_Fire[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/fire.gbapal");
static const u16 IkigaiScrollingBgPal_Water[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/water.gbapal");
static const u16 IkigaiScrollingBgPal_Grass[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/grass.gbapal");
static const u16 IkigaiScrollingBgPal_Electric[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/electric.gbapal");
static const u16 IkigaiScrollingBgPal_Psychic[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/psychic.gbapal");
static const u16 IkigaiScrollingBgPal_Ice[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/ice.gbapal");
static const u16 IkigaiScrollingBgPal_Dragon[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/dragon.gbapal");
static const u16 IkigaiScrollingBgPal_Dark[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/dark.gbapal");
static const u16 IkigaiScrollingBgPal_Fairy[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/fairy.gbapal");

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

        case TYPE_NORMAL:
            return IkigaiScrollingBgPal_Normal;
        case TYPE_FIGHTING:
            return IkigaiScrollingBgPal_Fighting;
        case TYPE_FLYING:
            return IkigaiScrollingBgPal_Flying;
        case TYPE_POISON:
            return IkigaiScrollingBgPal_Poison;
        case TYPE_GROUND:
            return IkigaiScrollingBgPal_Ground;
        case TYPE_ROCK:
            return IkigaiScrollingBgPal_Rock;
        case TYPE_BUG:
            return IkigaiScrollingBgPal_Bug;
        case TYPE_GHOST:
            return IkigaiScrollingBgPal_Ghost;
        case TYPE_STEEL:
            return IkigaiScrollingBgPal_Steel;
        case TYPE_FIRE:
            return IkigaiScrollingBgPal_Fire;
        case TYPE_WATER:
            return IkigaiScrollingBgPal_Water;
        case TYPE_GRASS:
            return IkigaiScrollingBgPal_Grass;
        case TYPE_ELECTRIC:
            return IkigaiScrollingBgPal_Electric;
        case TYPE_PSYCHIC:
            return IkigaiScrollingBgPal_Psychic;
        case TYPE_ICE:
            return IkigaiScrollingBgPal_Ice;
        case TYPE_DRAGON:
            return IkigaiScrollingBgPal_Dragon;
        case TYPE_DARK:
            return IkigaiScrollingBgPal_Dark;
        case TYPE_FAIRY:
            return IkigaiScrollingBgPal_Fairy;
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