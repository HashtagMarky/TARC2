#include "calendar.h"
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

#include "event_object_movement.h"
#include "ikigai_scrolling_background.h"
#include "international_string_util.h"
#include "constants/event_objects.h"

#define DAYS_IN_SEASON 28

struct CalendarUIState
{
    MainCallback savedCallback;
    u8 loadState;
    u8 spriteIdSeason;
    u8 spriteIdWeather;
    u8 spriteIdPlayer;
    u8 spriteIdGymType;
    u8 spriteIdDate[DAYS_IN_SEASON];
};

enum WindowIds
{
    WINDOW_SCHEDULE
};

static EWRAM_DATA struct CalendarUIState *sCalendarUIState = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;

static const struct BgTemplate sCalendarUIBgTemplates[] =
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

static const struct WindowTemplate sCalendarUIWindowTemplates[] =
{
    [WINDOW_SCHEDULE] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 5,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 1
    },
    DUMMY_WIN_TEMPLATE
};

static const u32 sCalendarUITiles[] = INCBIN_U32("graphics/calendar/tiles.4bpp.lz");
static const u32 sCalendarUITilemap[] = INCBIN_U32("graphics/calendar/tilemap.bin.lz");
static const u16 sCalendarUIPalette[] = INCBIN_U16("graphics/calendar/tiles.gbapal");

static const u32 sCalendarWeatherIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/sunny_day.4bpp.lz");
static const u16 sCalendarWeatherIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/sunny_day.gbapal");

enum FontColor
{
    FONT_BROWN,
};
static const u8 sCalendarUIWindowFontColors[][3] =
{
    [FONT_BROWN]  = {TEXT_COLOR_TRANSPARENT,    TEXT_COLOR_DARK_GRAY,   TEXT_COLOR_LIGHT_GRAY},
};

#define TEXT_REPLACEMENT_INDEX      5
#define SHADOW_REPLACEMENT_INDEX    11

#define TAG_WEATHER_ICON            7281

static const struct OamData sOamData_CalendarWeatherIcon =
{
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 1,
};

static const struct CompressedSpriteSheet sSpriteSheet_CalendarWeatherIcon =
{
    .data = sCalendarWeatherIconGfx,
    .size = 32*32*4/2,
    .tag = TAG_WEATHER_ICON,
};

static const struct SpritePalette sSpritePal_CalendarWeatherIcon =
{
    .data = sCalendarWeatherIconPal,
    .tag = TAG_WEATHER_ICON
};

static const struct SpriteTemplate sSpriteTemplate_CalendarWeatherIcon =
{
    .tileTag = TAG_WEATHER_ICON,
    .paletteTag = TAG_WEATHER_ICON,
    .oam = &sOamData_CalendarWeatherIcon,
    .anims = gDummySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

// Callbacks for the Calendar UI
static void CalendarUI_SetupCB(void);
static void CalendarUI_MainCB(void);
static void CalendarUI_VBlankCB(void);

// Calendar UI tasks
static void Task_CalendarUIWaitFadeIn(u8 taskId);
static void Task_CalendarUIMainInput(u8 taskId);
static void Task_CalendarUIWaitFadeAndBail(u8 taskId);
static void Task_CalendarUIWaitFadeAndExitGracefully(u8 taskId);

// Calendar UI helper functions
static void CalendarUI_Init(MainCallback callback);
static void CalendarUI_ResetGpuRegsAndBgs(void);
static bool8 CalendarUI_InitBgs(void);
static void CalendarUI_FadeAndBail(void);
static bool8 CalendarUI_LoadGraphics(void);
static void CalendarUI_InitWindows(void);
static void CalendarUI_PrintScheduleText(void);
static void CalendarUI_CreateSprites(void);
static void CalendarUI_FreeResources(void);

void Task_OpenCalendarUI(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        CalendarUI_Init(CB2_ReturnToFieldWithOpenMenu);
        DestroyTask(taskId);
    }
}

