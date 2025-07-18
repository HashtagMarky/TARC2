#include "pokesphere.h"
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
#include "international_string_util.h"
#include "random.h"

#include "comfy_anim.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_mugshot.h"
#include "ikigai_characters.h"
#include "ikigai_interface.h"
#include "type_icons.h"

#define CHARACTER_OFFSET                1
#define TEXT_REPLACEMENT_INDEX          4
#define MUGSHOT_BG_REPLACEMENT_INDEX    14

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
#define EXPLORE_COORDS_Y1       85
#define EXPLORE_COORDS_Y2       125
#define EXPLORE_CURSOR_X_OFFSET 4
#define EXPLORE_CURSOR_Y_OFFSET 3
#define EXPLORE_CURSOR_MASS     50
#define EXPLORE_CURSOR_TENSION  250
#define EXPLORE_CURSOR_FRICTION 1000
#define EXPLORE_CURSOR_PAL_SLOT 15
#define SPRITE_SLOT_FIRST_ICON  2 // Used to reset oam of sprites after the the mugshots when switching profiles

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

struct PokeSphereState
{
    MainCallback savedCallback;
    u8 loadState;
    enum Modes mode;
    u8 exploreOverworldSpriteId[EXPLORE_COORDS_COUNT];
    u8 exploreCursorSpriteId;
    enum ExploreProfilePositions exploreCursorPosition;
    enum CharacterId exploreCharacterStartId;
    enum CharacterId characterId;
    u8 characterMugshotSpriteId;
    u8 characterTypeHeartSpriteId;
    u8 characterAttitudeSpriteId;
    u8 partnerMugshotSpriteId;
};

static EWRAM_DATA struct PokeSphereState *sPokeSphereState = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;

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
    },
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .priority = 3
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

static const u32 sPokeSphereTiles[] = INCBIN_U32("graphics/pokesphere/tiles.4bpp.smol");
static const u16 sPokeSpherePalette[] = INCBIN_U16("graphics/pokesphere/tiles.gbapal");

static const u32 sPokeSphereTilemapExplore[] = INCBIN_U32("graphics/pokesphere/explore.bin.smolTM");
static const u32 sPokeSphereTilemapProfile[] = INCBIN_U32("graphics/pokesphere/profile.bin.smolTM");

static const u32 sPokeSphereExploreCursorGfx[] = INCBIN_U32("graphics/pokesphere/cursor.4bpp.smol");
static const u16 sPokeSphereExploreCursorPal[] = INCBIN_U16("graphics/pokesphere/cursor.gbapal");

static const u32 sPokeSphereHeartIconGfx[] = INCBIN_U32("graphics/pokesphere/heart.4bpp.smol");
static const u16 sPokeSphereHeartIconPal[] = INCBIN_U16("graphics/pokesphere/heart.gbapal");
static const u32 sPokeSphereInspiredIconGfx[] = INCBIN_U32("graphics/pokesphere/inspired.4bpp.smol");
static const u16 sPokeSphereInspiredIconPal[] = INCBIN_U16("graphics/pokesphere/inspired.gbapal");
static const u32 sPokeSphereHumbleIconGfx[] = INCBIN_U32("graphics/pokesphere/humble.4bpp.smol");
static const u16 sPokeSphereHumbleIconPal[] = INCBIN_U16("graphics/pokesphere/humble.gbapal");
static const u32 sPokeSphereDominantIconGfx[] = INCBIN_U32("graphics/pokesphere/dominant.4bpp.smol");
static const u16 sPokeSphereDominantIconPal[] = INCBIN_U16("graphics/pokesphere/dominant.gbapal");
static const u32 sPokeSphereCynicalIconGfx[] = INCBIN_U32("graphics/pokesphere/cynical.4bpp.smol");
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
    .size = 3*32*32*4/2,
    .tag = TAG_POKESPHERE_CURSOR,
};

static const struct SpritePalette sSpritePal_PokeSphereExploreCursor =
{
    .data = sPokeSphereExploreCursorPal,
    .tag = TAG_POKESPHERE_CURSOR
};

