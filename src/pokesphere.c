#include "pokesphere.h"

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
#include "international_string_util.h"
#include "random.h"

#include "bw_summary_screen.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_mugshot.h"
#include "ikigai_characters.h"

#define CHARACTER_OFFSET        1

enum ExploreProfilePositions
{
    EXPLORE_COORDS_X1_Y1,
    EXPLORE_COORDS_X2_Y1,
    EXPLORE_COORDS_X3_Y1,
    EXPLORE_COORDS_X4_Y1,
    EXPLORE_COORDS_X1_Y2,
    EXPLORE_COORDS_X2_Y2,
    EXPLORE_COORDS_X3_Y2,
    EXPLORE_COORDS_X4_Y2,
    EXPLORE_COORDS_COUNT,
};

struct SpriteCoordsStruct {
    u8 x;
    u8 y;
};

#define EXPLORE_COORDS_PER_ROW  EXPLORE_COORDS_COUNT / 2

#define EXPLORE_COORDS_X1       36
#define EXPLORE_COORDS_X2       92
#define EXPLORE_COORDS_X3       148
#define EXPLORE_COORDS_X4       204
#define EXPLORE_COORDS_Y1       77
#define EXPLORE_COORDS_Y2       125
#define EXPLORE_CURSOR_X_OFFSET 4
#define EXPLORE_CURSOR_Y_OFFSET 3

struct PokeSphereState
{
    MainCallback savedCallback;
    u8 loadState;
    u8 mode;
    u8 exploreOverworldSpriteId[EXPLORE_COORDS_COUNT];
    u8 exploreCursorSpriteId;
    u8 exploreCursorPosition;
    u8 exploreCharacterStartId;
    u8 characterId;
    u8 characterMugshotSpriteId;
    u8 characterTypeHeartSpriteId;
    u8 characterAttitudeSpriteId;
    u8 partnerMugshotSpriteId;
};

enum WindowIds
{
    WIN_UI_CONTROLS,
    WIN_CHARACTER_NAME,
    WIN_CHARACTER_RELATIONSHIPS_POSTS,
    WIN_CHARACTER_PROFILE_OPINION,
    WIN_CHARACTER_MUGSHOT,
};

enum Modes
{
    MODE_EXPLORE,
    MODE_PROFILE,
    MODE_POSTS,
    MODE_COUNT,
};

static EWRAM_DATA struct PokeSphereState *sPokeSphereState = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;

static const u8 *const sModeNames[MODE_COUNT] = {
    [MODE_EXPLORE]      = COMPOUND_STRING("Explore:"),
    [MODE_PROFILE]      = COMPOUND_STRING("Profile:"),
    [MODE_POSTS]        = COMPOUND_STRING("Posts:"),
};

static const struct SpriteCoordsStruct sExplorePageSpriteCords[EXPLORE_COORDS_COUNT] = {
    [EXPLORE_COORDS_X1_Y1] = {
        EXPLORE_COORDS_X1, EXPLORE_COORDS_Y1
    },
    [EXPLORE_COORDS_X2_Y1] = {
        EXPLORE_COORDS_X2, EXPLORE_COORDS_Y1
    },
    [EXPLORE_COORDS_X3_Y1] = {
        EXPLORE_COORDS_X3, EXPLORE_COORDS_Y1
    },
    [EXPLORE_COORDS_X4_Y1] = {
        EXPLORE_COORDS_X4, EXPLORE_COORDS_Y1
    },
    [EXPLORE_COORDS_X1_Y2] = {
        EXPLORE_COORDS_X1, EXPLORE_COORDS_Y2
    },
    [EXPLORE_COORDS_X2_Y2] = {
        EXPLORE_COORDS_X2, EXPLORE_COORDS_Y2
    },
    [EXPLORE_COORDS_X3_Y2] = {
        EXPLORE_COORDS_X3, EXPLORE_COORDS_Y2
    },
    [EXPLORE_COORDS_X4_Y2] = {
        EXPLORE_COORDS_X4, EXPLORE_COORDS_Y2
    },
};

static const struct BgTemplate sPokeSphereBgTemplates[] =
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
    }
};

static const struct WindowTemplate sPokeSphereWindowTemplates[] =
{
    [WIN_UI_CONTROLS] =
    {
        .bg = 0,
        .tilemapLeft = 20,
        .tilemapTop = 1,
        .width = 9,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_CHARACTER_NAME] =
    {
        .bg = 0,
        .tilemapLeft = 13,
        .tilemapTop = 2,
        .width = 6,
        .height = 3,
        .paletteNum = 15,
        .baseBlock = 1 + (9 * 5)
    },
    [WIN_CHARACTER_RELATIONSHIPS_POSTS] =
    {
        .bg = 0,
        .tilemapLeft = 20,
        .tilemapTop = 7,
        .width = 8,
        .height = 12,
        .paletteNum = 15,
        .baseBlock = 46 + (3 * 6)
    },
    [WIN_CHARACTER_PROFILE_OPINION] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 13,
        .width = 17,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 64 + (8 * 12)
    },
    [WIN_CHARACTER_MUGSHOT] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 13,
        .width = 17,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 160 + (17 * 6)
    },
    DUMMY_WIN_TEMPLATE
};

static const u32 sPokeSphereTiles[] = INCBIN_U32("graphics/pokesphere/tiles.4bpp.lz");
static const u16 sPokeSpherePalette[] = INCBIN_U16("graphics/pokesphere/tiles.gbapal");

static const u32 sPokeSphereTilemapExplore[] = INCBIN_U32("graphics/pokesphere/tilemap_explore.bin.lz");
static const u32 sPokeSphereTilemapProfile[] = INCBIN_U32("graphics/pokesphere/tilemap_profile.bin.lz");

static const u32 sPokeSphereExploreCursorGfx[] = INCBIN_U32("graphics/pokesphere/cursor.4bpp.lz");
static const u16 sPokeSphereExploreCursorPal[] = INCBIN_U16("graphics/pokesphere/cursor.gbapal");

static const u32 sPokeSphereHeartIconGfx[] = INCBIN_U32("graphics/pokesphere/heart.4bpp.lz");
static const u16 sPokeSphereHeartIconPal[] = INCBIN_U16("graphics/pokesphere/heart.gbapal");
static const u32 sPokeSphereInspiredIconGfx[] = INCBIN_U32("graphics/pokesphere/inspired.4bpp.lz");
static const u16 sPokeSphereInspiredIconPal[] = INCBIN_U16("graphics/pokesphere/inspired.gbapal");
static const u32 sPokeSphereHumbleIconGfx[] = INCBIN_U32("graphics/pokesphere/humble.4bpp.lz");
static const u16 sPokeSphereHumbleIconPal[] = INCBIN_U16("graphics/pokesphere/humble.gbapal");
static const u32 sPokeSphereDominantIconGfx[] = INCBIN_U32("graphics/pokesphere/dominant.4bpp.lz");
static const u16 sPokeSphereDominantIconPal[] = INCBIN_U16("graphics/pokesphere/dominant.gbapal");
static const u32 sPokeSphereCynicalIconGfx[] = INCBIN_U32("graphics/pokesphere/cynical.4bpp.lz");
static const u16 sPokeSphereCynicalIconPal[] = INCBIN_U16("graphics/pokesphere/cynical.gbapal");

