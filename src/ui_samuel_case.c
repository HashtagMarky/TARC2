#include "global.h"
#include "gba/gba.h"
#include "ui_samuel_case.h"
#include "strings.h"
#include "bg.h"
#include "battle_main.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "list_menu.h"
#include "item_icon.h"
#include "item_use.h"
#include "international_string_util.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "party_menu.h"
#include "random.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "overworld.h"
#include "event_data.h"
#include "constants/items.h"
#include "constants/field_weather.h"
#include "constants/songs.h"
#include "constants/rgb.h"
#include "trainer_pokemon_sprites.h"
#include "field_effect.h"
#include "pokedex.h"
#include "script_pokemon_util.h"
#include "pokeball.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "naming_screen.h"
#include "tv.h"
#include "ikigai_interface.h"

 /*
    9 Starter Selection Samuel Case

    This UI was coded by Archie with Graphics by Mudskip.

    HashtagMarky: Text printing could should be redone.

 */
 
//==========DEFINES==========//
struct MenuResources
{
    MainCallback savedCallback;     // determines callback to run when we exit. e.g. where do we want to go after closing the menu
    u8 gfxLoadState;
    u16 monSpriteId;
    u16 pokeballSpriteIds[9];
    u16 handSpriteId;
    u16 handPosition;
    u16 selector_x;
    u16 selector_y;
    u16 movingSelector;

};

enum WindowIds
{
    WINDOW_BOTTOM_BAR,
};

enum TextIds
{
    CHOOSE_MON,
    CONFIRM_SELECTION,
    RECIEVED_MON,
};

enum PageNumbers
{
    PAGE_ONE,
    PAGE_TWO,
    PAGE_COUNT,
};

enum Colors
{
    FONT_BLACK,
    FONT_WHITE,
    FONT_RED,
    FONT_BLUE,
};
static const u8 sMenuWindowFontColors[][3] = 
{
    [FONT_BLACK]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY},
    [FONT_WHITE]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_WHITE,  TEXT_COLOR_DARK_GRAY},
    [FONT_RED]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_GRAY},
    [FONT_BLUE]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_GRAY},
};


struct SpriteCordsStruct {
    u8 x;
    u8 y;
};

enum BallPositions
{
    BALL_TOP_FIRST,
    BALL_TOP_SECOND,
    BALL_TOP_THIRD,
    BALL_TOP_FOURTH,
    BALL_MIDDLE_FIRST,
    BALL_MIDDLE_SECOND,
    BALL_MIDDLE_THIRD,
    BALL_BOTTOM_FIRST,
    BALL_BOTTOM_SECOND,
};

struct MonChoiceData{ // This is the format used to define a mon, everything left out will default to 0 and be blank or use the in game defaults
    u16 species; // Mon Species ID
    u8 level;   // Mon Level 5
    u16 item;   // Held item, just ITEM_POTION
    u8 ball; // this ballid does not change the design of the ball in the case, only in summary/throwing out to battle 
    u8 nature; // NATURE_JOLLY, NATURE_ETC...
    u8 abilityNum; // this is either 0/1 in vanilla or 0/1/2 in Expansion, its the ability num your mon uses from its possible abilities, not the ability constant itself
    u8 gender; // MON_MALE, MON_FEMALE, MON_GENDERLESS
    u8 evs[6]; // use format {255, 255, 0, 0, 0, 0}
    u8 ivs[6]; // use format {31, 31, 31, 31, 31, 31}
    u16 moves[4]; // use format {MOVE_FIRE_BLAST, MOVE_SHEER_COLD, MOVE_NONE, MOVE_NONE}
    bool8 ggMaxFactor;      // only work in Expansion set to 0 otherwise or leave blank
    u8 teraType;            // only work in Expansion set to 0 otherwise or leave blank
    bool8 isShinyExpansion; // only work in Expansion set to 0 otherwise or leave blank
};

//
//  Making Changes Here Changes The Options In The UI. This is where you define your mons
//

const u16 gIkigaiStarters[NUMBER_OF_MON_TYPES][2] =
{
    [TYPE_NONE]       = {SPECIES_NONE,      SPECIES_NONE},      // No assigned starter
    [TYPE_NORMAL]     = {SPECIES_LILLIPUP,  SPECIES_LILLIPUP},
    [TYPE_FIGHTING]   = {SPECIES_MACHOP,    SPECIES_MACHOP},
    [TYPE_FLYING]     = {SPECIES_ROOKIDEE,  SPECIES_ROOKIDEE},
    [TYPE_POISON]     = {SPECIES_NIDORAN_M, SPECIES_NIDORAN_F},
    [TYPE_GROUND]     = {SPECIES_SANDILE,   SPECIES_SANDILE},
    [TYPE_ROCK]       = {SPECIES_ROLYCOLY,  SPECIES_ROLYCOLY},
    [TYPE_BUG]        = {SPECIES_GRUBBIN,   SPECIES_GRUBBIN},
    [TYPE_GHOST]      = {SPECIES_DUSKULL,   SPECIES_DUSKULL},
    [TYPE_STEEL]      = {SPECIES_KLINK,     SPECIES_KLINK},
    [TYPE_MYSTERY]    = {SPECIES_NONE,      SPECIES_NONE},      // No assigned starter
    [TYPE_FIRE]       = {SPECIES_MAGBY,     SPECIES_MAGBY},
    [TYPE_WATER]      = {SPECIES_TYMPOLE,   SPECIES_TYMPOLE},
    [TYPE_GRASS]      = {SPECIES_SEEDOT,    SPECIES_SEEDOT},
    [TYPE_ELECTRIC]   = {SPECIES_SHINX,     SPECIES_SHINX},
    [TYPE_PSYCHIC]    = {SPECIES_HATENNA,   SPECIES_HATENNA},
    [TYPE_ICE]        = {SPECIES_SWINUB,    SPECIES_SWINUB},
    [TYPE_DRAGON]     = {SPECIES_BAGON,     SPECIES_BAGON},
    [TYPE_DARK]       = {SPECIES_IMPIDIMP,  SPECIES_IMPIDIMP},
    [TYPE_FAIRY]      = {SPECIES_FLABEBE,   SPECIES_FLABEBE},
    [TYPE_STELLAR]    = {SPECIES_NONE,      SPECIES_NONE},      // No assigned starter
};