static const union AnimCmd sSpriteAnim_PokeSphereExploreCursor[] =
{
    ANIMCMD_FRAME(0, 12),
    ANIMCMD_FRAME(16, 12),
    ANIMCMD_FRAME(32, 12),
    ANIMCMD_FRAME(16, 12),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sSpriteAnimTable_PokeSphereExploreCursor[] =
{
    sSpriteAnim_PokeSphereExploreCursor,
};

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
#define CHARACTER_MUGSHOT_Y     61
#define CHARACTER_TYPE_X        91
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
    [FONT_GRAY]         = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_WHITE},
    [FONT_RED]          = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED,        TEXT_COLOR_WHITE},
    [FONT_GREEN]        = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_GREEN,      TEXT_COLOR_WHITE},
    [FONT_BLUE]         = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_BLUE,       TEXT_COLOR_WHITE},
    [FONT_LIGHT_BLUE]   = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_BLUE, TEXT_COLOR_WHITE},
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
static void Task_PokeSphereLoadSpritesAndTextAfterModeSwitch(u8 taskId);

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
static void PokeSphere_DrawCharacterMugshot(bool32 loadBg);
static void PokeSphere_DrawMugshotBackgrounds(enum CharacterId character);
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

// Sprite Callbacks
static void PokeSphereExploreCursorCallback(struct Sprite *sprite);
static void PokeSphere_TypeIconCallback(struct Sprite *sprite);

// Debug Functions
static void DEBUG_Task_OpenPokeSphere(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        PokeSphere_Init(CB2_ReturnToField);
        DestroyTask(taskId);
    }
}

void DEBUG_OpenPokeSphereUI(void)
{
    CreateTask(DEBUG_Task_OpenPokeSphere, 0);
}

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

static void PokeSphere_ResetGpuRegsAndBgs(void)
{
    SampleUI_ResetGpuRegsAndBgs();
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
        PlaySE(SE_DEX_PAGE);
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
    AdvanceComfyAnimations();
}

static void PokeSphere_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    StartIkigaiScrollingBackground(2);
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

        if (JOY_REPEAT(DPAD_UP) && !gPaletteFade.active)
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
                sPokeSphereState->exploreCharacterStartId -= EXPLORE_COORDS_PER_ROW;
                PokeSphere_Explore_DestroyObjectEvents();
                PokeSphere_Explore_CreateObjectEvents();
                PlaySE(SE_BALL_TRAY_BALL);
            }
        }

        if (JOY_REPEAT(DPAD_DOWN) && !gPaletteFade.active)
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
                sPokeSphereState->exploreCharacterStartId += EXPLORE_COORDS_PER_ROW;
                PokeSphere_Explore_DestroyObjectEvents();
                PokeSphere_Explore_CreateObjectEvents();
                PlaySE(SE_BALL_TRAY_BALL);
            }
        }

        if (JOY_REPEAT(DPAD_LEFT) && !gPaletteFade.active)
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

        if (JOY_REPEAT(DPAD_RIGHT) && !gPaletteFade.active)
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
        }

        if (JOY_REPEAT(DPAD_RIGHT))
        {
            PokeSphere_CycleCharacters(TRUE);
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
        ReleaseComfyAnims();
        DestroyTask(taskId);
    }
}

static bool8 PokeSphere_InitBgs(void)
{
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);
    sBg2TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);
    if (sBg1TilemapBuffer == NULL || sBg2TilemapBuffer == NULL)
    {
        return FALSE;
    }

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sPokeSphereBgTemplates, NELEMS(sPokeSphereBgTemplates));

    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);

    SetBgTilemapBuffer(2, sBg2TilemapBuffer);
    ScheduleBgCopyTilemapToVram(2);

    ShowBg(0);
    ShowBg(1);
    ShowBg(2);

    return TRUE;
}

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
        IkigaiScrollingBackground_CreateTiles(2);
        sPokeSphereState->loadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(sPokeSphereTilemapExplore, sBg1TilemapBuffer);
            IkigaiScrollingBackground_CreateTilemap(1, sBg2TilemapBuffer);
            sPokeSphereState->loadState++;
        }
        break;
    case 2:
        LoadPalette(sPokeSpherePalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        IkigaiScrollingBackground_LoadPalette(1, IKIGAI_PAL_INTERFACE);
        LoadPalette(GetTextWindowPalette(gSaveBlock2Ptr->optionsInterfaceColor + DEFAULT_TEXT_BOX_FRAME_PALETTES), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        LoadPalette(&sPokeSpherePalette[TEXT_REPLACEMENT_INDEX], BG_PLTT_ID(15) + TEXT_COLOR_DARK_GRAY, sizeof(&sPokeSpherePalette[TEXT_REPLACEMENT_INDEX]));
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
                DecompressDataWithHeaderWram(sPokeSphereTilemapProfile, sBg1TilemapBuffer);
            }
        }
        else if (sPokeSphereState->mode == MODE_EXPLORE)
        {
            if (FreeTempTileDataBuffersIfPossible() != TRUE)
            {
                PokeSphere_DestroyProfilePostPage();
                DecompressDataWithHeaderWram(sPokeSphereTilemapExplore, sBg1TilemapBuffer);
            }
        }
        ScheduleBgCopyTilemapToVram(1);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gTasks[taskId].tLoadSprites = FALSE;
        gTasks[taskId].func = Task_PokeSphereLoadSpritesAndTextAfterModeSwitch;
    }
}