#define TAG_POKESPHERE_CURSOR   21212
#define TAG_HEART_ICON          21213
#define TAG_ATTITUDE_ICON       21214
#define MUGSHOT_1               1
#define MUGSHOT_2               2

static const struct OamData sOamData_PokeSphereExploreCursor =
{
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 1,
};

static const struct CompressedSpriteSheet sSpriteSheet_PokeSphereExploreCursor =
{
    .data = sPokeSphereExploreCursorGfx,
    .size = 32*32*4/2,
    .tag = TAG_POKESPHERE_CURSOR,
};

static const struct SpritePalette sSpritePal_PokeSphereExploreCursor =
{
    .data = sPokeSphereExploreCursorPal,
    .tag = TAG_POKESPHERE_CURSOR
};

static const union AnimCmd sSpriteAnim_PokeSphereExploreCursor[] =
{
    ANIMCMD_FRAME(0, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sSpriteAnimTable_PokeSphereExploreCursor[] =
{
    sSpriteAnim_PokeSphereExploreCursor,
};

static void PokeSphereExploreCursorCallback(struct Sprite *sprite)
{
    struct SpriteCoordsStruct position;

    sprite->x = sExplorePageSpriteCords[sPokeSphereState->exploreCursorPosition].x + EXPLORE_CURSOR_X_OFFSET;
    sprite->y = sExplorePageSpriteCords[sPokeSphereState->exploreCursorPosition].y + EXPLORE_CURSOR_Y_OFFSET;
}

static const struct SpriteTemplate sSpriteTemplate_PokeSphereExploreCursor =
{
    .tileTag = TAG_POKESPHERE_CURSOR,
    .paletteTag = TAG_POKESPHERE_CURSOR,
    .oam = &sOamData_PokeSphereExploreCursor,
    .anims = sSpriteAnimTable_PokeSphereExploreCursor,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct OamData sOamData_PokeSphereHeartIcon =
{
    .size = SPRITE_SIZE(16x16),
    .shape = SPRITE_SHAPE(16x16),
    .priority = 1,
};

static const struct CompressedSpriteSheet sSpriteSheet_PokeSphereHeartIcon =
{
    .data = sPokeSphereHeartIconGfx,
    .size = 32*32*4/2,
    .tag = TAG_HEART_ICON,
};

static const struct SpritePalette sSpritePal_PokeSphereHeartIcon =
{
    .data = sPokeSphereHeartIconPal,
    .tag = TAG_HEART_ICON
};

static const struct SpriteTemplate sSpriteTemplate_PokeSphereHeartIcon =
{
    .tileTag = TAG_HEART_ICON,
    .paletteTag = TAG_HEART_ICON,
    .oam = &sOamData_PokeSphereHeartIcon,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct OamData sOamData_PokeSphereAttitudeIcon =
{
    .size = SPRITE_SIZE(16x16),
    .shape = SPRITE_SHAPE(16x16),
    .priority = 1,
};

static const struct SpritePalette sSpritePal_PokeSphereInspiredIcon =
{
    .data = sPokeSphereInspiredIconPal,
    .tag = TAG_ATTITUDE_ICON
};

static const struct SpritePalette sSpritePal_PokeSphereHumbleIcon =
{
    .data = sPokeSphereHumbleIconPal,
    .tag = TAG_ATTITUDE_ICON
};

static const struct SpritePalette sSpritePal_PokeSphereDominantIcon =
{
    .data = sPokeSphereDominantIconPal,
    .tag = TAG_ATTITUDE_ICON
};

static const struct SpritePalette sSpritePal_PokeSphereCynicalIcon =
{
    .data = sPokeSphereCynicalIconPal,
    .tag = TAG_ATTITUDE_ICON
};

static const struct CompressedSpriteSheet sSpriteSheet_PokeSphereInspiredIcon =
{
    .data = sPokeSphereInspiredIconGfx,
    .size = 5*32*32*4/2,
    .tag = TAG_ATTITUDE_ICON,
};

static const struct CompressedSpriteSheet sSpriteSheet_PokeSphereHumbleIcon =
{
    .data = sPokeSphereHumbleIconGfx,
    .size = 5*32*32*4/2,
    .tag = TAG_ATTITUDE_ICON,
};

static const struct CompressedSpriteSheet sSpriteSheet_PokeSphereDominantIcon =
{
    .data = sPokeSphereDominantIconGfx,
    .size = 5*32*32*4/2,
    .tag = TAG_ATTITUDE_ICON,
};

static const struct CompressedSpriteSheet sSpriteSheet_PokeSphereCynicalIcon =
{
    .data = sPokeSphereCynicalIconGfx,
    .size = 5*32*32*4/2,
    .tag = TAG_ATTITUDE_ICON,
};

static const union AnimCmd sSpriteAnim_PokeSphereAttitudeIcon[] =
{
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(4, 16),
    ANIMCMD_FRAME(8, 16),
    ANIMCMD_FRAME(12, 16),
    ANIMCMD_FRAME(16, 16),
    ANIMCMD_FRAME(12, 16, .hFlip = TRUE),
    ANIMCMD_FRAME(8, 16, .hFlip = TRUE),
    ANIMCMD_FRAME(4, 16, .hFlip = TRUE),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sSpriteAnimTable_PokeSphereAttitudeIcon[] =
{
    sSpriteAnim_PokeSphereAttitudeIcon,
};

static const struct SpriteTemplate sSpriteTemplate_PokeSphereInspiredIcon =
{
    .tileTag = TAG_ATTITUDE_ICON,
    .paletteTag = TAG_ATTITUDE_ICON,
    .oam = &sOamData_PokeSphereAttitudeIcon,
    .anims = sSpriteAnimTable_PokeSphereAttitudeIcon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct SpriteTemplate sSpriteTemplate_PokeSphereHumbleIcon =
{
    .tileTag = TAG_ATTITUDE_ICON,
    .paletteTag = TAG_ATTITUDE_ICON,
    .oam = &sOamData_PokeSphereAttitudeIcon,
    .anims = sSpriteAnimTable_PokeSphereAttitudeIcon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct SpriteTemplate sSpriteTemplate_PokeSphereDominantIcon =
{
    .tileTag = TAG_ATTITUDE_ICON,
    .paletteTag = TAG_ATTITUDE_ICON,
    .oam = &sOamData_PokeSphereAttitudeIcon,
    .anims = sSpriteAnimTable_PokeSphereAttitudeIcon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct SpriteTemplate sSpriteTemplate_PokeSphereAttitudeIcon =
{
    .tileTag = TAG_ATTITUDE_ICON,
    .paletteTag = TAG_ATTITUDE_ICON,
    .oam = &sOamData_PokeSphereAttitudeIcon,
    .anims = sSpriteAnimTable_PokeSphereAttitudeIcon,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

#define CHARACTER_MUGSHOT_X     57
#define CHARACTER_MUGSHOT_Y     59
#define CHARACTER_TYPE_X        75
#define CHARACTER_HEART_X       84
#define CHARACTER_ATTITUDE_X    28
#define CHARACTER_ICON_Y        26
#define CHARACTER_PARTNER_X     116
#define CHARACTER_PARTNER_Y     60

enum FontColor
{
    FONT_WHITE,
    FONT_GRAY,
    FONT_RED,
    FONT_GREEN,
    FONT_BLUE,
    FONT_LIGHT_BLUE,
};

static const u8 sPokeSphereWindowFontColors[][3] =
{
    [FONT_WHITE]        = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_DARK_GRAY},
    [FONT_GRAY]         = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY},
    [FONT_RED]          = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_GRAY},
    [FONT_GREEN]        = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_GREEN,      TEXT_COLOR_LIGHT_GRAY},
    [FONT_BLUE]         = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_GRAY},
    [FONT_LIGHT_BLUE]   = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_BLUE, TEXT_COLOR_LIGHT_GRAY},
};

// Callbacks for the PokeSphere
static void PokeSphere_SetupCB(void);
static void PokeSphere_MainCB(void);
static void PokeSphere_VBlankCB(void);

// PokeSphere tasks
static void Task_PokeSphereWaitFadeIn(u8 taskId);
static void Task_PokeSphereMainInput(u8 taskId);
static void Task_PokeSphereWaitFadeAndBail(u8 taskId);
static void Task_PokeSphereWaitFadeAndExitGracefully(u8 taskId);
static void Task_PokeSphereWaitFadeOutAndChangeBackground(u8 taskId);
static void Task_PokeSphereLoadSpritesAndTextAfterFade(u8 taskId);

// PokeSphere helper functions
static void PokeSphere_Init(MainCallback callback);
static void PokeSphere_ResetGpuRegsAndBgs(void);
static bool8 PokeSphere_InitBgs(void);
static void PokeSphere_FadeAndBail(void);
static bool8 PokeSphere_LoadGraphics(void);
static void PokeSphere_InitWindows(void);
static void PokeSphere_CreateExplorePage(void);
static void PokeSphere_DestroyExplorePage(void);
static void PokeSphere_CreateProfilePostPage(void);
static void PokeSphere_DestroyProfilePostPage(void);
static void PokeSphere_Explore_CreateObjectEvents(void);
static void PokeSphere_Explore_DestroyObjectEvents(void);
static void PokeSphere_Explore_CreateCursor(void);
static void PokeSphere_Explore_DestroyCursor(void);
static void PokeSphere_DrawCharacterMugshot(void);
static void PokeSphere_DrawCharacterAttitude(void);
static void PokeSphere_DrawCharacterTypeHeart(void);
static void PokeSphere_DrawPartnerMugshot(void);
static void PokeSphere_PrintUIControls(void);
static void PokeSphere_ReloadText(void);
static void PokeSphere_CycleCharacters(bool32 increment);
static void PokeSphere_ReloadProfile(void);
static void PokeSphere_PrintNames(void);
static void PokeSphere_PrintRelationships(void);
static void PokeSphere_PrintPosts(void);
static void PokeSphere_PrintProfile(void);
static void PokeSphere_PrintOpinion(void);
static void PokeSphere_FreeResources(void);

// Declared in pokesphere.h
void Task_OpenPokeSphere(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        PokeSphere_Init(CB2_ReturnToFieldWithOpenMenu);
        DestroyTask(taskId);
    }
}

static void PokeSphere_Init(MainCallback callback)
{
    sPokeSphereState = AllocZeroed(sizeof(struct PokeSphereState));
    if (sPokeSphereState == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sPokeSphereState->loadState = 0;
    sPokeSphereState->savedCallback = callback;

    SetMainCallback2(PokeSphere_SetupCB);
}

// Credit: Jaizu, pret
static void PokeSphere_ResetGpuRegsAndBgs(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    ChangeBgX(0, 0, BG_COORD_SET);
    ChangeBgY(0, 0, BG_COORD_SET);
    ChangeBgX(1, 0, BG_COORD_SET);
    ChangeBgY(1, 0, BG_COORD_SET);
    ChangeBgX(2, 0, BG_COORD_SET);
    ChangeBgY(2, 0, BG_COORD_SET);
    ChangeBgX(3, 0, BG_COORD_SET);
    ChangeBgY(3, 0, BG_COORD_SET);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WIN1H, 0);
    SetGpuReg(REG_OFFSET_WIN1V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    CpuFill16(0, (void *)VRAM, VRAM_SIZE);
    CpuFill32(0, (void *)OAM, OAM_SIZE);
}

static void PokeSphere_SetupCB(void)
{
    switch (gMain.state)
    {
    case 0:
        PokeSphere_ResetGpuRegsAndBgs();
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
        if (PokeSphere_InitBgs())
        {
            sPokeSphereState->loadState = 0;
            gMain.state++;
        }
        else
        {
            PokeSphere_FadeAndBail();
            return;
        }
        break;
    case 3:
        if (PokeSphere_LoadGraphics() == TRUE)
        {
            gMain.state++;
        }
        break;
    case 4:
        PokeSphere_InitWindows();
        gMain.state++;
        break;
    case 5:
        sPokeSphereState->exploreCharacterStartId = CHARACTER_PLAYER;
        sPokeSphereState->exploreCursorPosition = EXPLORE_COORDS_X1_Y1;
        PokeSphere_CreateExplorePage();
        CreateTask(Task_PokeSphereWaitFadeIn, 0);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    case 7:
        SetVBlankCallback(PokeSphere_VBlankCB);
        SetMainCallback2(PokeSphere_MainCB);
        break;
    }
}

static void PokeSphere_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void PokeSphere_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void Task_PokeSphereWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_PokeSphereMainInput;
    }
}

static void Task_PokeSphereMainInput(u8 taskId)
{
    if (sPokeSphereState->mode == MODE_EXPLORE)
    {
        if (JOY_NEW(A_BUTTON))
        {
            sPokeSphereState->characterId = sPokeSphereState->exploreCharacterStartId + sPokeSphereState->exploreCursorPosition;
            
            if (sPokeSphereState->characterId >= CHARACTER_RESIDENT_COUNT)
            {
                sPokeSphereState->characterId++;
            }

            if (!IkigaiCharacter_GetMetFlag(sPokeSphereState->characterId)
                || sPokeSphereState->characterId >= CHARACTER_COUNT_TOTAL)
            {
                PlaySE(SE_BOO);
            }
            else
            {
                sPokeSphereState->mode = MODE_PROFILE;
                PlaySE(SE_RG_BAG_POCKET);
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
                gTasks[taskId].func = Task_PokeSphereWaitFadeOutAndChangeBackground;
            }
        }

        if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_POKENAV_HANG_UP);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            gTasks[taskId].func = Task_PokeSphereWaitFadeAndExitGracefully;
        }

        if (JOY_REPEAT(DPAD_UP))
        {
            if (sPokeSphereState->exploreCursorPosition >= EXPLORE_COORDS_X1_Y2)
            {
                sPokeSphereState->exploreCursorPosition -= EXPLORE_COORDS_PER_ROW;
                PlaySE(SE_BALL_TRAY_BALL);
            }
            else if (sPokeSphereState->exploreCharacterStartId == CHARACTER_PLAYER)
            {
                PlaySE(SE_BALL);
            } 
            else
            {
                sPokeSphereState->exploreCharacterStartId = sPokeSphereState->exploreCharacterStartId - EXPLORE_COORDS_PER_ROW;
                PokeSphere_Explore_DestroyObjectEvents();
                PokeSphere_Explore_CreateObjectEvents();
                PlaySE(SE_BALL_TRAY_BALL);
            }
        }

        if (JOY_REPEAT(DPAD_DOWN))
        {
            if (sPokeSphereState->exploreCursorPosition < EXPLORE_COORDS_X1_Y2)
            {
                sPokeSphereState->exploreCursorPosition += EXPLORE_COORDS_PER_ROW;
                PlaySE(SE_BALL_TRAY_BALL);
            }
            else if (sPokeSphereState->exploreCharacterStartId >= CHARACTER_COUNT_TOTAL - EXPLORE_COORDS_COUNT - CHARACTER_OFFSET)
            {
                PlaySE(SE_BALL);
            } 
            else
            {
                sPokeSphereState->exploreCharacterStartId = sPokeSphereState->exploreCharacterStartId + EXPLORE_COORDS_PER_ROW;
                PokeSphere_Explore_DestroyObjectEvents();
                PokeSphere_Explore_CreateObjectEvents();
                PlaySE(SE_BALL_TRAY_BALL);
            }
        }

        if (JOY_REPEAT(DPAD_LEFT))
        {
            if (sPokeSphereState->exploreCursorPosition == EXPLORE_COORDS_X1_Y1)
            {
                sPokeSphereState->exploreCursorPosition = EXPLORE_COORDS_X4_Y1;
            }
            else if (sPokeSphereState->exploreCursorPosition == EXPLORE_COORDS_X1_Y2)
            {
                sPokeSphereState->exploreCursorPosition = EXPLORE_COORDS_X4_Y2;
            }
            else
            {
                sPokeSphereState->exploreCursorPosition--;
            }
            PlaySE(SE_BALL_TRAY_BALL);
        }

        if (JOY_REPEAT(DPAD_RIGHT))
        {
            if (sPokeSphereState->exploreCursorPosition == EXPLORE_COORDS_X4_Y1)
            {
                sPokeSphereState->exploreCursorPosition = EXPLORE_COORDS_X1_Y1;
            }
            else if (sPokeSphereState->exploreCursorPosition == EXPLORE_COORDS_X4_Y2)
            {
                sPokeSphereState->exploreCursorPosition = EXPLORE_COORDS_X1_Y2;
            }
            else
            {
                sPokeSphereState->exploreCursorPosition++;
            }
            PlaySE(SE_BALL_TRAY_BALL);
        }

        if (JOY_REPEAT(DPAD_ANY))
        {
            PokeSphere_PrintNames();
        }
    }


    else if (sPokeSphereState->mode == MODE_PROFILE || sPokeSphereState->mode == MODE_POSTS)
    {
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_RG_BAG_CURSOR);

            if (sPokeSphereState->mode == MODE_PROFILE)
            {
                sPokeSphereState->mode = MODE_POSTS;
            }
            else
            {
                sPokeSphereState->mode = MODE_PROFILE;
            }
            PokeSphere_ReloadText();
        }

        if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_RG_BALL_CLICK);
            sPokeSphereState->mode = MODE_EXPLORE;
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            gTasks[taskId].func = Task_PokeSphereWaitFadeOutAndChangeBackground;
        }
        
        if (JOY_REPEAT(DPAD_LEFT))
        {
            PokeSphere_CycleCharacters(FALSE);
            PokeSphere_ReloadProfile();
            PokeSphere_ReloadText();
        }

        if (JOY_REPEAT(DPAD_RIGHT))
        {
            PokeSphere_CycleCharacters(TRUE);
            PokeSphere_ReloadProfile();
            PokeSphere_ReloadText();
        }
    }
}