static const struct MonChoiceData sStarterChoices_Page1_Male[9] = 
{
    [BALL_TOP_FIRST]        = {gIkigaiStarters[TYPE_NORMAL][MALE],      5},
    [BALL_TOP_SECOND]       = {gIkigaiStarters[TYPE_FIGHTING][MALE],    5},
    [BALL_MIDDLE_FIRST]     = {gIkigaiStarters[TYPE_FLYING][MALE],      5},

    [BALL_TOP_THIRD]        = {gIkigaiStarters[TYPE_POISON][MALE],      5},
    [BALL_TOP_FOURTH]       = {gIkigaiStarters[TYPE_GROUND][MALE],      5},
    [BALL_MIDDLE_THIRD]     = {gIkigaiStarters[TYPE_ROCK][MALE],        5},

    [BALL_MIDDLE_SECOND]    = {gIkigaiStarters[TYPE_BUG][MALE],         5},
    [BALL_BOTTOM_FIRST]     = {gIkigaiStarters[TYPE_GHOST][MALE],       5},
    [BALL_BOTTOM_SECOND]    = {gIkigaiStarters[TYPE_STEEL][MALE],       5},
};

static const struct MonChoiceData sStarterChoices_Page1_Female[9] = 
{
    [BALL_TOP_FIRST]        = {gIkigaiStarters[TYPE_NORMAL][FEMALE],    5},
    [BALL_TOP_SECOND]       = {gIkigaiStarters[TYPE_FIGHTING][FEMALE],  5},
    [BALL_MIDDLE_FIRST]     = {gIkigaiStarters[TYPE_FLYING][FEMALE],    5},

    [BALL_TOP_THIRD]        = {gIkigaiStarters[TYPE_POISON][FEMALE],    5},
    [BALL_TOP_FOURTH]       = {gIkigaiStarters[TYPE_GROUND][FEMALE],    5},
    [BALL_MIDDLE_THIRD]     = {gIkigaiStarters[TYPE_ROCK][FEMALE],      5},

    [BALL_MIDDLE_SECOND]    = {gIkigaiStarters[TYPE_BUG][FEMALE],       5},
    [BALL_BOTTOM_FIRST]     = {gIkigaiStarters[TYPE_GHOST][FEMALE],     5},
    [BALL_BOTTOM_SECOND]    = {gIkigaiStarters[TYPE_STEEL][FEMALE],     5},
};

static const struct MonChoiceData sStarterChoices_Page2[9] = 
{
    [BALL_TOP_FIRST]        = {gIkigaiStarters[TYPE_FIRE][MALE],        5},
    [BALL_TOP_SECOND]       = {gIkigaiStarters[TYPE_WATER][MALE],       5},
    [BALL_MIDDLE_FIRST]     = {gIkigaiStarters[TYPE_GRASS][MALE],       5},

    [BALL_TOP_THIRD]        = {gIkigaiStarters[TYPE_ELECTRIC][MALE],    5},
    [BALL_TOP_FOURTH]       = {gIkigaiStarters[TYPE_PSYCHIC][MALE],     5},
    [BALL_MIDDLE_THIRD]     = {gIkigaiStarters[TYPE_ICE][MALE],         5},

    [BALL_MIDDLE_SECOND]    = {gIkigaiStarters[TYPE_DRAGON][MALE],      5},
    [BALL_BOTTOM_FIRST]     = {gIkigaiStarters[TYPE_DARK][MALE],        5},
    [BALL_BOTTOM_SECOND]    = {gIkigaiStarters[TYPE_FAIRY][MALE],       5},
};

//==========EWRAM==========//
static EWRAM_DATA struct MenuResources *sSamuelCaseDataPtr = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;
static u8 sCasePageNum;
static u8 sMonShiny = FALSE;

//==========STATIC=DEFINES==========//
static void SamuelCaseRunSetup(void);
static bool8 SamuelCaseDoGfxSetup(void);
static bool8 SamuelCase_InitBgs(void);
static void SamuelCaseFadeAndBail(void);
static bool8 SamuelCaseLoadGraphics(void);
static void SamuelCase_InitWindows(void);
static void PrintTextToBottomBar(u8 textId);
static void Task_SamuelCaseWaitFadeIn(u8 taskId);
static void Task_SamuelCaseMain(u8 taskId);
static void SampleUi_DrawMonIcon(u16 speciesId);
static void Task_DelayedSpriteLoad(u8 taskId);
static const struct MonChoiceData* ReturnStartersByPage(void);
static const struct SpriteCordsStruct (*ReturnBallPositionByPage(void))[4];
static void SamuelCaseChangeGraphics(void);
static void Task_WaitFadeOutAndChangeGraphics(u8 taskId);

