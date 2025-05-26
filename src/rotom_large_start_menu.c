#include "rotom_phone_start_menu.h"
#include "sample_ui.h"

#include "gba/types.h"
#include "gba/defines.h"
#include "global.h"
#include "main.h"
#include "bg.h"
#include "text_window.h"
#include "window.h"
#include "constants/characters.h"
#include "palette.h"
#include "task.h"
#include "overworld.h"
#include "malloc.h"
#include "gba/macro.h"
#include "menu_helpers.h"
#include "menu.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "constants/rgb.h"
#include "decompress.h"
#include "constants/songs.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "pokemon_icon.h"
#include "graphics.h"
#include "data.h"
#include "pokedex.h"
#include "gpu_regs.h"

/*
 * The code in this file assumes you have read and understood everything in `sample_ui_start_here.c'. The comments here
 * will only cover new/changed code.
 */

/*
 * Define dex regions here. We'll use an enum since that way we can use them to index into various arrays.
 * Recall that enums automatically assign starting from 0, so REGION_KANTO=0, REGION_JOHTO=1, etc.
 */
// enum Region A copy now exists in an include/constants/regions.h
// {
//     REGION_KANTO,
//     REGION_JOHTO,
//     REGION_HOENN,
//     REGION_SINNOH,
//     REGION_UNOVA,
//     REGION_KALOS,
//     REGION_NONE = 0xFF
// };

struct RotomPhone_LargeStartMenuState
{
    MainCallback savedCallback;
    u8 loadState;
    u8 mode;

    // Save the current dex region
    enum Region region;
    // This will store not the current dex region, but which region button is "hovering" in the panel
    enum Region selectedRegion;

    // Store the Y coordinate for the panel (for scrolling), and a flag if the panel is open or not
    u8 panelY;
    bool8 panelIsOpen;

    /*
     * Save the sprite IDs for each button so we can manipulate their positions and change their attributes through the
     * global sprite table. The array is indexed by the region defines below, so for e.g. Kanto is index 0, Sinnoh is
     * index 3, etc.
     */
    u8 regionButtonSpriteIds[REGIONS_COUNT];
};

enum WindowIds
{
    WIN_UI_HINTS,
    WIN_MON_INFO
};

static EWRAM_DATA struct RotomPhone_LargeStartMenuState *sRotomPhone_StartMenu = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
// We'll have an additional tilemap buffer for the sliding panel, which will live on BG2
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;

#define MON_ICON_X     39
#define MON_ICON_Y     36
// Define the dex number start/end of each region for easy reference
static const u16 sDexRanges[REGIONS_COUNT][2] = {
    // Kanto goes from Bulbasaur to Mew
    [REGION_KANTO]   = {NATIONAL_DEX_BULBASAUR, NATIONAL_DEX_MEW},
    // Johto goes from Chikorita to Celebi
    [REGION_JOHTO]   = {NATIONAL_DEX_CHIKORITA, NATIONAL_DEX_CELEBI},
    // Etc.
    [REGION_HOENN]   = {NATIONAL_DEX_TREECKO, NATIONAL_DEX_DEOXYS},
    [REGION_SINNOH]  = {NATIONAL_DEX_TURTWIG, NATIONAL_DEX_ARCEUS},
    [REGION_UNOVA]   = {NATIONAL_DEX_VICTINI, NATIONAL_DEX_GENESECT},
    [REGION_KALOS]   = {NATIONAL_DEX_CHESPIN, NATIONAL_DEX_VOLCANION}
};
static const u8 sRegionNameKanto[] =  _("Kanto");
static const u8 sRegionNameJohto[] =  _("Johto");
static const u8 sRegionNameHoenn[] =  _("Hoenn");
static const u8 sRegionNameSinnoh[] = _("Sinnoh");
static const u8 sRegionNameUnova[] =  _("Unova");
static const u8 sRegionNameKalos[] =  _("Kalos");
static const u8 *const sRegionNames[REGIONS_COUNT] = {
    [REGION_KANTO]   = sRegionNameKanto,
    [REGION_JOHTO]   = sRegionNameJohto,
    [REGION_HOENN]   = sRegionNameHoenn,
    [REGION_SINNOH]  = sRegionNameSinnoh,
    [REGION_UNOVA]   = sRegionNameUnova,
    [REGION_KALOS]   = sRegionNameKalos
};
/*
 * Define some colors for the main bg, we will use the palette loading function to hotswap them when the user changes
 * regions. These colors are encoded using BGR555 encoding. If you'd like to change these colors, you may find this
 * online BGR555 color picker helpful. Make sure to use Big Endian mode:
 * https://orangeglo.github.io/BGR555/
 */
static const u16 sRegionBgColors[] = {
    [REGION_KANTO]   = 0x6a93,
    [REGION_JOHTO]   = 0x527a,
    [REGION_HOENN]   = 0x4f55,
    [REGION_SINNOH]  = 0x4b7c,
    [REGION_UNOVA]   = 0x5ef7,
    [REGION_KALOS]   = 0x76fb
};
/*
 * We'll use this struct to define the button relationships. I.e. which button is above, to the left of, etc. Putting
 * these into an array like below will make it much easier to handle user input later.
 */
