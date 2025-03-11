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

#include "comfy_anim.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "ikigai_scrolling_background.h"
#include "international_string_util.h"
#include "rtc.h"
#include "type_icons.h"
#include "constants/event_objects.h"
#include "constants/weather.h"

#define tFinishedLoading data[0]
#define CALENDAR_LOAD_TIME 90 // Number of Frames

#define DAYS_IN_SEASON 28

#define MAX_NUM_GYM     9
#define MAX_NUM_WORKS   9

struct CalendarUIState
{
    MainCallback savedCallback;
    u8 loadState;
    u8 year;
    u8 season;
    u8 date;
    u8 time;
    u8 weather;
    u8 gymBattles;
    u8 buildProjects;
    u8 spriteIdSeason;
    u8 spriteIdPlayer;
    u8 spriteIdGymType;
    u8 spriteIdDate[DAYS_IN_SEASON + 1];
    u8 spriteIdWeather;
};

enum WindowIds
{
    WINDOW_SCHEDULE
};

static EWRAM_DATA struct CalendarUIState *sCalendarUIState = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg3TilemapBuffer = NULL;

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
        .priority = 1
    },
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .priority = 3
    },
    {
        .bg = 3,
        .charBaseIndex = 3,
        .mapBaseIndex = 28,
        .priority = 2
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
static const u32 sCalendarUITilemapSupplement[] = INCBIN_U32("graphics/calendar/tilemap_supplement.bin.lz");
static const u16 sCalendarUIPalette[] = INCBIN_U16("graphics/calendar/tiles.gbapal");

static const u32 sCalendarSunnyCloudsMorningIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/sunny_clouds_morning.4bpp.lz");
static const u16 sCalendarSunnyCloudsMorningIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/sunny_clouds_morning.gbapal");
static const u32 sCalendarSunnyCloudsDayIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/sunny_clouds_day.4bpp.lz");
static const u16 sCalendarSunnyCloudsDayIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/sunny_clouds_day.gbapal");
static const u32 sCalendarSunnyCloudsEveningIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/sunny_clouds_evening.4bpp.lz");
static const u16 sCalendarSunnyCloudsEveningIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/sunny_clouds_evening.gbapal");
static const u32 sCalendarFullMoonCloudsIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/full_moon_clouds.4bpp.lz");
static const u16 sCalendarFullMoonCloudsIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/full_moon_clouds.gbapal");
static const u32 sCalendarSunnyMorningIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/sunny_morning.4bpp.lz");
static const u16 sCalendarSunnyMorningIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/sunny_morning.gbapal");
static const u32 sCalendarSunnyDayIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/sunny_day.4bpp.lz");
static const u16 sCalendarSunnyDayIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/sunny_day.gbapal");
static const u32 sCalendarSunnyEveningIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/sunny_evening.4bpp.lz");
static const u16 sCalendarSunnyEveningIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/sunny_evening.gbapal");
static const u32 sCalendarFullMoonIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/full_moon.4bpp.lz");
static const u16 sCalendarFullMoonIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/full_moon.gbapal");
static const u32 sCalendarRainIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/rain.4bpp.lz");
static const u16 sCalendarRainIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/rain.gbapal");
static const u32 sCalendarSnowIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/snow.4bpp.lz");
static const u16 sCalendarSnowIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/snow.gbapal");
static const u32 sCalendarThunderstormIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/thunderstorm.4bpp.lz");
static const u16 sCalendarThunderstormIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/thunderstorm.gbapal");
static const u32 sCalendarFogIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/fog.4bpp.lz");
static const u16 sCalendarFogIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/fog.gbapal");
static const u32 sCalendarSandstormIconGfx[] = INCBIN_U32("graphics/calendar/weather_icons/sandstorm.4bpp.lz");
static const u16 sCalendarSandstormIconPal[] = INCBIN_U16("graphics/calendar/weather_icons/sandstorm.gbapal");

static const u32 sCalendarDateSelectorGfx[] = INCBIN_U32("graphics/calendar/numbers/date_selector.4bpp.lz");
static const u32 sCalendarDate1Gfx[] = INCBIN_U32("graphics/calendar/numbers/1.4bpp.lz");
static const u32 sCalendarDate2Gfx[] = INCBIN_U32("graphics/calendar/numbers/2.4bpp.lz");
static const u32 sCalendarDate3Gfx[] = INCBIN_U32("graphics/calendar/numbers/3.4bpp.lz");
static const u32 sCalendarDate4Gfx[] = INCBIN_U32("graphics/calendar/numbers/4.4bpp.lz");
static const u32 sCalendarDate5Gfx[] = INCBIN_U32("graphics/calendar/numbers/5.4bpp.lz");
static const u32 sCalendarDate6Gfx[] = INCBIN_U32("graphics/calendar/numbers/6.4bpp.lz");
static const u32 sCalendarDate7Gfx[] = INCBIN_U32("graphics/calendar/numbers/7.4bpp.lz");
static const u32 sCalendarDate8Gfx[] = INCBIN_U32("graphics/calendar/numbers/8.4bpp.lz");
static const u32 sCalendarDate9Gfx[] = INCBIN_U32("graphics/calendar/numbers/9.4bpp.lz");
static const u32 sCalendarDate10Gfx[] = INCBIN_U32("graphics/calendar/numbers/10.4bpp.lz");
static const u32 sCalendarDate11Gfx[] = INCBIN_U32("graphics/calendar/numbers/11.4bpp.lz");
static const u32 sCalendarDate12Gfx[] = INCBIN_U32("graphics/calendar/numbers/12.4bpp.lz");
static const u32 sCalendarDate13Gfx[] = INCBIN_U32("graphics/calendar/numbers/13.4bpp.lz");
static const u32 sCalendarDate14Gfx[] = INCBIN_U32("graphics/calendar/numbers/14.4bpp.lz");
static const u32 sCalendarDate15Gfx[] = INCBIN_U32("graphics/calendar/numbers/15.4bpp.lz");
static const u32 sCalendarDate16Gfx[] = INCBIN_U32("graphics/calendar/numbers/16.4bpp.lz");
static const u32 sCalendarDate17Gfx[] = INCBIN_U32("graphics/calendar/numbers/17.4bpp.lz");
static const u32 sCalendarDate18Gfx[] = INCBIN_U32("graphics/calendar/numbers/18.4bpp.lz");
static const u32 sCalendarDate19Gfx[] = INCBIN_U32("graphics/calendar/numbers/19.4bpp.lz");
static const u32 sCalendarDate20Gfx[] = INCBIN_U32("graphics/calendar/numbers/20.4bpp.lz");
static const u32 sCalendarDate21Gfx[] = INCBIN_U32("graphics/calendar/numbers/21.4bpp.lz");
static const u32 sCalendarDate22Gfx[] = INCBIN_U32("graphics/calendar/numbers/22.4bpp.lz");
static const u32 sCalendarDate23Gfx[] = INCBIN_U32("graphics/calendar/numbers/23.4bpp.lz");
static const u32 sCalendarDate24Gfx[] = INCBIN_U32("graphics/calendar/numbers/24.4bpp.lz");
static const u32 sCalendarDate25Gfx[] = INCBIN_U32("graphics/calendar/numbers/25.4bpp.lz");
static const u32 sCalendarDate26Gfx[] = INCBIN_U32("graphics/calendar/numbers/26.4bpp.lz");
static const u32 sCalendarDate27Gfx[] = INCBIN_U32("graphics/calendar/numbers/27.4bpp.lz");
static const u32 sCalendarDate28Gfx[] = INCBIN_U32("graphics/calendar/numbers/28.4bpp.lz");

struct WeatherIcons
{
    const u32 *gfx;
    const u16 *pal;
};

#define INDEX_COLOUR_TOP 1
#define INDEX_COLOUR_MIDDLE 2
#define INDEX_COLOUR_BOTTOM 3
#define TIME_COUNT TIME_NIGHT + 1
static const struct WeatherIcons sWeatherIcons[WEATHER_COUNT][TIME_COUNT] =
{
    [WEATHER_SUNNY_CLOUDS] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarSunnyCloudsMorningIconGfx,
            .pal = sCalendarSunnyCloudsMorningIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarSunnyCloudsDayIconGfx,
            .pal = sCalendarSunnyCloudsDayIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarSunnyCloudsEveningIconGfx,
            .pal = sCalendarSunnyCloudsEveningIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarFullMoonCloudsIconGfx,
            .pal = sCalendarFullMoonCloudsIconPal,
        },
    },

    [WEATHER_SUNNY] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarSunnyMorningIconGfx,
            .pal = sCalendarSunnyMorningIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarSunnyDayIconGfx,
            .pal = sCalendarSunnyDayIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarSunnyEveningIconGfx,
            .pal = sCalendarSunnyEveningIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarFullMoonIconGfx,
            .pal = sCalendarFullMoonIconPal,
        },
    },

    [WEATHER_RAIN] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarRainIconGfx,
            .pal = sCalendarRainIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarRainIconGfx,
            .pal = sCalendarRainIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarRainIconGfx,
            .pal = sCalendarRainIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarRainIconGfx,
            .pal = sCalendarRainIconPal,
        },
    },

    [WEATHER_SNOW] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarSnowIconGfx,
            .pal = sCalendarSnowIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarSnowIconGfx,
            .pal = sCalendarSnowIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarSnowIconGfx,
            .pal = sCalendarSnowIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarSnowIconGfx,
            .pal = sCalendarSnowIconPal,
        },
    },

    [WEATHER_RAIN_THUNDERSTORM] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarThunderstormIconGfx,
            .pal = sCalendarThunderstormIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarThunderstormIconGfx,
            .pal = sCalendarThunderstormIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarThunderstormIconGfx,
            .pal = sCalendarThunderstormIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarThunderstormIconGfx,
            .pal = sCalendarThunderstormIconPal,
        },
    },

    [WEATHER_FOG_HORIZONTAL] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarFogIconGfx,
            .pal = sCalendarFogIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarFogIconGfx,
            .pal = sCalendarFogIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarFogIconGfx,
            .pal = sCalendarFogIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarFogIconGfx,
            .pal = sCalendarFogIconPal,
        },
    },

    [WEATHER_SANDSTORM] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarSandstormIconGfx,
            .pal = sCalendarSandstormIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarSandstormIconGfx,
            .pal = sCalendarSandstormIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarSandstormIconGfx,
            .pal = sCalendarSandstormIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarSandstormIconGfx,
            .pal = sCalendarSandstormIconPal,
        },
    },

    [WEATHER_FOG_DIAGONAL] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarFogIconGfx,
            .pal = sCalendarFogIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarFogIconGfx,
            .pal = sCalendarFogIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarFogIconGfx,
            .pal = sCalendarFogIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarFogIconGfx,
            .pal = sCalendarFogIconPal,
        },
    },

    [WEATHER_DROUGHT] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarSunnyMorningIconGfx,
            .pal = sCalendarSunnyMorningIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarSunnyDayIconGfx,
            .pal = sCalendarSunnyDayIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarSunnyEveningIconGfx,
            .pal = sCalendarSunnyEveningIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarFullMoonIconGfx,
            .pal = sCalendarFullMoonIconPal,
        },
    },

    [WEATHER_DOWNPOUR] =
    {
        [TIME_MORNING] =
        {
            .gfx = sCalendarThunderstormIconGfx,
            .pal = sCalendarThunderstormIconPal,
        },
        [TIME_DAY] =
        {
            .gfx = sCalendarThunderstormIconGfx,
            .pal = sCalendarThunderstormIconPal,
        },
        [TIME_EVENING] =
        {
            .gfx = sCalendarThunderstormIconGfx,
            .pal = sCalendarThunderstormIconPal,
        },
        [TIME_NIGHT] =
        {
            .gfx = sCalendarThunderstormIconGfx,
            .pal = sCalendarThunderstormIconPal,
        },
    },
};