//==========CONST=DATA==========//
static const struct BgTemplate sMenuBgTemplates[] =
{
    {
        .bg = 0,    // windows, etc
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .priority = 0
    }, 
    {
        .bg = 1,    // this bg loads the UI tilemap
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .priority = 2
    },
    {
        .bg = 2,    // this bg loads the UI tilemap
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .priority = 1
    }
};

static const struct WindowTemplate sMenuWindowTemplates[] = 
{
    [WINDOW_BOTTOM_BAR] = 
    {
        .bg = 0,            // which bg to print text on
        .tilemapLeft = 0,   // position from left (per 8 pixels)
        .tilemapTop = 14,    // position from top (per 8 pixels)
        .width = 30,        // width (per 8 pixels)
        .height = 6,        // height (per 8 pixels)
        .paletteNum = 15,   // palette index to use for text
        .baseBlock = 1,     // tile start in VRAM
    },
    DUMMY_WIN_TEMPLATE
};


//
//  Graphics Pointers to Tilemaps, Tilesets, Spritesheets, Palettes
//
static const u32 sCaseTiles[]   = INCBIN_U32("graphics/ui_samuel_case/case_tiles.4bpp.lz");
static const u32 sCaseTilemapLeft[] = INCBIN_U32("graphics/ui_samuel_case/case_tiles_left.bin.lz");
static const u32 sCaseTilemapRight[] = INCBIN_U32("graphics/ui_samuel_case/case_tiles_right.bin.lz");
static const u16 sCasePalette[] = INCBIN_U16("graphics/ui_samuel_case/case_tiles.gbapal");

static const u32 sTextBgTiles[]   = INCBIN_U32("graphics/ui_samuel_case/text_bg_tiles.4bpp.lz");
static const u32 sTextBgTilemapLeft[] = INCBIN_U32("graphics/ui_samuel_case/text_bg_tiles_left.bin.lz");
static const u32 sTextBgTilemapRight[] = INCBIN_U32("graphics/ui_samuel_case/text_bg_tiles_right.bin.lz");

static const u32 sPokeballHand_Gfx[] = INCBIN_U32("graphics/ui_samuel_case/pokeball_hand.4bpp.lz");
static const u16 sPokeballHand_Pal[] = INCBIN_U16("graphics/ui_samuel_case/pokeball_hand.gbapal");

//
//  Sprite Data for Pokeball Hand Sprite
//
#define TAG_POKEBALL_CURSOR 20001
static const struct OamData sOamData_PokeballHand =
{
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 1,
};

static const struct CompressedSpriteSheet sSpriteSheet_PokeballHand =
{
    .data = sPokeballHand_Gfx,
    .size = 32*32*4/2,
    .tag = TAG_POKEBALL_CURSOR,
};

static const struct SpritePalette sSpritePal_PokeballHand =
{
    .data = sPokeballHand_Pal,
    .tag = TAG_POKEBALL_CURSOR
};