static void Task_PokeSphereLoadSpritesAndTextAfterModeSwitch(u8 taskId)
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
    {
        PokeSphere_DrawMugshotBackgrounds(sPokeSphereState->characterId);
        gTasks[taskId].func = Task_PokeSphereWaitFadeIn;
    }
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
    ReleaseComfyAnims();
    PokeSphere_Explore_DestroyObjectEvents();
    PokeSphere_Explore_DestroyCursor();
    FreeAllSpritePalettes();
    AllocSpriteTiles(0);
}

static void PokeSphere_CreateProfilePostPage(void)
{
    PokeSphere_DrawCharacterMugshot(FALSE);
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
    ReleaseComfyAnims();
    DestroyFieldMugshotSprite(sPokeSphereState->characterMugshotSpriteId, MUGSHOT_1);
    DestroyFieldMugshotSprite(sPokeSphereState->partnerMugshotSpriteId, MUGSHOT_2);
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->characterTypeHeartSpriteId]);
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->characterAttitudeSpriteId]);
    FillWindowPixelBuffer(WIN_CHARACTER_RELATIONSHIPS_POSTS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_CHARACTER_PROFILE_OPINION, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    CopyWindowToVram(WIN_CHARACTER_RELATIONSHIPS_POSTS, COPYWIN_FULL);
    CopyWindowToVram(WIN_CHARACTER_PROFILE_OPINION, COPYWIN_FULL);
    FreeAllSpritePalettes();
    AllocSpriteTiles(0);
}

static void PokeSphere_PrintUIControls(void)
{
    u8 textTop[20];
    u8 textMiddle[20];
    u8 textBottom[20];
    u8 x = 13, y = 1, yRow = 11;

    if (sPokeSphereState->mode == MODE_EXPLORE)
    {
        StringCopy(textTop, COMPOUND_STRING("Explore Profiles"));
        StringCopy(textMiddle, COMPOUND_STRING("View Profile"));
        StringCopy(textBottom, COMPOUND_STRING("Exit"));
    }
    else if (sPokeSphereState->mode == MODE_PROFILE || sPokeSphereState->mode == MODE_POSTS)
    {
        StringCopy(textTop, COMPOUND_STRING("Change Profile"));
        StringCopy(textMiddle, COMPOUND_STRING("Change View"));
        StringCopy(textBottom, COMPOUND_STRING("Explore Profiles"));
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
    enum CharacterId initialCharacter = sPokeSphereState->characterId;
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
    PokeSphere_ReloadProfile();
    PokeSphere_ReloadText();
}

static void PokeSphere_ReloadProfile(void)
{
    ReleaseComfyAnims();
    PokeSphere_PrintNames();
    DestroyFieldMugshotSprite(sPokeSphereState->characterMugshotSpriteId, MUGSHOT_1);
    DestroyFieldMugshotSprite(sPokeSphereState->partnerMugshotSpriteId, MUGSHOT_2);
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->characterTypeHeartSpriteId]);
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->characterAttitudeSpriteId]);
    // ResetOamRange(SPRITE_SLOT_FIRST_ICON, 128); // Hacky fix to prevent palette and tile issues in icons when switching profiles
    ResetOamRange(0, 128); // Less Hacky fix to prevent palette and tile issues in icons when switching profiles
    FreeAllSpritePalettes();
    AllocSpriteTiles(0);
    PokeSphere_DrawCharacterMugshot(TRUE);
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
        if (&gSprites[sPokeSphereState->exploreOverworldSpriteId[coord]] == &gSprites[sPokeSphereState->exploreCursorSpriteId])
            break;
        
        FreeSpritePalette(&gSprites[sPokeSphereState->exploreOverworldSpriteId[coord]]);
        DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->exploreOverworldSpriteId[coord]]);
    } // Come back here if Object Pals & Tiles Mess Up Again
    FreeAllSpritePalettes();
    // AllocSpriteTiles(0);
}

