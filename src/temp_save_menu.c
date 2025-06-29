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

struct RotomPhone_SaveScreenState
{
    u8 loadState;
};

enum WindowIds
{
    RP_SS_WIN_DIALOG,
};

static EWRAM_DATA struct RotomPhone_SaveScreenState *sRotomPhone_SaveScreenState = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;

static const struct BgTemplate sRotomPhone_SaveScreen_BgTemplates[] =
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

static const struct WindowTemplate sRotomPhone_SaveScreen_WindowTemplates[] =
{
    [RP_SS_WIN_DIALOG] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0xAA
    },
    DUMMY_WIN_TEMPLATE
};

static const u32 sRotomPhone_SaveScreenTiles[] = INCBIN_U32("graphics/sample_ui/tiles.4bpp.smol");

static const u32 sRotomPhone_SaveScreenTilemap[] = INCBIN_U32("graphics/sample_ui/tilemap.bin.smolTM");

static const u16 sRotomPhone_SaveScreenPalette[] = INCBIN_U16("graphics/sample_ui/00.gbapal");

enum FontColor
{
    FONT_WHITE,
    FONT_RED
};
static const u8 sRotomPhone_SaveScreenWindowFontColors[][3] =
{
    [FONT_WHITE]  = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_DARK_GRAY},
    [FONT_RED]    = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_GRAY},
};

// Callbacks for the sample UI
static void RotomPhone_SaveScreen_SetupCB(void);
static void RotomPhone_SaveScreen_MainCB(void);
static void RotomPhone_SaveScreen_VBlankCB(void);

// Sample UI tasks
static void Task_RotomPhone_SaveScreen_WaitFadeIn(u8 taskId);
static void Task_RotomPhone_SaveScreen_WaitSaveGame(u8 taskId);
static void Task_RotomPhone_SaveScreen_WaitFadeAndBail(u8 taskId);
static void Task_RotomPhone_SaveScreen_WaitFadeAndExit(u8 taskId);

// Sample UI helper functions
// static void RotomPhone_SaveScreen_Init(void);
static void RotomPhone_SaveScreen_ResetGpuRegsAndBgs(void);
static bool8 RotomPhone_SaveScreen_InitBgs(void);
static void RotomPhone_SaveScreen_FadeAndBail(void);
static bool8 RotomPhone_SaveScreen_LoadGraphics(void);
static void RotomPhone_SaveScreen_InitWindows(void);
static void RotomPhone_SaveScreen_FreeResources(void);

// Declared in sample_ui.h
void Task_OpenRotomPhone_SaveScreen_BlankTemplate2(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        RotomPhone_SaveScreen_Init();
        DestroyTask(taskId);
    }
}

void RotomPhone_SaveScreen_Init(void)
{
    SetMainCallback2(CB2_ReturnToField);
    sRotomPhone_SaveScreenState = AllocZeroed(sizeof(struct RotomPhone_SaveScreenState));
    if (sRotomPhone_SaveScreenState == NULL)
    {
        return;
    }

    sRotomPhone_SaveScreenState->loadState = 0;
    SetMainCallback2(RotomPhone_SaveScreen_SetupCB);
}

static void RotomPhone_SaveScreen_ResetGpuRegsAndBgs(void)
{
    SampleUI_ResetGpuRegsAndBgs();
}

static void RotomPhone_SaveScreen_SetupCB(void)
{
    switch (gMain.state)
    {
    case 0:
        RotomPhone_SaveScreen_ResetGpuRegsAndBgs();
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
        if (RotomPhone_SaveScreen_InitBgs())
        {
            sRotomPhone_SaveScreenState->loadState = 0;
            gMain.state++;
        }
        else
        {
            RotomPhone_SaveScreen_FadeAndBail();
            return;
        }
        break;
    case 3:
        if (RotomPhone_SaveScreen_LoadGraphics() == TRUE)
        {
            gMain.state++;
        }
        break;
    case 4:
        RotomPhone_SaveScreen_InitWindows();
        gMain.state++;
        break;
    case 5:
        CreateTask(Task_RotomPhone_SaveScreen_WaitFadeIn, 0);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    case 7:
        SetVBlankCallback(RotomPhone_SaveScreen_VBlankCB);
        SetMainCallback2(RotomPhone_SaveScreen_MainCB);
        break;
    }
}

static void RotomPhone_SaveScreen_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void RotomPhone_SaveScreen_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}
#include "start_menu.h"
static void Task_RotomPhone_SaveScreen_WaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_RotomPhone_SaveScreen_WaitSaveGame;
        SaveGame();
    }
}

static void Task_RotomPhone_SaveScreen_WaitSaveGame(u8 taskId)
{
    if (!FuncIsActiveTask(SaveGameTask))
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_RotomPhone_SaveScreen_WaitFadeAndExit;
    }
    
}

static void Task_RotomPhone_SaveScreen_WaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(CB2_ReturnToField);
        RotomPhone_SaveScreen_FreeResources();
        DestroyTask(taskId);
    }
}

static void Task_RotomPhone_SaveScreen_WaitFadeAndExit(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(CB2_ReturnToField);
        RotomPhone_SaveScreen_FreeResources();
        DestroyTask(taskId);
    }
}

static bool8 RotomPhone_SaveScreen_InitBgs(void)
{
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);
    if (sBg1TilemapBuffer == NULL)
    {
        return FALSE;
    }

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sRotomPhone_SaveScreen_BgTemplates, NELEMS(sRotomPhone_SaveScreen_BgTemplates));

    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);

    ShowBg(0);
    ShowBg(1);

    return TRUE;
}

static void RotomPhone_SaveScreen_FadeAndBail(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_RotomPhone_SaveScreen_WaitFadeAndBail, 0);
    SetVBlankCallback(RotomPhone_SaveScreen_VBlankCB);
    SetMainCallback2(RotomPhone_SaveScreen_MainCB);
}

static bool8 RotomPhone_SaveScreen_LoadGraphics(void)
{
    switch (sRotomPhone_SaveScreenState->loadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sRotomPhone_SaveScreenTiles, 0, 0, 0);
        sRotomPhone_SaveScreenState->loadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(sRotomPhone_SaveScreenTilemap, sBg1TilemapBuffer);
            sRotomPhone_SaveScreenState->loadState++;
        }
        break;
    case 2:
        LoadPalette(sRotomPhone_SaveScreenPalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        LoadPalette(GetTextWindowPalette(gSaveBlock2Ptr->optionsInterfaceColor + DEFAULT_TEXT_BOX_FRAME_PALETTES), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        sRotomPhone_SaveScreenState->loadState++;
    default:
        sRotomPhone_SaveScreenState->loadState = 0;
        return TRUE;
    }
    return FALSE;
}

static void RotomPhone_SaveScreen_InitWindows(void)
{
    InitWindows(sRotomPhone_SaveScreen_WindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    FillWindowPixelBuffer(RP_SS_WIN_DIALOG, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    PutWindowTilemap(RP_SS_WIN_DIALOG);
    CopyWindowToVram(RP_SS_WIN_DIALOG, 3);
}

static void RotomPhone_SaveScreen_FreeResources(void)
{
    if (sRotomPhone_SaveScreenState != NULL)
    {
        Free(sRotomPhone_SaveScreenState);
    }
    if (sBg1TilemapBuffer != NULL)
    {
        Free(sBg1TilemapBuffer);
    }
    FreeAllWindowBuffers();
    ResetSpriteData();
}