// Right Hand Animations (Same as original, with renamed variables)
static const union AnimCmd sSpriteAnim_PokeballStatic[] =
{
    ANIMCMD_FRAME(0, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSpriteAnim_PokeballRockBackAndForth[] =
{
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(16, 16),
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(32, 16),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSpriteAnim_Hand_Left[] =
{
    ANIMCMD_FRAME(48, 32, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSpriteAnim_Hand_Right[] =
{
    ANIMCMD_FRAME(48, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sSpriteAnimTable_PokeballHand[] =
{
    sSpriteAnim_PokeballStatic,
    sSpriteAnim_PokeballRockBackAndForth,
    sSpriteAnim_Hand_Left,
    sSpriteAnim_Hand_Right,
};

static const struct SpriteTemplate sSpriteTemplate_PokeballHandMap =
{
    .tileTag = TAG_POKEBALL_CURSOR,
    .paletteTag = TAG_POKEBALL_CURSOR,
    .oam = &sOamData_PokeballHand,
    .anims = sSpriteAnimTable_PokeballHand,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

#define POKEBALL_HAND_ANIM_LEFT     2
#define POKEBALL_HAND_ANIM_RIGHT    3


//
//  This is the Callback for the Hand Cursor that Updates its sprite position when moved by the input control functions
//
#define TOP_ROW_Y 35
#define MIDDLE_ROW_Y 67
#define BOTTOM_ROW_Y 94

static const struct SpriteCordsStruct sBallSpriteCords_Page1[3][4] = {
        {{55, TOP_ROW_Y}, {103, TOP_ROW_Y}, {167, TOP_ROW_Y}, {215, TOP_ROW_Y}},
        {{79, MIDDLE_ROW_Y}, {135, MIDDLE_ROW_Y}, {191, MIDDLE_ROW_Y}},
        {{103, BOTTOM_ROW_Y}, {167, BOTTOM_ROW_Y}},
};

static const struct SpriteCordsStruct sBallSpriteCords_Page2[3][4] = {
        {{25, TOP_ROW_Y}, {73, TOP_ROW_Y}, {137, TOP_ROW_Y}, {185, TOP_ROW_Y}},
        {{49, MIDDLE_ROW_Y}, {105, MIDDLE_ROW_Y}, {161, MIDDLE_ROW_Y}},
        {{73, BOTTOM_ROW_Y}, {137, BOTTOM_ROW_Y}},
};

static void CursorCallback(struct Sprite *sprite)
{
    struct SpriteCordsStruct current_position = {0,0};
    if(sSamuelCaseDataPtr->handPosition <= 3)
        current_position = ReturnBallPositionByPage()[0][sSamuelCaseDataPtr->handPosition];
    else if(sSamuelCaseDataPtr->handPosition <= 6)  
        current_position = ReturnBallPositionByPage()[1][sSamuelCaseDataPtr->handPosition - 4];
    else
        current_position = ReturnBallPositionByPage()[2][sSamuelCaseDataPtr->handPosition - 7];

    sprite->x = current_position.x;
    sprite->y = current_position.y - 6;

    if(sSamuelCaseDataPtr->movingSelector != TRUE)
    {
        if(sprite->data[5] <= 30)
        {
            sprite->y -= 1;
        }
        else if(sprite->data[5] > 30 && sprite->data[5] < 60)
        {
            sprite->y += 1;
        }
        else
        {
            sprite->y -= 1;
            sprite->data[5] = 0;
        }
        sprite->data[5]++;
    }
    
}

//
//  Create The Hande Cursor Sprite
//
static void CreateHandSprite()
{
    u16 i = 0;
    u16 x, y;
    struct SpriteCordsStruct current_position = {0,0};

    for(i=0; i<9; i++)
    {
        if(ReturnStartersByPage()[i].species == SPECIES_NONE) // Choose Non Empty Slot To Start In
            continue;
    
        if(sSamuelCaseDataPtr->handPosition <= 3)
        {
            current_position = ReturnBallPositionByPage()[0][sSamuelCaseDataPtr->handPosition];
            break;
        }
        else if(sSamuelCaseDataPtr->handPosition <= 6)  
        {
            current_position = ReturnBallPositionByPage()[1][sSamuelCaseDataPtr->handPosition - 4];
            break;
        }
        else
        {
            current_position = ReturnBallPositionByPage()[2][sSamuelCaseDataPtr->handPosition - 7];
            break;
        }
    }

    x = current_position.x;
    y = current_position.y - 6;
    sSamuelCaseDataPtr->handPosition = i;
    if (sSamuelCaseDataPtr->handSpriteId == SPRITE_NONE)
        sSamuelCaseDataPtr->handSpriteId = CreateSpriteAtEnd(&sSpriteTemplate_PokeballHandMap, x, y, 0);
    gSprites[sSamuelCaseDataPtr->handSpriteId].invisible = FALSE;
    gSprites[sSamuelCaseDataPtr->handSpriteId].callback = CursorCallback;
    StartSpriteAnim(&gSprites[sSamuelCaseDataPtr->handSpriteId], POKEBALL_HAND_ANIM_LEFT);
    StartSpriteAnim(&gSprites[sSamuelCaseDataPtr->pokeballSpriteIds[sSamuelCaseDataPtr->handPosition]], 1);
    SampleUi_DrawMonIcon(ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].species);
    
    return;
}

static void DestroyHandSprite()
{
    u8 i = 0;
    for(i = 0; i < 9; i++)
    {
        DestroySprite(&gSprites[sSamuelCaseDataPtr->handSpriteId]);
        sSamuelCaseDataPtr->handSpriteId = SPRITE_NONE;
    }
}

//
//  Create The Pokeball Sprites For Each Slot if Not SPECIES_NONE
//
static void CreatePokeballSprites()
{
    u16 i = 0;

    for(i=0; i<9; i++)
    {
        u16 x, y;
        if(ReturnStartersByPage()[i].species == SPECIES_NONE)
            continue;

        if(i <= 3)
        {
            x = ReturnBallPositionByPage()[0][i].x;
            y = ReturnBallPositionByPage()[0][i].y;
        }
        else if(i <= 6)
        {
            
            x = ReturnBallPositionByPage()[1][i - 4].x;
            y = ReturnBallPositionByPage()[1][i - 4].y;
        }
        else
        {
            x = ReturnBallPositionByPage()[2][i - 7].x;
            y = ReturnBallPositionByPage()[2][i - 7].y;
        }
        if (sSamuelCaseDataPtr->pokeballSpriteIds[i] == SPRITE_NONE)
            sSamuelCaseDataPtr->pokeballSpriteIds[i] = CreateSpriteAtEnd(&sSpriteTemplate_PokeballHandMap, x, y, 1);
        gSprites[sSamuelCaseDataPtr->pokeballSpriteIds[i]].invisible = FALSE;
        StartSpriteAnim(&gSprites[sSamuelCaseDataPtr->pokeballSpriteIds[i]], 0);

    }   
    return;
}

static void DestroyPokeballSprites()
{
    u8 i = 0;
    for(i = 0; i < 9; i++)
    {
        DestroySprite(&gSprites[sSamuelCaseDataPtr->pokeballSpriteIds[i]]);
        sSamuelCaseDataPtr->pokeballSpriteIds[i] = SPRITE_NONE;
    }
}


//
//  Draw The Pokemon Sprites
//
#define MON_ICON_X_PAGE_ONE     208
#define MON_ICON_X_PAGE_TWO     32
#define MON_ICON_Y     104
#define TAG_MON_SPRITE 30003
static void SampleUi_DrawMonIcon(u16 speciesId)
{
    u8 x = sCasePageNum ? MON_ICON_X_PAGE_ONE : MON_ICON_X_PAGE_TWO;
    
    sSamuelCaseDataPtr->monSpriteId = CreateMonPicSprite_Affine(speciesId, sMonShiny, 0x8000, TRUE, x, MON_ICON_Y, 5, TAG_NONE);
    gSprites[sSamuelCaseDataPtr->monSpriteId].oam.priority = 0;
}

static void ReloadNewPokemon(u8 taskId) // reload the pokeball after a 4 frame delay to prevent palette problems
{
    gSprites[sSamuelCaseDataPtr->monSpriteId].invisible = TRUE;
    FreeResourcesAndDestroySprite(&gSprites[sSamuelCaseDataPtr->monSpriteId], sSamuelCaseDataPtr->monSpriteId);
    sSamuelCaseDataPtr->movingSelector = TRUE;
    gTasks[taskId].func = Task_DelayedSpriteLoad;
    gTasks[taskId].data[11] = 0;
}

static void ChangePositionUpdateSpriteAnims(u16 oldPosition, u8 taskId) // turn off Ball Shaking on old ball and start it on new ball, reload pokemon and text
{
    StartSpriteAnim(&gSprites[sSamuelCaseDataPtr->pokeballSpriteIds[oldPosition]], 0);
    StartSpriteAnim(&gSprites[sSamuelCaseDataPtr->pokeballSpriteIds[sSamuelCaseDataPtr->handPosition]], 1);
    ReloadNewPokemon(taskId);
    PrintTextToBottomBar(CHOOSE_MON);
}

static void SamuelCase_GiveMon() // Function that calls the GiveMon function pulled from Expansion by Lunos and Ghoulslash
{
    u8 *evs = (u8 *) ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].evs;
    u8 *ivs = (u8 *) ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].ivs;
    u16 *moves = (u16 *) ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].moves;
    FlagSet(FLAG_SYS_POKEMON_GET);
    gSpecialVar_Result = SamuelCase_GiveMonParameterized(ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].species, ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].level, \
                ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].item, ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].ball, \
                ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].nature, ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].abilityNum, \
                ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].gender, evs, ivs, moves, \
                ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].ggMaxFactor, ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].teraType,\
                sMonShiny);
}