struct RegionSelection
{
    u8 upRegion;
    u8 downRegion;
    u8 leftRegion;
    u8 rightRegion;
};
// The actual button relationships
static const struct RegionSelection sRegionSelections[] =
{
    /*
     * E.g. the Kanto button will be in the top left, so there is no region above it, Sinnoh will be below it, there is
     * no region to the left of it, and Johto will be right of it.
     */
    [REGION_KANTO] = {
        .upRegion    = REGION_NONE,
        .downRegion  = REGION_SINNOH,
        .leftRegion  = REGION_NONE,
        .rightRegion = REGION_JOHTO
    },
    [REGION_JOHTO] = {
        .upRegion    = REGION_NONE,
        .downRegion  = REGION_UNOVA,
        .leftRegion  = REGION_KANTO,
        .rightRegion = REGION_HOENN
    },
    [REGION_HOENN] = {
        .upRegion    = REGION_NONE,
        .downRegion  = REGION_KALOS,
        .leftRegion  = REGION_JOHTO,
        .rightRegion = REGION_NONE
    },
    [REGION_SINNOH] = {
        .upRegion    = REGION_KANTO,
        .downRegion  = REGION_NONE,
        .leftRegion  = REGION_NONE,
        .rightRegion = REGION_UNOVA
    },
    [REGION_UNOVA] = {
        .upRegion    = REGION_JOHTO,
        .downRegion  = REGION_NONE,
        .leftRegion  = REGION_SINNOH,
        .rightRegion = REGION_KALOS
    },
    [REGION_KALOS] = {
        .upRegion    = REGION_HOENN,
        .downRegion  = REGION_NONE,
        .leftRegion  = REGION_UNOVA,
        .rightRegion = REGION_NONE
    }
};

#define MODE_INFO   0
#define MODE_STATS  1
#define MODE_OTHER  2
static const u8 sModeNameInfo[] =  _("Info");
static const u8 sModeNameStats[] =  _("Stats");
static const u8 sModeNameOther[] =  _("Other");
static const u8 *const sModeNames[3] = {
    [MODE_INFO]   = sModeNameInfo,
    [MODE_STATS]  = sModeNameStats,
    [MODE_OTHER]  = sModeNameOther
};

static const struct BgTemplate sRotomPhone_LargeStartMenuBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .priority = 1
    },
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .priority = 2
    },
    {
        /*
         * We'll add another BG for our sliding button panel. For this BG we'll be using the same tileset so we set the
         * charblock to 3, just like BG1. Priority is 0 so the sliding panel draws on top of everything else, including
         * the text in the dex info window.
         */
        .bg = 2,
        .charBaseIndex = 3,
        .mapBaseIndex = 29,
        .priority = 0
    }
};

static const struct WindowTemplate sRotomPhone_LargeStartMenuWindowTemplates[] =
{
    [WIN_UI_HINTS] =
    {
        .bg = 0,
        .tilemapLeft = 14,
        .tilemapTop = 0,
        .width = 16,
        .height = 7,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_MON_INFO] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 9,
        .width = 26,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 1 + (16 * 7)
    },
    DUMMY_WIN_TEMPLATE
};

static const u32 sRotomPhone_LargeStartMenuTiles[] = INCBIN_U32("graphics/sample_ui/tiles.4bpp.lz");

// New graphics for the buttons. Create these from 4bpp indexed PNGs, just like before.
static const u32 sRotomPhone_LargeStartMenuKantoButton[] = INCBIN_U32("graphics/sample_ui/kanto.4bpp");
static const u32 sRotomPhone_LargeStartMenuJohtoButton[] = INCBIN_U32("graphics/sample_ui/johto.4bpp");
static const u32 sRotomPhone_LargeStartMenuHoennButton[] = INCBIN_U32("graphics/sample_ui/hoenn.4bpp");
static const u32 sRotomPhone_LargeStartMenuSinnohButton[] = INCBIN_U32("graphics/sample_ui/sinnoh.4bpp");
static const u32 sRotomPhone_LargeStartMenuUnovaButton[] = INCBIN_U32("graphics/sample_ui/unova.4bpp");
static const u32 sRotomPhone_LargeStartMenuKalosButton[] = INCBIN_U32("graphics/sample_ui/kalos.4bpp");

static const u32 sRotomPhone_LargeStartMenuTilemap[] = INCBIN_U32("graphics/sample_ui/tilemap.bin.lz");
static const u32 sRotomPhone_LargeStartMenuPanelTilemap[] = INCBIN_U32("graphics/sample_ui/panel_tilemap.bin.lz");

static const u16 sRotomPhone_LargeStartMenuPalette[] = INCBIN_U16("graphics/sample_ui/00.gbapal");
static const u16 sRotomPhone_LargeStartMenu_KantoJohtoHoennPalette[] = INCBIN_U16("graphics/sample_ui/kanto_johto_hoenn.gbapal");
static const u16 sRotomPhone_LargeStartMenu_SinnohUnovaKalosPalette[] = INCBIN_U16("graphics/sample_ui/sinnoh_unova_kalos.gbapal");

