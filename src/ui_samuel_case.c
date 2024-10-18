#include "global.h"
#include "ui_samuel_case.h"
#include "strings.h"
#include "bg.h"
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
#include "naming_screen.h"
#include "tv.h"

 /*
    9 Starter Selection Birch Case

    This UI was coded by Archie with Graphics by Mudskip.

 */
 
//==========DEFINES==========//
struct MenuResources
{
    MainCallback savedCallback;     // determines callback to run when we exit. e.g. where do we want to go after closing the menu
    u8 gfxLoadState;
    u16 monSpriteId;
    u16 pokeballSpriteIds[18];
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
    BALL_FIRST_FIRST,
    BALL_FIRST_SECOND,
    BALL_FIRST_THIRD,
    BALL_FIRST_FOURTH,
    BALL_FIRST_FIFTH,
    BALL_FIRST_SIXTH,

    BALL_SECOND_FIRST,
    BALL_SECOND_SECOND,
    BALL_SECOND_THIRD,
    BALL_SECOND_FOURTH,

    BALL_THIRD_FIRST,
    BALL_THIRD_SECOND,
    BALL_THIRD_THIRD,
    BALL_THIRD_FOURTH,

    BALL_FOURTH_FIRST,
    BALL_FOURTH_SECOND,
    BALL_FOURTH_THIRD,
    BALL_FOURTH_FOURTH,
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
static const struct MonChoiceData sStarterChoices[18] = 
{
    [BALL_FIRST_FIRST]          = {SPECIES_BIDOOF, 5, ITEM_POTION, BALL_NET, NATURE_JOLLY, 1, MON_MALE, {255, 255, 0, 0, 0, 0}, {31, 31, 31, 31, 31, 31}, {MOVE_FIRE_BLAST, MOVE_SHEER_COLD, MOVE_WATER_GUN, MOVE_THUNDER}, 0, 0, 0},
    [BALL_FIRST_SECOND]         = {SPECIES_MACHOP, 5},
    [BALL_FIRST_THIRD]          = {SPECIES_STARLY, 5},
    [BALL_FIRST_FOURTH]         = {SPECIES_EKANS, 5},
    [BALL_FIRST_FIFTH]          = {SPECIES_SANDILE, 5},
    [BALL_FIRST_SIXTH]          = {SPECIES_GEODUDE, 5},

    [BALL_SECOND_FIRST]         = {SPECIES_WURMPLE, 5},
    [BALL_SECOND_SECOND]        = {SPECIES_GASTLY, 5},
    [BALL_SECOND_THIRD]         = {SPECIES_ARON, 5},
    [BALL_SECOND_FOURTH]        = {SPECIES_PONYTA, 5},

    [BALL_THIRD_FIRST]          = {SPECIES_FINIZEN, 5},
    [BALL_THIRD_SECOND]         = {SPECIES_BUDEW, 5},
    [BALL_THIRD_THIRD]          = {SPECIES_PIKACHU, 5},
    [BALL_THIRD_FOURTH]         = {SPECIES_ABRA, 5},

    [BALL_FOURTH_FIRST]         = {SPECIES_SNOM, 5},
    [BALL_FOURTH_SECOND]        = {SPECIES_DREEPY, 5},
    [BALL_FOURTH_THIRD]         = {SPECIES_ABSOL, 5},
    [BALL_FOURTH_FOURTH]        = {SPECIES_TINKATINK, 5},
};

//==========EWRAM==========//
static EWRAM_DATA struct MenuResources *sBirchCaseDataPtr = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;

//==========STATIC=DEFINES==========//
static void BirchCaseRunSetup(void);
static bool8 BirchCaseDoGfxSetup(void);
static bool8 SamuelCase_InitBgs(void);
static void BirchCaseFadeAndBail(void);
static bool8 BirchCaseLoadGraphics(void);
static void SamuelCase_InitWindows(void);
static void PrintTextToBottomBar(u8 textId);
static void Task_BirchCaseWaitFadeIn(u8 taskId);
static void Task_BirchCaseMain(u8 taskId);
static void SampleUi_DrawMonIcon(u16 speciesId);
static void Task_DelayedSpriteLoad(u8 taskId);

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
static const u32 sCaseTilemap[] = INCBIN_U32("graphics/ui_samuel_case/case_tiles.bin.lz");
static const u16 sCasePalette[] = INCBIN_U16("graphics/ui_samuel_case/case_tiles.gbapal");

static const u32 sTextBgTiles[]   = INCBIN_U32("graphics/ui_samuel_case/text_bg_tiles.4bpp.lz");
static const u32 sTextBgTilemap[] = INCBIN_U32("graphics/ui_samuel_case/text_bg_tiles.bin.lz");
static const u16 sTextBgPalette[] = INCBIN_U16("graphics/ui_samuel_case/text_bg_tiles.gbapal");

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

static const union AnimCmd sSpriteAnim_Hand[] =
{
    ANIMCMD_FRAME(48, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sSpriteAnimTable_PokeballHand[] =
{
    sSpriteAnim_PokeballStatic,
    sSpriteAnim_PokeballRockBackAndForth,
    sSpriteAnim_Hand,
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


//
//  This is the Callback for the Hand Cursor that Updates its sprite position when moved by the input control functions
//
#define FIRST_ROW_Y 36
#define SECOND_ROW_Y 51
#define THIRD_ROW_Y 66
#define FOURTH_ROW_Y 80

static const struct SpriteCordsStruct sBallSpriteCords[4][6] = {
        {{40, FIRST_ROW_Y}, {72, FIRST_ROW_Y}, {104, FIRST_ROW_Y}, {136, FIRST_ROW_Y}, {168, FIRST_ROW_Y}, {180, FIRST_ROW_Y}},
        {{48, SECOND_ROW_Y}, {96, SECOND_ROW_Y}, {144, SECOND_ROW_Y}, {192, SECOND_ROW_Y}},
        {{56, THIRD_ROW_Y}, {88, THIRD_ROW_Y}, {152, THIRD_ROW_Y}, {184, THIRD_ROW_Y}},
        {{48, FOURTH_ROW_Y}, {96, FOURTH_ROW_Y}, {144, FOURTH_ROW_Y}, {240, FOURTH_ROW_Y}},
};

static void CursorCallback(struct Sprite *sprite)
{
    struct SpriteCordsStruct current_position = {0, 0};
    
    if (sBirchCaseDataPtr->handPosition < 6)
    {
        // First row (0 to 5)
        current_position = sBallSpriteCords[0][sBirchCaseDataPtr->handPosition];
    }
    else if (sBirchCaseDataPtr->handPosition < 10)
    {
        // Second row (6 to 9)
        current_position = sBallSpriteCords[1][sBirchCaseDataPtr->handPosition - 6];
    }
    else if (sBirchCaseDataPtr->handPosition < 14)
    {
        // Third row (10 to 13)
        current_position = sBallSpriteCords[2][sBirchCaseDataPtr->handPosition - 10];
    }
    else if (sBirchCaseDataPtr->handPosition < 18)
    {
        // Fourth row (14 to 17)
        current_position = sBallSpriteCords[3][sBirchCaseDataPtr->handPosition - 14];
    }

    sprite->x = current_position.x;
    sprite->y = current_position.y - 6;

    if (sBirchCaseDataPtr->movingSelector != TRUE)
    {
        if (sprite->data[5] <= 30)
        {
            sprite->y -= 1;
        }
        else if (sprite->data[5] > 30 && sprite->data[5] < 60)
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

static void CreateHandSprite()
{
    u16 i = 0;
    u16 x, y;
    struct SpriteCordsStruct current_position = {0, 0};

    for (i = 0; i < 18; i++)
    {
        if (sStarterChoices[i].species == SPECIES_NONE) // Skip empty slots
            continue;
        
        if (i < 6)
        {
            current_position = sBallSpriteCords[0][i];
            sBirchCaseDataPtr->handPosition = i;
            break;
        }
        else if (i < 10)
        {
            current_position = sBallSpriteCords[1][i - 6];
            sBirchCaseDataPtr->handPosition = i;
            break;
        }
        else if (i < 14)
        {
            current_position = sBallSpriteCords[2][i - 10];
            sBirchCaseDataPtr->handPosition = i;
            break;
        }
        else if (i < 18)
        {
            current_position = sBallSpriteCords[3][i - 14];
            sBirchCaseDataPtr->handPosition = i;
            break;
        }
    }

    x = current_position.x;
    y = current_position.y - 6;

    if (sBirchCaseDataPtr->handSpriteId == SPRITE_NONE)
        sBirchCaseDataPtr->handSpriteId = CreateSpriteAtEnd(&sSpriteTemplate_PokeballHandMap, x, y, 0);
    gSprites[sBirchCaseDataPtr->handSpriteId].invisible = FALSE;
    gSprites[sBirchCaseDataPtr->handSpriteId].callback = CursorCallback;
    StartSpriteAnim(&gSprites[sBirchCaseDataPtr->handSpriteId], 2);
    StartSpriteAnim(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[sBirchCaseDataPtr->handPosition]], 1);
    SampleUi_DrawMonIcon(sStarterChoices[sBirchCaseDataPtr->handPosition].species);
}

static void DestroyHandSprite()
{
    u8 i = 0;
    for(i = 0; i < 18; i++)
    {
        DestroySprite(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[i]]);
        sBirchCaseDataPtr->pokeballSpriteIds[i] = SPRITE_NONE;
    }
}

//
//  Create The Pokeball Sprites For Each Slot if Not SPECIES_NONE
//
static void CreatePokeballSprites()
{
    u16 i = 0;

    for (i = 0; i < 18; i++)
    {
        u16 x, y;
        if (sStarterChoices[i].species == SPECIES_NONE)
            continue;

        if (i < 6)
        {
            // First row (0 to 5)
            x = sBallSpriteCords[0][i].x;
            y = sBallSpriteCords[0][i].y;
        }
        else if (i < 10)
        {
            // Second row (6 to 9)
            x = sBallSpriteCords[1][i - 6].x;
            y = sBallSpriteCords[1][i - 6].y;
        }
        else if (i < 14)
        {
            // Third row (10 to 13)
            x = sBallSpriteCords[2][i - 10].x;
            y = sBallSpriteCords[2][i - 10].y;
        }
        else if (i < 18)
        {
            // Fourth row (14 to 17)
            x = sBallSpriteCords[3][i - 14].x;
            y = sBallSpriteCords[3][i - 14].y;
        }

        if (sBirchCaseDataPtr->pokeballSpriteIds[i] == SPRITE_NONE)
            sBirchCaseDataPtr->pokeballSpriteIds[i] = CreateSpriteAtEnd(&sSpriteTemplate_PokeballHandMap, x, y, 1);
        gSprites[sBirchCaseDataPtr->pokeballSpriteIds[i]].invisible = FALSE;
        StartSpriteAnim(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[i]], 0);
    }
}

static void DestroyPokeballSprites()
{
    u8 i = 0;
    for (i = 0; i < 18; i++)
    {
        DestroySprite(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[i]]);
        sBirchCaseDataPtr->pokeballSpriteIds[i] = SPRITE_NONE;
    }
}


//
//  Draw The Pokemon Sprites
//
#define MON_ICON_X     208
#define MON_ICON_Y     104
#define TAG_MON_SPRITE 30003
static void SampleUi_DrawMonIcon(u16 speciesId)
{
    sBirchCaseDataPtr->monSpriteId = CreateMonPicSprite_Affine(speciesId, 0, 0x8000, TRUE, MON_ICON_X, MON_ICON_Y, 5, TAG_NONE);
    gSprites[sBirchCaseDataPtr->monSpriteId].oam.priority = 0;
}

static void ReloadNewPokemon(u8 taskId) // reload the pokeball after a 4 frame delay to prevent palette problems
{
    gSprites[sBirchCaseDataPtr->monSpriteId].invisible = TRUE;
    FreeResourcesAndDestroySprite(&gSprites[sBirchCaseDataPtr->monSpriteId], sBirchCaseDataPtr->monSpriteId);
    sBirchCaseDataPtr->movingSelector = TRUE;
    gTasks[taskId].func = Task_DelayedSpriteLoad;
    gTasks[taskId].data[11] = 0;
}

static void ChangePositionUpdateSpriteAnims(u16 oldPosition, u8 taskId) // turn off Ball Shaking on old ball and start it on new ball, reload pokemon and text
{
    StartSpriteAnim(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[oldPosition]], 0);
    StartSpriteAnim(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[sBirchCaseDataPtr->handPosition]], 1);
    ReloadNewPokemon(taskId);
    PrintTextToBottomBar(CHOOSE_MON);
}