//==========FUNCTIONS==========//
// UI loader template functions by Ghoulslash
void Task_OpenSamuelCase(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        SamuelCase_Init(CB2_ReturnToFieldContinueScriptPlayMapMusic);
        DestroyTask(taskId);
    }
}

// This is our main initialization function if you want to call the menu from elsewhere
void SamuelCase_Init(MainCallback callback)
{
    u16 i = 0;
    if ((sSamuelCaseDataPtr = AllocZeroed(sizeof(struct MenuResources))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }
    
    // initialize stuff
    sSamuelCaseDataPtr->gfxLoadState = 0;
    sSamuelCaseDataPtr->savedCallback = callback;

    sSamuelCaseDataPtr->handSpriteId = SPRITE_NONE;

    sCasePageNum = PAGE_ONE;
    if (Random() < SHINY_ODDS)
        sMonShiny = TRUE;
    // RandomiseMonChoiceData(sStarterChoices_Page1_Male, ARRAY_COUNT(sStarterChoices_Page1_Male));
    // RandomiseMonChoiceData(sStarterChoices_Page2, ARRAY_COUNT(sStarterChoices_Page2));

    SetIkigaiStarterPokedexFlags();

    for(i=0; i < 9; i++)
    {
        sSamuelCaseDataPtr->pokeballSpriteIds[i] = SPRITE_NONE;
    }
    
    SetMainCallback2(SamuelCaseRunSetup);
}

static void SamuelCaseRunSetup(void)
{
    while (1)
    {
        if (SamuelCaseDoGfxSetup() == TRUE)
            break;
    }
}

static void SamuelCaseMainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void SamuelCaseVBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static bool8 SamuelCaseDoGfxSetup(void)
{
    switch (gMain.state)
    {
    case 0:
        DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000)
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        ResetVramOamAndBgCntRegs();
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
        if (SamuelCase_InitBgs())
        {
            sSamuelCaseDataPtr->gfxLoadState = 0;
            gMain.state++;
        }
        else
        {
            SamuelCaseFadeAndBail();
            return TRUE;
        }
        break;
    case 3:
        if (SamuelCaseLoadGraphics() == TRUE)
            gMain.state++;
        break;
    case 4:
        LoadMessageBoxAndBorderGfx();
        SamuelCase_InitWindows();
        gMain.state++;
        break;
    case 5:
        CreatePokeballSprites(); // Create Sprites and Print Text
        CreateHandSprite();
        if (sMonShiny)
            PlaySE(SE_SHINY);
        PrintTextToBottomBar(CHOOSE_MON);
        CreateTask(Task_SamuelCaseWaitFadeIn, 0);
        BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(SamuelCaseVBlankCB);
        SetMainCallback2(SamuelCaseMainCB);
        return TRUE;
    }
    return FALSE;
}

#define try_free(ptr) ({        \
    void ** ptr__ = (void **)&(ptr);   \
    if (*ptr__ != NULL)                \
        Free(*ptr__);                  \
})

static void SamuelCaseFreeResources(void)
{
    try_free(sSamuelCaseDataPtr);
    try_free(sBg1TilemapBuffer);
    try_free(sBg2TilemapBuffer);
    FreeResourcesAndDestroySprite(&gSprites[sSamuelCaseDataPtr->monSpriteId], sSamuelCaseDataPtr->monSpriteId);
    DestroyPokeballSprites();
    DestroyHandSprite();
    FreeAllWindowBuffers();
}

static void Task_SamuelCaseWaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sSamuelCaseDataPtr->savedCallback);
        SamuelCaseFreeResources();
        DestroyTask(taskId);
    }
}

static void SamuelCaseFadeAndBail(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_SamuelCaseWaitFadeAndBail, 0);
    SetVBlankCallback(SamuelCaseVBlankCB);
    SetMainCallback2(SamuelCaseMainCB);
}

static void Task_SamuelCaseWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_SamuelCaseMain;
}