static void Task_PokeSphereWaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sPokeSphereState->savedCallback);
        PokeSphere_FreeResources();
        DestroyTask(taskId);
    }
}

static void Task_PokeSphereWaitFadeAndExitGracefully(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sPokeSphereState->savedCallback);
        PokeSphere_FreeResources();
        DestroyTask(taskId);
    }
}
#define TILEMAP_BUFFER_SIZE (1024 * 2)
static bool8 PokeSphere_InitBgs(void)
{
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
    if (sBg1TilemapBuffer == NULL)
    {
        return FALSE;
    }

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sPokeSphereBgTemplates, NELEMS(sPokeSphereBgTemplates));

    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);

    ShowBg(0);
    ShowBg(1);

    return TRUE;
}
#undef TILEMAP_BUFFER_SIZE

static void PokeSphere_FadeAndBail(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_PokeSphereWaitFadeAndBail, 0);
    SetVBlankCallback(PokeSphere_VBlankCB);
    SetMainCallback2(PokeSphere_MainCB);
}

static bool8 PokeSphere_LoadGraphics(void)
{
    switch (sPokeSphereState->loadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sPokeSphereTiles, 0, 0, 0);
        sPokeSphereState->loadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sPokeSphereTilemapExplore, sBg1TilemapBuffer);
            sPokeSphereState->loadState++;
        }
        break;
    case 2:
        LoadPalette(sPokeSpherePalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        LoadPalette(GetTextWindowPalette(gSaveBlock2Ptr->optionsInterfaceColor + DEFAULT_TEXT_BOX_FRAME_PALETTES), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        sPokeSphereState->loadState++;
    default:
        sPokeSphereState->loadState = 0;
        return TRUE;
    }
    return FALSE;
}