static void BirchCase_GiveMon() // Function that calls the GiveMon function pulled from Expansion by Lunos and Ghoulslash
{
    u8 *evs = (u8 *) sStarterChoices[sBirchCaseDataPtr->handPosition].evs;
    u8 *ivs = (u8 *) sStarterChoices[sBirchCaseDataPtr->handPosition].ivs;
    u16 *moves = (u16 *) sStarterChoices[sBirchCaseDataPtr->handPosition].moves;
    FlagSet(FLAG_SYS_POKEMON_GET);
    gSpecialVar_Result = SamuelCase_GiveMonParameterized(sStarterChoices[sBirchCaseDataPtr->handPosition].species, sStarterChoices[sBirchCaseDataPtr->handPosition].level, \
                sStarterChoices[sBirchCaseDataPtr->handPosition].item, sStarterChoices[sBirchCaseDataPtr->handPosition].ball, \
                sStarterChoices[sBirchCaseDataPtr->handPosition].nature, sStarterChoices[sBirchCaseDataPtr->handPosition].abilityNum, \
                sStarterChoices[sBirchCaseDataPtr->handPosition].gender, evs, ivs, moves, \
                sStarterChoices[sBirchCaseDataPtr->handPosition].ggMaxFactor, sStarterChoices[sBirchCaseDataPtr->handPosition].teraType,\
                sStarterChoices[sBirchCaseDataPtr->handPosition].isShinyExpansion);
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
    if ((sBirchCaseDataPtr = AllocZeroed(sizeof(struct MenuResources))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }
    
    // initialize stuff
    sBirchCaseDataPtr->gfxLoadState = 0;
    sBirchCaseDataPtr->savedCallback = callback;

    sBirchCaseDataPtr->handSpriteId = SPRITE_NONE;

    for(i=0; i < 9; i++)
    {
        sBirchCaseDataPtr->pokeballSpriteIds[i] = SPRITE_NONE;
    }
    
    SetMainCallback2(BirchCaseRunSetup);
}

static void BirchCaseRunSetup(void)
{
    while (1)
    {
        if (BirchCaseDoGfxSetup() == TRUE)
            break;
    }
}

static void BirchCaseMainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void BirchCaseVBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static bool8 BirchCaseDoGfxSetup(void)
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
            sBirchCaseDataPtr->gfxLoadState = 0;
            gMain.state++;
        }
        else
        {
            BirchCaseFadeAndBail();
            return TRUE;
        }
        break;
    case 3:
        if (BirchCaseLoadGraphics() == TRUE)
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
        PrintTextToBottomBar(CHOOSE_MON);
        CreateTask(Task_BirchCaseWaitFadeIn, 0);
        BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(BirchCaseVBlankCB);
        SetMainCallback2(BirchCaseMainCB);
        return TRUE;
    }
    return FALSE;
}