/*
 * Define some palette tags for our buttons. We have two different button palettes, each being used for 3 of the region
 * buttons. The palette tagging system helps the game keep track of which sprite palette slots are in use. Think of it
 * almost like a reference counter. The game uses the SpritePalette structure to associate a tag with a given palette.
 * Then, using the special `LoadSpritePalette' function, you can load the palette into VRAM and the game will only
 * perform the load if that palette tag is not already `active'. We'll see how to connect your sprites up with your
 * SpritePalettes a little further down.
 */
/*
 * Also note: these tags MUST be unique within the given context. In the case of our sample UI here, there are only two
 * palette tags so I just picked some numbers at random. If you are modifying one of the existing game screens/contexts,
 * you'll need to make sure you don't accidentally duplicate a palette tag value, else the game will start to mix up
 * the various sprite palettes and your sprites will look incorrect.
 */
#define PALETTE_TAG_KANTO_JOHTO_HOENN 0x1000
#define PALETTE_TAG_SINNOH_UNOVA_KALOS 0x1001
static const struct SpritePalette sKantoJohtoHoennButtonsSpritePalette =
{
    .data = sRotomPhone_LargeStartMenu_KantoJohtoHoennPalette,
    .tag = PALETTE_TAG_KANTO_JOHTO_HOENN
};
static  const struct SpritePalette sSinnohUnovaKalosButtonsSpritePalette =
{
    .data = sRotomPhone_LargeStartMenu_SinnohUnovaKalosPalette,
    .tag = PALETTE_TAG_SINNOH_UNOVA_KALOS
};

// Indices for the anim arrays
#define DEFAULT_ANIM  0
#define SELECTED_ANIM 0

/*
 * The AnimCmd structure is a quick way to define how you want your sprite to be animated. It uses a clever macro/union
 * system to allow for maximum flexibility. If you are unsure of how unions work, check here:
 *      https://beej.us/guide/bgc/html/split/structs-ii-more-fun-with-structs.html#unions
 * The long-short of it is that the below AnimCmd array will instruct the sprite system to use the sprite's 0th frame
 * for a duration of 30 before ending the animation (I am not exactly sure if this means 30 game-frames, but the upshot
 * is you can change this value to make a sprite frame last longer or shorter). In our case, the buttons are static so
 * this animation doesn't actually do anything. You can change the duration value to no effect.
 *
 * Below, we'll see what exactly is meant by `sprite's 0th frame'.
 */
static const union AnimCmd sButtonDefaultAnim[] =
{
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_END
};
/*
 * Sprites can have multiple animations. So we'll store an array of AnimCmd arrays. In our case, we only need one button
 * anim so this array has one element. However, you could store other AnimCmd arrays here and change which sprite anim
 * plays based on game events.
 */
static const union AnimCmd *const sButtonAnims[] =
{
    [DEFAULT_ANIM] = sButtonDefaultAnim
};
/*
 * Sprites can also have affine anims, which allow for some cool effects like scaling and rotation. We'll be using this
 * affine anim when a button is currently selected so that it slowly grows and shrinks. This AffineAnimCmd array is
 * saying to scale up by a small amount (2) for a duration of 15, then scale back down by a small amount for a duration
 * of 15, then jump to the beginning and start again.
 */
static const union AffineAnimCmd sButtonSelectedAnim[] =
{
    AFFINEANIMCMD_FRAME(2, 2, 0, 15),
    AFFINEANIMCMD_FRAME(-2, -2, 0, 15),
    AFFINEANIMCMD_JUMP(0)
};
static const union AffineAnimCmd * const sButtonAffineAnims[] =
{
    [SELECTED_ANIM] = sButtonSelectedAnim
};
/*
 * We'll be storing our sprite frames using the pic table format. The game provides a few different ways to store our
 * sprites. One way is using a spritesheet and a tile tag. We won't be covering that in this tutorial. Instead, we'll
 * use the SpriteFrameImage structure to bring in the graphics of our buttons. In this format, each sprite frame is
 * expected as a separate graphics file, and you lay them out in an array (pic table) like below. Now the `frame' value
 * in our AnimCmds should make sense. The `sprite's 0th frame' refers to the 0th index in this array. If you wanted a
 * multi-framed sprite, you'd need to add each frame graphic to this table, and then set up your AnimCmd to reference
 * which frames you want and in which order.
 */
static const struct SpriteFrameImage sKantoButtonPicTable[] =
{
    /*
     * `obj_frame_tiles' is a macro that auto-magically creates a SpriteFrameImage structure with the data set to our
     * graphics data defined in the `sRotomPhone_LargeStartMenuKantoButton' array and the size properly set to the size of this array.
     */
    obj_frame_tiles(sRotomPhone_LargeStartMenuKantoButton)
};

/*
 * Define some template OAM (Object Attribute Memory) data for our sprite button. The `0' values here aren't actually
 * 0s, they just mean those pieces of data are either unused or are going to be filled in by other sprite code. For
 * example, `paletteNum = 0' does not actually mean use palette 0. Instead, we'll be using the palette defined by our
 * palette tag, and the `gflib/sprite.c' code will fill the correct palette number into the OAM data for us.
 */