static void Task_SamuelCaseTurnOff(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sSamuelCaseDataPtr->savedCallback);
        SamuelCaseFreeResources();
        DestroyTask(taskId);
    }
}

static bool8 SamuelCase_InitBgs(void) // Init the bgs and bg tilemap buffers and turn sprites on, also set the bgs to blend
{
    ResetAllBgsCoordinates();
    sBg1TilemapBuffer = Alloc(0x800);
    if (sBg1TilemapBuffer == NULL)
        return FALSE;
    memset(sBg1TilemapBuffer, 0, 0x800);

    sBg2TilemapBuffer = Alloc(0x800);
    if (sBg2TilemapBuffer == NULL)
        return FALSE;
    memset(sBg2TilemapBuffer, 0, 0x800);
    
    
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sMenuBgTemplates, NELEMS(sMenuBgTemplates));
    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    SetBgTilemapBuffer(2, sBg2TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG2);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(10, 6));
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    return TRUE;
}

static bool8 SamuelCaseLoadGraphics(void) // load tilesets, tilemaps, spritesheets, and palettes
{
    switch (sSamuelCaseDataPtr->gfxLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sCaseTiles, 0, 0, 0);
        DecompressAndCopyTileDataToVram(2, sTextBgTiles, 0, 0, 0);
        sSamuelCaseDataPtr->gfxLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(sCaseTilemapLeft, sBg1TilemapBuffer);
            DecompressDataWithHeaderWram(sTextBgTilemapLeft, sBg2TilemapBuffer);
            sSamuelCaseDataPtr->gfxLoadState++;
        }
        break;
    case 2:
        LoadCompressedSpriteSheet(&sSpriteSheet_PokeballHand);
        LoadSpritePalette(&sSpritePal_PokeballHand);
        LoadPalette(sCasePalette, 32, 32);
        IkigaiScrollingBackground_LoadPalette(1, IKIGAI_PAL_DEFAULT);
        sSamuelCaseDataPtr->gfxLoadState++;
        break;
    default:
        sSamuelCaseDataPtr->gfxLoadState = 0;
        return TRUE;
    }
    return FALSE;
}

static void SamuelCase_InitWindows(void)
{
    InitWindows(sMenuWindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    
    FillWindowPixelBuffer(WINDOW_BOTTOM_BAR, 0);
    LoadUserWindowBorderGfx(WINDOW_BOTTOM_BAR, 720, 14 * 16);
    PutWindowTilemap(WINDOW_BOTTOM_BAR);
    CopyWindowToVram(WINDOW_BOTTOM_BAR, 3);
    
    ScheduleBgCopyTilemapToVram(2);
}


//
//  Text Printing Function
//
static const u8 sText_RevealR[] = _("              Slide Briefcase {R_BUTTON}");
static const u8 sText_RevealL[] = _("{L_BUTTON} Slide Briefcase");
static const u8 sText_AreYouSure[] = _("Are you sure?    {A_BUTTON} Yes  {B_BUTTON} No");
static const u8 sText_RecievedMon[] = _("Give your Pokémon a Nickname?   {A_BUTTON} Yes  {B_BUTTON} No");
static void PrintTextToBottomBar(u8 textId)
{
    u8 speciesCategoryArray[64];
    const u8 *mainBarAlternatingText;
    const u8 * speciesTypeText;

    u8 x = 1 + 4;
    u8 y = 1 + 18;

    u16 species = ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].species;
    // u16 dexNum = SpeciesToNationalPokedexNum(species);    

    FillWindowPixelBuffer(WINDOW_BOTTOM_BAR, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    switch(textId)
    {
        default:
        case 0:
            mainBarAlternatingText = (sCasePageNum == PAGE_ONE) ? sText_RevealR : sText_RevealL;
            break;
        case 1:
            mainBarAlternatingText = sText_AreYouSure;
            break;
        case 2:
            mainBarAlternatingText = sText_RecievedMon;
            break;
    }
    if (sCasePageNum == PAGE_ONE && mainBarAlternatingText != sText_RecievedMon)
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, x + 83, y, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, mainBarAlternatingText);
    else
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, x + 2, y, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, mainBarAlternatingText);

    if(ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].species == SPECIES_NONE)
    {
        PutWindowTilemap(WINDOW_BOTTOM_BAR);
        CopyWindowToVram(WINDOW_BOTTOM_BAR, 3);
        return;
    }
    

    // StringCopy(gStringVar1, &gText_NumberClear01[0]);
    // ConvertIntToDecimalStringN(gStringVar2, dexNum, STR_CONV_MODE_LEADING_ZEROS, 3);
    // StringAppend(gStringVar1, gStringVar2);
    if (sCasePageNum == PAGE_TWO)
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, x + 21, 1 + 2, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, COMPOUND_STRING("The"));

    // AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, x + 32, 1 + 2, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gText_Dash);

    StringCopy(&speciesCategoryArray[0], GetSpeciesCategory(species));
    StringAppend(&speciesCategoryArray[0], COMPOUND_STRING(" Pokémon"));
    if (sCasePageNum == PAGE_ONE)
    {
        StringCopy(gStringVar2, COMPOUND_STRING("The "));
        StringAppend(gStringVar2, &speciesCategoryArray[0]);
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, GetStringRightAlignXOffset(FONT_NORMAL, gStringVar2, 228), 1 + 2, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar2);
    }
    else
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, x + 42, 1 + 2, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, &speciesCategoryArray[0]);
    }

    if(textId != 2)
    {
        speciesTypeText = gTypesInfo[GetSpeciesPrimaryType(species)].name;
        StringCopy(gStringVar1, speciesTypeText);
        StringAppend(gStringVar1, COMPOUND_STRING(" Type"));
        if (sCasePageNum == PAGE_ONE)
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, ReturnNarrowTextFont(), x + 4, y, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);
        else
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, ReturnNarrowTextFont(), x + 169 + GetStringRightAlignXOffset(ReturnNarrowTextFont(), gStringVar1, 52), y, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);
    }

    PutWindowTilemap(WINDOW_BOTTOM_BAR);
    CopyWindowToVram(WINDOW_BOTTOM_BAR, 3);

    // expansion takes speciesid const u8 *GetSpeciesCategory(u16 species).natDexNum = NATIONAL_DEX_GROTLE,
    //    .categoryName = _("Grove"),
    // vanilla takes dexnum const u8 *GetPokedexCategoryName(u16 dexNum)
}


