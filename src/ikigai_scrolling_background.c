#include "ikigai_scrolling_background.h"
#include "global.h"
#include "main.h"
#include "decompress.h"
#include "menu.h"
#include "bg.h"
#include "palette.h"


static const u32 IkigaiScrollingBgTiles[]               = INCBIN_U32("graphics/ikigai_scrolling_background/scroll_tiles.4bpp.lz");

static const u32 IkigaiScrollingBgTilemap_PalZero[]     = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/zero.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalOne[]      = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/one.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalTwo[]      = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/two.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalThree[]    = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/three.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalFour[]     = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/four.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalFive[]     = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/five.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalSix[]      = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/six.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalSeven[]    = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/seven.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalEight[]    = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/eight.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalNine[]     = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/nine.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalTen[]      = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/ten.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalEleven[]   = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/eleven.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalTwelve[]   = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/twelve.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalThirteen[] = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/thirteen.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalFourteen[] = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/fourteen.bin.lz");
static const u32 IkigaiScrollingBgTilemap_PalFifteen[]  = INCBIN_U32("graphics/ikigai_scrolling_background/tilemaps/fifteen.bin.lz");

static const u16 IkigaiScrollingBgPal_Default[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_bg_palettes/default.gbapal");
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

static const u32 *const sIkigaiScrollingBgTilemaps[16] =
{
    IkigaiScrollingBgTilemap_PalZero,
    IkigaiScrollingBgTilemap_PalOne,
    IkigaiScrollingBgTilemap_PalTwo,
    IkigaiScrollingBgTilemap_PalThree,
    IkigaiScrollingBgTilemap_PalFour,
    IkigaiScrollingBgTilemap_PalFive,
    IkigaiScrollingBgTilemap_PalSix,
    IkigaiScrollingBgTilemap_PalSeven,
    IkigaiScrollingBgTilemap_PalEight,
    IkigaiScrollingBgTilemap_PalNine,
    IkigaiScrollingBgTilemap_PalTen,
    IkigaiScrollingBgTilemap_PalEleven,
    IkigaiScrollingBgTilemap_PalTwelve,
    IkigaiScrollingBgTilemap_PalThirteen,
    IkigaiScrollingBgTilemap_PalFourteen,
    IkigaiScrollingBgTilemap_PalFifteen
};

#define SCROLLING_SPEED         64

void StartIkigaiScrollingBackground(u8 background)
{
    ChangeBgX(background, SCROLLING_SPEED, BG_COORD_SUB);
    ChangeBgY(background, SCROLLING_SPEED, BG_COORD_ADD);
}

void IkigaiScrollingBackground_CreateTiles(u8 background)
{
    DecompressAndCopyTileDataToVram(background, &IkigaiScrollingBgTiles, 0, 0, 0);
}

void IkigaiScrollingBackground_CreateTilemap(u8 paletteSlot, void *dest)
{
    paletteSlot = paletteSlot > NELEMS(sIkigaiScrollingBgTilemaps) ? 0 : paletteSlot;
    LZDecompressWram(sIkigaiScrollingBgTilemaps[paletteSlot], dest);
}

void IkigaiScrollingBackground_LoadPalette(u8 paletteSlot, enum IkigaiBackgroundTypes type)
{
    const u16 *palette;
    switch (type)
    {
        case IKIGAI_BG_INTERFACE:
        palette = Ikigai_ReturnScrollingBackgroundPalette();
        break;
    
        case IKIGAI_BG_GYM:
        palette = Ikigai_ReturnScrollingBackgroundGymPalette();
        break;
    
        case IKIGAI_BG_SEASON:
        palette = Ikigai_ReturnScrollingBackgroundSeasonPalette(SEASON_COUNT);
        break;
    
        case IKIGAI_BG_DEFAULT:
        default:
        palette = IkigaiScrollingBgPal_Default;
        break;
    }

    LoadPalette(palette, BG_PLTT_ID(paletteSlot), PLTT_SIZE_4BPP);
}

void IkigaiUI_LoadPalette(u8 paletteSlot, enum IkigaiBackgroundTypes type)
{
    const u16 *palette;
    switch (type)
    {
        case IKIGAI_BG_GYM:
        palette = Ikigai_ReturnUIGymPalette();
    
        case IKIGAI_BG_INTERFACE:
        default:
        palette = Ikigai_ReturnUIPalette();
        break;
    }

    LoadPalette(palette, BG_PLTT_ID(paletteSlot), PLTT_SIZE_4BPP);
}

const u16 *Ikigai_ReturnScrollingBackgroundPalette(void)
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
            return Ikigai_ReturnScrollingBackgroundGymPalette();
    }
}

const u16 *Ikigai_ReturnScrollingBackgroundGymPalette(void)
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

const u16 *Ikigai_ReturnScrollingBackgroundSeasonPalette(enum Seasons season)
{
    switch (season)
    {
        case SEASON_SPRING:
            return IkigaiScrollingBgPal_Enamorus;

        case SEASON_SUMMER:
            return IkigaiScrollingBgPal_Tornadus;
        
        case SEASON_AUTUMN:
            return IkigaiScrollingBgPal_Landorus;
        
        case SEASON_WINTER:
            return IkigaiScrollingBgPal_Thundurus;

        case SEASON_COUNT:
        default:
            return IkigaiScrollingBgPal_Default;
    }
}

const u16 *Ikigai_ReturnUIPalette(void)
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
            return Ikigai_ReturnUIGymPalette();
    }
}

const u16 *Ikigai_ReturnUIGymPalette(void)
{
    switch (gSaveBlock2Ptr->ikigaiGymType)
    {
        case TYPE_NONE:
        default:
            return IkigaiUIPal_Default;
    }
}