static const struct OamData sKantoButtonOam =
{
    // y=0 here because we will set y-coord later when we actually create the sprite
    .y = 0,
    // Default to affine off. We will turn the affine anim on when the button is selected using a special function.
    .affineMode = ST_OAM_AFFINE_OFF,
    /*
     * We are a normal sprite object. There are also window and blend sprite objects, but those are outside the scope
     * of this tutorial.
     */
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    // This sprite is 4bpp, very rarely does the game use an 8bpp sprite
    .bpp = ST_OAM_4BPP,
    /*
     * Our button sprites use the 2:1 length:height sprite shape, so specify that here. Typically, you'll just want your
     * sprite shape to match your sprite size. So if you are making a 64x32 pixel sprite, specify that as your sprite
     * shape.
     */
    .shape = SPRITE_SHAPE(64x32),
    // x=0 here because we will set x-coord later when we actually create the sprite
    .x = 0,
    /*
     * The matrix number has to do with the sprite's affine transformation matrix. We set it to 0 because the gflib
     * sprite code will handle it for us. For more on affine matrices:
     *      https://www.coranac.com/tonc/text/affine.htm
     */
    .matrixNum = 0,
    // Our button sprites are 64x32 pixels
    .size = SPRITE_SIZE(64x32),
    /*
     * We don't need to set this since we are using a pic table, very rarely would you ever manually change this value,
     * typically you'd let the sprite library code update this for you
     */
    .tileNum = 0,
    // Set priority to 0 so our button sprite draws on top of everything
    .priority = 0,
    // We don't set this since we are going to use a SpritePalette and a palette tag to handle palette tracking
    .paletteNum = 0,
    // Used for the affine matrix, no need to set it here
    .affineParam = 0,
    /*
     * If you want more details on how the .shape and .size parameters affect the ultimate on-screen appearance of your
     * sprite, check out the Object Attribute section of the TONC sprite tutorial, specifically table 8.4:
     *      https://www.coranac.com/tonc/text/regobj.htm#sec-oam-entry
     */
};

/*
 * The SpriteTemplate is a structure provided by `gflib/sprite.h' that allows us to easily group and define some basic
 * sprite properties that we can instantiate into actual sprites later. Here, we define a template for our Kanto button.
 */
static const struct SpriteTemplate sKantoButtonSpriteTemplate =
{
    /*
     * We'll set the tile tag to TAG_NONE since we will be using a pic table for this sprite. If you were using a
     * spritesheet, you would want to set the tag here to match the tag you gave the SpriteSheet structure.
     */
    .tileTag = TAG_NONE,
    // Here we will use the palette tag that matches the SpritePalette for the Kanto button.
    .paletteTag = PALETTE_TAG_KANTO_JOHTO_HOENN,
    // Set OAM to our define OAM structure
    .oam = &sKantoButtonOam,
    // Set anims to our defined anims
    .anims = sButtonAnims,
    // Set the pic table for this sprite. If you were using a spritesheet, then this would be NULL
    .images = sKantoButtonPicTable,
    // Set affineAnims to our defined affine anims
    .affineAnims = sButtonAffineAnims,
    /*
     * Sprite callbacks are called as part of `AnimateSprites()', which we are calling in our Main CB. You can use them
     * to perform additional movements and animation updates that aren't handled by your AnimCmds. We don't actually
     * need a special callback for our button sprites so just set the callback to the provided dummy. Check out the code
     * in `AnimateSprites()' in `gflib/sprite.c' to see how this callback is used.
     */
    .callback = SpriteCallbackDummy,
};

/*
 * Now define the necessary objects for the rest of the buttons. All this code is pretty similar to the Kanto button
 * definition code above.
 */

static const struct SpriteFrameImage sJohtoButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuJohtoButton)
};
static const struct OamData sJohtoButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sJohtoButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_KANTO_JOHTO_HOENN,
    .oam = &sJohtoButtonOam,
    .anims = sButtonAnims,
    .images = sJohtoButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteFrameImage sHoennButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuHoennButton)
};
static const struct OamData sHoennButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sHoennButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_KANTO_JOHTO_HOENN,
    .oam = &sHoennButtonOam,
    .anims = sButtonAnims,
    .images = sHoennButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteFrameImage sSinnohButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuSinnohButton)
};
static const struct OamData sSinnohButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sSinnohButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_SINNOH_UNOVA_KALOS,
    .oam = &sSinnohButtonOam,
    .anims = sButtonAnims,
    .images = sSinnohButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteFrameImage sUnovaButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuUnovaButton)
};
static const struct OamData sUnovaButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sUnovaButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_SINNOH_UNOVA_KALOS,
    .oam = &sUnovaButtonOam,
    .anims = sButtonAnims,
    .images = sUnovaButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteFrameImage sKalosButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuKalosButton)
};
static const struct OamData sKalosButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sKalosButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_SINNOH_UNOVA_KALOS,
    .oam = &sKalosButtonOam,
    .anims = sButtonAnims,
    .images = sKalosButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