struct DateNumbers
{
    const u32 *gfx;
    u8 x;
    u8 y;
    u8 offset;
};

enum Dates
{
    DAY_1 = 1,
    DAY_2,
    DAY_3,
    DAY_4,
    DAY_5,
    DAY_6,
    DAY_7,
    DAY_8,
    DAY_9,
    DAY_10,
    DAY_11,
    DAY_12,
    DAY_13,
    DAY_14,
    DAY_15,
    DAY_16,
    DAY_17,
    DAY_18,
    DAY_19,
    DAY_20,
    DAY_21,
    DAY_22,
    DAY_23,
    DAY_24,
    DAY_25,
    DAY_26,
    DAY_27,
    DAY_28,
};

#define MONDAY_X 76
#define TUESDAY_X 100
#define WEDNESDAY_X 124
#define THURSDAY_X 148
#define FRIDAY_X 172
#define SATURDAY_X 196
#define SUNDAY_X 220
#define WEEK_1_Y 70
#define WEEK_2_Y 94
#define WEEK_3_Y 118
#define WEEK_4_Y 142

static const struct DateNumbers sDateNumbers[DAYS_IN_SEASON + 1] =
{
    [DAY_1]  = {sCalendarDate1Gfx,  MONDAY_X,    WEEK_1_Y, 0},
    [DAY_2]  = {sCalendarDate2Gfx,  TUESDAY_X,   WEEK_1_Y, 0},
    [DAY_3]  = {sCalendarDate3Gfx,  WEDNESDAY_X, WEEK_1_Y, 0},
    [DAY_4]  = {sCalendarDate4Gfx,  THURSDAY_X,  WEEK_1_Y, 0},
    [DAY_5]  = {sCalendarDate5Gfx,  FRIDAY_X,    WEEK_1_Y, 0},
    [DAY_6]  = {sCalendarDate6Gfx,  SATURDAY_X,  WEEK_1_Y, 0},
    [DAY_7]  = {sCalendarDate7Gfx,  SUNDAY_X,    WEEK_1_Y, 0},

    [DAY_8]  = {sCalendarDate8Gfx,  MONDAY_X,    WEEK_2_Y, 0},
    [DAY_9]  = {sCalendarDate9Gfx,  TUESDAY_X,   WEEK_2_Y, 0},
    [DAY_10] = {sCalendarDate10Gfx, WEDNESDAY_X, WEEK_2_Y, 0},
    [DAY_11] = {sCalendarDate11Gfx, THURSDAY_X,  WEEK_2_Y, 0},
    [DAY_12] = {sCalendarDate12Gfx, FRIDAY_X,    WEEK_2_Y, 0},
    [DAY_13] = {sCalendarDate13Gfx, SATURDAY_X,  WEEK_2_Y, 0},
    [DAY_14] = {sCalendarDate14Gfx, SUNDAY_X,    WEEK_2_Y, 0},

    [DAY_15] = {sCalendarDate15Gfx, MONDAY_X,    WEEK_3_Y, 0},
    [DAY_16] = {sCalendarDate16Gfx, TUESDAY_X,   WEEK_3_Y, 0},
    [DAY_17] = {sCalendarDate17Gfx, WEDNESDAY_X, WEEK_3_Y, 0},
    [DAY_18] = {sCalendarDate18Gfx, THURSDAY_X,  WEEK_3_Y, 0},
    [DAY_19] = {sCalendarDate19Gfx, FRIDAY_X,    WEEK_3_Y, 0},
    [DAY_20] = {sCalendarDate20Gfx, SATURDAY_X,  WEEK_3_Y, 0},
    [DAY_21] = {sCalendarDate21Gfx, SUNDAY_X,    WEEK_3_Y, 0},

    [DAY_22] = {sCalendarDate22Gfx, MONDAY_X,    WEEK_4_Y, 0},
    [DAY_23] = {sCalendarDate23Gfx, TUESDAY_X,   WEEK_4_Y, 0},
    [DAY_24] = {sCalendarDate24Gfx, WEDNESDAY_X, WEEK_4_Y, 0},
    [DAY_25] = {sCalendarDate25Gfx, THURSDAY_X,  WEEK_4_Y, 0},
    [DAY_26] = {sCalendarDate26Gfx, FRIDAY_X,    WEEK_4_Y, 0},
    [DAY_27] = {sCalendarDate27Gfx, SATURDAY_X,  WEEK_4_Y, 0},
    [DAY_28] = {sCalendarDate28Gfx, SUNDAY_X,    WEEK_4_Y, 0},
};