#define tLoadSprites data[0]
static void Task_PokeSphereWaitFadeOutAndChangeBackground(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        if (sPokeSphereState->mode == MODE_PROFILE || sPokeSphereState->mode == MODE_POSTS)
        {
            if (FreeTempTileDataBuffersIfPossible() != TRUE)
            {
                PokeSphere_DestroyExplorePage();
                LZDecompressWram(sPokeSphereTilemapProfile, sBg1TilemapBuffer);
            }
        }
        else if (sPokeSphereState->mode == MODE_EXPLORE)
        {
            if (FreeTempTileDataBuffersIfPossible() != TRUE)
            {
                PokeSphere_DestroyProfilePostPage();
                LZDecompressWram(sPokeSphereTilemapExplore, sBg1TilemapBuffer);
            }
        }
        ScheduleBgCopyTilemapToVram(1);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gTasks[taskId].tLoadSprites = FALSE;
        gTasks[taskId].func = Task_PokeSphereLoadSpritesAndTextAfterFade;
    }
}

static void Task_PokeSphereLoadSpritesAndTextAfterFade(u8 taskId)
{
    if (gPaletteFade.active && gTasks[taskId].tLoadSprites == FALSE)
    {
        if (sPokeSphereState->mode == MODE_PROFILE || sPokeSphereState->mode == MODE_POSTS)
        {
            PokeSphere_CreateProfilePostPage();
        }
        else if (sPokeSphereState->mode == MODE_EXPLORE)
        {
            PokeSphere_CreateExplorePage();
        }
        gTasks[taskId].tLoadSprites = TRUE;
    }
    else
        gTasks[taskId].func = Task_PokeSphereWaitFadeIn;
}
#undef tLoadSprites