static void PokeSphere_Explore_CreateCursor(void)
{
    struct ComfyAnimSpringConfig xConfig;
    struct ComfyAnimSpringConfig yConfig;
    InitComfyAnimConfig_Spring(&xConfig);
    InitComfyAnimConfig_Spring(&yConfig);

    u8 x = sExplorePageSpriteCords[sPokeSphereState->exploreCursorPosition].x + EXPLORE_CURSOR_X_OFFSET;
    u8 y = sExplorePageSpriteCords[sPokeSphereState->exploreCursorPosition].y + EXPLORE_CURSOR_Y_OFFSET;

    xConfig.from = Q_24_8(x);
    xConfig.to = Q_24_8(x);
    xConfig.mass = Q_24_8(EXPLORE_CURSOR_MASS);
    xConfig.tension = Q_24_8(EXPLORE_CURSOR_TENSION);
    xConfig.friction = Q_24_8(EXPLORE_CURSOR_FRICTION);

    yConfig.from = Q_24_8(y);
    yConfig.to = Q_24_8(y);
    yConfig.mass = Q_24_8(EXPLORE_CURSOR_MASS);
    yConfig.tension = Q_24_8(EXPLORE_CURSOR_TENSION);
    yConfig.friction = Q_24_8(EXPLORE_CURSOR_FRICTION);

    LoadCompressedSpriteSheet(&sSpriteSheet_PokeSphereExploreCursor);
    LoadPalette(sPokeSphereExploreCursorPal, OBJ_PLTT_ID(EXPLORE_CURSOR_PAL_SLOT), PLTT_SIZE_4BPP);
    sPokeSphereState->exploreCursorSpriteId = CreateSprite(&sSpriteTemplate_PokeSphereExploreCursor,
        x,
        y,
        0
    );
    gSprites[sPokeSphereState->exploreCursorSpriteId].callback = PokeSphereExploreCursorCallback;
    gSprites[sPokeSphereState->exploreCursorSpriteId].sComfyAnimX = CreateComfyAnim_Spring(&xConfig);
    gSprites[sPokeSphereState->exploreCursorSpriteId].sComfyAnimY = CreateComfyAnim_Spring(&yConfig);
}

static void PokeSphereExploreCursorCallback(struct Sprite *sprite)
{
    struct ComfyAnim *xAnim = &gComfyAnims[sprite->sComfyAnimX];
    struct ComfyAnim *yAnim = &gComfyAnims[sprite->sComfyAnimY];
    struct ComfyAnimSpringConfig *xConfig = &xAnim->config.data.spring;
    struct ComfyAnimSpringConfig *yConfig = &yAnim->config.data.spring;

    xConfig->to = Q_24_8(sExplorePageSpriteCords[sPokeSphereState->exploreCursorPosition].x + EXPLORE_CURSOR_X_OFFSET);
    yConfig->to = Q_24_8(sExplorePageSpriteCords[sPokeSphereState->exploreCursorPosition].y + EXPLORE_CURSOR_Y_OFFSET);

    sprite->x = ReadComfyAnimValueSmooth(xAnim);
    sprite->y = ReadComfyAnimValueSmooth(yAnim);
}

static void PokeSphere_Explore_DestroyCursor(void)
{
    DestroySpriteAndFreeResources(&gSprites[sPokeSphereState->exploreCursorSpriteId]);
}