//
//  Control Flow Tasks for Switching Positions in the Grid and Confirming and Naming a Mon
//
static void Task_DelayedSpriteLoad(u8 taskId) // wait 4 frames after changing the mon you're editing so there are no palette problems
{   
    if (gTasks[taskId].data[11] >= 4)
    {
        if(ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].species != SPECIES_NONE)
            SampleUi_DrawMonIcon(ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].species);
        gTasks[taskId].func = Task_SamuelCaseMain;
        sSamuelCaseDataPtr->movingSelector = FALSE;
        return;
    }
    else
    {
        gTasks[taskId].data[11]++;
    }
}

static void Task_WaitForFadeAndOpenNamingScreen(u8 taskId)
{   
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sSamuelCaseDataPtr->savedCallback);
        SamuelCaseFreeResources();
        DestroyTask(taskId);
        VarSet(VAR_0x8004, gPlayerPartyCount - 1);
        ChangePokemonNickname();
    }
}

static void Task_SamuelCaseRecievedMon(u8 taskId)
{
    if(JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_WaitForFadeAndOpenNamingScreen;
        return;
    }
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_SamuelCaseTurnOff;
        return;
    }
}

static void Task_SamuelCaseConfirmSelection(u8 taskId)
{
    if(JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        PrintTextToBottomBar(RECIEVED_MON);
        SamuelCase_GiveMon();
        gTasks[taskId].func = Task_SamuelCaseRecievedMon;
        return;
    }
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        PrintTextToBottomBar(CHOOSE_MON);
        gTasks[taskId].func = Task_SamuelCaseMain;
        return;
    }
}