static void PokeSphere_InitWindows(void)
{
    InitWindows(sPokeSphereWindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    
    FillWindowPixelBuffer(WIN_UI_CONTROLS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_CHARACTER_NAME, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_CHARACTER_RELATIONSHIPS_POSTS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_CHARACTER_PROFILE_OPINION, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    PutWindowTilemap(WIN_UI_CONTROLS);
    PutWindowTilemap(WIN_CHARACTER_NAME);
    PutWindowTilemap(WIN_CHARACTER_RELATIONSHIPS_POSTS);
    PutWindowTilemap(WIN_CHARACTER_PROFILE_OPINION);
    
    CopyWindowToVram(WIN_UI_CONTROLS, COPYWIN_FULL);
    CopyWindowToVram(WIN_CHARACTER_NAME, COPYWIN_FULL);
    CopyWindowToVram(WIN_CHARACTER_RELATIONSHIPS_POSTS, COPYWIN_FULL);
    CopyWindowToVram(WIN_CHARACTER_PROFILE_OPINION, COPYWIN_FULL);
}

static void PokeSphere_CreateExplorePage(void)
{
    PokeSphere_Explore_CreateObjectEvents();
    PokeSphere_Explore_CreateCursor();
    PokeSphere_PrintUIControls();
    PokeSphere_PrintNames();
}

static void PokeSphere_DestroyExplorePage(void)
{
    PokeSphere_Explore_DestroyObjectEvents();
    PokeSphere_Explore_DestroyCursor();
    FreeAllSpritePalettes();
}

static void PokeSphere_CreateProfilePostPage(void)
{
    PokeSphere_DrawCharacterMugshot();
    PokeSphere_DrawPartnerMugshot();
    PokeSphere_DrawCharacterAttitude();
    PokeSphere_DrawCharacterTypeHeart();
    PokeSphere_PrintUIControls();
    PokeSphere_PrintNames();
    PokeSphere_PrintRelationships();
    PokeSphere_PrintProfile();
}

static void PokeSphere_DestroyProfilePostPage(void)
{
    DestroyFieldMugshotSprite(sPokeSphereState->characterMugshotSpriteId, MUGSHOT_1);
    DestroyFieldMugshotSprite(sPokeSphereState->partnerMugshotSpriteId, MUGSHOT_2);
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->characterTypeHeartSpriteId]);
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->characterAttitudeSpriteId]);
    FillWindowPixelBuffer(WIN_CHARACTER_RELATIONSHIPS_POSTS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_CHARACTER_PROFILE_OPINION, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    CopyWindowToVram(WIN_CHARACTER_RELATIONSHIPS_POSTS, COPYWIN_FULL);
    CopyWindowToVram(WIN_CHARACTER_PROFILE_OPINION, COPYWIN_FULL);
    FreeAllSpritePalettes();
}

static void PokeSphere_PrintUIControls(void)
{
    u8 textTop[20];
    u8 textMiddle[20];
    u8 textBottom[20];
    u8 x = 5, y = 1, yRow = 11;

    if (sPokeSphereState->mode == MODE_EXPLORE)
    {
        StringCopy(textTop, COMPOUND_STRING("{DPAD_LEFTRIGHT} Explore Profiles"));
        StringCopy(textMiddle, COMPOUND_STRING("{A_BUTTON} View Profile"));
        StringCopy(textBottom, COMPOUND_STRING("{B_BUTTON} Exit"));
    }
    else if (sPokeSphereState->mode == MODE_PROFILE || sPokeSphereState->mode == MODE_POSTS)
    {
        StringCopy(textTop, COMPOUND_STRING("{DPAD_LEFTRIGHT} Change Profile"));
        StringCopy(textMiddle, COMPOUND_STRING("{A_BUTTON} Change View"));
        StringCopy(textBottom, COMPOUND_STRING("{B_BUTTON} Explore Profiles"));
    }

    FillWindowPixelBuffer(WIN_UI_CONTROLS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    AddTextPrinterParameterized4(WIN_UI_CONTROLS, FONT_SMALL_NARROWER, x, y + (yRow * 0), 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        textTop
    );
    AddTextPrinterParameterized4(WIN_UI_CONTROLS, FONT_SMALL_NARROWER, x, y + (yRow * 1), 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        textMiddle
    );
    AddTextPrinterParameterized4(WIN_UI_CONTROLS, FONT_SMALL_NARROWER, x, y + (yRow * 2), 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        textBottom
    );

    CopyWindowToVram(WIN_UI_CONTROLS, COPYWIN_GFX);
}

static void PokeSphere_CycleCharacters(bool32 increment)
{
    u32 initialCharacter = sPokeSphereState->characterId;
    s32 characterNext = initialCharacter;

    while (TRUE)
    {
        if (increment)
            characterNext++;
        else
            characterNext--;

        if (characterNext > CHARACTER_LAST)
            characterNext = CHARACTER_PLAYER;
        else if (characterNext < CHARACTER_PLAYER)
            characterNext = CHARACTER_LAST;

        if (characterNext == initialCharacter)
        {
            PlaySE(SE_BALL);
            return;
        }

        if (characterNext == CHARACTER_RESIDENT_COUNT)
        {
            if (increment)
                characterNext++;
            else
                characterNext--;
        }

        if (IkigaiCharacter_GetMetFlag(characterNext))
            break;
    }

    sPokeSphereState->characterId = characterNext;
    PlaySE(SE_BALL_TRAY_BALL);
}

static void PokeSphere_ReloadProfile(void)
{
    PokeSphere_PrintNames();
    DestroyFieldMugshotSprite(sPokeSphereState->characterMugshotSpriteId, MUGSHOT_1);
    DestroyFieldMugshotSprite(sPokeSphereState->partnerMugshotSpriteId, MUGSHOT_2);
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->characterTypeHeartSpriteId]);
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->characterAttitudeSpriteId]);
    FreeAllSpritePalettes();
    PokeSphere_DrawCharacterMugshot();
    PokeSphere_DrawPartnerMugshot();
    PokeSphere_DrawCharacterAttitude();
    PokeSphere_DrawCharacterTypeHeart();
}

static void PokeSphere_ReloadText(void)
{
    switch (sPokeSphereState->mode)
    {
    case MODE_POSTS:
        PokeSphere_PrintPosts();
        PokeSphere_PrintOpinion();
        break;

    case MODE_PROFILE:
        PokeSphere_PrintRelationships();
        PokeSphere_PrintProfile();
        break;

    case MODE_EXPLORE:
    default:
        break;
    }
}