enum FontColor
{
    FONT_BLACK,
    FONT_WHITE,
    FONT_RED,
    FONT_BLUE,
};
static const u8 sRotomPhone_LargeStartMenuWindowFontColors[][3] =
{
    [FONT_BLACK]  = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY},
    [FONT_WHITE]  = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_DARK_GRAY},
    [FONT_RED]    = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_GRAY},
    [FONT_BLUE]   = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_GRAY},
};

// Callbacks for the sample UI
static void RotomPhone_LargeStartMenu_SetupCB(void);
static void RotomPhone_LargeStartMenu_MainCB(void);
static void RotomPhone_LargeStartMenu_VBlankCB(void);

// Sample UI tasks
static void Task_RotomPhone_LargeStartMenu_WaitFadeIn(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_MainInput(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_PanelInput(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_PanelSlide(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_WaitFadeAndBail(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_WaitFadeAndExitGracefully(u8 taskId);

// Sample UI helper functions
static void RotomPhone_LargeStartMenu_Init(MainCallback callback);
static void RotomPhone_LargeStartMenu_ResetGpuRegsAndBgs(void);
static bool8 RotomPhone_LargeStartMenu_InitBgs(void);
static void RotomPhone_LargeStartMenu_FadeAndBail(void);
static bool8 RotomPhone_LargeStartMenu_LoadGraphics(void);
static void RotomPhone_LargeStartMenu_InitWindows(void);
static void RotomPhone_LargeStartMenu_PrintUiButtonHints(void);
static void RotomPhone_LargeStartMenu_CreateRegionButtons(void);
static void RotomPhone_LargeStartMenu_StartRegionButtonAnim(enum Region region);
static void RotomPhone_LargeStartMenu_StopRegionButtonAnim(enum Region region);
static void RotomPhone_LargeStartMenu_FreeResources(void);


void Task_OpenRotomPhone_LargeStartMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        RotomPhone_LargeStartMenu_Init(CB2_ReturnToFieldWithOpenMenu);
        DestroyTask(taskId);
    }
}

static void RotomPhone_LargeStartMenu_Init(MainCallback callback)
{
    sRotomPhone_StartMenu = AllocZeroed(sizeof(struct RotomPhone_LargeStartMenuState));
    if (sRotomPhone_StartMenu == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sRotomPhone_StartMenu->loadState = 0;
    sRotomPhone_StartMenu->savedCallback = callback;

    SetMainCallback2(RotomPhone_LargeStartMenu_SetupCB);
}

// Credit: Jaizu, pret
static void RotomPhone_LargeStartMenu_ResetGpuRegsAndBgs(void)
{
    SampleUI_ResetGpuRegsAndBgs();
}

static void RotomPhone_LargeStartMenu_SetupCB(void)
{
    switch (gMain.state)
    {
    case 0:
        RotomPhone_LargeStartMenu_ResetGpuRegsAndBgs();
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        gMain.state++;
        break;
    case 2:
        if (RotomPhone_LargeStartMenu_InitBgs())
        {
            sRotomPhone_StartMenu->loadState = 0;
            gMain.state++;
        }
        else
        {
            RotomPhone_LargeStartMenu_FadeAndBail();
            return;
        }
        break;
    case 3:
        if (RotomPhone_LargeStartMenu_LoadGraphics() == TRUE)
        {
            gMain.state++;
        }
        break;
    case 4:
        RotomPhone_LargeStartMenu_InitWindows();
        gMain.state++;
        break;
    case 5:
        sRotomPhone_StartMenu->region = REGION_KANTO;
        sRotomPhone_StartMenu->selectedRegion = REGION_KANTO;

        RotomPhone_LargeStartMenu_PrintUiButtonHints();

        sRotomPhone_StartMenu->panelY = 0;
        sRotomPhone_StartMenu->panelIsOpen = FALSE;

        // Create sprites for the region buttons off screen
        RotomPhone_LargeStartMenu_CreateRegionButtons();
        // Start button select animation for the current region, i.e. Kanto
        RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_StartMenu->selectedRegion);

        CreateTask(Task_RotomPhone_LargeStartMenu_WaitFadeIn, 0);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    case 7:
        SetVBlankCallback(RotomPhone_LargeStartMenu_VBlankCB);
        SetMainCallback2(RotomPhone_LargeStartMenu_MainCB);
        break;
    }
}

static void RotomPhone_LargeStartMenu_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void RotomPhone_LargeStartMenu_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void Task_RotomPhone_LargeStartMenu_WaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_MainInput;
    }
}

static void Task_RotomPhone_LargeStartMenu_MainInput(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_PC_OFF);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_WaitFadeAndExitGracefully;
    }
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (sRotomPhone_StartMenu->mode == MODE_OTHER)
        {
            sRotomPhone_StartMenu->mode = MODE_INFO;
        }
        else
        {
            sRotomPhone_StartMenu->mode++;
        }
        RotomPhone_LargeStartMenu_PrintUiButtonHints();
    }
    if (JOY_NEW(START_BUTTON))
    {
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelSlide;
        PlaySE(SE_SELECT);
    }
}