static void CalendarUI_Init(MainCallback callback)
{
    sCalendarUIState = AllocZeroed(sizeof(struct CalendarUIState));
    if (sCalendarUIState == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    sCalendarUIState->loadState = 0;
    sCalendarUIState->savedCallback = callback;

    SetMainCallback2(CalendarUI_SetupCB);
}

static void CalendarUI_ResetGpuRegsAndBgs(void)
{
    SampleUI_ResetGpuRegsAndBgs();
}

static void CalendarUI_SetupCB(void)
{
    switch (gMain.state)
    {
    case 0:
        CalendarUI_ResetGpuRegsAndBgs();
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
        if (CalendarUI_InitBgs())
        {
            sCalendarUIState->loadState = 0;
            gMain.state++;
        }
        else
        {
            CalendarUI_FadeAndBail();
            return;
        }
        break;
    case 3:
        if (CalendarUI_LoadGraphics() == TRUE)
        {
            gMain.state++;
        }
        break;
    case 4:
        CalendarUI_InitWindows();
        gMain.state++;
        break;
    case 5:
        CalendarUI_PrintScheduleText();
        CalendarUI_CreateSprites();
        CreateTask(Task_CalendarUIWaitFadeIn, 0);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    case 7:
        SetVBlankCallback(CalendarUI_VBlankCB);
        SetMainCallback2(CalendarUI_MainCB);
        break;
    }
}

static void CalendarUI_MainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void CalendarUI_VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    StartIkigaiScrollingBackground(2);
}

static void Task_CalendarUIWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_CalendarUIMainInput;
    }
}

static void Task_CalendarUIMainInput(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_PC_OFF);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_CalendarUIWaitFadeAndExitGracefully;
    }
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
    }
}

static void Task_CalendarUIWaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sCalendarUIState->savedCallback);
        CalendarUI_FreeResources();
        DestroyTask(taskId);
    }
}

static void Task_CalendarUIWaitFadeAndExitGracefully(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sCalendarUIState->savedCallback);
        CalendarUI_FreeResources();
        DestroyTask(taskId);
    }
}
#define TILEMAP_BUFFER_SIZE (1024 * 2)
static bool8 CalendarUI_InitBgs(void)
{
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
    sBg2TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
    if (sBg1TilemapBuffer == NULL || sBg2TilemapBuffer == NULL)
    {
        return FALSE;
    }

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sCalendarUIBgTemplates, NELEMS(sCalendarUIBgTemplates));

    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);

    SetBgTilemapBuffer(2, sBg2TilemapBuffer);
    ScheduleBgCopyTilemapToVram(2);

    ShowBg(0);
    ShowBg(1);
    ShowBg(2);

    return TRUE;
}
#undef TILEMAP_BUFFER_SIZE

static void CalendarUI_FadeAndBail(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_CalendarUIWaitFadeAndBail, 0);
    SetVBlankCallback(CalendarUI_VBlankCB);
    SetMainCallback2(CalendarUI_MainCB);
}

static bool8 CalendarUI_LoadGraphics(void)
{
    switch (sCalendarUIState->loadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sCalendarUITiles, 0, 0, 0);
        DecompressAndCopyTileDataToVram(2, IkigaiScrollingBgTiles, 0, 0, 0);
        sCalendarUIState->loadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sCalendarUITilemap, sBg1TilemapBuffer);
            LZDecompressWram(IkigaiScrollingBgTilemap_PalOne, sBg2TilemapBuffer);
            sCalendarUIState->loadState++;
        }
        break;
    case 2:
        LoadPalette(sCalendarUIPalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        LoadPalette(ReturnScrollingBackgroundSeasonPalette(0), BG_PLTT_ID(1), PLTT_SIZE_4BPP);
        LoadPalette(GetTextWindowPalette(gSaveBlock2Ptr->optionsInterfaceColor + DEFAULT_TEXT_BOX_FRAME_PALETTES), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        LoadPalette(&sCalendarUIPalette[TEXT_REPLACEMENT_INDEX], BG_PLTT_ID(15) + TEXT_COLOR_DARK_GRAY, sizeof(&sCalendarUIPalette[TEXT_REPLACEMENT_INDEX]));
        LoadPalette(&sCalendarUIPalette[SHADOW_REPLACEMENT_INDEX], BG_PLTT_ID(15) + TEXT_COLOR_LIGHT_GRAY, sizeof(&sCalendarUIPalette[SHADOW_REPLACEMENT_INDEX]));
        sCalendarUIState->loadState++;
    default:
        sCalendarUIState->loadState = 0;
        return TRUE;
    }
    return FALSE;
}