static void PokeSphere_Explore_CreateObjectEvents(void)
{
    u16 objEvent;
    u8 x, y, character, characterStart = sPokeSphereState->exploreCharacterStartId; 
    for (u8 coord = 0; coord < EXPLORE_COORDS_COUNT; coord++)
    {
        if (characterStart + coord == CHARACTER_RESIDENT_COUNT)
        {
            characterStart++;
        }
        else if (characterStart + coord >= CHARACTER_COUNT_TOTAL)
        {
            break;
        }
        character = characterStart + coord;
        x = sExplorePageSpriteCords[coord].x;
        y = sExplorePageSpriteCords[coord].y;
        if (character == CHARACTER_PLAYER)
            objEvent = gSaveBlock2Ptr->playerGender ? OBJ_EVENT_GFX_ANKA_NORMAL : OBJ_EVENT_GFX_KOLE_NORMAL;
        else
            objEvent = gIkigaiCharactersInfo[character].overworldGraphicsId;
        sPokeSphereState->exploreOverworldSpriteId[coord] = CreateObjectGraphicsSprite(objEvent, SpriteCallbackDummy, x, y, 102);

        if (IkigaiCharacter_GetMetFlag(character))
            StartSpriteAnim(&gSprites[sPokeSphereState->exploreOverworldSpriteId[coord]], ANIM_STD_GO_SOUTH);
        else
            LoadPalette(
                (u16 [PLTT_SIZE_4BPP]) { // Black Palette to show 'locked' state.
                    RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
                    RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
                    RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
                    RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0),
                },
                OBJ_PLTT_ID(IndexOfSpritePaletteTag(gSprites[sPokeSphereState->exploreOverworldSpriteId[coord]].template->paletteTag)),
                PLTT_SIZE_4BPP
            );
    }
}

static void PokeSphere_Explore_DestroyObjectEvents(void)
{
    for (u8 coord = 0; coord < EXPLORE_COORDS_COUNT; coord++)
    {
        DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->exploreOverworldSpriteId[coord]]);
    }
    FreeAllSpritePalettes();
}

static void PokeSphere_Explore_CreateCursor(void)
{
    LoadCompressedSpriteSheet(&sSpriteSheet_PokeSphereExploreCursor);
    LoadSpritePalette(&sSpritePal_PokeSphereExploreCursor);
    sPokeSphereState->exploreCursorSpriteId = CreateSprite(&sSpriteTemplate_PokeSphereExploreCursor,
        sExplorePageSpriteCords[sPokeSphereState->exploreCursorPosition].x + EXPLORE_CURSOR_X_OFFSET,
        sExplorePageSpriteCords[sPokeSphereState->exploreCursorPosition].y + EXPLORE_CURSOR_Y_OFFSET,
        0
    );
    gSprites[sPokeSphereState->exploreCursorSpriteId].callback = PokeSphereExploreCursorCallback;
}

static void PokeSphere_Explore_DestroyCursor(void)
{
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->exploreCursorSpriteId]);
}

static void PokeSphere_PrintNames(void)
{   
    u8 x, y = 0;
    u32 character = sPokeSphereState->characterId;
    u32 textColour = FONT_GRAY;
    const u8 *name = gIkigaiCharactersInfo[character].name;
    
    if (character == CHARACTER_PLAYER)
        name = gSaveBlock2Ptr->playerName;

    switch (gIkigaiCharactersInfo[character].personality)
    {
    case ATTITUDE_CYNICAL:
        textColour = FONT_LIGHT_BLUE;
        break;
        
    case ATTITUDE_DOMINANT:
        textColour = FONT_RED;
        break;
        
    case ATTITUDE_HUMBLE:
        textColour = FONT_GREEN;
        break;
        
    case ATTITUDE_INSPIRED:
        textColour = FONT_BLUE;
        break;
    }

    FillWindowPixelBuffer(WIN_CHARACTER_NAME, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    if (sPokeSphereState->mode == MODE_EXPLORE)
    {
        u8 character = sPokeSphereState->exploreCharacterStartId + sPokeSphereState->exploreCursorPosition;
        if (character >= CHARACTER_RESIDENT_COUNT)
            character++;

        if (!IkigaiCharacter_GetMetFlag(character)
            || character >= CHARACTER_COUNT_TOTAL)
            name = gIkigaiCharactersInfo[CHARACTER_DEFAULT].name;
        else if (character != CHARACTER_PLAYER)
            name = gIkigaiCharactersInfo[character].name;
        
        y = 4;
        textColour = FONT_GRAY;
    }

    x = GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
        name,
        48
    );
    AddTextPrinterParameterized4(WIN_CHARACTER_NAME, FONT_SMALL_NARROWER, x, y, 0, 0,
        sPokeSphereWindowFontColors[textColour], TEXT_SKIP_DRAW,
        name
    );

    if (sPokeSphereState->mode != MODE_EXPLORE && character != CHARACTER_PLAYER)
    {
        x = GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
            gSpeciesInfo[gIkigaiCharactersInfo[character].partnerPokemon].speciesName,
            48
        );
        AddTextPrinterParameterized4(WIN_CHARACTER_NAME, FONT_SMALL_NARROWER, x, 9, 0, 0,
            sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
            gSpeciesInfo[gIkigaiCharactersInfo[character].partnerPokemon].speciesName
        );
    }
    else if (sPokeSphereState->mode != MODE_EXPLORE && character == CHARACTER_PLAYER)
    {
        x = GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
            gSpeciesInfo[VarGet(VAR_STARTER_MON)].speciesName,
            48
        );
        AddTextPrinterParameterized4(WIN_CHARACTER_NAME, FONT_SMALL_NARROWER, x, 9, 0, 0,
            sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
            gSpeciesInfo[VarGet(VAR_STARTER_MON)].speciesName
        );
    }
    

    CopyWindowToVram(WIN_CHARACTER_NAME, COPYWIN_GFX);
}

static void PokeSphere_PrintRelationships(void)
{
    u8 x, relationship;
    u8 y, i = 0;
    const u8 *stringBuffer;

    FillWindowPixelBuffer(WIN_CHARACTER_RELATIONSHIPS_POSTS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    stringBuffer = COMPOUND_STRING("Tagged Profiles");
    x = GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
        stringBuffer,
        64
    );
    AddTextPrinterParameterized4(WIN_CHARACTER_RELATIONSHIPS_POSTS, FONT_SMALL_NARROWER, x, 2, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        stringBuffer
    );

    y = 5;
    for (i = 0; i < MAX_RELATIONSHIPS; i++)
    {
        relationship = gIkigaiCharactersInfo[sPokeSphereState->characterId].relationships[i].characterId;
        stringBuffer = gIkigaiCharactersInfo[sPokeSphereState->characterId].relationships[i].title;

        if (relationship != CHARACTER_DEFAULT && IkigaiCharacter_GetMetFlag(relationship))
        {
            y += 11;
            x = GetStringRightAlignXOffset(FONT_SMALL_NARROWER,
                gIkigaiCharactersInfo[relationship].name,
                64
            );
            AddTextPrinterParameterized4(WIN_CHARACTER_RELATIONSHIPS_POSTS, FONT_SMALL_NARROWER, x - 5, y, 0, 0,
                sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
                gIkigaiCharactersInfo[relationship].name
            );
            y += 11;
            x = GetStringRightAlignXOffset(FONT_SMALL_NARROWER,
                stringBuffer,
                64
            );
            AddTextPrinterParameterized4(WIN_CHARACTER_RELATIONSHIPS_POSTS, FONT_SMALL_NARROWER, x - 5, y, 0, 0,
                sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
                stringBuffer
            );
            y += 3;
        }
    }
    
    CopyWindowToVram(WIN_CHARACTER_RELATIONSHIPS_POSTS, COPYWIN_GFX);
}