#define try_free(ptr) ({        \
    void ** ptr__ = (void **)&(ptr);   \
    if (*ptr__ != NULL)                \
        Free(*ptr__);                  \
})

static void BirchCaseFreeResources(void)
{
    try_free(sBirchCaseDataPtr);
    try_free(sBg1TilemapBuffer);
    try_free(sBg2TilemapBuffer);
    FreeResourcesAndDestroySprite(&gSprites[sBirchCaseDataPtr->monSpriteId], sBirchCaseDataPtr->monSpriteId);
    DestroyPokeballSprites();
    DestroyHandSprite();
    FreeAllWindowBuffers();
}

static void Task_BirchCaseWaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sBirchCaseDataPtr->savedCallback);
        BirchCaseFreeResources();
        DestroyTask(taskId);
    }
}

static void BirchCaseFadeAndBail(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_BirchCaseWaitFadeAndBail, 0);
    SetVBlankCallback(BirchCaseVBlankCB);
    SetMainCallback2(BirchCaseMainCB);
}

static void Task_BirchCaseWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_BirchCaseMain;
}

static void Task_BirchCaseTurnOff(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sBirchCaseDataPtr->savedCallback);
        BirchCaseFreeResources();
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
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(1, 8));
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    return TRUE;
}

static bool8 BirchCaseLoadGraphics(void) // load tilesets, tilemaps, spritesheets, and palettes
{
    switch (sBirchCaseDataPtr->gfxLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sCaseTiles, 0, 0, 0);
        DecompressAndCopyTileDataToVram(2, sTextBgTiles, 0, 0, 0);
        sBirchCaseDataPtr->gfxLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sCaseTilemap, sBg1TilemapBuffer);
            LZDecompressWram(sTextBgTilemap, sBg2TilemapBuffer);
            sBirchCaseDataPtr->gfxLoadState++;
        }
        break;
    case 2:
        LoadCompressedSpriteSheet(&sSpriteSheet_PokeballHand);
        LoadSpritePalette(&sSpritePal_PokeballHand);
        LoadPalette(sCasePalette, 32, 32);
        LoadPalette(sTextBgPalette, 16, 16);
        sBirchCaseDataPtr->gfxLoadState++;
        break;
    default:
        sBirchCaseDataPtr->gfxLoadState = 0;
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
static const u8 sText_ChooseMon[] = _("Release a Pokémon!");
static const u8 sText_AreYouSure[] = _("Are you sure?    {A_BUTTON} Yes  {B_BUTTON} No");
static const u8 sText_RecievedMon[] = _("Give your Pokémon a Nickname?   {A_BUTTON} Yes  {B_BUTTON} No");
static void PrintTextToBottomBar(u8 textId)
{
    u8 speciesNameArray[16];
    const u8 *mainBarAlternatingText;
    const u8 * speciesCategoryText;

    u8 x = 1 + 4;
    u8 y = 1 + 18;

    u16 species = sStarterChoices[sBirchCaseDataPtr->handPosition].species;
    u16 dexNum = SpeciesToNationalPokedexNum(species);    

    FillWindowPixelBuffer(WINDOW_BOTTOM_BAR, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    switch(textId)
    {
        case 0:
            mainBarAlternatingText = sText_ChooseMon;
            break;
        case 1:
            mainBarAlternatingText = sText_AreYouSure;
            break;
        case 2:
            mainBarAlternatingText = sText_RecievedMon;
            break;
        default:
            mainBarAlternatingText = sText_ChooseMon;
            break;
    } 
    AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, x, y, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, mainBarAlternatingText);

    if(sStarterChoices[sBirchCaseDataPtr->handPosition].species == SPECIES_NONE)
    {
        PutWindowTilemap(WINDOW_BOTTOM_BAR);
        CopyWindowToVram(WINDOW_BOTTOM_BAR, 3);
        return;
    }
    

    StringCopy(gStringVar1, &gText_NumberClear01[0]);
    ConvertIntToDecimalStringN(gStringVar2, dexNum, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringAppend(gStringVar1, gStringVar2);
    AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, x, 1 + 2, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

    AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, x + 32, 1 + 2, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gText_Dash);

#ifdef POKEMON_EXPANSION
    StringCopy(&speciesNameArray[0], GetSpeciesName(species));
#else
    StringCopy(&speciesNameArray[0], &gSpeciesNames[species][0]);
#endif
    AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, x + 40, 1 + 2, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, &speciesNameArray[0]);

    if(textId != 2)
    {
#ifdef POKEMON_EXPANSION
        speciesCategoryText = GetSpeciesCategory(species);
#else
        speciesCategoryText = GetPokedexCategoryName(dexNum);
#endif
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NARROW, x + 178 + GetStringCenterAlignXOffset(FONT_NARROW, speciesCategoryText, 52), y, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, speciesCategoryText);
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
        if(sStarterChoices[sBirchCaseDataPtr->handPosition].species != SPECIES_NONE)
            SampleUi_DrawMonIcon(sStarterChoices[sBirchCaseDataPtr->handPosition].species);
        gTasks[taskId].func = Task_BirchCaseMain;
        sBirchCaseDataPtr->movingSelector = FALSE;
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
        SetMainCallback2(sBirchCaseDataPtr->savedCallback);
        BirchCaseFreeResources();
        DestroyTask(taskId);
        VarSet(VAR_0x8004, gPlayerPartyCount - 1);
        ChangePokemonNickname();
    }
}