static void Task_RotomPhone_LargeStartMenu_PanelInput(u8 taskId)
{
    // Exit panel when Start or B is pressed
    if (JOY_NEW(START_BUTTON | B_BUTTON))
    {
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelSlide;
        PlaySE(SE_SELECT);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        sRotomPhone_StartMenu->region = sRotomPhone_StartMenu->selectedRegion;
        // Sneakily swap out color 2 in BG1's palette for the new region-specific color
        LoadPalette(&sRegionBgColors[sRotomPhone_StartMenu->region], BG_PLTT_ID(0) + 2, 2);
        RotomPhone_LargeStartMenu_PrintUiButtonHints();
        PlaySE(SE_SELECT);
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelSlide;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sRegionSelections[sRotomPhone_StartMenu->selectedRegion].upRegion != REGION_NONE)
        {
            RotomPhone_LargeStartMenu_StopRegionButtonAnim(sRotomPhone_StartMenu->selectedRegion);
            sRotomPhone_StartMenu->selectedRegion = sRegionSelections[sRotomPhone_StartMenu->selectedRegion].upRegion;
            RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_StartMenu->selectedRegion);
            PlaySE(SE_SELECT);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sRegionSelections[sRotomPhone_StartMenu->selectedRegion].downRegion != REGION_NONE)
        {
            RotomPhone_LargeStartMenu_StopRegionButtonAnim(sRotomPhone_StartMenu->selectedRegion);
            sRotomPhone_StartMenu->selectedRegion = sRegionSelections[sRotomPhone_StartMenu->selectedRegion].downRegion;
            RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_StartMenu->selectedRegion);
            PlaySE(SE_SELECT);
        }
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (sRegionSelections[sRotomPhone_StartMenu->selectedRegion].leftRegion != REGION_NONE)
        {
            RotomPhone_LargeStartMenu_StopRegionButtonAnim(sRotomPhone_StartMenu->selectedRegion);
            sRotomPhone_StartMenu->selectedRegion = sRegionSelections[sRotomPhone_StartMenu->selectedRegion].leftRegion;
            RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_StartMenu->selectedRegion);
            PlaySE(SE_SELECT);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if (sRegionSelections[sRotomPhone_StartMenu->selectedRegion].rightRegion != REGION_NONE)
        {
            RotomPhone_LargeStartMenu_StopRegionButtonAnim(sRotomPhone_StartMenu->selectedRegion);
            sRotomPhone_StartMenu->selectedRegion = sRegionSelections[sRotomPhone_StartMenu->selectedRegion].rightRegion;
            RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_StartMenu->selectedRegion);
            PlaySE(SE_SELECT);
        }
    }
}

static void Task_RotomPhone_LargeStartMenu_PanelSlide(u8 taskId)
{
    #define PANEL_MAX_Y 95
    /*
     * Register BG2VOFS controls the vertical offset of background 2. Our sliding panel lives on BG2, so by setting the
     * value of this register we can change the starting Y position of the background. We increase it a bit each frame
     * to make the BG look like it is sliding into view.
     */
    SetGpuReg(REG_OFFSET_BG2VOFS, sRotomPhone_StartMenu->panelY);

    // Panel is open, so slide it down out of view
    if (sRotomPhone_StartMenu->panelIsOpen)
    {
        if (sRotomPhone_StartMenu->panelY > 0)
        {
            sRotomPhone_StartMenu->panelY -= 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_KANTO]].y += 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_JOHTO]].y += 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_HOENN]].y += 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_SINNOH]].y += 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_UNOVA]].y += 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_KALOS]].y += 5;
        }
        else if (sRotomPhone_StartMenu->panelY == 0)
        {
            // Panel is done closing, so set state to closed and change task to read main input
            sRotomPhone_StartMenu->panelIsOpen = FALSE;
            gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_MainInput;
        }
    }
    // Panel is closed, so slide it up into view
    else
    {
        if (sRotomPhone_StartMenu->panelY < PANEL_MAX_Y)
        {
            sRotomPhone_StartMenu->panelY += 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_KANTO]].y -= 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_JOHTO]].y -= 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_HOENN]].y -= 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_SINNOH]].y -= 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_UNOVA]].y -= 5;
            gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_KALOS]].y -= 5;
        }
        else if (sRotomPhone_StartMenu->panelY == PANEL_MAX_Y)
        {
            // Panel is done opening, so set state to open and change task to read panel input
            sRotomPhone_StartMenu->panelIsOpen = TRUE;
            gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelInput;
        }
    }
    #undef PANEL_MAX_Y
}

static void Task_RotomPhone_LargeStartMenu_WaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sRotomPhone_StartMenu->savedCallback);
        RotomPhone_LargeStartMenu_FreeResources();
        DestroyTask(taskId);
    }
}