static void PokeSphere_PrintPosts(void)
{
    const u8 *stringBuffer = COMPOUND_STRING("Latest Posts");
    u8 x, y, i, printed;
    bool32 bonus;

    // Define a struct to hold function index and type (kindness/strength)
    typedef struct
    {
        u8 index;
        bool32 isKindness; // TRUE for kindness, FALSE for strength
    } OpinionEntry;

    OpinionEntry entries[OB_FUNCTION_COUNT * 2]; // Double the size for both types
    u8 totalEntries = 0;

    // Fill the entries array with indices for both kindness and strength
    for (i = 0; i < OB_FUNCTION_COUNT; i++)
    {
        entries[totalEntries++] = (OpinionEntry){ .index = i, .isKindness = TRUE };
        entries[totalEntries++] = (OpinionEntry){ .index = i, .isKindness = FALSE };
    }

    // Shuffle the entries array
    for (i = totalEntries - 1; i > 0; i--)
    {
        u8 j = Random() % (i + 1); // Random index
        OpinionEntry temp = entries[i];
        entries[i] = entries[j];
        entries[j] = temp;
    }

    FillWindowPixelBuffer(WIN_CHARACTER_RELATIONSHIPS_POSTS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    x = GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
        stringBuffer,
        64
    );
    AddTextPrinterParameterized4(WIN_CHARACTER_RELATIONSHIPS_POSTS, FONT_SMALL_NARROWER, x, 2, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        stringBuffer
    );

    y = 17;
    printed = 0;

    if (IkigaiCharacter_GetRomanticFlag(sPokeSphereState->characterId))
    {
        AddTextPrinterParameterized4(WIN_CHARACTER_RELATIONSHIPS_POSTS, FONT_SMALL_NARROWER, 5, y, 0, 1,
                sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW, COMPOUND_STRING(
                    "We have a very\nclose bond."
                )
        );
        y += 25;
        printed++;
    }

    // Iterate through the shuffled entries
    for (i = 0; i < totalEntries; i++)
    {
        if (printed == MAX_POSTS)
            break;
        
        u8 funcIndex = entries[i].index;
        bool32 isKindness = entries[i].isKindness;

        // Call the appropriate function based on the entry type
        bonus = gOpinionBonusFunction[funcIndex].function(sPokeSphereState->characterId, isKindness);

        // Print the associated text if the bonus is non-zero
        if (bonus != 0)
        {
            const u8 *textToPrint = isKindness
                ? gOpinionBonusFunction[funcIndex].stringKindness
                : gOpinionBonusFunction[funcIndex].stringStrength;

            AddTextPrinterParameterized4(WIN_CHARACTER_RELATIONSHIPS_POSTS, FONT_SMALL_NARROWER, 5, y, 0, 1,
                sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW, textToPrint
            );
            y += 25;
            printed++;
        }
    }

    CopyWindowToVram(WIN_CHARACTER_RELATIONSHIPS_POSTS, COPYWIN_GFX);
}

static void PokeSphere_PrintProfile(void)
{
    const u8 *stringBuffer = COMPOUND_STRING("Highlights");

    FillWindowPixelBuffer(WIN_CHARACTER_PROFILE_OPINION, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    AddTextPrinterParameterized4(WIN_CHARACTER_PROFILE_OPINION, FONT_SMALL_NARROWER,
        GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
                stringBuffer,
                136
        ),
        1, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        stringBuffer
    );

    AddTextPrinterParameterized4(WIN_CHARACTER_PROFILE_OPINION, FONT_SMALL_NARROWER, 5, 13, 0, 1,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        gIkigaiCharactersInfo[sPokeSphereState->characterId].highlights
    );

    CopyWindowToVram(WIN_CHARACTER_PROFILE_OPINION, COPYWIN_GFX);
}

static void PokeSphere_PrintOpinion(void)
{
    u8 string[100];
    u8 opinion[3];
    u32 character = sPokeSphereState->characterId;
    s8 opinionKindness = IkigaiCharacter_GetKindness(character);
    s8 opinionStrength = IkigaiCharacter_GetStrength(character);

    if (character > CHARACTER_RESIDENT_COUNT)
    {
        opinionKindness = IkigaiCharacter_GetKindness_Wayfarer(character);
        opinionStrength = IkigaiCharacter_GetStrength_Wayfarer(character);
    }

    FillWindowPixelBuffer(WIN_CHARACTER_PROFILE_OPINION, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    StringCopy(string, COMPOUND_STRING("Vyraton Gym Leader Opinion"));
    AddTextPrinterParameterized4(WIN_CHARACTER_PROFILE_OPINION, FONT_SMALL_NARROWER,
        GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
                string,
                136
        ),
        1, 0, 0,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        string
    );

    if (character != CHARACTER_PLAYER)
    {
        StringCopy(string, COMPOUND_STRING("Here's what I think about "));

        if (IkigaiCharacter_NicknameInsteadOfName(character))
        {
            StringAppend(string, gSaveBlock3Ptr->characters.playerNickname);
        }
        else
        {
            StringAppend(string, gSaveBlock2Ptr->playerName);
        }

        StringAppend(string, COMPOUND_STRING(
            ".\nI rate their kindness "
        ));

        if (opinionKindness < 0)
        {
            opinionKindness *= -1;
            StringAppend(string, COMPOUND_STRING("-"));
        }

        if (opinionKindness != 0)
        {
            ConvertIntToDecimalStringN(opinion, opinionKindness, STR_CONV_MODE_LEFT_ALIGN, 3);
            StringAppend(string, opinion);
        }
        else
        {
            StringAppend(string, gDialogueAttitudes[ATTITUDE_NEUTRAL].nameLower);
        }

        StringAppend(string, COMPOUND_STRING(
            ".\nI rate their strength "
        ));

        if (opinionStrength < 0)
        {
            opinionStrength *= -1;
            StringAppend(gStringVar1, COMPOUND_STRING("-"));
        }

        if (opinionStrength != 0)
        {
            ConvertIntToDecimalStringN(opinion, opinionStrength, STR_CONV_MODE_LEFT_ALIGN, 3);
            StringAppend(string, opinion);
        }
        else
        {
            StringAppend(string, gDialogueAttitudes[ATTITUDE_NEUTRAL].nameLower);
        }

        StringAppend(string, COMPOUND_STRING(
            "."
        ));
    }
    else
    {
        opinionKindness = IkigaiCharacter_GetAverageKindness();
        opinionStrength = IkigaiCharacter_GetAverageStrength();

        StringCopy(string, COMPOUND_STRING("I think that I am "));
        StringAppend(string, gDialogueAttitudes[IkigaiCharacter_GetPlayerAttitude()].nameLowerColour);

        StringAppend(string, COMPOUND_STRING(
            ".\nPeople rate my kindness "
        ));

        if (opinionKindness < 0)
        {
            opinionKindness *= -1;
            StringAppend(string, COMPOUND_STRING("-"));
        }

        if (opinionKindness != 0)
        {
            ConvertIntToDecimalStringN(opinion, opinionKindness, STR_CONV_MODE_LEFT_ALIGN, 3);
            StringAppend(string, opinion);
        }
        else
        {
            StringAppend(string, gDialogueAttitudes[ATTITUDE_NEUTRAL].nameLower);
        }

        StringAppend(string, COMPOUND_STRING(
            ".\nPeople rate my strength "
        ));

        if (opinionStrength < 0)
        {
            opinionStrength *= -1;
            StringAppend(gStringVar1, COMPOUND_STRING("-"));
        }

        if (opinionStrength != 0)
        {
            ConvertIntToDecimalStringN(opinion, opinionStrength, STR_CONV_MODE_LEFT_ALIGN, 3);
            StringAppend(string, opinion);
        }
        else
        {
            StringAppend(string, gDialogueAttitudes[ATTITUDE_NEUTRAL].nameLower);
        }
        
        StringAppend(string, COMPOUND_STRING(
            "."
        ));
    }

    AddTextPrinterParameterized4(WIN_CHARACTER_PROFILE_OPINION, FONT_SMALL_NARROWER, 5, 13, 0, 1,
        sPokeSphereWindowFontColors[FONT_GRAY], TEXT_SKIP_DRAW,
        string
    );

    CopyWindowToVram(WIN_CHARACTER_PROFILE_OPINION, COPYWIN_GFX);
}

