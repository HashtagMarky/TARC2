#include "rotom_phone_start_menu.h"

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

struct RotomPhoneLargeState
{
    MainCallback savedCallback;
    u8 loadState;
    u8 mode;
};

enum WindowIds
{
    WINDOW_0
};

static EWRAM_DATA struct RotomPhoneLargeState *sRotomPhone_StartMenu = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;

static const struct BgTemplate sRotomPhone_LargeStartMenu_BgTemplates[] =
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

static const struct WindowTemplate sRotomPhone_LargeStartMenu_WindowTemplates[] =
{
    [WINDOW_0] =
    {
        .bg = 0,
        .tilemapLeft = 14,
        .tilemapTop = 0,
        .width = 16,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 1
    },
    DUMMY_WIN_TEMPLATE
};

static const u32 sRotomPhoneLargeTiles[] = INCBIN_U32("graphics/sample_ui/tiles.4bpp.lz");

static const u32 sRotomPhoneLargeTilemap[] = INCBIN_U32("graphics/sample_ui/tilemap.bin.lz");

static const u16 sRotomPhoneLargePalette[] = INCBIN_U16("graphics/sample_ui/00.gbapal");

enum FontColor
{
    FONT_WHITE,
    FONT_RED
};
static const u8 sRotomPhoneLargeWindowFontColors[][3] =
{
    [FONT_WHITE]  = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_DARK_GRAY},
    [FONT_RED]    = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_GRAY},
};

// Callbacks for the sample UI
static void RotomPhone_LargeStartMenu_SetupCB(void);
static void RotomPhone_LargeStartMenu_MainCB(void);
static void RotomPhone_LargeStartMenu_VBlankCB(void);

// Sample UI tasks
static void Task_RotomPhone_LargeStartMenu_WaitFadeIn(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_MainInput(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_WaitFadeAndBail(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_WaitFadeAndExitGracefully(u8 taskId);

// Sample UI helper functions
static void RotomPhone_LargeStartMenu_Init(MainCallback callback);
static void RotomPhone_LargeStartMenu_ResetGpuRegsAndBgs(void);
static bool8 RotomPhone_LargeStartMenu_InitBgs(void);
static void RotomPhone_LargeStartMenu_FadeAndBail(void);
static bool8 RotomPhone_LargeStartMenu_LoadGraphics(void);
static void RotomPhone_LargeStartMenu_InitWindows(void);
static void RotomPhone_LargeStartMenu_PrintUiSampleWindowText(void);
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
    sRotomPhone_StartMenu = AllocZeroed(sizeof(struct RotomPhoneLargeState));
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
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON);
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
        RotomPhone_LargeStartMenu_PrintUiSampleWindowText();
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
    }
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

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sRotomPhone_LargeStartMenu_BgTemplates, NELEMS(sRotomPhone_LargeStartMenu_BgTemplates));

    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);

    ShowBg(0);
    ShowBg(1);

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
        DecompressAndCopyTileDataToVram(1, sRotomPhoneLargeTiles, 0, 0, 0);
        sRotomPhone_StartMenu->loadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sRotomPhoneLargeTilemap, sBg1TilemapBuffer);
            sRotomPhone_StartMenu->loadState++;
        }
        break;
    case 2:
        LoadPalette(sRotomPhoneLargePalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
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
    InitWindows(sRotomPhone_LargeStartMenu_WindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    FillWindowPixelBuffer(WINDOW_0, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    PutWindowTilemap(WINDOW_0);
    CopyWindowToVram(WINDOW_0, 3);
}

static const u8 sText_Text1[] = _("Hello, world!");
static const u8 sText_Text2[] = _("Press {A_BUTTON} to make a sound!");
static void RotomPhone_LargeStartMenu_PrintUiSampleWindowText(void)
{
    FillWindowPixelBuffer(WINDOW_0, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    AddTextPrinterParameterized4(WINDOW_0, FONT_NORMAL, 0, 3, 0, 0,
        sRotomPhoneLargeWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sText_Text1);
    AddTextPrinterParameterized4(WINDOW_0, FONT_SMALL, 0, 15, 0, 0,
        sRotomPhoneLargeWindowFontColors[FONT_RED], TEXT_SKIP_DRAW, sText_Text2);

    CopyWindowToVram(WINDOW_0, COPYWIN_GFX);
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
    FreeAllWindowBuffers();
    ResetSpriteData();
}