enum FontColor
{
    FONT_BROWN,
};
static const u8 sCalendarUIWindowFontColors[][3] =
{
    [FONT_BROWN]  = {TEXT_COLOR_TRANSPARENT,    TEXT_COLOR_DARK_GRAY,   TEXT_COLOR_LIGHT_GRAY},
};

#define DEFAULT_TEXT_REPLACEMENT_INDEX      5
#define DEFAULT_SHADOW_REPLACEMENT_INDEX    11

#define DATE_TEXT_REPLACEMENT_INDEX         3
#define DATE_SHADOW_REPLACEMENT_INDEX       10

#define TAG_WEATHER_ICON            7281
#define TAG_DATE_ICON               1827

static const struct OamData sOamData_CalendarWeatherIcon =
{
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 1,
};

static const struct OamData sOamData_CalendarDate =
{
    .size = SPRITE_SIZE(16x16),
    .shape = SPRITE_SHAPE(16x16),
    .priority = 1,
};

static const struct SpritePalette sSpritePal_CalendarDate =
{
    .data = sCalendarUIPalette,
    .tag = TAG_DATE_ICON
};

static const union AnimCmd sSpriteAnim_CalendarDate[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSpriteAnim_CalendarDateChecked[] =
{
    ANIMCMD_FRAME(4, 1),
    ANIMCMD_END,
};

static const union AnimCmd sSpriteAnim_CalendarDateCircled[] =
{
    ANIMCMD_FRAME(8, 1),
    ANIMCMD_END,
};

static const union AnimCmd *const sSpriteAnimTable_CalendarDate[] =
{
    sSpriteAnim_CalendarDate,
    sSpriteAnim_CalendarDateChecked,
    sSpriteAnim_CalendarDateCircled,
};

enum DateAnims
{
    DATE_ANIM_NONE,
    DATE_ANIM_CHECKED,
    DATE_ANIM_CIRCLED,
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
static void CalendarUI_GetData(void);
static void CalendarUI_PrintScheduleText(void);
static void CalendarUI_CreateSprites(void);
static void CalendarUI_FreeResources(void);

// Sprite Callbacks
static void PokeSphere_TypeIconCallback(struct Sprite *sprite);

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
        CalendarUI_GetData();
        CalendarUI_PrintScheduleText();
        CalendarUI_CreateSprites();
        PlaySE(SE_RG_CARD_FLIPPING);
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
    AdvanceComfyAnimations();
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
        gTasks[taskId].tFinishedLoading = 0;
    }
}