static void Task_RotomPhone_LargeStartMenu_WaitFadeAndExitGracefully(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sRotomPhone_StartMenu->savedCallback);
        RotomPhone_LargeStartMenu_FreeResources();
        DestroyTask(taskId);
    }
}
#define TILEMAP_BUFFER_SIZE (1024 * 2)
static bool8 RotomPhone_LargeStartMenu_InitBgs(void)
{
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
    if (sBg1TilemapBuffer == NULL)
    {
        return FALSE;
    }
    sBg2TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
    if (sBg2TilemapBuffer == NULL)
    {
        return FALSE;
    }

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sRotomPhone_LargeStartMenuBgTemplates, NELEMS(sRotomPhone_LargeStartMenuBgTemplates));

    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    SetBgTilemapBuffer(2, sBg2TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);

    ShowBg(0);
    ShowBg(1);
    ShowBg(2);

    return TRUE;
}
#undef TILEMAP_BUFFER_SIZE

static void RotomPhone_LargeStartMenu_FadeAndBail(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_RotomPhone_LargeStartMenu_WaitFadeAndBail, 0);
    SetVBlankCallback(RotomPhone_LargeStartMenu_VBlankCB);
    SetMainCallback2(RotomPhone_LargeStartMenu_MainCB);
}

static bool8 RotomPhone_LargeStartMenu_LoadGraphics(void)
{
    switch (sRotomPhone_StartMenu->loadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sRotomPhone_LargeStartMenuTiles, 0, 0, 0);
        sRotomPhone_StartMenu->loadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sRotomPhone_LargeStartMenuTilemap, sBg1TilemapBuffer);
            LZDecompressWram(sRotomPhone_LargeStartMenuPanelTilemap, sBg2TilemapBuffer);
            sRotomPhone_StartMenu->loadState++;
        }
        break;
    case 2:
        LoadPalette(sRotomPhone_LargeStartMenuPalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        /*
         * We are going to dynamically change the BG color depending on the region. We set up our tiles so that the UI
         * BG color is stored in Palette 0, slot 2. So we hot swap that to our saved color for Kanto, since the UI
         * starts in Kanto region. We will need to perform this mini-swap each time the user changes regions.
         */
        LoadPalette(&sRegionBgColors[REGION_KANTO], BG_PLTT_ID(0) + 2, 2);
        LoadPalette(GetTextWindowPalette(gSaveBlock2Ptr->optionsInterfaceColor + DEFAULT_TEXT_BOX_FRAME_PALETTES), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        sRotomPhone_StartMenu->loadState++;
    default:
        sRotomPhone_StartMenu->loadState = 0;
        return TRUE;
    }
    return FALSE;
}

static void RotomPhone_LargeStartMenu_InitWindows(void)
{
    InitWindows(sRotomPhone_LargeStartMenuWindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    FillWindowPixelBuffer(WIN_UI_HINTS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_MON_INFO, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    PutWindowTilemap(WIN_UI_HINTS);
    PutWindowTilemap(WIN_MON_INFO);
    CopyWindowToVram(WIN_UI_HINTS, 3);
    CopyWindowToVram(WIN_MON_INFO, 3);
}

static const u8 sText_RotomPhone_LargeStartMenuButtonHint1[] = _("{DPAD_UPDOWN}Change POKéMON");
static const u8 sText_RotomPhone_LargeStartMenuButtonHint2[] = _("{A_BUTTON}Mode: {STR_VAR_1}");
static const u8 sText_RotomPhone_LargeStartMenuButtonHint3[] = _("{START_BUTTON}Region");
static const u8 sText_RotomPhone_LargeStartMenuButtonHint4[] = _("{B_BUTTON}Exit");
static void RotomPhone_LargeStartMenu_PrintUiButtonHints(void)
{
    FillWindowPixelBuffer(WIN_UI_HINTS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    StringCopy(gStringVar1, sModeNames[sRotomPhone_StartMenu->mode]);
    StringExpandPlaceholders(gStringVar2, sText_RotomPhone_LargeStartMenuButtonHint2);

    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_NORMAL, 0, 3, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sRegionNames[sRotomPhone_StartMenu->region]);
    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_SMALL, 47, 0, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sText_RotomPhone_LargeStartMenuButtonHint1);
    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_SMALL, 47, 10, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, gStringVar2);
    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_SMALL, 47, 20, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sText_RotomPhone_LargeStartMenuButtonHint3);
    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_SMALL, 47, 30, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sText_RotomPhone_LargeStartMenuButtonHint4);

    CopyWindowToVram(WIN_UI_HINTS, COPYWIN_GFX);
}