static void PokeSphere_PrintNames(void)
{   
    u8 x, y = 0;
    enum CharacterId character = sPokeSphereState->characterId;
    u32 textColour = FONT_GRAY;
    const u8 *name = gIkigaiCharactersInfo[character].name;
    u8 nameBlank[3] = _("");
    
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

    case ATTITUDE_NEUTRAL:
    case ATTITUDE_COUNT:
        break;
    }

    FillWindowPixelBuffer(WIN_CHARACTER_NAME, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    if (sPokeSphereState->mode == MODE_EXPLORE
        || (VarGet(VAR_STARTER_MON) == SPECIES_NONE
        && character == CHARACTER_PLAYER))
    {
        u8 character = sPokeSphereState->exploreCharacterStartId + sPokeSphereState->exploreCursorPosition;
        if (character >= CHARACTER_RESIDENT_COUNT)
            character++;

        if (!IkigaiCharacter_GetMetFlag(character))
            name = gIkigaiCharactersInfo[CHARACTER_DEFAULT].name;
        else if (character >= CHARACTER_COUNT_TOTAL)
            name = nameBlank;
        else if (character != CHARACTER_PLAYER)
            name = gIkigaiCharactersInfo[character].name;
        else if (character == CHARACTER_PLAYER)
            name = gSaveBlock2Ptr->playerName;
        
        y = 4;
        textColour = FONT_GRAY;
    }

    if (character == CHARACTER_PLAYER && sPokeSphereState->mode != MODE_EXPLORE)
        name = gSaveBlock2Ptr->playerName;

    x = GetStringCenterAlignXOffset(FONT_SMALL_NARROWER,
        name,
        48
    );
    AddTextPrinterParameterized4(WIN_CHARACTER_NAME, FONT_SMALL_NARROWER, x, y, 0, 0,
        sPokeSphereWindowFontColors[textColour], TEXT_SKIP_DRAW,
        name
    );

    if (VarGet(VAR_STARTER_MON) == SPECIES_NONE
        && character == CHARACTER_PLAYER
        && sPokeSphereState->mode != MODE_EXPLORE)
    {
        // Do not print unchosen partner name if on profile or post page.
    }
    else if (sPokeSphereState->mode != MODE_EXPLORE && character != CHARACTER_PLAYER)
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
    enum CharacterId character = sPokeSphereState->characterId;
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

static void PokeSphere_DrawCharacterMugshot(bool32 loadBg)
{
    enum CharacterId character = sPokeSphereState->characterId;
    enum MugshotIDs mugshotId = gIkigaiCharactersInfo[character].mugshotId;
    enum MugshotEmoteIDs mugshotEmotion = gIkigaiCharactersInfo[character].defaultEmotion;

    if (character == CHARACTER_PLAYER)
    {
        mugshotId = gSaveBlock2Ptr->playerGender ? MUGSHOT_ANKA : MUGSHOT_KOLE;
        mugshotEmotion = gSaveBlock2Ptr->playerEmote;
    }

    if (loadBg)
        PokeSphere_DrawMugshotBackgrounds(character);
    
    FillWindowPixelBuffer(WIN_CHARACTER_MUGSHOT, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    sPokeSphereState->characterMugshotSpriteId = CreateFieldMugshotSprite(mugshotId, mugshotEmotion, FALSE, FALSE, MUGSHOT_1);
    gSprites[sPokeSphereState->characterMugshotSpriteId].oam.priority = 3;
    gSprites[sPokeSphereState->characterMugshotSpriteId].x = CHARACTER_MUGSHOT_X;
    gSprites[sPokeSphereState->characterMugshotSpriteId].y = CHARACTER_MUGSHOT_Y;
    gSprites[sPokeSphereState->characterMugshotSpriteId].anims = gSpriteAnimTable_Mugshot_Flipped;
    StartSpriteAnim(&gSprites[sPokeSphereState->characterMugshotSpriteId], 0);
}

static void PokeSphere_DrawMugshotBackgrounds(enum CharacterId character)
{
    if (character == CHARACTER_PLAYER)
    {
        IkigaiScrollingBackground_LoadPalette(1, IKIGAI_PAL_INTERFACE);
    }
    else
    {
        u16 colour = sPokeSpherePalette[MUGSHOT_BG_REPLACEMENT_INDEX];
        LoadPalette(
                (u16 [PLTT_SIZE_4BPP]) { // Original Colour of PokéSphere Tiles.
                    colour, colour, colour, colour,
                    colour, colour, colour, colour,
                    colour, colour, colour, colour,
                    colour, colour, colour, colour,
                },
                BG_PLTT_ID(1),
                PLTT_SIZE_4BPP
            );
    }
}

static void PokeSphere_DrawCharacterAttitude(void)
{
    enum CharacterId character = sPokeSphereState->characterId;
    enum AttitudeId attitude = character ? IkigaiCharacter_GetPlayerAttitude_Character(character)
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

    case ATTITUDE_NEUTRAL:
    case ATTITUDE_COUNT:
        break;
    }

    if (attitude)
    {
        sPokeSphereState->characterAttitudeSpriteId = CreateSprite(&sSpriteTemplate_PokeSphereAttitudeIcon,
            CHARACTER_ATTITUDE_X,
            CHARACTER_ICON_Y,
            0
        );
        gSprites[sPokeSphereState->characterAttitudeSpriteId].oam.priority = 3;
        StartSpriteAnim(&gSprites[sPokeSphereState->characterAttitudeSpriteId], 0);
    }
}

static void PokeSphere_DrawCharacterTypeHeart(void)
{
    enum CharacterId character = sPokeSphereState->characterId;

    if (character == CHARACTER_PLAYER)
    {
        struct ComfyAnimEasingConfig config;

        InitComfyAnimConfig_Easing(&config);
        config.durationFrames = 35;
        config.from = Q_24_8(CHARACTER_TYPE_X + NUM_FRAMES_HIDE_TYPE_ICON);
        config.to = Q_24_8(CHARACTER_TYPE_X);
        config.easingFunc = ComfyAnimEasing_EaseInOutBack;
        
        sPokeSphereState->characterTypeHeartSpriteId = CreateBattlenMoveTypeIcon(
            CHARACTER_TYPE_X + NUM_FRAMES_HIDE_TYPE_ICON,
            CHARACTER_ICON_Y + 3,
            0, gSaveBlock2Ptr->ikigaiGymType
        );
        gSprites[sPokeSphereState->characterTypeHeartSpriteId].oam.priority = 3;
        gSprites[sPokeSphereState->characterTypeHeartSpriteId].callback = PokeSphere_TypeIconCallback;
        gSprites[sPokeSphereState->characterTypeHeartSpriteId].sComfyAnimX = CreateComfyAnim_Easing(&config);
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
        gSprites[sPokeSphereState->characterTypeHeartSpriteId].oam.priority = 3;
    }
}

static void PokeSphere_TypeIconCallback(struct Sprite *sprite)
{
    int animId = sprite->sComfyAnimX;
    sprite->x = ReadComfyAnimValueSmooth(&gComfyAnims[animId]);
    if (gComfyAnims[animId].completed)
    {
        ReleaseComfyAnim(animId);
        sprite->callback = SpriteCallbackDummy;
    }
}

static void PokeSphere_DrawPartnerMugshot(void)
{
    enum CharacterId character = sPokeSphereState->characterId;
    u32 speciesId = gIkigaiCharactersInfo[character].partnerPokemon;
    enum MugshotEmoteIDs mugshotEmotion = gIkigaiCharactersInfo[character].defaultEmotion;
    bool32 typeMon = TRUE;

    if (character == CHARACTER_PLAYER)
    {
        speciesId = VarGet(VAR_STARTER_MON);
        mugshotEmotion = gSaveBlock2Ptr->playerEmote;
    }

    if (VarGet(VAR_STARTER_MON) == SPECIES_NONE
        && character == CHARACTER_PLAYER)
    {
        speciesId = MUGSHOT_BLANK;
        typeMon = FALSE;
    }
    
    sPokeSphereState->partnerMugshotSpriteId = CreateFieldMugshotSprite(speciesId, mugshotEmotion, typeMon, FALSE, MUGSHOT_2);
    gSprites[sPokeSphereState->partnerMugshotSpriteId].oam.priority = 3;
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
    if (sBg2TilemapBuffer != NULL)
    {
        Free(sBg2TilemapBuffer);
    }
    FreeAllWindowBuffers();
    ResetSpriteData();
}