/* Main Grid Based Movement Control Flow*/
static void Task_SamuelCaseMain(u8 taskId)
{
    u16 oldPosition = sSamuelCaseDataPtr->handPosition;
    if (((JOY_NEW(R_BUTTON) && sCasePageNum == PAGE_ONE) || (JOY_NEW(L_BUTTON) && sCasePageNum == PAGE_TWO)) && !gPaletteFade.active)
    {
        PlaySE(SE_BALL_TRAY_ENTER);
        SamuelCaseChangeGraphics();
    }
    if(JOY_NEW(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        if(sSamuelCaseDataPtr->handPosition <= BALL_TOP_FOURTH) // top row move up
        {
            if(sSamuelCaseDataPtr->handPosition < BALL_TOP_THIRD)
                sSamuelCaseDataPtr->handPosition = BALL_BOTTOM_FIRST;
            else
                sSamuelCaseDataPtr->handPosition = BALL_BOTTOM_SECOND;
        }
        else if(sSamuelCaseDataPtr->handPosition <= BALL_MIDDLE_THIRD)  // middle row move up
        {
            if(sSamuelCaseDataPtr->handPosition == BALL_MIDDLE_FIRST)
                sSamuelCaseDataPtr->handPosition = BALL_TOP_FIRST;
            else if (sSamuelCaseDataPtr->handPosition == BALL_MIDDLE_SECOND)
                sSamuelCaseDataPtr->handPosition = BALL_TOP_SECOND;
            else
                sSamuelCaseDataPtr->handPosition = BALL_TOP_THIRD;
        }
        else  // bottom row move up
        {
            sSamuelCaseDataPtr->handPosition = BALL_MIDDLE_SECOND;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if(JOY_NEW(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        if(sSamuelCaseDataPtr->handPosition <= BALL_TOP_FOURTH) // top row move down
        {
            if(sSamuelCaseDataPtr->handPosition < BALL_TOP_THIRD)
                sSamuelCaseDataPtr->handPosition = BALL_MIDDLE_FIRST;
            else if(sSamuelCaseDataPtr->handPosition == BALL_TOP_THIRD)
                sSamuelCaseDataPtr->handPosition = BALL_MIDDLE_SECOND;
            else
                sSamuelCaseDataPtr->handPosition = BALL_MIDDLE_THIRD;
        }
        else if(sSamuelCaseDataPtr->handPosition <= BALL_MIDDLE_THIRD)  // middle row move down
        {
            if(sSamuelCaseDataPtr->handPosition < BALL_MIDDLE_SECOND)
                sSamuelCaseDataPtr->handPosition = BALL_BOTTOM_FIRST;
            else
                sSamuelCaseDataPtr->handPosition = BALL_BOTTOM_SECOND;
        }
        else  // bottom row move down
        {
            if(sSamuelCaseDataPtr->handPosition == BALL_BOTTOM_FIRST)
                sSamuelCaseDataPtr->handPosition = BALL_TOP_SECOND;
            else
                sSamuelCaseDataPtr->handPosition = BALL_TOP_THIRD;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if(JOY_NEW(DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
        if(sSamuelCaseDataPtr->handPosition <= BALL_TOP_FOURTH) // top row move down
        {
            if(sSamuelCaseDataPtr->handPosition == BALL_TOP_FOURTH) // top row move down
                sSamuelCaseDataPtr->handPosition = BALL_TOP_FIRST;
            else
                sSamuelCaseDataPtr->handPosition += 1;
        }
        else if(sSamuelCaseDataPtr->handPosition <= BALL_MIDDLE_THIRD)  // middle row move down
        {
            if(sSamuelCaseDataPtr->handPosition == BALL_MIDDLE_THIRD) // top row move down
                sSamuelCaseDataPtr->handPosition = BALL_MIDDLE_FIRST;
            else
                sSamuelCaseDataPtr->handPosition += 1;
        }
        else  // bottom row move down
        {
            if(sSamuelCaseDataPtr->handPosition == BALL_BOTTOM_SECOND) // top row move down
                sSamuelCaseDataPtr->handPosition = BALL_BOTTOM_FIRST;
            else
                sSamuelCaseDataPtr->handPosition += 1;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if(JOY_NEW(DPAD_LEFT))
    {
        PlaySE(SE_SELECT);
        if(sSamuelCaseDataPtr->handPosition <= BALL_TOP_FOURTH) // top row move down
        {
            if(sSamuelCaseDataPtr->handPosition == BALL_TOP_FIRST) // top row move down
                sSamuelCaseDataPtr->handPosition = BALL_TOP_FOURTH;
            else
                sSamuelCaseDataPtr->handPosition -= 1;
        }
        else if(sSamuelCaseDataPtr->handPosition <= BALL_MIDDLE_THIRD)  // middle row move down
        {
            if(sSamuelCaseDataPtr->handPosition == BALL_MIDDLE_FIRST) // top row move down
                sSamuelCaseDataPtr->handPosition = BALL_MIDDLE_THIRD;
            else
                sSamuelCaseDataPtr->handPosition -= 1;
        }
        else  // bottom row move down
        {
            if(sSamuelCaseDataPtr->handPosition == BALL_BOTTOM_FIRST) // top row move down
                sSamuelCaseDataPtr->handPosition = BALL_BOTTOM_SECOND;
            else
                sSamuelCaseDataPtr->handPosition -= 1;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if(JOY_NEW(A_BUTTON))
    {
        if(ReturnStartersByPage()[sSamuelCaseDataPtr->handPosition].species != SPECIES_NONE) // If spot empty don't go to next control flow state
        {
            PlaySE(SE_SELECT);
            PrintTextToBottomBar(CONFIRM_SELECTION);
            gTasks[taskId].func = Task_SamuelCaseConfirmSelection;
            return;
        }
        else
        {
            PlaySE(SE_BOO);
            return;
        }
    }
}

static const struct MonChoiceData* ReturnStartersByPage(void)
{
    if (sCasePageNum == PAGE_ONE)
    {
        return (gSaveBlock2Ptr->playerGender == MALE) ? sStarterChoices_Page1_Male : sStarterChoices_Page1_Female;
    }
    else
    {
        return sStarterChoices_Page2;
    }
}

static void SamuelCaseChangeGraphics(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_WaitFadeOutAndChangeGraphics, 0);
}

static void Task_WaitFadeOutAndChangeGraphics(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        sCasePageNum ^= 1;
        if (sCasePageNum == PAGE_ONE)
        {
            DecompressDataWithHeaderWram(sCaseTilemapLeft, sBg1TilemapBuffer);
            DecompressDataWithHeaderWram(sTextBgTilemapLeft, sBg2TilemapBuffer);
            StartSpriteAnim(&gSprites[sSamuelCaseDataPtr->handSpriteId], POKEBALL_HAND_ANIM_LEFT);
        }
        else
        {
            DecompressDataWithHeaderWram(sCaseTilemapRight, sBg1TilemapBuffer);
            DecompressDataWithHeaderWram(sTextBgTilemapRight, sBg2TilemapBuffer);
            StartSpriteAnim(&gSprites[sSamuelCaseDataPtr->handSpriteId], POKEBALL_HAND_ANIM_RIGHT);
        }
        ScheduleBgCopyTilemapToVram(1);
        ScheduleBgCopyTilemapToVram(2);
        DestroyPokeballSprites();
        CreatePokeballSprites();
        ChangePositionUpdateSpriteAnims(sSamuelCaseDataPtr->handPosition, FindTaskIdByFunc(Task_SamuelCaseMain));
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        DestroyTask(taskId);
    }
}

static const struct SpriteCordsStruct (*ReturnBallPositionByPage(void))[4]
{
    if (sCasePageNum == PAGE_ONE)
    {
        return sBallSpriteCords_Page1;
    }
    else
    {
        return sBallSpriteCords_Page2;
    }
}

void SetIkigaiStarterPokedexFlags(void)
{
    u16 i;
    for(i=0; i < 9; i++)
    {
        (gSaveBlock2Ptr->playerGender == MALE) ? GetSetPokedexFlag(SpeciesToNationalPokedexNum(sStarterChoices_Page1_Male[i].species), FLAG_SET_SEEN) : GetSetPokedexFlag(SpeciesToNationalPokedexNum(sStarterChoices_Page1_Female[i].species), FLAG_SET_SEEN);
        GetSetPokedexFlag(SpeciesToNationalPokedexNum(sStarterChoices_Page2[i].species), FLAG_SET_SEEN);
    }
}