static void CalendarUI_InitWindows(void)
{
    InitWindows(sCalendarUIWindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    FillWindowPixelBuffer(WINDOW_SCHEDULE, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    PutWindowTilemap(WINDOW_SCHEDULE);
    CopyWindowToVram(WINDOW_SCHEDULE, 3);
}

static void CalendarUI_PrintScheduleText(void)
{
    FillWindowPixelBuffer(WINDOW_SCHEDULE, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    u32 y = 5;
    u8 numGym[1], numWorks[1];

    ConvertIntToDecimalStringN(numGym, 0, STR_CONV_MODE_RIGHT_ALIGN, 1);
    ConvertIntToDecimalStringN(numWorks, 0, STR_CONV_MODE_RIGHT_ALIGN, 1);

    // AddTextPrinterParameterized4(WINDOW_SCHEDULE, FONT_BW_SUMMARY_SCREEN, 
    //     GetStringCenterAlignXOffset(FONT_BW_SUMMARY_SCREEN,
    //         COMPOUND_STRING("Today"),
    //         40
    //     ), y, 0, 0,
    //     sCalendarUIWindowFontColors[FONT_BROWN], TEXT_SKIP_DRAW,
    //     COMPOUND_STRING("Today")
    // );

    y += 13;

    AddTextPrinterParameterized4(WINDOW_SCHEDULE, FONT_SHORT_NARROWER, 
        6, y, 0, 0,
        sCalendarUIWindowFontColors[FONT_BROWN], TEXT_SKIP_DRAW,
        COMPOUND_STRING("Gym")
    );

    AddTextPrinterParameterized4(WINDOW_SCHEDULE, FONT_SHORT_NARROW, 
        GetStringRightAlignXOffset(FONT_BW_SUMMARY_SCREEN,
            numGym,
            40
        ) - 4, y, 0, 0,
        sCalendarUIWindowFontColors[FONT_BROWN], TEXT_SKIP_DRAW,
        numGym
    );

    y += 12;

    AddTextPrinterParameterized4(WINDOW_SCHEDULE, FONT_SHORT_NARROWER, 
        6, y, 0, 0,
        sCalendarUIWindowFontColors[FONT_BROWN], TEXT_SKIP_DRAW,
        COMPOUND_STRING("Works")
    );

    AddTextPrinterParameterized4(WINDOW_SCHEDULE, FONT_SHORT_NARROW, 
        GetStringRightAlignXOffset(FONT_BW_SUMMARY_SCREEN,
            numWorks,
            40
        ) - 4, y, 0, 0,
        sCalendarUIWindowFontColors[FONT_BROWN], TEXT_SKIP_DRAW,
        numWorks
    );

    CopyWindowToVram(WINDOW_SCHEDULE, COPYWIN_GFX);
}

static void CalendarUI_CreateSprites(void)
{
    // spriteIdSeason

    LoadCompressedSpriteSheet(&sSpriteSheet_CalendarWeatherIcon);
    LoadSpritePalette(&sSpritePal_CalendarWeatherIcon);
    sCalendarUIState->spriteIdWeather = CreateSprite(&sSpriteTemplate_CalendarWeatherIcon, 36, 86, 0);
    gSprites[sCalendarUIState->spriteIdWeather].oam.priority = 3;

    u16 objEvent = gSaveBlock2Ptr->playerGender ? OBJ_EVENT_GFX_ANKA_NORMAL : OBJ_EVENT_GFX_KOLE_NORMAL;
    sCalendarUIState->spriteIdPlayer = CreateObjectGraphicsSprite(objEvent, SpriteCallbackDummy, 33, 131, 102);
    StartSpriteAnim(&gSprites[sCalendarUIState->spriteIdPlayer], ANIM_STD_GO_SOUTH);

    // spriteIdGymType

    // spriteIdDate[DAYS_IN_SEASON]
}

static void CalendarUI_FreeResources(void)
{
    if (sCalendarUIState != NULL)
    {
        Free(sCalendarUIState);
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