static void Task_CalendarUIMainInput(u8 taskId)
{
    gTasks[taskId].tFinishedLoading++;
    if (gTasks[taskId].tFinishedLoading >= CALENDAR_LOAD_TIME
        && JOY_NEW(A_BUTTON | B_BUTTON | START_BUTTON | SELECT_BUTTON))
    {
        PlaySE(SE_RG_CARD_FLIP);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_CalendarUIWaitFadeAndExitGracefully;
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
        ReleaseComfyAnims();
        DestroyTask(taskId);
    }
}
#define TILEMAP_BUFFER_SIZE (1024 * 2)
static bool8 CalendarUI_InitBgs(void)
{
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
    sBg2TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
    sBg3TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
    if (sBg1TilemapBuffer == NULL || sBg2TilemapBuffer == NULL || sBg3TilemapBuffer == NULL)
    {
        return FALSE;
    }

    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sCalendarUIBgTemplates, NELEMS(sCalendarUIBgTemplates));

    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);

    SetBgTilemapBuffer(2, sBg2TilemapBuffer);
    ScheduleBgCopyTilemapToVram(2);

    SetBgTilemapBuffer(3, sBg3TilemapBuffer);
    ScheduleBgCopyTilemapToVram(3);

    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    ShowBg(3);

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
            LZDecompressWram(sCalendarUITilemapSupplement, sBg3TilemapBuffer);
            sCalendarUIState->loadState++;
        }
        break;
    case 2:
        LoadPalette(sCalendarUIPalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        LoadPalette(ReturnScrollingBackgroundSeasonPalette(0), BG_PLTT_ID(1), PLTT_SIZE_4BPP);
        LoadPalette(GetTextWindowPalette(gSaveBlock2Ptr->optionsInterfaceColor + DEFAULT_TEXT_BOX_FRAME_PALETTES), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        LoadPalette(&sCalendarUIPalette[DEFAULT_TEXT_REPLACEMENT_INDEX], BG_PLTT_ID(15) + TEXT_COLOR_DARK_GRAY, sizeof(&sCalendarUIPalette[DEFAULT_TEXT_REPLACEMENT_INDEX]));
        LoadPalette(&sCalendarUIPalette[DEFAULT_SHADOW_REPLACEMENT_INDEX], BG_PLTT_ID(15) + TEXT_COLOR_LIGHT_GRAY, sizeof(&sCalendarUIPalette[DEFAULT_SHADOW_REPLACEMENT_INDEX]));
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

static void CalendarUI_GetData(void)
{
    sCalendarUIState->year = VarGet(VAR_TEMP_0);
    sCalendarUIState->season = VarGet(VAR_TEMP_1);
    sCalendarUIState->date = VarGet(VAR_TEMP_2);
    sCalendarUIState->time = VarGet(VAR_TEMP_3);
    sCalendarUIState->weather = VarGet(VAR_TEMP_4);
    sCalendarUIState->gymBattles = VarGet(VAR_TEMP_5);
    sCalendarUIState->buildProjects = VarGet(VAR_TEMP_6);
}

static void CalendarUI_PrintScheduleText(void)
{
    FillWindowPixelBuffer(WINDOW_SCHEDULE, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    u32 y = 18;
    u32 numGym = sCalendarUIState->gymBattles;
    u32 numWorks = sCalendarUIState->buildProjects;
    u8 stringNumGym[1], stringNumWorks[1];

    if (numGym > MAX_NUM_GYM)
        numGym = MAX_NUM_GYM;

    if (numWorks > MAX_NUM_WORKS)
        numWorks = MAX_NUM_WORKS;

    ConvertIntToDecimalStringN(stringNumGym, numGym, STR_CONV_MODE_RIGHT_ALIGN, 1);
    ConvertIntToDecimalStringN(stringNumWorks, numWorks, STR_CONV_MODE_RIGHT_ALIGN, 1);

    AddTextPrinterParameterized4(WINDOW_SCHEDULE, FONT_SHORT_NARROWER, 
        6, y, 0, 0,
        sCalendarUIWindowFontColors[FONT_BROWN], TEXT_SKIP_DRAW,
        COMPOUND_STRING("Gym")
    );

    AddTextPrinterParameterized4(WINDOW_SCHEDULE, FONT_SHORT_NARROW, 
        GetStringRightAlignXOffset(FONT_BW_SUMMARY_SCREEN,
            stringNumGym,
            40
        ) - 4, y, 0, 0,
        sCalendarUIWindowFontColors[FONT_BROWN], TEXT_SKIP_DRAW,
        stringNumGym
    );

    y += 12;

    AddTextPrinterParameterized4(WINDOW_SCHEDULE, FONT_SHORT_NARROWER, 
        6, y, 0, 0,
        sCalendarUIWindowFontColors[FONT_BROWN], TEXT_SKIP_DRAW,
        COMPOUND_STRING("Works")
    );

    AddTextPrinterParameterized4(WINDOW_SCHEDULE, FONT_SHORT_NARROW, 
        GetStringRightAlignXOffset(FONT_BW_SUMMARY_SCREEN,
            stringNumWorks,
            40
        ) - 4, y, 0, 0,
        sCalendarUIWindowFontColors[FONT_BROWN], TEXT_SKIP_DRAW,
        stringNumWorks
    );

    CopyWindowToVram(WINDOW_SCHEDULE, COPYWIN_GFX);
}

static void CalendarUI_CreateSprites_Season(void)
{

}

static void CalendarUI_CreateSprites_Weather(void)
{
    u32 weather = sCalendarUIState->weather;
    u32 time = sCalendarUIState->time;

    if (time >= TIME_COUNT)
        time = TIME_DAY;
    
    switch (weather)
    {
    case WEATHER_SUNNY_CLOUDS:
    case WEATHER_SUNNY:
    case WEATHER_RAIN:
    case WEATHER_SNOW:
    case WEATHER_RAIN_THUNDERSTORM:
    case WEATHER_FOG_HORIZONTAL:
    case WEATHER_SANDSTORM:
    case WEATHER_FOG_DIAGONAL:
    case WEATHER_DROUGHT:
    case WEATHER_DOWNPOUR:
        break;

    case WEATHER_SHADE:
    case WEATHER_VOLCANIC_ASH:
    case WEATHER_UNDERWATER:
    case WEATHER_UNDERWATER_BUBBLES:
    case WEATHER_ABNORMAL:
        weather = WEATHER_SUNNY_CLOUDS;
        break;
    
    case WEATHER_NONE:
    default:
        weather = WEATHER_SUNNY_CLOUDS;
        break;
    }

    struct CompressedSpriteSheet sSpriteSheet_CalendarWeatherIcon;
    sSpriteSheet_CalendarWeatherIcon.data = sWeatherIcons[weather][time].gfx;
    sSpriteSheet_CalendarWeatherIcon.size = 32*32*2/4;
    sSpriteSheet_CalendarWeatherIcon.tag = TAG_WEATHER_ICON;
    
    struct SpritePalette sSpritePal_CalendarWeatherIcon;
    sSpritePal_CalendarWeatherIcon.data = sWeatherIcons[weather][time].pal;
    sSpritePal_CalendarWeatherIcon.tag = TAG_WEATHER_ICON;
    
    struct SpriteTemplate sSpriteTemplate_CalendarWeatherIcon;
    sSpriteTemplate_CalendarWeatherIcon.tileTag = TAG_WEATHER_ICON;
    sSpriteTemplate_CalendarWeatherIcon.paletteTag = TAG_WEATHER_ICON;
    sSpriteTemplate_CalendarWeatherIcon.oam = &sOamData_CalendarWeatherIcon;
    sSpriteTemplate_CalendarWeatherIcon.anims = gDummySpriteAnimTable;
    sSpriteTemplate_CalendarWeatherIcon.images = NULL;
    sSpriteTemplate_CalendarWeatherIcon.affineAnims = gDummySpriteAffineAnimTable;
    sSpriteTemplate_CalendarWeatherIcon.callback = SpriteCallbackDummy;

    LoadCompressedSpriteSheet(&sSpriteSheet_CalendarWeatherIcon);
    LoadSpritePalette(&sSpritePal_CalendarWeatherIcon);
    sCalendarUIState->spriteIdWeather = CreateSprite(&sSpriteTemplate_CalendarWeatherIcon, 36, 86, 0);
    gSprites[sCalendarUIState->spriteIdWeather].oam.priority = 3;
    
    u16 colourTop = sWeatherIcons[WEATHER_SUNNY][time].pal[INDEX_COLOUR_TOP];
    u16 colourMiddle = sWeatherIcons[WEATHER_SUNNY][time].pal[INDEX_COLOUR_MIDDLE];
    u16 colourBottom = sWeatherIcons[WEATHER_SUNNY][time].pal[INDEX_COLOUR_BOTTOM];
    
    LoadPalette(
        &colourTop,
        OBJ_PLTT_ID(IndexOfSpritePaletteTag(
            gSprites[sCalendarUIState->spriteIdWeather].template->paletteTag)
        ) + INDEX_COLOUR_TOP,
        sizeof(colourTop)
    );
    LoadPalette(
        &colourMiddle,
        OBJ_PLTT_ID(IndexOfSpritePaletteTag(
            gSprites[sCalendarUIState->spriteIdWeather].template->paletteTag)
        ) + INDEX_COLOUR_MIDDLE,
        sizeof(colourMiddle)
    );
    LoadPalette(
        &colourBottom,
        OBJ_PLTT_ID(IndexOfSpritePaletteTag(
            gSprites[sCalendarUIState->spriteIdWeather].template->paletteTag)
        ) + INDEX_COLOUR_BOTTOM,
        sizeof(colourBottom)
    );
}

static void CalendarUI_CreateSprites_Player(void)
{
    u16 objEvent = gSaveBlock2Ptr->playerGender ? OBJ_EVENT_GFX_ANKA_NORMAL : OBJ_EVENT_GFX_KOLE_NORMAL;
    sCalendarUIState->spriteIdPlayer = CreateObjectGraphicsSprite(objEvent, SpriteCallbackDummy, 33, 130, 102);
    StartSpriteAnim(&gSprites[sCalendarUIState->spriteIdPlayer], ANIM_STD_GO_SOUTH);
}

static void CalendarUI_CreateSprites_TypeIcon(void)
{
    struct ComfyAnimEasingConfig config;

    InitComfyAnimConfig_Easing(&config);
    config.durationFrames = 60;
    config.from = Q_24_8(49 + NUM_FRAMES_HIDE_TYPE_ICON);
    config.to = Q_24_8(49);
    config.easingFunc = ComfyAnimEasing_EaseInOutBack;
    
    sCalendarUIState->spriteIdGymType = CreateBattlenMoveTypeIcon(
        49 + NUM_FRAMES_HIDE_TYPE_ICON,
        129,
        0, gSaveBlock2Ptr->ikigaiGymType
    );
    gSprites[sCalendarUIState->spriteIdGymType].oam.priority = 2;
    gSprites[sCalendarUIState->spriteIdGymType].callback = PokeSphere_TypeIconCallback;
    gSprites[sCalendarUIState->spriteIdGymType].sComfyAnimX = CreateComfyAnim_Easing(&config);
}

static void CalendarUI_CreateSprites_Dates(void)
{
    u32 date = sCalendarUIState->date;
    date = (date < DAY_1) ? DAY_1 : (date > DAY_28) ? DAY_28 : date;

    for (u32 dateCount = DAY_1; dateCount < DAYS_IN_SEASON + 1; dateCount++)
    {
        struct CompressedSpriteSheet sSpriteSheet_CalendarDate;
        sSpriteSheet_CalendarDate.data = sDateNumbers[dateCount].gfx;
        sSpriteSheet_CalendarDate.size = 2*16*16*4/2;
        sSpriteSheet_CalendarDate.tag = TAG_DATE_ICON + dateCount;
    
        struct SpriteTemplate sSpriteTemplate_CalendarDate;
        sSpriteTemplate_CalendarDate.tileTag = TAG_DATE_ICON + dateCount;
        sSpriteTemplate_CalendarDate.paletteTag = TAG_DATE_ICON;
        sSpriteTemplate_CalendarDate.oam = &sOamData_CalendarDate;
        sSpriteTemplate_CalendarDate.anims = sSpriteAnimTable_CalendarDate;
        sSpriteTemplate_CalendarDate.images = NULL;
        sSpriteTemplate_CalendarDate.affineAnims = gDummySpriteAffineAnimTable;
        sSpriteTemplate_CalendarDate.callback = SpriteCallbackDummy;

        LoadCompressedSpriteSheet(&sSpriteSheet_CalendarDate);
        sCalendarUIState->spriteIdDate[dateCount] = CreateSprite(&sSpriteTemplate_CalendarDate,
            sDateNumbers[dateCount].x,
            sDateNumbers[dateCount].y,
            1
        );

        if (dateCount < date)
            StartSpriteAnim(&gSprites[sCalendarUIState->spriteIdDate[dateCount]], DATE_ANIM_CHECKED);

        if (dateCount == date)
            StartSpriteAnim(&gSprites[sCalendarUIState->spriteIdDate[dateCount]], DATE_ANIM_CIRCLED);
        
        // LoadPalette(&sCalendarUIPalette[DEFAULT_TEXT_REPLACEMENT_INDEX],
        //     OBJ_PLTT_ID(IndexOfSpritePaletteTag(
        //         gSprites[sCalendarUIState->spriteIdDate[dateCount]].template->paletteTag)
        //     ) + DATE_TEXT_REPLACEMENT_INDEX,
        //     sizeof(&sCalendarUIPalette[DEFAULT_TEXT_REPLACEMENT_INDEX]));
        LoadPalette(&sCalendarUIPalette[DEFAULT_SHADOW_REPLACEMENT_INDEX],
            OBJ_PLTT_ID(IndexOfSpritePaletteTag(
                gSprites[sCalendarUIState->spriteIdDate[dateCount]].template->paletteTag)
            ) + DATE_SHADOW_REPLACEMENT_INDEX,
            sizeof(&sCalendarUIPalette[DEFAULT_SHADOW_REPLACEMENT_INDEX]));
    }
}

static void CalendarUI_CreateSprites(void)
{
    LoadSpritePalette(&sSpritePal_CalendarDate);
    CalendarUI_CreateSprites_Season();
    CalendarUI_CreateSprites_Player();
    CalendarUI_CreateSprites_TypeIcon();
    CalendarUI_CreateSprites_Dates();
    CalendarUI_CreateSprites_Weather();
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
    if (sBg3TilemapBuffer != NULL)
    {
        Free(sBg3TilemapBuffer);
    }
    FreeAllWindowBuffers();
    ResetSpriteData();
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
#undef tFinishedLoading