static void PokeSphere_DrawCharacterMugshot(void)
{
    u32 character = sPokeSphereState->characterId;
    u32 mugshotId = gIkigaiCharactersInfo[character].mugshotId;
    u32 mugshotEmotion = gIkigaiCharactersInfo[character].defaultEmotion;

    if (character == CHARACTER_PLAYER)
    {
        mugshotId = gSaveBlock2Ptr->playerGender ? MUGSHOT_ANKA : MUGSHOT_KOLE;
        mugshotEmotion = gSaveBlock2Ptr->playerEmote;
    }
    
    FillWindowPixelBuffer(WIN_CHARACTER_MUGSHOT, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    sPokeSphereState->characterMugshotSpriteId = CreateFieldMugshotSprite(mugshotId, mugshotEmotion, FALSE, MUGSHOT_1);
    gSprites[sPokeSphereState->characterMugshotSpriteId].oam.priority = 0;
    gSprites[sPokeSphereState->characterMugshotSpriteId].x = CHARACTER_MUGSHOT_X;
    gSprites[sPokeSphereState->characterMugshotSpriteId].y = CHARACTER_MUGSHOT_Y;
    gSprites[sPokeSphereState->characterMugshotSpriteId].anims = gSpriteAnimTable_Mugshot_Flipped;
    StartSpriteAnim(&gSprites[sPokeSphereState->characterMugshotSpriteId], 0);
}

static void PokeSphere_DrawCharacterAttitude(void)
{
    u32 character = sPokeSphereState->characterId;
    u32 attitude = character ? IkigaiCharacter_GetPlayerAttitude_Character(character)
                             : IkigaiCharacter_GetPlayerAttitude();

    switch (attitude)
    {
    case ATTITUDE_INSPIRED:
        LoadCompressedSpriteSheet(&sSpriteSheet_PokeSphereInspiredIcon);
        LoadSpritePalette(&sSpritePal_PokeSphereInspiredIcon);
        break;
    
    case ATTITUDE_HUMBLE:
        LoadCompressedSpriteSheet(&sSpriteSheet_PokeSphereHumbleIcon);
        LoadSpritePalette(&sSpritePal_PokeSphereHumbleIcon);
        break;
    
    case ATTITUDE_DOMINANT:
        LoadCompressedSpriteSheet(&sSpriteSheet_PokeSphereDominantIcon);
        LoadSpritePalette(&sSpritePal_PokeSphereDominantIcon);
        break;
    
    case ATTITUDE_CYNICAL:
        LoadCompressedSpriteSheet(&sSpriteSheet_PokeSphereCynicalIcon);
        LoadSpritePalette(&sSpritePal_PokeSphereCynicalIcon);
        break;
    }

    if (attitude)
    {
        sPokeSphereState->characterAttitudeSpriteId = CreateSprite(&sSpriteTemplate_PokeSphereAttitudeIcon,
            CHARACTER_ATTITUDE_X,
            CHARACTER_ICON_Y,
            0
        );
        StartSpriteAnim(&gSprites[sPokeSphereState->characterAttitudeSpriteId], 0);
    }
}

static void PokeSphere_DrawCharacterTypeHeart(void)
{
    u32 character = sPokeSphereState->characterId;

    if (character == CHARACTER_PLAYER)
    {
        sPokeSphereState->characterTypeHeartSpriteId = CreateBWSummaryScreenMoveTypeIcon(
            CHARACTER_TYPE_X,
            CHARACTER_ICON_Y,
            0, 13
        );
        StartSpriteAnim(&gSprites[sPokeSphereState->characterTypeHeartSpriteId], gSaveBlock2Ptr->ikigaiGymType);
    }
    else if (character != CHARACTER_PLAYER && IkigaiCharacter_GetRomanticFlag(character))
    {
        LoadCompressedSpriteSheet(&sSpriteSheet_PokeSphereHeartIcon);
        LoadSpritePalette(&sSpritePal_PokeSphereHeartIcon);
        sPokeSphereState->characterTypeHeartSpriteId = CreateSprite(&sSpriteTemplate_PokeSphereHeartIcon,
            CHARACTER_HEART_X,
            CHARACTER_ICON_Y,
            0
        );
    }
}

static void PokeSphere_DrawPartnerMugshot(void)
{
    u32 character = sPokeSphereState->characterId;
    u32 speciesId = gIkigaiCharactersInfo[character].partnerPokemon;
    u32 mugshotEmotion = gIkigaiCharactersInfo[character].defaultEmotion;

    if (character == CHARACTER_PLAYER)
    {
        speciesId = VarGet(VAR_STARTER_MON);
        mugshotEmotion = gSaveBlock2Ptr->playerEmote;
    }
    
    sPokeSphereState->partnerMugshotSpriteId = CreateFieldMugshotSprite(speciesId, mugshotEmotion, TRUE, MUGSHOT_2);
    gSprites[sPokeSphereState->partnerMugshotSpriteId].oam.priority = 0;
    gSprites[sPokeSphereState->partnerMugshotSpriteId].x = CHARACTER_PARTNER_X;
    gSprites[sPokeSphereState->partnerMugshotSpriteId].y = CHARACTER_PARTNER_Y;
}

static void PokeSphere_FreeResources(void)
{
    if (sPokeSphereState != NULL)
    {
        Free(sPokeSphereState);
    }
    if (sBg1TilemapBuffer != NULL)
    {
        Free(sBg1TilemapBuffer);
    }
    FreeAllWindowBuffers();
    ResetSpriteData();
}