static void Task_BirchCaseRecievedMon(u8 taskId)
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
        gTasks[taskId].func = Task_BirchCaseTurnOff;
        return;
    }
}

static void Task_BirchCaseConfirmSelection(u8 taskId)
{
    if(JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        PrintTextToBottomBar(RECIEVED_MON);
        BirchCase_GiveMon();
        gTasks[taskId].func = Task_BirchCaseRecievedMon;
        return;
    }
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        PrintTextToBottomBar(CHOOSE_MON);
        gTasks[taskId].func = Task_BirchCaseMain;
        return;
    }
}


/* Main Grid Based Movement Control Flow*/
static void Task_BirchCaseMain(u8 taskId)
{
    u16 oldPosition = sBirchCaseDataPtr->handPosition;
    if (JOY_NEW(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        // Move up
        switch (sBirchCaseDataPtr->handPosition)
        {
            case 0: case 1: case 4: case 5: // Columns with 4 rows
                sBirchCaseDataPtr->handPosition += 14; // Move to Row 4
                break;
            case 2: case 3: // Columns with only top row
                sBirchCaseDataPtr->handPosition = sBirchCaseDataPtr->handPosition; // Stay in place
                break;
            case 6: case 7: case 8: case 9: // Row 2 to Row 1
                sBirchCaseDataPtr->handPosition -= 6;
                break;
            case 10: case 11: case 12: case 13: // Row 3 to Row 2
                sBirchCaseDataPtr->handPosition -= 4;
                break;
            case 14: case 15: case 16: case 17: // Row 4 to Row 3
                sBirchCaseDataPtr->handPosition -= 4;
                break;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if (JOY_NEW(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        // Move down
        switch (sBirchCaseDataPtr->handPosition)
        {
            case 0: case 1: case 4: case 5: // Row 1 to Row 2
                sBirchCaseDataPtr->handPosition += 6;
                break;
            case 2: case 3: // Columns with only top row
                sBirchCaseDataPtr->handPosition = sBirchCaseDataPtr->handPosition; // Stay in place
                break;
            case 6: case 7: case 8: case 9: // Row 2 to Row 3
                sBirchCaseDataPtr->handPosition += 4;
                break;
            case 10: case 11: case 12: case 13: // Row 3 to Row 4
                sBirchCaseDataPtr->handPosition += 4;
                break;
            case 14: case 15: case 16: case 17: // Bottom row, no further down
                sBirchCaseDataPtr->handPosition = sBirchCaseDataPtr->handPosition;
                break;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
        // Move right
        switch (sBirchCaseDataPtr->handPosition)
        {
            case 5: sBirchCaseDataPtr->handPosition = 0; break; // Wrap around Row 1
            case 9: sBirchCaseDataPtr->handPosition = 6; break; // Wrap around Row 2
            case 13: sBirchCaseDataPtr->handPosition = 10; break; // Wrap around Row 3
            case 17: sBirchCaseDataPtr->handPosition = 14; break; // Wrap around Row 4
            default: sBirchCaseDataPtr->handPosition += 1; break; // Move right
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        PlaySE(SE_SELECT);
        // Move left
        switch (sBirchCaseDataPtr->handPosition)
        {
            case 0: sBirchCaseDataPtr->handPosition = 5; break; // Wrap around Row 1
            case 6: sBirchCaseDataPtr->handPosition = 9; break; // Wrap around Row 2
            case 10: sBirchCaseDataPtr->handPosition = 13; break; // Wrap around Row 3
            case 14: sBirchCaseDataPtr->handPosition = 17; break; // Wrap around Row 4
            default: sBirchCaseDataPtr->handPosition -= 1; break; // Move left
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if (JOY_NEW(A_BUTTON))
    {
        if (sStarterChoices[sBirchCaseDataPtr->handPosition].species != SPECIES_NONE) // If spot is not empty
        {
            PlaySE(SE_SELECT);
            PrintTextToBottomBar(CONFIRM_SELECTION);
            gTasks[taskId].func = Task_BirchCaseConfirmSelection;
            return;
        }
        else
        {
            PlaySE(SE_BOO);
            return;
        }
    }
}