static void RotomPhone_LargeStartMenu_CreateRegionButtons(void)
{
    #define BUTTON_START_X 50
    #define BUTTON_START_Y 184
    /*
     * Load the palettes for our sprites into VRAM. Note here we are passing the SpritePalette palette templates we
     * defined earlier. If a palette with the tag given in one of these templates is already loaded, the
     * `LoadSpritePalette' method will skip the actual load step. Otherwise, it finds the first free palette slot and
     * loads the palette there. Note that if you have manually loaded other palettes into OBJ palette memory without
     * tagging them in the sprite system, this loading code may clobber your palette. Also, the `LoadSpritePalette'
     * method returns the palette index it ended up using. If it returns 0xFF that means the load failed. So if you want
     * you can check the result to make sure your load was successful and handle the problem if it wasn't. In our case,
     * we just assume the load succeeded.
     */
    LoadSpritePalette(&sKantoJohtoHoennButtonsSpritePalette);
    LoadSpritePalette(&sSinnohUnovaKalosButtonsSpritePalette);
    /*
     * Create a sprite for each button at the given X/Y position. We set subpriority to 0 so they draw on top. Sprite
     * subpriorities provide you a way to have multiple different "priorities" within the same layer. So for example,
     * say you want Sprite A and Sprite B to draw on top of a BG with priority 1. You can set the priority of Sprite A
     * and Sprite B to 0 for that. But now suppose you also want Sprite A to draw on top of Sprite B. For that, set
     * Sprite A's subpriority to 0 and Sprite B's subpriority to 1.
     */
    sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_KANTO] =
        CreateSprite(&sKantoButtonSpriteTemplate, BUTTON_START_X, BUTTON_START_Y, 0);
    sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_JOHTO] =
        CreateSprite(&sJohtoButtonSpriteTemplate, BUTTON_START_X + 70, BUTTON_START_Y, 0);
    sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_HOENN] =
        CreateSprite(&sHoennButtonSpriteTemplate, BUTTON_START_X + 2*70, BUTTON_START_Y, 0);
    sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_SINNOH] =
        CreateSprite(&sSinnohButtonSpriteTemplate, BUTTON_START_X, BUTTON_START_Y + 40, 0);
    sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_UNOVA] =
        CreateSprite(&sUnovaButtonSpriteTemplate, BUTTON_START_X + 70, BUTTON_START_Y + 40, 0);
    sRotomPhone_StartMenu->regionButtonSpriteIds[REGION_KALOS] =
        CreateSprite(&sKalosButtonSpriteTemplate, BUTTON_START_X + 2*70, BUTTON_START_Y + 40, 0);
    #undef BUTTON_START_X
    #undef BUTTON_START_Y
}

static void RotomPhone_LargeStartMenu_StartRegionButtonAnim(enum Region region)
{
    /*
     * To start the button animation, the first thing we want to do is use the double-size affine mode. We need this for
     * our case since we are growing the sprites. Otherwise, the sprite image itself will grow but the bounds will stay
     * the same, and the sprite will look like it's getting cut off. We'll set the sprite to double sized while the
     * affine shrink/grow is running so that there is plenty of room to draw the bigger sprite.
     *
     * For more details on this "clipping artifact", please see the relevant TONC page:
     *      https://www.coranac.com/tonc/text/affobj.htm#sec-artifact
     */
    gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[region]].oam.affineMode = ST_OAM_AFFINE_DOUBLE;

    /*
     * Starts the given affine anim on the given sprite. In this case, animate the sprite associated with the given
     * region ID, and use the only affine anim we defined. `SELECTED_ANIM' is just an index into our AffineAnimCmd table
     * we defined way earlier.
     */
    StartSpriteAffineAnim(&gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[region]], SELECTED_ANIM);

    /*
     * The `CalcCenterToCornerVec' function is used by the game to calculate a vector that points from the sprite's
     * logical center point to its top-left corner. The game then adds this vector to your provided sprite positions.
     * This creates the illusion that the coordinates you provide for a sprite are based on the center of the sprite,
     * even though the GBA hardware actually uses top-left-corner based sprite coordinates.
     *
     * This last bit is crucial since we activated double-sizing. We need to recalculate the sprite's offset vector now
     * that we have changed its size. Otherwise the sprite position will appear to change. Try commenting out this call
     * and see what happens when you select a button.
     */
    CalcCenterToCornerVec(&gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[region]],
        SPRITE_SHAPE(64x32), SPRITE_SIZE(64x32), ST_OAM_AFFINE_DOUBLE);
}

static void RotomPhone_LargeStartMenu_StopRegionButtonAnim(enum Region region)
{
    /*
     * This function works just like the above function, but does everything in reverse.
     */

    // Disable double-size mode
    gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[region]].oam.affineMode = ST_OAM_AFFINE_OFF;
    // Return our sprite to the regular static anim
    StartSpriteAnim(&gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[region]], DEFAULT_ANIM);
    // Recalculate the offset vector for a regular sized sprite
    CalcCenterToCornerVec(&gSprites[sRotomPhone_StartMenu->regionButtonSpriteIds[region]],
        SPRITE_SHAPE(64x32), SPRITE_SIZE(64x32), ST_OAM_AFFINE_OFF);
}

static void RotomPhone_LargeStartMenu_FreeResources(void)
{
    if (sRotomPhone_StartMenu != NULL)
    {
        Free(sRotomPhone_StartMenu);
    }
    if (sBg1TilemapBuffer != NULL)
    {
        Free(sBg1TilemapBuffer);
    }
    if (sBg2TilemapBuffer != NULL)
    {
        Free(sBg2TilemapBuffer);
    }
    FreeAllWindowBuffers();
    ResetSpriteData();
}
