#include "option_menu.h"
#include "rotom_start_menu.h"
#include "global.h"
#include "battle_pike.h"
#include "battle_pyramid.h"
#include "battle_pyramid_bag.h"
#include "bg.h"
#include "decompress.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_object_lock.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "field_specials.h"
#include "field_weather.h"
#include "field_screen_effect.h"
#include "frontier_pass.h"
#include "frontier_util.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item_menu.h"
#include "link.h"
#include "load_save.h"
#include "main.h"
#include "malloc.h"
#include "map_name_popup.h"
#include "menu.h"
#include "new_game.h"
#include "option_menu.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokedex.h"
#include "pokenav.h"
#include "safari_zone.h"
#include "save.h"
#include "scanline_effect.h"
#include "script.h"
#include "sprite.h"
#include "sound.h"
#include "start_menu.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trainer_card.h"
#include "window.h"
#include "union_room.h"
#include "constants/battle_frontier.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "rtc.h"
#include "event_object_movement.h"
#include "gba/isagbprint.h"
#include "random.h"
#include "fake_rtc.h"
#include "region_map.h"
#include "sample_ui.h"
#include "debug.h"
#include "vyraton.h"
#include "constants/weather.h"
#include "gba/gba.h"
#include "m4a.h"
#include "dexnav.h"
#include "wallclock.h"
#include "comfy_anim.h"
#include "pokemon_icon.h"
#include "daycare.h"
#include "trig.h"
#include "start_menu.h"


#define PHONE_OFFSCREEN_Y                   (RP_NOT_FLIP_PHONE ? 98 : 96)
#define PHONE_BASE_COLOUR_INDEX             5
#define PHONE_BG_PAL_SLOT                   14
#define TAG_ROTOM_FACE_GFX                  1234
#define TAG_PHONE_ICON_GFX                  1235
#define TAG_ROTOM_FACE_ICON_PAL             0x4654 | BLEND_IMMUNE_FLAG

#define PHONE_COMFY_SLIDE_DURATION          30
#define FACE_ICON_COMFY_SPRING_MASS         200
#define FACE_ICON_COMFY_SPRING_TENSION      25
#define FACE_ICON_COMFY_SPRING_FRICTION     800
#define FACE_ICON_COMFY_SPRING_CLAMP_AFTER  1

#define FADE_COLOUR_MAX                     0xFF
#define FADE_COLOUR_MID                     0x80
#define FADE_COLOUR_MIN                     0x00


static void RotomPhone_SmallStartMenu_ContinueInit(bool32 firstInit);
static void Task_RotomPhone_SmallStartMenu_PhoneSlideOpen(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_PhoneSlideClose(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_HandleMainInput(u8 taskId);
static void RotomPhone_SmallStartMenu_UpdateIconPaletteFade(u8 taskId);
static void RotomPhone_SmallStartMenu_RotomShutdownPreparation(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_RotomShutdown(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_CloseAndSave(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_CloseForSafari(u8 taskId);

static void RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(MainCallback callback);
static u8 UNUSED RotomPhone_SmallStartMenu_DoCleanUpAndCreateTask(TaskFunc func, u8 priority);
static void RotomPhone_SmallStartMenu_DoCleanUpAndChangeTaskFunc(u8 taskId, TaskFunc func);
static void RotomPhone_SmallStartMenu_DoCleanUpAndDestroyTask(u8 taskId, bool32 overworldCleanup);

static void RotomPhone_SmallStartMenu_LoadSprites(void);
static void RotomPhone_SmallStartMenu_CreateRotomFaceSprite(bool32 rotomLoad);
static void RotomPhone_SmallStartMenu_CreateAllSprites(void);
static void RotomPhone_SmallStartMenu_LoadBgGfx(bool32 firstInit);
static void RotomPhone_SmallStartMenu_CreateSpeechWindows(void);
static void RotomPhone_SmallStartMenu_CreateFlipPhoneWindow(void);
static void RotomPhone_SmallStartMenu_PrintGreeting(void);
static void RotomPhone_SmallStartMenu_CheckUpdateMessage(u8 taskId);
static void RotomPhone_SmallStartMenu_PrintGoodbye(u8 taskId);
static void RotomPhone_SmallStartMenu_PrintTime(u8 taskId);
static void RotomPhone_SmallStartMenu_PrintSafari(u8 taskId);
static void RotomPhone_SmallStartMenu_PrintDateWeather(u8 taskId);
static void RotomPhone_SmallStartMenu_PrintHaveFun(u8 taskId);
static void RotomPhone_SmallStartMenu_Personality(u8 taskId);
static void RotomPhone_SmallStartMenu_PrintAdventure(u8 taskId);
static void RotomPhone_SmallStartMenu_UpdateMenuPrompt(u8 taskId);
static const u8 *GetWeatherAction(u32 weatherId);


static void RotomPhone_LargeStartMenu_SetupCB(void);
static void RotomPhone_LargeStartMenu_MainCB(void);
static void RotomPhone_LargeStartMenu_VBlankCB(void);


static void Task_RotomPhone_LargeStartMenu_WaitFadeIn(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_MainInput(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_PanelInput(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_PanelSlide(u8 taskId);
static void RotomPhone_LargeStartMenu_StartPanelSlide(void);
static void Task_RotomPhone_LargeStartMenu_WaitFadeAndBail(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_WaitFadeAndExitGracefully(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_WaitFadeForSelection(u8 taskId);


static void RotomPhone_LargeStartMenu_ResetGpuRegsAndBgs(void);
static bool8 RotomPhone_LargeStartMenu_InitBgs(void);
static void RotomPhone_LargeStartMenu_FadeAndBail(void);
static bool8 RotomPhone_LargeStartMenu_LoadGraphics(void);
static void RotomPhone_LargeStartMenu_InitWindows(void);
static void RotomPhone_LargeStartMenu_PrintTopBar(void);
static void RotomPhone_LargeStartMenu_FreeResources(void);

static void RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(MainCallback callback);
static u8 RotomPhone_LargeStartMenu_DoCleanUpAndCreateTask(TaskFunc func, u8 priority);
static void UNUSED RotomPhone_LargeStartMenu_DoCleanUpAndChangeTaskFunc(u8 taskId, TaskFunc func);
static void RotomPhone_LargeStartMenu_DoCleanUpAndDestroyTask(u8 taskId);


static bool32 RotomPhone_StartMenu_UnlockedFunc_Unlocked(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_Unlocked_FullScreen(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_Pokedex(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_Pokemon(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_PokeNav(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_Save(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_SafariFlag(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_FullScreen(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_DexNav(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_Clock(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_Shortcut(void);

static void RotomPhone_StartMenu_SelectedFunc_Shortcut(void);
static void RotomPhone_StartMenu_SelectedFunc_Pokedex(void);
static void RotomPhone_StartMenu_SelectedFunc_Pokemon(void);
static void RotomPhone_StartMenu_SelectedFunc_Bag(void);
static void RotomPhone_StartMenu_SelectedFunc_PokeNav(void);
static void RotomPhone_StartMenu_SelectedFunc_Trainer(void);
static void RotomPhone_StartMenu_SelectedFunc_Save(void);
static void RotomPhone_StartMenu_SelectedFunc_Settings(void);
static void RotomPhone_StartMenu_SelectedFunc_SafariFlag(void);
static void RotomPhone_StartMenu_SelectedFunc_FullScreen(void);
static void RotomPhone_StartMenu_SelectedFunc_DexNav(void);
static void RotomPhone_StartMenu_SelectedFunc_Clock(void);
static void RotomPhone_StartMenu_SelectedFunc_Daycare(void);


static const u32 sSmallRotomTiles[] =                       INCBIN_U32("graphics/rotom_start_menu/rotom_phone_tiles.4bpp.smol");
static const u32 sSmallRotomTilemap[] =                     INCBIN_U32("graphics/rotom_start_menu/rotom_phone.bin.smolTM");
static const u32 sSmallRotomSpeechTilemap[] =               INCBIN_U32("graphics/rotom_start_menu/rotom_phone_speech.bin.smolTM");
static const u32 sFlipPhoneTiles[] =                        INCBIN_U32("graphics/rotom_start_menu/flip_phone_tiles.4bpp.smol");
static const u32 sFlipPhoneOpenTilemap[] =                  INCBIN_U32("graphics/rotom_start_menu/flip_phone_open.bin.smolTM");
static const u32 sFlipPhoneClosedTilemap[] =                INCBIN_U32("graphics/rotom_start_menu/flip_phone_closed.bin.smolTM");
static const u16 sPhoneMenuPal[] =                          INCBIN_U16("graphics/rotom_start_menu/phones.gbapal");
static const u32 sRotomPhoneFace[] =                        INCBIN_U32("graphics/rotom_start_menu/rotom_face.4bpp.smol");
static const u32 sIconsSmallGfx[] =                         INCBIN_U32("graphics/rotom_start_menu/icons.4bpp.smol");
static const u16 sIconsRotomFacePal[] =                     INCBIN_U16("graphics/rotom_start_menu/icons.gbapal");
static const u32 sRotomPhone_LargeStartMenuTiles[] =        INCBIN_U32("graphics/rotom_start_menu/rotom_phone_tiles.4bpp.smol");
static const u32 sRotomPhone_LargeStartMenuTilemap[] =      INCBIN_U32("graphics/rotom_start_menu/rotom_full_screen.bin.smolTM");
static const u32 sRotomPhone_LargeStartMenuPanelTilemap[] = INCBIN_U32("graphics/rotom_start_menu/rotom_full_screen_panel.bin.smolTM");
static const u16 sRotomPhone_LargeStartMenuPalette[] =      INCBIN_U16("graphics/rotom_start_menu/rotom_phone_tiles.gbapal");
static const u32 sRotomPhone_DaycareCompatability_Gfx[] =   INCBIN_U32("graphics/rotom_start_menu/panel/daycare_heart.4bpp.smol");
static const u16 sRotomPhone_DaycareCompatability_Pal[] =   INCBIN_U16("graphics/rotom_start_menu/panel/daycare_heart.gbapal");


#define FLIP_PHONE_BG_COLOUR        12
#define FLIP_PHONE_TEXT_FG_COLOUR   4
#define FLIP_PHONE_TEXT_BG_COLOUR   9
#define ROTOM_PHONE_BG_COLOUR       14
#define ROTOM_PHONE_TEXT_FG_COLOUR  4
#define ROTOM_PHONE_TEXT_BG_COLOUR  9
enum FontColor
{
    FONT_BLACK,
    FONT_WHITE,
    FONT_RED,
    FONT_BLUE,
    FONT_FLIP_PHONE,
    FONT_SMALL_ROTOM_PHONE,
};
static const u8 sRotomPhone_StartMenuFontColors[][3] =
{
    [FONT_BLACK]                = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_DARK_GRAY,       TEXT_COLOR_LIGHT_GRAY},
    [FONT_WHITE]                = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_WHITE,           TEXT_COLOR_DARK_GRAY},
    [FONT_RED]                  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_RED,             TEXT_COLOR_LIGHT_GRAY},
    [FONT_BLUE]                 = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_BLUE,            TEXT_COLOR_LIGHT_GRAY},
    [FONT_FLIP_PHONE]           = {FLIP_PHONE_BG_COLOUR,    FLIP_PHONE_TEXT_FG_COLOUR,  FLIP_PHONE_TEXT_BG_COLOUR},
    [FONT_SMALL_ROTOM_PHONE]    = {TEXT_COLOR_TRANSPARENT,  ROTOM_PHONE_TEXT_FG_COLOUR, ROTOM_PHONE_TEXT_BG_COLOUR},
};


enum RotomPhoneMenuItems
{
    RP_MENU_FULL_SCREEN,
    RP_MENU_FLAG,
    RP_MENU_SHORTCUT,
    RP_MENU_CLOCK,
    RP_MENU_POKEDEX,
    RP_MENU_PARTY,
    RP_MENU_DAYCARE,
    RP_MENU_BAG,
    RP_MENU_DEXNAV,
    RP_MENU_POKENAV,
    RP_MENU_TRAINER_CARD,
    RP_MENU_SAVE,
    RP_MENU_OPTIONS,
    RP_MENU_COUNT,
};
#define RP_MENU_FIRST_OPTION RP_MENU_COUNT - RP_MENU_COUNT
#define RP_MENU_LAST_OPTION  RP_MENU_COUNT - 1
static enum RotomPhoneMenuItems RotomPhone_StartMenu_GetShortcutOption(void)
{
    return RP_MENU_POKEDEX;
}

enum RotomPhoneSmallOptions
{
    RP_SMALL_OPTION_1,
    RP_SMALL_OPTION_2,
    RP_SMALL_OPTION_3,
    RP_SMALL_OPTION_4,
    RP_SMALL_OPTION_5,
    RP_SMALL_OPTION_6,
    RP_SMALL_OPTION_COUNT,
};

enum RotomPhoneFaceExpressions
{
    RP_FACE_HAPPY_UP,
    RP_FACE_SHOCKED_UP,
    RP_FACE_HAPPY,
    RP_FACE_SHOCKED,
    RP_FACE_DAZED,
    RP_FACE_AWE,
    RP_FACE_SHEEPISH,
    RP_FACE_CONFUSED,
    RP_FACE_HAPPY_WITH,
    RP_FACE_SHOCKED_WITH,
    RP_FACE_COUNT,
};
#define RP_FACE_LOOK_UP_ANIMS 2

enum RotomPhoneMessages
{
    RP_MESSAGE_GOODBYE,
    RP_MESSAGE_TIME,
    RP_MESSAGE_SAFARI,
    RP_MESSAGE_DATE_WEATHER,
    RP_MESSAGE_PERSONALITY,
    RP_MESSAGE_FUN,
    RP_MESSAGE_ADVENTURE,
    RP_MESSAGE_COUNT,
};

enum RotomPhoneMessages_Greeting
{
    RP_MESSAGE_GREETING_GOOD_DAY,
    RP_MESSAGE_GREETING_HELLO,
    RP_MESSAGE_GREETING_HI,
    RP_MESSAGE_GREETING_HOW_ARE_YOU,
    RP_MESSAGE_GREETING_COUNT,
};

enum RotomPhoneMessages_Goodbye
{
    RP_MESSAGE_GOODBYE_GOODBYE,
    RP_MESSAGE_GOODBYE_SEE_YA,
    RP_MESSAGE_GOODBYE_LOGGING_OFF,
    RP_MESSAGE_GOODBYE_POWERING_DOWN,
    RP_MESSAGE_GOODBYE_COUNT,
};

enum RotomPhoneMessages_DateWeather
{
    RP_MESSAGE_DATE_WEATHER_DATE,
    RP_MESSAGE_DATE_WEATHER_SEASON,
    RP_MESSAGE_DATE_WEATHER_CURRENT_WEATHER,
    RP_MESSAGE_DATE_WEATHER_NEXT_WEATHER,
    RP_MESSAGE_DATE_WEATHER_COUNT,
};

enum RotomPhoneMessages_Personality
{
    RP_MESSAGE_PERSONALITY_MEEP_MORP,
    RP_MESSAGE_PERSONALITY_HANDS,
    RP_MESSAGE_PERSONALITY_RINGTONE,
    RP_MESSAGE_PERSONALITY_SCANNING,
    RP_MESSAGE_PERSONALITY_COUNT,
};

enum RotomPhoneMessages_Fun
{
    RP_MESSAGE_FUN_WINNING_GRINNING,
    RP_MESSAGE_FUN_ANYMORE_BATTLES,
    RP_MESSAGE_FUN_FUN_DETECTED,
    RP_MESSAGE_FUN_SOMETHING_SILLY,
    RP_MESSAGE_FUN_COUNT,
};

enum RotomPhoneMessages_Adventure
{
    RP_MESSAGE_ADVENTURE_TO_DO,
    RP_MESSAGE_ADVENTURE_GET_LOST,
    RP_MESSAGE_ADVENTURE_STEP_STORY,
    RP_MESSAGE_ADVENTURE_PACK_CURIOSITY,
    RP_MESSAGE_ADVENTURE_COUNT,
};

enum SlidingPanelSprites
{
    RP_PANEL_SPRITE_ONE,
    RP_PANEL_SPRITE_TWO,
    RP_PANEL_SPRITE_THREE,
    RP_PANEL_SPRITE_FOUR,
    RP_PANEL_SPRITE_FIVE,
    RP_PANEL_SPRITE_SIX,
    RP_PANEL_SPRITE_COUNT,
};

enum SlidingPanelWindows
{
    RP_PANEL_WIN_ONE,
    RP_PANEL_WIN_TWO,
    RP_PANEL_WIN_THREE,
    RP_PANEL_WIN_FOUR,
    RP_PANEL_WIN_FIVE,
    RP_PANEL_WIN_SIX,
    RP_PANEL_WIN_COUNT,
};

enum IconRotomFacePaletteIndex
{
    PAL_FACE_ICON_TRANSPARENT,
    PAL_ICON_RED,
    PAL_ICON_GREEN,
    PAL_ICON_BLUE,
    PAL_ICON_YELLOW,
    PAL_ICON_PURPLE,
    PAL_ICON_PINK,
    PAL_ICON_ORANGE,
    PAL_ICON_BROWN,
    PAL_ICON_GREY,
    PAL_ICON_WHITE,
    PAL_ROTOM_OUTLINE,
    PAL_ROTOM_EYE_WHITE,
    PAL_ROTOM_EYE_TOP,
    PAL_ROTOM_EYE_BOTTOM,
    PAL_ROTOM_ARC,
};

enum RotomPhoneDaycareCompatabilityAnims
{
    RP_DAYCARE_COMPATABILITY_ANIM_NON,
    RP_DAYCARE_COMPATABILITY_ANIM_LOW,
    RP_DAYCARE_COMPATABILITY_ANIM_MED,
    RP_DAYCARE_COMPATABILITY_ANIM_MAX,
    RP_DAYCARE_COMPATABILITY_ANIM_COUNT,
};


struct RotomPhoneMenuOptions
{
    const u8 *menuName;
    const u8 *rotomAction;
    bool32 (*unlockedFunc)(void);
    void (*selectedFunc)(void);
    u32 iconPalSlot;
    bool32 fullScreenPanel;
};

struct RotomPhone_StartMenu
{
    bool32 isLoading;
    enum RotomPhoneMenuItems menuSmallOptions[RP_SMALL_OPTION_COUNT];
    u32 menuSmallRotomFaceSpriteId;
    u32 menuSmallRotomFaceFlashSpriteId;
    u32 menuSmallSpriteId[RP_SMALL_OPTION_COUNT];
    u32 menuSmallFlashSpriteId[RP_SMALL_OPTION_COUNT];
    u32 windowIdRotomSpeech_Top;
    u32 windowIdRotomSpeech_Bottom;
    u32 windowIdFlipPhone;
};

struct RotomPhone_LargeStartMenuState
{
    u8 loadState;
    u8 mode;

    u8 panelY;
    bool8 panelIsOpen;

    u8 panelSpriteIds[RP_PANEL_SPRITE_COUNT];
    u8 panelWindowIds[RP_PANEL_WIN_COUNT];
};

static EWRAM_DATA struct RotomPhone_StartMenu *sRotomPhone_SmallStartMenu = NULL;
static EWRAM_DATA bool32 menuFullScreen;
// Separate memory allocation so it persist between destroying of menu.
static EWRAM_DATA enum RotomPhoneMenuItems menuSelectedSmall;
static EWRAM_DATA enum RotomPhoneMenuItems menuSelectedLarge;
static EWRAM_DATA struct RotomPhone_LargeStartMenuState *sRotomPhone_LargeStartMenu = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;

bool32 RotomPhone_StartMenu_IsFullScreen(void)
{
    return menuFullScreen;
}


static u16 RotomPhone_GetPhoneBackgroundColour(u8 palSlot)
{
    return sPhoneMenuPal[PHONE_BASE_COLOUR_INDEX];
}

static u16 RotomPhone_GetFaceIconPaletteOriginalColour(u8 palSlot)
{
    if (RP_NOT_FLIP_PHONE && !(RP_GREY_ICONS && palSlot < PAL_ICON_WHITE))
        return sIconsRotomFacePal[palSlot];
    else
        return sIconsRotomFacePal[PAL_ICON_GREY];
}

static u16 RotomPhone_GetFaceIconPaletteHighlightColour(u8 palSlot)
{
    #define MAX_RGB_COMPONENT 28
    #define LIGHTEN_FACTOR    8
    u16 colour = RotomPhone_GetFaceIconPaletteOriginalColour(palSlot);

    s32 r = GET_R(colour);
    s32 g = GET_G(colour);
    s32 b = GET_B(colour);

    r = (r + LIGHTEN_FACTOR > MAX_RGB_COMPONENT) ? MAX_RGB_COMPONENT : r + LIGHTEN_FACTOR;
    g = (g + LIGHTEN_FACTOR > MAX_RGB_COMPONENT) ? MAX_RGB_COMPONENT : g + LIGHTEN_FACTOR;
    b = (b + LIGHTEN_FACTOR > MAX_RGB_COMPONENT) ? MAX_RGB_COMPONENT : b + LIGHTEN_FACTOR;

    return RGB(r, g, b);
    #undef MAX_RGB_COMPONENT
    #undef LIGHTEN_FACTOR
}

static void UpdateRotomSpriteFadeColours(struct Sprite* sprite, enum IconRotomFacePaletteIndex index, u8 frameNum)
{
    if (index == PAL_FACE_ICON_TRANSPARENT)
        return;
    
    s32 intensity = (((Cos(frameNum, 128) + 128) * 10) / 250);
    s32 r;
    s32 g;
    s32 b;
    u16 colour;
    u16 colourTo;
    u16 colourFrom;

    switch (index)
    {
    case PAL_ROTOM_OUTLINE:
    case PAL_ROTOM_EYE_WHITE:
    case PAL_ROTOM_EYE_TOP:
    case PAL_ROTOM_EYE_BOTTOM:
    case PAL_ROTOM_ARC:
        colourFrom = RotomPhone_GetPhoneBackgroundColour(index);
        colourTo = RotomPhone_GetFaceIconPaletteOriginalColour(index);
        break;
    
    default:
        colourFrom = RotomPhone_GetFaceIconPaletteOriginalColour(index);
        colourTo = RotomPhone_GetFaceIconPaletteHighlightColour(index);
        break;
    }

    if (intensity == 0)
    {
        colour = colourTo;
    }
    else
    {
        if (GET_R(colourFrom) <= GET_R(colourTo))
            r = (GET_R(colourTo) - (((GET_R(colourTo) - GET_R(colourFrom)) * intensity) / 10));
        else
            r = (GET_R(colourTo) + (((GET_R(colourFrom) - GET_R(colourTo)) * intensity) / 10));

        if (GET_G(colourFrom) <= GET_G(colourTo))
            g = (GET_G(colourTo) - (((GET_G(colourTo) - GET_G(colourFrom)) * intensity) / 10));
        else
            g = (GET_G(colourTo) + (((GET_G(colourFrom) - GET_G(colourTo)) * intensity) / 10));

        if (GET_B(colourFrom) <= GET_B(colourTo))
            b = (GET_B(colourTo) - (((GET_B(colourTo) - GET_B(colourFrom)) * intensity) / 10));
        else
            b = (GET_B(colourTo) + (((GET_B(colourFrom) - GET_B(colourTo)) * intensity) / 10));

        colour = RGB(r, g, b);
    }
    
    LoadPalette(&colour, OBJ_PLTT_ID(IndexOfSpritePaletteTag(sprite->template->paletteTag)) + index, sizeof(colour));
}

#define ROTOM_SPEECH_WINDOW_WIDTH   18
#define ROTOM_SPEECH_WINDOW_WIDTH_PXL ROTOM_SPEECH_WINDOW_WIDTH * 8
#define ROTOM_SPEECH_WINDOW_HEIGHT  2
#define ROTOM_SPEECH_WINDOW_LEFT    1
#define ROTOM_SPEECH_WINDOW_TOP     15
#define PHONE_STARTING_BASE_BLOCK   0xFF
static const struct WindowTemplate sWindowTemplate_RotomSpeech_Top = {
  .bg = 0, 
  .tilemapLeft = ROTOM_SPEECH_WINDOW_LEFT, 
  .tilemapTop = ROTOM_SPEECH_WINDOW_TOP, 
  .width = ROTOM_SPEECH_WINDOW_WIDTH,
  .height = ROTOM_SPEECH_WINDOW_HEIGHT, 
  .paletteNum = PHONE_BG_PAL_SLOT,
  .baseBlock = PHONE_STARTING_BASE_BLOCK
};

static const struct WindowTemplate sWindowTemplate_RotomSpeech_Bottom = {
    .bg = 0, 
    .tilemapLeft = ROTOM_SPEECH_WINDOW_LEFT, 
    .tilemapTop = ROTOM_SPEECH_WINDOW_TOP + 2, 
    .width = ROTOM_SPEECH_WINDOW_WIDTH, 
    .height = ROTOM_SPEECH_WINDOW_HEIGHT, 
    .paletteNum = PHONE_BG_PAL_SLOT,
    .baseBlock = PHONE_STARTING_BASE_BLOCK + (ROTOM_SPEECH_WINDOW_WIDTH*ROTOM_SPEECH_WINDOW_WIDTH)
};

static const struct WindowTemplate sWindowTemplate_FlipPhone = {
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 17,
    .width = 7,
    .height = 2,
    .paletteNum = PHONE_BG_PAL_SLOT,
    .baseBlock = PHONE_STARTING_BASE_BLOCK
};

enum WindowIds
{
    WIN_UI_TOP_BAR,
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
        .bg = 2,
        .charBaseIndex = 3,
        .mapBaseIndex = 29,
        .priority = 0
    }
};

static const struct WindowTemplate sRotomPhone_LargeStartMenuWindowTemplates[] =
{
    [WIN_UI_TOP_BAR] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1
    },
    DUMMY_WIN_TEMPLATE
};
#define ROTOM_FULL_SCREEN_NEXT_WIN_BASE_BLOCK                           \
sRotomPhone_LargeStartMenuWindowTemplates[WIN_UI_TOP_BAR].baseBlock +   \
(                                                                       \
    sRotomPhone_LargeStartMenuWindowTemplates[WIN_UI_TOP_BAR].height *  \
    sRotomPhone_LargeStartMenuWindowTemplates[WIN_UI_TOP_BAR].width     \
)

static const struct SpritePalette sSpritePal_Icon[] =
{
    {sIconsRotomFacePal, TAG_ROTOM_FACE_ICON_PAL},
    {NULL},
};

static const struct CompressedSpriteSheet sSpriteSheet_IconsSmall[] = 
{
    {sIconsSmallGfx, 32*352/2 , TAG_PHONE_ICON_GFX},
    {NULL},
};

static const struct CompressedSpriteSheet sSpriteSheet_RotomFace[] = 
{
    {sRotomPhoneFace, 64*768/2 , TAG_ROTOM_FACE_GFX},
    {NULL},
};

static const struct OamData gRotomIcons_Oam = {
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 0,
};

static const struct OamData sRotomFace_Oam = {
    .size = SPRITE_SIZE(64x64),
    .shape = SPRITE_SHAPE(64x64),
    .priority = 0,
};

static const union AnimCmd sAnimCmd_RotomPhone_Blank[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_FullScreen[] = {
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_Flag[] = {
    ANIMCMD_FRAME(32, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_Clock[] = {
    ANIMCMD_FRAME(48, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_Pokedex[] = {
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_Party[] = {
    ANIMCMD_FRAME(80, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_DexNav[] = {
    ANIMCMD_FRAME(96, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_Bag[] = {
    ANIMCMD_FRAME(112, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_PokeNav[] = {
    ANIMCMD_FRAME(128, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_Save[] = {
    ANIMCMD_FRAME(144, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomPhone_Options[] = {
    ANIMCMD_FRAME(160, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sSmallIconAnims[RP_MENU_COUNT] = {
    sAnimCmd_RotomPhone_FullScreen,
    sAnimCmd_RotomPhone_Flag,
    sAnimCmd_RotomPhone_Blank,          // RP_MENU_SHORTCUT
    sAnimCmd_RotomPhone_Clock,
    sAnimCmd_RotomPhone_Pokedex,
    sAnimCmd_RotomPhone_Party,
    sAnimCmd_RotomPhone_Blank,          // RP_MENU_DAYCARE
    sAnimCmd_RotomPhone_Bag,
    sAnimCmd_RotomPhone_DexNav,
    sAnimCmd_RotomPhone_PokeNav,
    sAnimCmd_RotomPhone_Blank,          // RP_MENU_TRAINER_CARD
    sAnimCmd_RotomPhone_Save,
    sAnimCmd_RotomPhone_Options,
};

static const union AnimCmd sAnimCmd_RotomFace_Happy[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomFace_Shocked[] = {
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomFace_Dazed[] = {
    ANIMCMD_FRAME(128, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomFace_Awe[] = {
    ANIMCMD_FRAME(192, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomFace_Sheepish[] = {
    ANIMCMD_FRAME(256, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomFace_Confused[] = {
    ANIMCMD_FRAME(320, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomFace_HappyWith[] = {
    ANIMCMD_FRAME(384, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomFace_ShockedWith[] = {
    ANIMCMD_FRAME(448, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomFace_HappyUp[] = {
    ANIMCMD_FRAME(512, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmd_RotomFace_ShockedUp[] = {
    ANIMCMD_FRAME(576, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sRotomFaceAnims[RP_FACE_COUNT] = {
    sAnimCmd_RotomFace_HappyUp,
    sAnimCmd_RotomFace_ShockedUp,
    sAnimCmd_RotomFace_Happy,
    sAnimCmd_RotomFace_Shocked,
    sAnimCmd_RotomFace_Dazed,
    sAnimCmd_RotomFace_Awe,
    sAnimCmd_RotomFace_Sheepish,
    sAnimCmd_RotomFace_Confused,
    sAnimCmd_RotomFace_HappyWith,
    sAnimCmd_RotomFace_ShockedWith,
};

static const struct CompressedSpriteSheet sSpriteSheet_CompatabilityIcon = {
    .data = sRotomPhone_DaycareCompatability_Gfx,
    .size = 32 * 32 * RP_DAYCARE_COMPATABILITY_ANIM_COUNT / 2,
    .tag = TAG_PHONE_ICON_GFX,
};

static const struct OamData sOam_IconDaycareCompatatbility = {
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 0,
};

static const union AnimCmd sAnim_IconDaycareCompatatbility_Not[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_IconDaycareCompatatbility_Low[] = {
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_IconDaycareCompatatbility_Med[] = {
    ANIMCMD_FRAME(32, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnim_IconDaycareCompatatbility_Max[] = {
    ANIMCMD_FRAME(48, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnims_IconDaycareCompatatbility[] =
{
    sAnim_IconDaycareCompatatbility_Not,
    sAnim_IconDaycareCompatatbility_Low,
    sAnim_IconDaycareCompatatbility_Med,
    sAnim_IconDaycareCompatatbility_Max,
};

#define sFrameNumComfyAnimId sprite->data[0]
static void SpriteCB_RotomPhoneSmall_RotomFace_Load(struct Sprite* sprite)
{
    TryAdvanceComfyAnim(&gComfyAnims[sFrameNumComfyAnimId]);
    if (gComfyAnims[sFrameNumComfyAnimId].completed)
    {
        ReleaseComfyAnim(sFrameNumComfyAnimId);
        sprite->callback = SpriteCallbackDummy;
        RotomPhone_SmallStartMenu_ContinueInit(TRUE);
        return;
    }

    u32 frameNum = ReadComfyAnimValueSmooth(&gComfyAnims[sFrameNumComfyAnimId]);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_OUTLINE, frameNum);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_EYE_WHITE, frameNum);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_EYE_TOP, frameNum);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_ARC, frameNum);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_EYE_BOTTOM, frameNum);
}

static void SpriteCB_RotomPhoneSmall_RotomFace_Unload(struct Sprite* sprite)
{
    TryAdvanceComfyAnim(&gComfyAnims[sFrameNumComfyAnimId]);
    if (gComfyAnims[sFrameNumComfyAnimId].completed)
    {
        ReleaseComfyAnim(sFrameNumComfyAnimId);
        sprite->callback = SpriteCallbackDummy;
        return;
    }

    u32 frameNum = ReadComfyAnimValueSmooth(&gComfyAnims[sFrameNumComfyAnimId]);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_OUTLINE, frameNum);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_EYE_WHITE, frameNum);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_EYE_TOP, frameNum);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_ARC, frameNum);
    UpdateRotomSpriteFadeColours(sprite, PAL_ROTOM_EYE_BOTTOM, frameNum);
}

static const struct SpriteTemplate sSpriteTemplate_RotomSmallIcon = {
    .tileTag = TAG_PHONE_ICON_GFX,
    .paletteTag = TAG_ROTOM_FACE_ICON_PAL,
    .oam = &gRotomIcons_Oam,
    .anims = sSmallIconAnims,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSpriteTemplate_RotomSmallFace = {
    .tileTag = TAG_ROTOM_FACE_GFX,
    .paletteTag = TAG_ROTOM_FACE_ICON_PAL,
    .oam = &sRotomFace_Oam,
    .callback = SpriteCallbackDummy,
    .anims = sRotomFaceAnims,
    .affineAnims = gDummySpriteAffineAnimTable,
};


static struct RotomPhoneMenuOptions sRotomPhoneOptions[RP_MENU_COUNT] =
{
    [RP_MENU_POKEDEX] =
    {
        .menuName = COMPOUND_STRING("Pokédex"),
        .rotomAction = COMPOUND_STRING("to open the Pokédex?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Pokedex,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Pokedex,
        .iconPalSlot = PAL_ICON_RED,
    },
    [RP_MENU_DEXNAV] =
    {
        .menuName = COMPOUND_STRING("DexNav"),
        .rotomAction = COMPOUND_STRING("to use the DexNav?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_DexNav,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_DexNav,
        .iconPalSlot = PAL_ICON_ORANGE,
    },
    [RP_MENU_PARTY] =
    {
        .menuName = COMPOUND_STRING("Party"),
        .rotomAction = COMPOUND_STRING("to view your Party?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Pokemon,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Pokemon,
        .iconPalSlot = PAL_ICON_YELLOW,
    },
    [RP_MENU_BAG] =
    {
        .menuName = COMPOUND_STRING("Bag"),
        .rotomAction = COMPOUND_STRING("to check your Storage?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Bag,
        .iconPalSlot = PAL_ICON_BLUE,
    },
    [RP_MENU_POKENAV] =
    {
        .menuName = COMPOUND_STRING("PokéNav"),
        .rotomAction = COMPOUND_STRING("to browse the PokéNav?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_PokeNav,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_PokeNav,
        .iconPalSlot = PAL_ICON_ORANGE,
    },
    [RP_MENU_TRAINER_CARD] =
    {
        .menuName = COMPOUND_STRING("Trainer"),
        .rotomAction = COMPOUND_STRING("to view your ID Card?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked_FullScreen,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Trainer,
    },
    [RP_MENU_SAVE] =
    {
        .menuName = COMPOUND_STRING("Save"),
        .rotomAction = COMPOUND_STRING("to write in your Journal?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Save,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Save,
        .iconPalSlot = PAL_ICON_PURPLE,
    },
    [RP_MENU_OPTIONS] =
    {
        .menuName = COMPOUND_STRING("Settings"),
        .rotomAction = COMPOUND_STRING("to change the Settings?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Settings,
        .iconPalSlot = PAL_ICON_GREEN,
    },
    [RP_MENU_FLAG] =
    {
        .menuName = COMPOUND_STRING("Retire"),
        .rotomAction = COMPOUND_STRING("to end the Safari?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_SafariFlag,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_SafariFlag,
        .iconPalSlot = PAL_ICON_GREY,
    },
    [RP_MENU_FULL_SCREEN] =
    {
        .menuName = COMPOUND_STRING("Full Screen"),
        .rotomAction = COMPOUND_STRING("to enter Full Screen?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_FullScreen,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_FullScreen,
        .iconPalSlot = PAL_ICON_GREY,
    },
    [RP_MENU_CLOCK] =
    {
        .menuName = COMPOUND_STRING("Clock"),
        .rotomAction = COMPOUND_STRING("to check the time?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Clock,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Clock,
        .iconPalSlot = PAL_ICON_GREY,
    },
    [RP_MENU_SHORTCUT] =
    {
        .menuName = COMPOUND_STRING("Shortcut"),
        .rotomAction = COMPOUND_STRING("Shortcut Action"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Shortcut,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Shortcut,
    },
    [RP_MENU_DAYCARE] =
    {
        .menuName = COMPOUND_STRING("Daycare"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked_FullScreen,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Daycare,
        .fullScreenPanel = TRUE,
    },
};

static const u8 *const gDayNameStringsTableShortned[] =
{
    COMPOUND_STRING("Fri,"),
    COMPOUND_STRING("Sat,"),
    COMPOUND_STRING("Sun,"),
    COMPOUND_STRING("Mon,"),
    COMPOUND_STRING("Tue,"),
    COMPOUND_STRING("Wed,"),
    COMPOUND_STRING("Thu,"),
};

static const u8 *const gDayNameStringsTable[] =
{
    COMPOUND_STRING("Friday"),
    COMPOUND_STRING("Saturday"),
    COMPOUND_STRING("Sunday"),
    COMPOUND_STRING("Monday"),
    COMPOUND_STRING("Tuesday"),
    COMPOUND_STRING("Wednesday"),
    COMPOUND_STRING("Thursday"),
};

static const u8 sWeatherActions[WEATHER_COUNT][24] =
{
    [WEATHER_NONE]               = _("sunny"),
    [WEATHER_SUNNY_CLOUDS]       = _("cloudy"),
    [WEATHER_SUNNY]              = _("sunny"),
    [WEATHER_RAIN]               = _("raining"),
    [WEATHER_SNOW]               = _("snowing"),
    [WEATHER_RAIN_THUNDERSTORM]  = _("thunderstorming"),
    [WEATHER_FOG_HORIZONTAL]     = _("foggy"),
    [WEATHER_VOLCANIC_ASH]       = _("ashens"),
    [WEATHER_SANDSTORM]          = _("sandstorming"),
    [WEATHER_FOG_DIAGONAL]       = _("foggy"),
    [WEATHER_UNDERWATER]         = _("dark"),
    [WEATHER_SHADE]              = _("shady"),
    [WEATHER_DROUGHT]            = _("very hot"),
    [WEATHER_DOWNPOUR]           = _("raining heavily"),
    [WEATHER_FOG]                = _("foggy"),
    [WEATHER_UNDERWATER_BUBBLES] = _("dark"),
};


static enum RotomPhoneMenuItems RotomPhone_SetFirstSelectedMenu(void)
{
    for (enum RotomPhoneMenuItems menuOption = RP_MENU_FIRST_OPTION; menuOption < RP_MENU_COUNT; menuOption++)
    {
        if (sRotomPhoneOptions[menuOption].unlockedFunc && sRotomPhoneOptions[menuOption].unlockedFunc())
        {
            return menuOption;
        }
    }

    return RP_MENU_OPTIONS;
}

#define tRotomUpdateTimer gTasks[taskId].data[0]
#define tRotomUpdateMessage gTasks[taskId].data[1]
#define tRotomMessageSoundEffect gTasks[taskId].data[2]
#define tRotomPanelComfyAnimId gTasks[taskId].data[3]
#define tRotomPanelLastY gTasks[taskId].data[4]
#define tPhoneY gTasks[taskId].data[5]
#define tPhoneComfyAnimId gTasks[taskId].data[6]
#define tPhoneCloseToSave gTasks[taskId].data[7]
#define tPhoneHighlightComfyAnimId gTasks[taskId].data[8]
void RotomPhone_SmallStartMenu_Init(bool32 firstInit)
{
    u8 taskId;

    if (!IsOverworldLinkActive())
    {
        FreezeObjectEvents();
        PlayerFreeze();
        StopPlayerAvatar();
    }

    HideMapNamePopUpWindow();
    LockPlayerFieldControls();

    if (sRotomPhone_SmallStartMenu == NULL)
    {
        sRotomPhone_SmallStartMenu = AllocZeroed(sizeof(struct RotomPhone_StartMenu));
    }

    if (sRotomPhone_SmallStartMenu == NULL)
    {
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        return;
    }

    if (RP_NOT_FLIP_PHONE && RP_UPDATE_MESSAGE_SOUND)
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x80);

    sRotomPhone_SmallStartMenu->isLoading = FALSE;
    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top = 0;
    menuFullScreen = FALSE;

    sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId = SPRITE_NONE;
    sRotomPhone_SmallStartMenu->menuSmallRotomFaceFlashSpriteId = SPRITE_NONE;
    for (enum RotomPhoneSmallOptions smallOptions = RP_SMALL_OPTION_1; smallOptions < RP_SMALL_OPTION_COUNT; smallOptions++)
    {
        sRotomPhone_SmallStartMenu->menuSmallSpriteId[smallOptions] = SPRITE_NONE;
        sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[smallOptions] = SPRITE_NONE;
    }

    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top = WINDOW_NONE;
    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom = WINDOW_NONE;
    sRotomPhone_SmallStartMenu->windowIdFlipPhone = WINDOW_NONE;

    RotomPhone_SmallStartMenu_LoadBgGfx(firstInit);
    RotomPhone_SmallStartMenu_LoadSprites();

    if (firstInit)
    {
        SetGpuReg(REG_OFFSET_BG0VOFS, -PHONE_OFFSCREEN_Y);
        taskId = CreateTask(Task_RotomPhone_SmallStartMenu_PhoneSlideOpen, 0);
        tPhoneY = PHONE_OFFSCREEN_Y;
        return;
    }

    RotomPhone_SmallStartMenu_ContinueInit(FALSE);
}

static void RotomPhone_SmallStartMenu_ContinueInit(bool32 firstInit)
{
    u8 taskId = FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_PhoneSlideOpen);

    RotomPhone_SmallStartMenu_CreateAllSprites();
    RotomPhone_SmallStartMenu_CreateRotomFaceSprite(FALSE);
    RotomPhone_SmallStartMenu_CreateSpeechWindows();
    RotomPhone_SmallStartMenu_CreateFlipPhoneWindow();
    ScheduleBgCopyTilemapToVram(0);

    if (!sRotomPhoneOptions[menuSelectedSmall].unlockedFunc || !sRotomPhoneOptions[menuSelectedSmall].unlockedFunc())
        menuSelectedSmall = RotomPhone_SetFirstSelectedMenu();

    if (firstInit)
        gTasks[taskId].func = Task_RotomPhone_SmallStartMenu_HandleMainInput;
    else
        taskId = CreateTask(Task_RotomPhone_SmallStartMenu_HandleMainInput, 0);

    struct ComfyAnimSpringConfig config;
    InitComfyAnimConfig_Spring(&config);
    config.from = Q_24_8(FADE_COLOUR_MIN);
    config.to = Q_24_8(FADE_COLOUR_MAX);
    config.mass = Q_24_8(FACE_ICON_COMFY_SPRING_MASS);
    config.tension = Q_24_8(FACE_ICON_COMFY_SPRING_TENSION);
    config.friction = Q_24_8(FACE_ICON_COMFY_SPRING_FRICTION);
    config.clampAfter = FACE_ICON_COMFY_SPRING_CLAMP_AFTER;
    tPhoneHighlightComfyAnimId = CreateComfyAnim_Spring(&config);

    tRotomUpdateTimer = RP_MESSAGE_UPDATE_TIMER / RP_NUM_MINUTES_TO_UPDATE;
    tRotomUpdateMessage = RP_MESSAGE_TIME;

    if (GetSafariZoneFlag())
        tRotomUpdateMessage = RP_MESSAGE_SAFARI;

    if (firstInit)
        RotomPhone_SmallStartMenu_PrintGreeting();
    else
        tRotomUpdateTimer = FALSE;

    RotomPhone_SmallStartMenu_UpdateMenuPrompt(taskId);
}

static void RotomPhone_SmallStartMenu_LoadSprites(void)
{
    u32 index;
    LoadSpritePalette(sSpritePal_Icon);
    index = IndexOfSpritePaletteTag(TAG_ROTOM_FACE_ICON_PAL);
    LoadPalette(sIconsRotomFacePal, OBJ_PLTT_ID(index), PLTT_SIZE_4BPP); 
    if (!RP_NOT_FLIP_PHONE || RP_GREY_ICONS)
    {
        for (enum IconRotomFacePaletteIndex colour = PAL_FACE_ICON_TRANSPARENT + 1; colour < PAL_ICON_WHITE; colour++)
        {
            LoadPalette(&sIconsRotomFacePal[PAL_ICON_GREY], OBJ_PLTT_ID(index) + colour, sizeof(u16)); 
        }
    }
    
    LoadCompressedSpriteSheet(sSpriteSheet_IconsSmall);
    LoadCompressedSpriteSheet(sSpriteSheet_RotomFace);
}

static void RotomPhone_SmallStartMenu_CreateRotomFaceSprite(bool32 rotomLoad)
{
    if (!RP_NOT_FLIP_PHONE || sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId != SPRITE_NONE)
        return;

    bool32 flash = FALSE;
    u32 x = 196;
    u32 y = 132;

    if (GetFlashLevel() > 0 || InBattlePyramid_())
        flash = TRUE;

    if (flash)
    {
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
        SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WINOBJ_OBJ);
    }

    sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId = CreateSprite(
        &sSpriteTemplate_RotomSmallFace,
        x,
        y,
        0
    );
    if (rotomLoad)
    {
        PlaySE(PMD_EVENT_SIGN_HATENA_03);
        struct Sprite *sprite = &gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId];
        sprite->callback = SpriteCB_RotomPhoneSmall_RotomFace_Load;
        StartSpriteAnim(sprite, RP_FACE_HAPPY);

        struct ComfyAnimSpringConfig config;
        InitComfyAnimConfig_Spring(&config);
        config.from = Q_24_8(FADE_COLOUR_MIN);
        config.to = Q_24_8(FADE_COLOUR_MID);
        config.mass = Q_24_8(FACE_ICON_COMFY_SPRING_MASS);
        config.tension = Q_24_8(FACE_ICON_COMFY_SPRING_TENSION);
        config.friction = Q_24_8(FACE_ICON_COMFY_SPRING_FRICTION);
        config.clampAfter = FACE_ICON_COMFY_SPRING_CLAMP_AFTER;
        sFrameNumComfyAnimId = CreateComfyAnim_Spring(&config);
    }

    if (flash)
    {
        sRotomPhone_SmallStartMenu->menuSmallRotomFaceFlashSpriteId = CreateSprite(
            &sSpriteTemplate_RotomSmallFace,
            x,
            y,
            0
        );
    }
}

static void RotomPhone_SmallStartMenu_CreateSprite(enum RotomPhoneMenuItems menuItem, enum RotomPhoneSmallOptions spriteId)
{
    bool32 flash = FALSE;
    s32 x = 190;
    s32 y = 58;
    s32 xAdd = 24;
    s32 yAdd = 22;
    u32 iconRow;
    u32 iconColumn;
    u32 animNum;
    if (menuItem != RP_MENU_SHORTCUT)
        animNum = menuItem;
    else
        animNum = RotomPhone_StartMenu_GetShortcutOption();

    if (!RP_NOT_FLIP_PHONE)
    {
        y += 24;
        yAdd = 25;
    }

    iconColumn = spriteId % 2;
    iconRow = spriteId / 2;

    if (GetFlashLevel() > 0 || InBattlePyramid_())
        flash = TRUE;

    if (flash)
    {
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJWIN_ON);
        SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WINOBJ_OBJ);
    }

    sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId] = CreateSprite(
        &sSpriteTemplate_RotomSmallIcon,
        x + (iconColumn * xAdd),
        y + (iconRow * yAdd),
        0
    );
    StartSpriteAnim(&gSprites[sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId]], animNum);

    if (flash)
    {
        sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId] = CreateSprite(
            &sSpriteTemplate_RotomSmallIcon,
            x + (iconColumn * xAdd),
            y + (iconRow * yAdd),
            0
        );
        gSprites[sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId]].oam.objMode = ST_OAM_OBJ_WINDOW;
        StartSpriteAnim(&gSprites[sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId]], animNum);
    }
}

static void RotomPhone_SmallStartMenu_CreateAllSprites(void)
{
    enum RotomPhoneSmallOptions drawn = RP_SMALL_OPTION_1;
    u32 drawnCount = RP_SMALL_OPTION_COUNT;
    if (!RP_NOT_FLIP_PHONE)
        drawnCount -= 2;
    

    for (enum RotomPhoneMenuItems menuId = RP_MENU_FIRST_OPTION; menuId < RP_MENU_COUNT && drawn < drawnCount; menuId++)
    {
        const struct RotomPhoneMenuOptions *menuOption = &sRotomPhoneOptions[menuId];

        if (menuOption->unlockedFunc && menuOption->unlockedFunc())
        {
            enum RotomPhoneSmallOptions optionSlot = RP_SMALL_OPTION_1 + drawn;

            RotomPhone_SmallStartMenu_CreateSprite(menuId, optionSlot);
            sRotomPhone_SmallStartMenu->menuSmallOptions[optionSlot] = menuId;
            drawn++;
        }
    }

    for (; drawn < RP_SMALL_OPTION_COUNT; drawn++)
    {
        sRotomPhone_SmallStartMenu->menuSmallOptions[drawn] = RP_MENU_COUNT;
    }
}

static void RotomPhone_SmallStartMenu_LoadBgGfx(bool32 firstInit)
{
    u8* buf = GetBgTilemapBuffer(0);
    const u32 *tilemap;
    LoadBgTilemap(0, 0, 0, 0);
    if (RP_NOT_FLIP_PHONE)
    {
        DecompressAndCopyTileDataToVram(0, sSmallRotomTiles, 0, 0, 0);
        DecompressDataWithHeaderWram(sSmallRotomTilemap, buf);
    }
    else
    {
        if (firstInit)
            tilemap = sFlipPhoneClosedTilemap;
        else
            tilemap = sFlipPhoneOpenTilemap;
        
        DecompressAndCopyTileDataToVram(0, sFlipPhoneTiles, 0, 0, 0);
        DecompressDataWithHeaderWram(tilemap, buf);
    }
    
    LoadPalette(sPhoneMenuPal, BG_PLTT_ID(PHONE_BG_PAL_SLOT), PLTT_SIZE_4BPP);
    ScheduleBgCopyTilemapToVram(0);
}

#define ROTOM_SPEECH_TOP_ROW_Y      1
#define ROTOM_SPEECH_BOTTOM_ROW_Y   1
static void RotomPhone_SmallStartMenu_CreateSpeechWindows(void)
{
    if (!RP_NOT_FLIP_PHONE)
        return;

    DecompressDataWithHeaderWram(sSmallRotomSpeechTilemap, GetBgTilemapBuffer(0));

    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top = AddWindow(&sWindowTemplate_RotomSpeech_Top);
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top, PIXEL_FILL(ROTOM_PHONE_BG_COLOUR));
    PutWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top);

    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom = AddWindow(&sWindowTemplate_RotomSpeech_Bottom);
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom, PIXEL_FILL(ROTOM_PHONE_BG_COLOUR));
    PutWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom);
}

static void RotomPhone_SmallStartMenu_CreateFlipPhoneWindow(void)
{
    if (RP_NOT_FLIP_PHONE)
        return;
    
    sRotomPhone_SmallStartMenu->windowIdFlipPhone = AddWindow(&sWindowTemplate_FlipPhone);
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdFlipPhone, PIXEL_FILL(FLIP_PHONE_BG_COLOUR));
    PutWindowTilemap(sRotomPhone_SmallStartMenu->windowIdFlipPhone);
}

static const u8 sText_ClearWindow[] = COMPOUND_STRING("{CLEAR_TO 190}");
static void RotomPhone_SmallStartMenu_PrintRotomSpeech(u8 textBuffer[80], bool32 top, bool32 copy)
{
    u8 fontId = GetFontIdToFit(textBuffer, ReturnNormalTextFont(), 0, ROTOM_SPEECH_WINDOW_WIDTH_PXL);
    u32 windowId;
    windowId = (top == TRUE) ? sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top : sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom;

    FillWindowPixelBuffer(windowId, PIXEL_FILL(ROTOM_PHONE_BG_COLOUR));
    AddTextPrinterParameterized4(windowId, fontId,
        GetStringCenterAlignXOffset(fontId, textBuffer, ROTOM_SPEECH_WINDOW_WIDTH_PXL),
        ROTOM_SPEECH_TOP_ROW_Y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_SMALL_ROTOM_PHONE], TEXT_SKIP_DRAW, textBuffer);
    
    if (copy)
        CopyWindowToVram(windowId, COPYWIN_GFX);
}

static void RotomPhone_SmallStartMenu_PrintGreeting(void)
{
    if (!RP_NOT_FLIP_PHONE)
        return;
    
    u8 textBuffer[80];
    enum RotomPhoneMessages_Greeting messageRotom = Random() % RP_MESSAGE_GREETING_COUNT;

    switch (messageRotom)
    {
    default:
    case RP_MESSAGE_GREETING_GOOD_DAY:
        switch (gTimeOfDay)
        {
        case TIME_MORNING:
            StringCopy(textBuffer, COMPOUND_STRING("Good morning, "));
            break;

        default:
        case TIME_DAY:
            StringCopy(textBuffer, COMPOUND_STRING("Good day, "));
            break;

        case TIME_EVENING:
            StringCopy(textBuffer, COMPOUND_STRING("Good evening, "));
            break;

        case TIME_NIGHT:
            StringCopy(textBuffer, COMPOUND_STRING("Good night, "));
            break;
        }
        break;
    
    case RP_MESSAGE_GREETING_HELLO:
        StringCopy(textBuffer, COMPOUND_STRING("Hello there, "));
        break;
    
    case RP_MESSAGE_GREETING_HI:
        StringCopy(textBuffer, COMPOUND_STRING("Hi, "));
        break;
    
    case RP_MESSAGE_GREETING_HOW_ARE_YOU:
        StringCopy(textBuffer, COMPOUND_STRING("How are you, "));
        break;
    }

    StringAppend(textBuffer, gSaveBlock3Ptr->characters.playerNickname);

    if (messageRotom != RP_MESSAGE_GREETING_HOW_ARE_YOU)
        StringAppend(textBuffer, COMPOUND_STRING("."));
    else
        StringAppend(textBuffer, COMPOUND_STRING("?"));

    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    PlaySE(PMD_EVENT_SIGN_NOTICE_01);
}

static enum RotomPhoneMessages RotomPhone_SmallStartMenu_GetRandomMessage(void)
{
    if (!RP_UPDATE_MESSAGE)
        return RP_MESSAGE_TIME;
    
    enum RotomPhoneMessages messageRandom;
    messageRandom = Random() % RP_MESSAGE_COUNT;
    while (messageRandom == RP_MESSAGE_GOODBYE
    || messageRandom == RP_MESSAGE_TIME
    || messageRandom == RP_MESSAGE_SAFARI)
    {
        messageRandom = Random() % RP_MESSAGE_COUNT;
    }

    return messageRandom;
}

static void RotomPhone_SmallStartMenu_CheckUpdateMessage(u8 taskId)
{
    if (!tRotomUpdateTimer && RP_NOT_FLIP_PHONE)
    {
        switch (tRotomUpdateMessage)
        {
        case RP_MESSAGE_GOODBYE:
            RotomPhone_SmallStartMenu_PrintGoodbye(taskId);
            break;
        
        default:
        case RP_MESSAGE_TIME:
            RotomPhone_SmallStartMenu_PrintTime(taskId);
            break;

        case RP_MESSAGE_SAFARI:
            RotomPhone_SmallStartMenu_PrintSafari(taskId);
            break;
        
        case RP_MESSAGE_DATE_WEATHER:
            RotomPhone_SmallStartMenu_PrintDateWeather(taskId);
            break;

        case RP_MESSAGE_PERSONALITY:
            RotomPhone_SmallStartMenu_Personality(taskId);
            break;

        case RP_MESSAGE_FUN:
            RotomPhone_SmallStartMenu_PrintHaveFun(taskId);
            break;

        case RP_MESSAGE_ADVENTURE:
            RotomPhone_SmallStartMenu_PrintAdventure(taskId);
            break;
        }
        tRotomUpdateTimer = RP_MESSAGE_UPDATE_TIMER;
        if (!RP_UPDATE_MESSAGE && !GetSafariZoneFlag() && tRotomUpdateMessage != RP_MESSAGE_GOODBYE)
            tRotomUpdateTimer *= 2;
        
        if (RP_UPDATE_MESSAGE_SOUND && tRotomUpdateMessage != RP_MESSAGE_GOODBYE)
            tRotomMessageSoundEffect = PMD_EVENT_SIGN_HATENA_02;
        
        if ((Random() % 100) < RP_FACE_UPDATE_PERCENT)
        {
            u32 rotomFace;
            do {
                rotomFace = Random() % (RP_FACE_COUNT - RP_FACE_LOOK_UP_ANIMS);
                rotomFace += RP_FACE_LOOK_UP_ANIMS;
            } while (rotomFace == gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId].animNum);
            StartSpriteAnim(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId], rotomFace);
        }
    }
}

static void RotomPhone_SmallStartMenu_PrintGoodbye(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_Goodbye messageRotom = Random() % RP_MESSAGE_GOODBYE_COUNT;

    switch (messageRotom)
    {
    default:
    case RP_MESSAGE_GOODBYE_GOODBYE:
        StringCopy(textBuffer, COMPOUND_STRING("Goodbye, "));
        StringAppend(textBuffer, gSaveBlock3Ptr->characters.playerNickname);
        StringAppend(textBuffer, COMPOUND_STRING("."));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, FALSE);

        StringCopy(textBuffer, COMPOUND_STRING("I'll see you later!"));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, FALSE);
        break;
    
    case RP_MESSAGE_GOODBYE_SEE_YA:
        StringCopy(textBuffer, COMPOUND_STRING("See ya later, "));
        StringAppend(textBuffer, gSaveBlock3Ptr->characters.playerNickname);
        StringAppend(textBuffer, COMPOUND_STRING("!"));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, FALSE);

        StringCopy(textBuffer, COMPOUND_STRING("Don't miss me too much."));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, FALSE);
        break;
    
    case RP_MESSAGE_GOODBYE_LOGGING_OFF:
        StringCopy(textBuffer, COMPOUND_STRING("Logging off for now…"));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, FALSE);

        StringCopy(textBuffer, COMPOUND_STRING("Catch you later, "));
        StringAppend(textBuffer, gSaveBlock3Ptr->characters.playerNickname);
        StringAppend(textBuffer, COMPOUND_STRING("."));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, FALSE);
        break;
    
    case RP_MESSAGE_GOODBYE_POWERING_DOWN:
        StringCopy(textBuffer, COMPOUND_STRING("3… 2… 1… Powering down…"));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, FALSE);

        StringCopy(textBuffer, COMPOUND_STRING("Until next time, "));
        StringAppend(textBuffer, gSaveBlock3Ptr->characters.playerNickname);
        StringAppend(textBuffer, COMPOUND_STRING("."));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, FALSE);
        break;
    }

    if (FuncIsActiveTask(Task_RotomPhone_SmallStartMenu_CloseAndSave))
    {
        StringCopy(textBuffer, COMPOUND_STRING("Preparing to record progress…"));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, FALSE);
    }

    CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top, COPYWIN_GFX);
    CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom, COPYWIN_GFX);
}

static void RotomPhone_SmallStartMenu_PrintTime(u8 taskId)
{
    u8 textBuffer[80];
    u8 time[24];

    RtcCalcLocalTime();
    FormatDecimalTimeWithoutSeconds(time, gLocalTime.hours, gLocalTime.minutes, RP_24_HOUR_MODE);
    StringCopy(textBuffer, COMPOUND_STRING("It is "));
    StringAppend(textBuffer, time);
    StringAppend(textBuffer, COMPOUND_STRING(" on "));
    StringAppend(textBuffer, gDayNameStringsTable[(gLocalTime.days % WEEKDAY_COUNT)]);
    StringAppend(textBuffer, COMPOUND_STRING("."));
    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = RotomPhone_SmallStartMenu_GetRandomMessage();
    if (GetSafariZoneFlag())
        tRotomUpdateMessage = RP_MESSAGE_SAFARI;
}

static void RotomPhone_SmallStartMenu_PrintSafari(u8 taskId)
{
    u8 textBuffer[80];
    u8 numBalls[2];
    u8 nameItem[20];

    ConvertIntToDecimalStringN(numBalls, gNumSafariBalls, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopy(textBuffer, COMPOUND_STRING("You have "));
    StringAppend(textBuffer, numBalls);
    StringAppend(textBuffer, COMPOUND_STRING(" "));
    CopyItemNameHandlePlural(ITEM_SAFARI_BALL, nameItem, gNumSafariBalls);
    StringAppend(textBuffer, nameItem);
    StringAppend(textBuffer, COMPOUND_STRING(" remaining."));
    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = RP_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_PrintDateWeather(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_DateWeather messageRotom = Random() % RP_MESSAGE_DATE_WEATHER_COUNT;

    if (messageRotom == RP_MESSAGE_DATE_WEATHER_SEASON)
    {
        switch (Ikigai_FetchSeason())
        {
        default:
        case SEASON_SPRING:
            StringCopy(textBuffer, COMPOUND_STRING("Everything's blooming, including you!"));
            break;
        
        case SEASON_SUMMER:
            StringCopy(textBuffer, COMPOUND_STRING("It's hot, it's bright, it's calling!"));
            break;
        
        case SEASON_AUTUMN:
            StringCopy(textBuffer, COMPOUND_STRING("Time for cozy vives and crunchy leaves!"));
            break;
        
        case SEASON_WINTER:
            StringCopy(textBuffer, COMPOUND_STRING("Bundle up! Even my pixels get chilly."));
            break;
        }
    }
    else if (messageRotom == RP_MESSAGE_DATE_WEATHER_CURRENT_WEATHER)
    {
        StringCopy(textBuffer, COMPOUND_STRING("Looking like it is "));
        StringAppend(textBuffer, GetWeatherAction(GetCurrentWeather()));
        StringAppend(textBuffer, COMPOUND_STRING(" right now."));
    }
    else if (messageRotom == RP_MESSAGE_DATE_WEATHER_NEXT_WEATHER && gTimeOfDay < TIME_NIGHT)
    {
        StringCopy(textBuffer, COMPOUND_STRING("It feels like it will be "));
        StringAppend(textBuffer, GetWeatherAction(Ikigai_GetCurrentVyratonWeather(gTimeOfDay + 1)));
        StringAppend(textBuffer, COMPOUND_STRING(" later."));
    }
    else
    {
        u8 textDate[2];
        u8 textYear[3];

        RtcCalcLocalTime();
        StringCopy(textBuffer, COMPOUND_STRING("The date is "));
        ConvertIntToDecimalStringN(textDate, Ikigai_GetDateFromDays(gLocalTime.days), STR_CONV_MODE_LEADING_ZEROS, 2);
        StringAppend(textBuffer, textDate);
        StringAppend(textBuffer, COMPOUND_STRING(" "));
        StringAppend(textBuffer, gSeasonNames[Ikigai_FetchSeason()]);
        StringAppend(textBuffer, COMPOUND_STRING(", Year "));
        ConvertIntToDecimalStringN(textYear, Ikigai_GetYearFromDays(gLocalTime.days), STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(textBuffer, textYear);
        StringAppend(textBuffer, COMPOUND_STRING("."));
    }

    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = RP_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_Personality(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_Personality messageRotom = Random() % RP_MESSAGE_PERSONALITY_COUNT;

    switch (messageRotom)
    {
    default:
    case RP_MESSAGE_PERSONALITY_MEEP_MORP:
        StringCopy(textBuffer, COMPOUND_STRING("Meep-morp!"));
        break;
    
    case RP_MESSAGE_PERSONALITY_HANDS:
        StringCopy(textBuffer, COMPOUND_STRING("If I had hands, I'd give you a high-five!"));
        break;
    
    case RP_MESSAGE_PERSONALITY_RINGTONE:
        StringCopy(textBuffer, COMPOUND_STRING("I've got a new ringtone to show you!"));
        break;
    
    case RP_MESSAGE_PERSONALITY_SCANNING:
        StringCopy(textBuffer, COMPOUND_STRING("Scanning… Yup, you're still awesome!"));
        break;
    }
    
    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = RP_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_PrintHaveFun(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_Fun messageRotom = Random() % RP_MESSAGE_FUN_COUNT;

    switch (messageRotom)
    {
    default:
    case RP_MESSAGE_FUN_WINNING_GRINNING:
        StringCopy(textBuffer, COMPOUND_STRING("If you're winning, I am grinning!"));
        break;
    
    case RP_MESSAGE_FUN_ANYMORE_BATTLES:
        StringCopy(textBuffer, COMPOUND_STRING("Do we have anymore battles today?"));
        break;
    
    case RP_MESSAGE_FUN_FUN_DETECTED:
        StringCopy(textBuffer, COMPOUND_STRING("Fun detected, AMPLIFYING!"));
        break;
    
    case RP_MESSAGE_FUN_SOMETHING_SILLY:
        StringCopy(textBuffer, COMPOUND_STRING("Wanna do something silly? I am in!"));
        break;
    }

    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = RP_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_PrintAdventure(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_Adventure messageRotom = Random() % RP_MESSAGE_ADVENTURE_COUNT;

    switch (messageRotom)
    {
    default:
    case RP_MESSAGE_ADVENTURE_TO_DO:
        u8 location[16];
        StringCopy(textBuffer, COMPOUND_STRING("What's there to do in "));
        GetMapName(location, GetCurrentRegionMapSectionId(), 0);
        StringAppend(textBuffer, location);
        StringAppend(textBuffer, COMPOUND_STRING("?"));
        break;
    
    case RP_MESSAGE_ADVENTURE_GET_LOST:
        StringCopy(textBuffer, COMPOUND_STRING("Let's get lost, in the best way."));
        break;
    
    case RP_MESSAGE_ADVENTURE_STEP_STORY:
        StringCopy(textBuffer, COMPOUND_STRING("One more step, one more story."));
        break;
    
    case RP_MESSAGE_ADVENTURE_PACK_CURIOSITY:
        StringCopy(textBuffer, COMPOUND_STRING("Pack your curiosity, we're going exploring."));
        break;
    }

    
    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = RP_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_UpdateMenuPrompt(u8 taskId)
{
    u8 fontId;
    if (RP_NOT_FLIP_PHONE)
    {
        u8 textBuffer[80];

        if (sRotomPhoneOptions[menuSelectedSmall].rotomAction == NULL)
        {
            StringCopy(textBuffer, COMPOUND_STRING("Invalid Option"));
            RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, TRUE);
            return;
        }

        if (Random() % 2 == TRUE)
            StringCopy(textBuffer, COMPOUND_STRING("Would you like "));
        else
            StringCopy(textBuffer, COMPOUND_STRING("Do you want "));
        
        if (menuSelectedSmall == RP_MENU_SHORTCUT)
            StringAppend(textBuffer, sRotomPhoneOptions[RotomPhone_StartMenu_GetShortcutOption()].rotomAction);
        else
            StringAppend(textBuffer, sRotomPhoneOptions[menuSelectedSmall].rotomAction);
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, TRUE);
    }
    else
    {
        u8 menuName[24];
        StringCopy(menuName, sRotomPhoneOptions[menuSelectedSmall].menuName);
        if (!StringCompare(menuName, sRotomPhoneOptions[RP_MENU_SHORTCUT].menuName))
            StringCopy(menuName, sRotomPhoneOptions[RotomPhone_StartMenu_GetShortcutOption()].menuName);

        fontId = GetFontIdToFit(menuName, ReturnNormalTextFont(), 0, sWindowTemplate_FlipPhone.width * 8);
        FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdFlipPhone, PIXEL_FILL(FLIP_PHONE_BG_COLOUR));
        AddTextPrinterParameterized4(sRotomPhone_SmallStartMenu->windowIdFlipPhone, fontId,
        GetStringCenterAlignXOffset(fontId, menuName, sWindowTemplate_FlipPhone.width * 8),
        ROTOM_SPEECH_BOTTOM_ROW_Y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_FLIP_PHONE], TEXT_SKIP_DRAW, menuName);
        CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdFlipPhone, COPYWIN_GFX);
        tRotomMessageSoundEffect = SE_BALL_TRAY_EXIT;
    }
}

static const u8 *GetWeatherAction(u32 weatherId)
{
    if ((weatherId == WEATHER_NONE || weatherId == WEATHER_SUNNY) && gTimeOfDay == TIME_NIGHT)
        weatherId = WEATHER_UNDERWATER;

    return sWeatherActions[weatherId];
}

static void RotomPhone_SmallStartMenu_RemoveWindows(void)
{
    if (sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom != WINDOW_NONE)
    {
        FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
        ClearWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom);
        CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom, COPYWIN_GFX);
        RemoveWindow(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom);
    }

    if (sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top != WINDOW_NONE)
    {
        FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
        ClearWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top);
        CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top, COPYWIN_GFX);
        RemoveWindow(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top);
    }

    if (sRotomPhone_SmallStartMenu->windowIdFlipPhone != WINDOW_NONE)
    {
        FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdFlipPhone, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
        ClearWindowTilemap(sRotomPhone_SmallStartMenu->windowIdFlipPhone);
        CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdFlipPhone, COPYWIN_GFX);
        RemoveWindow(sRotomPhone_SmallStartMenu->windowIdFlipPhone);
    }
}

static void RotomPhone_SmallStartMenu_DestroySprites(void)
{
    if (sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId != SPRITE_NONE)
    {
        FreeSpriteOamMatrix(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId]);
        DestroySprite(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId]);
    }
    if (sRotomPhone_SmallStartMenu->menuSmallRotomFaceFlashSpriteId != SPRITE_NONE)
    {
        FreeSpriteOamMatrix(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceFlashSpriteId]);
        DestroySprite(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceFlashSpriteId]);
    }
    for (enum RotomPhoneSmallOptions spriteId = RP_SMALL_OPTION_1; spriteId < RP_SMALL_OPTION_COUNT; spriteId++)
    {
        if (sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId] != SPRITE_NONE)
        {
            FreeSpriteOamMatrix(&gSprites[sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId]]);
            DestroySprite(&gSprites[sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId]]);
        }
        if (sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId] != SPRITE_NONE)
        {
            FreeSpriteOamMatrix(&gSprites[sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId]]);
            DestroySprite(&gSprites[sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId]]);
        }
    }
}

static void RotomPhone_SmallStartMenu_ExitAndClearTilemap(void)
{
    u32 i;
    u8 *buf = GetBgTilemapBuffer(0);

    RotomPhone_SmallStartMenu_RemoveWindows();

    for (i=0; i<2048; i++)
    {
        buf[i] = 0;
    }
    ScheduleBgCopyTilemapToVram(0);

    RotomPhone_SmallStartMenu_DestroySprites();

    if (sRotomPhone_SmallStartMenu != NULL)
    {
        FreeSpriteTilesByTag(TAG_PHONE_ICON_GFX); 
        FreeSpriteTilesByTag(TAG_ROTOM_FACE_GFX);  
        Free(sRotomPhone_SmallStartMenu);
        sRotomPhone_SmallStartMenu = NULL;
    }

    ReleaseComfyAnims();
    ScriptUnfreezeObjectEvents();  
    UnlockPlayerFieldControls();
}

static void RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(MainCallback callback)
{
    if (!gPaletteFade.active)
    {
        RotomPhone_SmallStartMenu_DoCleanUpAndDestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput), TRUE);
        SetMainCallback2(callback);
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;
    }
}

static u8 UNUSED RotomPhone_SmallStartMenu_DoCleanUpAndCreateTask(TaskFunc func, u8 priority)
{
    PlayRainStoppingSoundEffect();
    RotomPhone_SmallStartMenu_ExitAndClearTilemap();
    CleanupOverworldWindowsAndTilemaps();
    return CreateTask(func, priority);
}

static void RotomPhone_SmallStartMenu_DoCleanUpAndChangeTaskFunc(u8 taskId, TaskFunc func)
{
    PlayRainStoppingSoundEffect();
    RotomPhone_SmallStartMenu_ExitAndClearTilemap();
    CleanupOverworldWindowsAndTilemaps();
    gTasks[taskId].func = func;
}

static void RotomPhone_SmallStartMenu_DoCleanUpAndDestroyTask(u8 taskId, bool32 overworldCleanup)
{
    PlayRainStoppingSoundEffect();
    RotomPhone_SmallStartMenu_ExitAndClearTilemap();
    if (overworldCleanup)
        CleanupOverworldWindowsAndTilemaps();
    DestroyTask(taskId);
}

static void RotomPhone_SmallStartMenu_HandleInput(u8 taskId)
{
    enum RotomPhoneSmallOptions optionCurrent = RP_SMALL_OPTION_1;
    enum RotomPhoneFaceExpressions rotomFace;
    s32 offset;
    u32 nextIndex;

    for (enum RotomPhoneSmallOptions i = RP_SMALL_OPTION_1; i < RP_SMALL_OPTION_COUNT; i++)
    {
        if (sRotomPhone_SmallStartMenu->menuSmallOptions[i] == menuSelectedSmall)
        {
            optionCurrent = i;
            break;
        }
    }

    if (JOY_NEW(DPAD_UP))
        offset = -2;
    else if (JOY_NEW(DPAD_LEFT))
        offset = (optionCurrent % 2 == 1) ? -1 : RP_SMALL_OPTION_COUNT;
    else  if (JOY_NEW(DPAD_RIGHT))
        offset = (optionCurrent % 2 == 0) ? 1 : RP_SMALL_OPTION_COUNT;
    else
        offset = 2;

    nextIndex = optionCurrent + offset;
    if (nextIndex >= RP_SMALL_OPTION_COUNT
        || nextIndex < RP_SMALL_OPTION_1
        || sRotomPhone_SmallStartMenu->menuSmallOptions[nextIndex] == RP_MENU_COUNT)
    {
        if (RP_NOT_FLIP_PHONE)
            tRotomMessageSoundEffect = PMD_EVENT_SIGN_ANGER_02;
        else
            tRotomMessageSoundEffect = SE_CLICK;
        return;
    }

    if (menuSelectedSmall != sRotomPhone_SmallStartMenu->menuSmallOptions[nextIndex]
        && sRotomPhone_SmallStartMenu->isLoading == FALSE && !gPaletteFade.active)
    {
        u32 index = IndexOfSpritePaletteTag(TAG_ROTOM_FACE_ICON_PAL);
        LoadPalette(sIconsRotomFacePal, OBJ_PLTT_ID(index), PLTT_SIZE_4BPP);
        if (!RP_NOT_FLIP_PHONE || RP_GREY_ICONS)
        {
            for (enum IconRotomFacePaletteIndex colour = PAL_FACE_ICON_TRANSPARENT + 1; colour < PAL_ICON_WHITE; colour++)
            {
                LoadPalette(&sIconsRotomFacePal[PAL_ICON_GREY], OBJ_PLTT_ID(index) + colour, sizeof(u16)); 
            }
        }
    }
    gComfyAnims[tPhoneHighlightComfyAnimId].config.data.spring.to = Q_24_8(FADE_COLOUR_MAX);
    gComfyAnims[tPhoneHighlightComfyAnimId].position = 0;
    menuSelectedSmall = sRotomPhone_SmallStartMenu->menuSmallOptions[nextIndex];
    if (RP_NOT_FLIP_PHONE)
        tRotomMessageSoundEffect = PMD_EVENT_SIGN_ASE_01;
    else
        tRotomMessageSoundEffect = SE_CLICK;

    do {
        rotomFace = Random() % RP_FACE_LOOK_UP_ANIMS;
    } while (rotomFace == gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId].animNum);
    StartSpriteAnim(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId], rotomFace);

    RotomPhone_SmallStartMenu_UpdateMenuPrompt(taskId);
}

static void Task_RotomPhone_SmallStartMenu_PhoneSlideOpen(u8 taskId)
{
    TryAdvanceComfyAnim(&gComfyAnims[tPhoneComfyAnimId]);
    if (tPhoneY == PHONE_OFFSCREEN_Y)
    {
        struct ComfyAnimEasingConfig config;
        InitComfyAnimConfig_Easing(&config);
        config.durationFrames = PHONE_COMFY_SLIDE_DURATION;
        config.from = Q_24_8(PHONE_OFFSCREEN_Y);
        config.to = Q_24_8(0);
        config.easingFunc = ComfyAnimEasing_EaseOutQuad;
        tPhoneComfyAnimId = CreateComfyAnim_Easing(&config);
        TryAdvanceComfyAnim(&gComfyAnims[tPhoneComfyAnimId]);

        tPhoneY = ReadComfyAnimValueSmooth(&gComfyAnims[tPhoneComfyAnimId]);
    }
    else if (GetEasingComfyAnim_CurrentFrame(&gComfyAnims[tPhoneComfyAnimId]) == PHONE_COMFY_SLIDE_DURATION / 2
        && !RP_NOT_FLIP_PHONE)
    {
        DecompressDataWithHeaderWram(sFlipPhoneOpenTilemap, GetBgTilemapBuffer(0));
        ScheduleBgCopyTilemapToVram(0);
    }
    else if (tPhoneY > 0)
    {
        tPhoneY = ReadComfyAnimValueSmooth(&gComfyAnims[tPhoneComfyAnimId]);
        SetGpuReg(REG_OFFSET_BG0VOFS, -tPhoneY);
    }
    else
    {
        ReleaseComfyAnim(tPhoneComfyAnimId);
        if (RP_NOT_FLIP_PHONE)
            RotomPhone_SmallStartMenu_CreateRotomFaceSprite(TRUE);
        else
            RotomPhone_SmallStartMenu_ContinueInit(TRUE);
    }
}

static void Task_RotomPhone_SmallStartMenu_PhoneSlideClose(u8 taskId)
{
    TryAdvanceComfyAnim(&gComfyAnims[tPhoneComfyAnimId]);
    if (tPhoneY == FALSE)
    {
        struct ComfyAnimEasingConfig config;
        InitComfyAnimConfig_Easing(&config);
        config.durationFrames = PHONE_COMFY_SLIDE_DURATION;
        config.from = Q_24_8(0);
        config.to = Q_24_8(PHONE_OFFSCREEN_Y);
        config.easingFunc = ComfyAnimEasing_EaseOutQuad;
        tPhoneComfyAnimId = CreateComfyAnim_Easing(&config);
        TryAdvanceComfyAnim(&gComfyAnims[tPhoneComfyAnimId]);

        RotomPhone_SmallStartMenu_RemoveWindows();
        RotomPhone_SmallStartMenu_DestroySprites();
        if (RP_NOT_FLIP_PHONE)
            DecompressDataWithHeaderWram(sSmallRotomTilemap, GetBgTilemapBuffer(0));
        else
            DecompressDataWithHeaderWram(sFlipPhoneClosedTilemap, GetBgTilemapBuffer(0));
        ScheduleBgCopyTilemapToVram(0);
        tPhoneY = ReadComfyAnimValueSmooth(&gComfyAnims[tPhoneComfyAnimId]);
        SetGpuReg(REG_OFFSET_BG0VOFS, -tPhoneY);
    }
    else if (tPhoneY < PHONE_OFFSCREEN_Y)
    {
        tPhoneY = ReadComfyAnimValueSmooth(&gComfyAnims[tPhoneComfyAnimId]);
        SetGpuReg(REG_OFFSET_BG0VOFS, -tPhoneY);
    }
    else if (tPhoneY == PHONE_OFFSCREEN_Y)
    {
        tPhoneY++;
        RotomPhone_SmallStartMenu_ExitAndClearTilemap();
    }
    else
    {
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        ReleaseComfyAnim(tPhoneComfyAnimId);
        DestroyTask(taskId);
    }
}

static void RotomPhone_SmallStartMenu_UpdateIconPaletteFade(u8 taskId)
{
    if (gPaletteFade.active)
        return;
    
    u32 iconPal = sRotomPhoneOptions[menuSelectedSmall].iconPalSlot;

    if (menuSelectedSmall == RP_MENU_SHORTCUT)
        iconPal = sRotomPhoneOptions[RotomPhone_StartMenu_GetShortcutOption()].iconPalSlot;
    
    TryAdvanceComfyAnim(&gComfyAnims[tPhoneHighlightComfyAnimId]);
    u32 frameNum = ReadComfyAnimValueSmooth(&gComfyAnims[tPhoneHighlightComfyAnimId]);
    UpdateRotomSpriteFadeColours(
        // Uses first option as all sprites will use the same palette
        &gSprites[sRotomPhone_SmallStartMenu->menuSmallSpriteId[RP_MENU_FIRST_OPTION]],
        iconPal, 
        frameNum
    );

    if (frameNum == FADE_COLOUR_MAX)
        gComfyAnims[tPhoneHighlightComfyAnimId].config.data.spring.to = Q_24_8(FADE_COLOUR_MIN);
    else if (frameNum == FADE_COLOUR_MIN)
        gComfyAnims[tPhoneHighlightComfyAnimId].config.data.spring.to = Q_24_8(FADE_COLOUR_MAX);
}

static void Task_RotomPhone_SmallStartMenu_HandleMainInput(u8 taskId)
{
    tRotomMessageSoundEffect = MUS_DUMMY;
    RotomPhone_SmallStartMenu_CheckUpdateMessage(taskId);
    RotomPhone_SmallStartMenu_UpdateIconPaletteFade(taskId);

    if (tRotomUpdateTimer && sRotomPhone_SmallStartMenu->isLoading == FALSE && !gPaletteFade.active)
        tRotomUpdateTimer--;
    
    if (JOY_NEW(A_BUTTON))
    {
        if (sRotomPhone_SmallStartMenu->isLoading == FALSE)
        {
            if (menuSelectedSmall == RP_MENU_FULL_SCREEN)
                FadeScreen(FADE_TO_WHITE, 0);
            else if (menuSelectedSmall != RP_MENU_SAVE && menuSelectedSmall != RP_MENU_FLAG && menuSelectedSmall != RP_MENU_CLOCK)
                FadeScreen(FADE_TO_BLACK, 0);
            
            sRotomPhone_SmallStartMenu->isLoading = TRUE;
        }
    }
    else if (JOY_NEW(B_BUTTON) && sRotomPhone_SmallStartMenu->isLoading == FALSE)
    {
        if (RP_NOT_FLIP_PHONE)
        {
            gTasks[taskId].func = Task_RotomPhone_SmallStartMenu_RotomShutdown;
            RotomPhone_SmallStartMenu_RotomShutdownPreparation(taskId);
        }
        else
        {
            PlaySE(SE_BALL_TRAY_ENTER);
            tPhoneY = FALSE;
            gTasks[taskId].func = Task_RotomPhone_SmallStartMenu_PhoneSlideClose;
        }
        return;
    }
    else if (gMain.newKeys & DPAD_ANY && sRotomPhone_SmallStartMenu->isLoading == FALSE)
    {
        RotomPhone_SmallStartMenu_HandleInput(taskId);
    }
    else if (sRotomPhone_SmallStartMenu->isLoading == TRUE && sRotomPhoneOptions[menuSelectedSmall].selectedFunc)
    {
        sRotomPhoneOptions[menuSelectedSmall].selectedFunc();
    }

    if (tRotomMessageSoundEffect && !IsSEPlaying())
        PlaySE(tRotomMessageSoundEffect);
}

static void RotomPhone_SmallStartMenu_RotomShutdownPreparation(u8 taskId)
{
    PlaySE(PMD_EVENT_MOTION_HARAHERI);
    tRotomUpdateMessage = RP_MESSAGE_GOODBYE;
    tRotomUpdateTimer = FALSE;
    RotomPhone_SmallStartMenu_CheckUpdateMessage(taskId);
    struct Sprite *sprite = &gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId];
    sprite->callback = SpriteCB_RotomPhoneSmall_RotomFace_Unload;
    StartSpriteAnim(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId], RP_FACE_HAPPY_WITH);
    
    struct ComfyAnimSpringConfig config;
    InitComfyAnimConfig_Spring(&config);
    config.from = Q_24_8(FADE_COLOUR_MID);
    config.to = Q_24_8(FADE_COLOUR_MAX);
    config.mass = Q_24_8(FACE_ICON_COMFY_SPRING_MASS);
    config.tension = Q_24_8(FACE_ICON_COMFY_SPRING_TENSION);
    config.friction = Q_24_8(FACE_ICON_COMFY_SPRING_FRICTION);
    config.clampAfter = FACE_ICON_COMFY_SPRING_CLAMP_AFTER;
    sFrameNumComfyAnimId = CreateComfyAnim_Spring(&config);
}

static void Task_RotomPhone_SmallStartMenu_RotomShutdown(u8 taskId)
{
    if (gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId].callback == SpriteCallbackDummy)
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 256);
        gTasks[taskId].func = Task_RotomPhone_SmallStartMenu_PhoneSlideClose;
    }
}

static void Task_RotomPhone_SmallStartMenu_CloseAndSave(u8 taskId)
{
    TaskFunc func;
    if (RP_NOT_FLIP_PHONE)
        func = Task_RotomPhone_SmallStartMenu_RotomShutdown;
    else
        func = Task_RotomPhone_SmallStartMenu_PhoneSlideClose;

    if (!FuncIsActiveTask(Task_RotomPhone_SmallStartMenu_RotomShutdown)
        && !FuncIsActiveTask(Task_RotomPhone_SmallStartMenu_PhoneSlideClose)
        && tPhoneCloseToSave == FALSE)
    {
        CreateTask(func, 0);
        tPhoneCloseToSave = TRUE;
    }
    else if (!FuncIsActiveTask(Task_RotomPhone_SmallStartMenu_RotomShutdown)
        && !FuncIsActiveTask(Task_RotomPhone_SmallStartMenu_PhoneSlideClose)
        && tPhoneCloseToSave == TRUE)
    {
        LoadMessageBoxAndBorderGfx();
        DestroyTask(taskId);
        SaveGame();
    }
}

static void Task_RotomPhone_SmallStartMenu_CloseForSafari(u8 taskId)
{
    if (!FuncIsActiveTask(Task_RotomPhone_SmallStartMenu_PhoneSlideClose)
        && tPhoneCloseToSave == FALSE)
    {
        CreateTask(Task_RotomPhone_SmallStartMenu_PhoneSlideClose, 0);
        tPhoneCloseToSave = TRUE;
    }
    else if (!FuncIsActiveTask(Task_RotomPhone_SmallStartMenu_PhoneSlideClose)
        && tPhoneCloseToSave == TRUE)
    {
        DestroyTask(taskId);
        SafariZoneRetirePrompt();
    }
}



void Task_OpenRotomPhone_LargeStartMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        RotomPhone_LargeStartMenu_Init();
        DestroyTask(taskId);
    }
}

void RotomPhone_LargeStartMenu_Init(void)
{
    sRotomPhone_LargeStartMenu = AllocZeroed(sizeof(struct RotomPhone_LargeStartMenuState));
    if (sRotomPhone_LargeStartMenu == NULL)
    {
        menuFullScreen = FALSE;
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        return;
    }

    sRotomPhone_LargeStartMenu->loadState = 0;

    SetMainCallback2(RotomPhone_LargeStartMenu_SetupCB);
}

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
            sRotomPhone_LargeStartMenu->loadState = 0;
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
        sRotomPhone_LargeStartMenu->panelY = 0;
        sRotomPhone_LargeStartMenu->panelIsOpen = FALSE;

        for (enum SlidingPanelSprites spritePanel = RP_PANEL_SPRITE_ONE; spritePanel < RP_PANEL_SPRITE_COUNT; spritePanel++)
        {
            sRotomPhone_LargeStartMenu->panelSpriteIds[spritePanel] = SPRITE_NONE;
        }

        for (enum SlidingPanelWindows windowPanel = RP_PANEL_WIN_ONE; windowPanel < RP_PANEL_WIN_COUNT; windowPanel++)
        {
            sRotomPhone_LargeStartMenu->panelWindowIds[windowPanel] = SPRITE_NONE;
        }

        CreateTask(Task_RotomPhone_LargeStartMenu_WaitFadeIn, 0);
        gMain.state++;
        break;
    case 6:
        if (!RotomPhone_StartMenu_IsFullScreen())
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_WHITE);
        else
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    case 7:
        SetVBlankCallback(RotomPhone_LargeStartMenu_VBlankCB);
        SetMainCallback2(RotomPhone_LargeStartMenu_MainCB);
        gMain.state++;
    case 8:
        menuFullScreen = TRUE;
        if (!sRotomPhoneOptions[menuSelectedLarge].unlockedFunc || !sRotomPhoneOptions[menuSelectedLarge].unlockedFunc())
            menuSelectedLarge = RotomPhone_SetFirstSelectedMenu();

        RotomPhone_LargeStartMenu_PrintTopBar();
        break;
    }
}

static void RotomPhone_LargeStartMenu_MainCB(void)
{
    RunTasks();
    AdvanceComfyAnimations();
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
    if (JOY_NEW(DPAD_LEFT))
    {
        PlaySE(SE_SELECT);
        do {
            if (menuSelectedLarge == RP_MENU_FIRST_OPTION)
                menuSelectedLarge = RP_MENU_LAST_OPTION;
            else
                menuSelectedLarge--;
        } while (!sRotomPhoneOptions[menuSelectedLarge].unlockedFunc());
        RotomPhone_LargeStartMenu_PrintTopBar();
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
        do {
            if (menuSelectedLarge == RP_MENU_LAST_OPTION)
                menuSelectedLarge = RP_MENU_FIRST_OPTION;
            else
                menuSelectedLarge++;
        } while (!sRotomPhoneOptions[menuSelectedLarge].unlockedFunc());
        RotomPhone_LargeStartMenu_PrintTopBar();
    }
    if (JOY_NEW(A_BUTTON | START_BUTTON))
    {
        if (JOY_NEW(START_BUTTON))
            menuSelectedLarge = RotomPhone_StartMenu_GetShortcutOption();
        
        PlaySE(SE_SELECT);
        if (!sRotomPhoneOptions[menuSelectedLarge].fullScreenPanel)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_WaitFadeForSelection;
        }
        else if (sRotomPhoneOptions[menuSelectedLarge].selectedFunc)
        {
            RotomPhone_LargeStartMenu_StartPanelSlide();
            sRotomPhoneOptions[menuSelectedLarge].selectedFunc();
        }
    }
}

static void Task_RotomPhone_LargeStartMenu_PanelInput(u8 taskId)
{
    if (JOY_NEW(START_BUTTON | A_BUTTON | B_BUTTON))
    {
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelSlide;
        tRotomPanelComfyAnimId = INVALID_COMFY_ANIM;
        PlaySE(SE_SELECT);
    }
}

static void Task_RotomPhone_LargeStartMenu_PanelSlide(u8 taskId)
{
    #define PANEL_MIN_Y 0
    #define PANEL_MAX_Y 96
    #define PANEL_SLIDE_DOWN_FRAMES 50
    #define PANEL_SLIDE_UP_FRAMES 40
    
    SetGpuReg(REG_OFFSET_BG2VOFS, sRotomPhone_LargeStartMenu->panelY);

    if (sRotomPhone_LargeStartMenu->panelIsOpen)
    {
        if (sRotomPhone_LargeStartMenu->panelY > PANEL_MIN_Y)
        {
            if (tRotomPanelComfyAnimId == INVALID_COMFY_ANIM)
            {
                struct ComfyAnimEasingConfig config;
                InitComfyAnimConfig_Easing(&config);
                config.durationFrames = PANEL_SLIDE_DOWN_FRAMES;
                config.from = Q_24_8(PANEL_MAX_Y);
                config.to = Q_24_8(PANEL_MIN_Y);
                config.easingFunc = ComfyAnimEasing_EaseOutCubic;
                tRotomPanelComfyAnimId = CreateComfyAnim_Easing(&config);
            }
            
            tRotomPanelLastY = sRotomPhone_LargeStartMenu->panelY;
            sRotomPhone_LargeStartMenu->panelY = ReadComfyAnimValueSmooth(&gComfyAnims[tRotomPanelComfyAnimId]);
            s8 yDifference = tRotomPanelLastY - sRotomPhone_LargeStartMenu->panelY;
            if (RP_PANEL_SPRITE_ONE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_ONE]].y += yDifference;
            if (RP_PANEL_SPRITE_TWO != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_TWO]].y += yDifference;
            if (RP_PANEL_SPRITE_THREE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_THREE]].y += yDifference;
            if (RP_PANEL_SPRITE_FOUR != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_FOUR]].y += yDifference;
            if (RP_PANEL_SPRITE_FIVE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_FIVE]].y += yDifference;
            if (RP_PANEL_SPRITE_SIX != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_SIX]].y += yDifference;
        }
        else if (sRotomPhone_LargeStartMenu->panelY == PANEL_MIN_Y)
        {
            if (sRotomPhoneOptions[menuSelectedLarge].selectedFunc)
                sRotomPhoneOptions[menuSelectedLarge].selectedFunc();
            sRotomPhone_LargeStartMenu->panelIsOpen = FALSE;
            ReleaseComfyAnim(tRotomPanelComfyAnimId);
            gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_MainInput;
        }
    }
    else
    {
        if (sRotomPhone_LargeStartMenu->panelY < PANEL_MAX_Y)
        {
            if (tRotomPanelComfyAnimId == INVALID_COMFY_ANIM)
            {
                struct ComfyAnimEasingConfig config;
                InitComfyAnimConfig_Easing(&config);
                config.durationFrames = PANEL_SLIDE_UP_FRAMES;
                config.from = Q_24_8(PANEL_MIN_Y);
                config.to = Q_24_8(PANEL_MAX_Y);
                config.easingFunc = ComfyAnimEasing_EaseInOutCubic;
                tRotomPanelComfyAnimId = CreateComfyAnim_Easing(&config);
            }
            
            tRotomPanelLastY = sRotomPhone_LargeStartMenu->panelY;
            sRotomPhone_LargeStartMenu->panelY = ReadComfyAnimValueSmooth(&gComfyAnims[tRotomPanelComfyAnimId]);
            s8 yDifference = tRotomPanelLastY - sRotomPhone_LargeStartMenu->panelY;
            if (RP_PANEL_SPRITE_ONE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_ONE]].y += yDifference;
            if (RP_PANEL_SPRITE_TWO != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_TWO]].y += yDifference;
            if (RP_PANEL_SPRITE_THREE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_THREE]].y += yDifference;
            if (RP_PANEL_SPRITE_FOUR != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_FOUR]].y += yDifference;
            if (RP_PANEL_SPRITE_FIVE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_FIVE]].y += yDifference;
            if (RP_PANEL_SPRITE_SIX != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_SIX]].y += yDifference;
        }
        else if (sRotomPhone_LargeStartMenu->panelY == PANEL_MAX_Y)
        {
            sRotomPhone_LargeStartMenu->panelIsOpen = TRUE;
            ReleaseComfyAnim(tRotomPanelComfyAnimId);
            gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelInput;
        }
    }
    #undef PANEL_MIN_Y
    #undef PANEL_MAX_Y
    #undef PANEL_SLIDE_DOWN_FRAMES
    #undef PANEL_SLIDE_UP_FRAMES
}

static void RotomPhone_LargeStartMenu_StartPanelSlide(void)
{
    u8 taskId = FindTaskIdByFunc(Task_RotomPhone_LargeStartMenu_MainInput);
    gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelSlide;
    tRotomPanelComfyAnimId = INVALID_COMFY_ANIM;
}

static void Task_RotomPhone_LargeStartMenu_WaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        menuFullScreen = FALSE;
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        RotomPhone_LargeStartMenu_DoCleanUpAndDestroyTask(taskId);
    }
}

static void Task_RotomPhone_LargeStartMenu_WaitFadeAndExitGracefully(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        menuFullScreen = FALSE;
        SetMainCallback2(CB2_ReturnToField);
        RotomPhone_LargeStartMenu_DoCleanUpAndDestroyTask(taskId);
    }
}

static void Task_RotomPhone_LargeStartMenu_WaitFadeForSelection(u8 taskId)
{
    if (!gPaletteFade.active && sRotomPhoneOptions[menuSelectedLarge].selectedFunc)
    {
        DestroyTask(taskId);
        sRotomPhoneOptions[menuSelectedLarge].selectedFunc();
    }
}

static void RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(MainCallback callback)
{
    if (!gPaletteFade.active)
    {
        gMain.savedCallback = CB2_ReturnToFieldWithOpenMenu;
        RotomPhone_LargeStartMenu_FreeResources();
        SetMainCallback2(callback);
    }
}

static u8 RotomPhone_LargeStartMenu_DoCleanUpAndCreateTask(TaskFunc func, u8 priority)
{
    RotomPhone_LargeStartMenu_FreeResources();
    return CreateTask(func, priority);
}

static void UNUSED RotomPhone_LargeStartMenu_DoCleanUpAndChangeTaskFunc(u8 taskId, TaskFunc func)
{
    RotomPhone_LargeStartMenu_FreeResources();
    gTasks[taskId].func = func;
}

static void RotomPhone_LargeStartMenu_DoCleanUpAndDestroyTask(u8 taskId)
{
    RotomPhone_LargeStartMenu_FreeResources();
    DestroyTask(taskId);
}

static bool8 RotomPhone_LargeStartMenu_InitBgs(void)
{
    ResetAllBgsCoordinates();

    sBg1TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);
    if (sBg1TilemapBuffer == NULL)
    {
        return FALSE;
    }
    sBg2TilemapBuffer = AllocZeroed(BG_SCREEN_SIZE);
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

static void RotomPhone_LargeStartMenu_FadeAndBail(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_RotomPhone_LargeStartMenu_WaitFadeAndBail, 0);
    SetVBlankCallback(RotomPhone_LargeStartMenu_VBlankCB);
    SetMainCallback2(RotomPhone_LargeStartMenu_MainCB);
}

static bool8 RotomPhone_LargeStartMenu_LoadGraphics(void)
{
    switch (sRotomPhone_LargeStartMenu->loadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sRotomPhone_LargeStartMenuTiles, 0, 0, 0);
        sRotomPhone_LargeStartMenu->loadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            DecompressDataWithHeaderWram(sRotomPhone_LargeStartMenuTilemap, sBg1TilemapBuffer);
            DecompressDataWithHeaderWram(sRotomPhone_LargeStartMenuPanelTilemap, sBg2TilemapBuffer);
            sRotomPhone_LargeStartMenu->loadState++;
        }
        break;
    case 2:
        LoadPalette(sRotomPhone_LargeStartMenuPalette, BG_PLTT_ID(PHONE_BG_PAL_SLOT), PLTT_SIZE_4BPP);
        LoadPalette(GetTextWindowPalette(gSaveBlock2Ptr->optionsInterfaceColor + DEFAULT_TEXT_BOX_FRAME_PALETTES), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
        sRotomPhone_LargeStartMenu->loadState++;
    default:
        sRotomPhone_LargeStartMenu->loadState = 0;
        return TRUE;
    }
    return FALSE;
}

static void RotomPhone_LargeStartMenu_InitWindows(void)
{
    InitWindows(sRotomPhone_LargeStartMenuWindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    FillWindowPixelBuffer(WIN_UI_TOP_BAR, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    PutWindowTilemap(WIN_UI_TOP_BAR);
    CopyWindowToVram(WIN_UI_TOP_BAR, COPYWIN_FULL);
}

static void RotomPhone_LargeStartMenu_PrintTopBar(void)
{
    FillWindowPixelBuffer(WIN_UI_TOP_BAR, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    AddTextPrinterParameterized4(WIN_UI_TOP_BAR, FONT_NORMAL,
        GetStringCenterAlignXOffset(FONT_NORMAL, sRotomPhoneOptions[menuSelectedLarge].menuName, sRotomPhone_LargeStartMenuWindowTemplates[WIN_UI_TOP_BAR].width * 8),
        0, 0, 0,
        sRotomPhone_StartMenuFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sRotomPhoneOptions[menuSelectedLarge].menuName);

    CopyWindowToVram(WIN_UI_TOP_BAR, COPYWIN_GFX);
}

static void RotomPhone_LargeStartMenu_FreeResources(void)
{
    if (sRotomPhone_LargeStartMenu != NULL)
    {
        Free(sRotomPhone_LargeStartMenu);
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
    ReleaseComfyAnims();
    ResetSpriteData();
}


static bool32 RotomPhone_StartMenu_UnlockedFunc_Unlocked(void)
{
    return TRUE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Unlocked_FullScreen(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        return FALSE;
    else
        return TRUE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Pokedex(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen() && !GetSafariZoneFlag())
        return FALSE;
    else
        return FlagGet(FLAG_SYS_POKEDEX_GET);
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Pokemon(void)
{
    return FlagGet(FLAG_SYS_POKEMON_GET);
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_PokeNav(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        return FALSE;
    else
        return FlagGet(FLAG_SYS_POKENAV_GET);
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Save(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        return !GetSafariZoneFlag();
    else
        return TRUE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_SafariFlag(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        return GetSafariZoneFlag();
    else
        return FALSE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_FullScreen(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        return RP_NOT_FLIP_PHONE && !GetSafariZoneFlag();
    else
        return FALSE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_DexNav(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen() && !GetSafariZoneFlag())
        return FALSE;
    else
        return FlagGet(DN_FLAG_DEXNAV_GET);
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Clock(void)
{
    if (!RP_NOT_FLIP_PHONE || (RotomPhone_StartMenu_IsFullScreen()))
        return TRUE;
    else
        return FALSE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Shortcut(void)
{
    if (RP_NOT_FLIP_PHONE && (!RotomPhone_StartMenu_IsFullScreen()) && !GetSafariZoneFlag())
        return TRUE;
    else
        return FALSE;
}


static void RotomPhone_StartMenu_SelectedFunc_Shortcut(void)
{
    sRotomPhoneOptions[RotomPhone_StartMenu_GetShortcutOption()].selectedFunc();
}

static void RotomPhone_StartMenu_SelectedFunc_Pokedex(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_OpenPokedex);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_OpenPokedex);
}

static void RotomPhone_StartMenu_SelectedFunc_Pokemon(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_PartyMenuFromStartMenu);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_PartyMenuFromStartMenu);
}

static void RotomPhone_StartMenu_SelectedFunc_Bag(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_BagMenuFromStartMenu);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_BagMenuFromStartMenu);
}

static void RotomPhone_StartMenu_SelectedFunc_PokeNav(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_InitPokeNav);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_InitPokeNav);
}

static void RotomPhone_StartMenu_ChooseTrainerCard(void)
{
    if (IsOverworldLinkActive() || InUnionRoom())
        ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
    else if (FlagGet(FLAG_SYS_FRONTIER_PASS))
        ShowFrontierPass(CB2_ReturnToFieldWithOpenMenu); // Display frontier pass
    else
        ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
}

static void RotomPhone_StartMenu_SelectedFunc_Trainer(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
    {
        if (!gPaletteFade.active)
        {
            RotomPhone_SmallStartMenu_DoCleanUpAndDestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput), TRUE);
            RotomPhone_StartMenu_ChooseTrainerCard();
        }
    }
    else
    {
        RotomPhone_LargeStartMenu_DoCleanUpAndDestroyTask(FindTaskIdByFunc(Task_RotomPhone_LargeStartMenu_MainInput));
        RotomPhone_StartMenu_ChooseTrainerCard();
    }
}

static void RotomPhone_StartMenu_SelectedFunc_Save(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
    {
        if (!gPaletteFade.active)
        {
            LockPlayerFieldControls();
            u8 taskId = FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput);
            gTasks[taskId].func = Task_RotomPhone_SmallStartMenu_CloseAndSave;
            tPhoneCloseToSave = FALSE;
            if (RP_NOT_FLIP_PHONE)
                RotomPhone_SmallStartMenu_RotomShutdownPreparation(taskId);
        }
    }
    else
    {
        SaveGame();
        // ^ May be able to use after developing Save screen.
    }
}

static void RotomPhone_StartMenu_SelectedFunc_Settings(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_InitOptionMenu);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_InitOptionMenu);
}

static void RotomPhone_StartMenu_SelectedFunc_SafariFlag(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
    {
        if (!gPaletteFade.active)
        {
            FreezeObjectEvents();
            LockPlayerFieldControls();
            gTasks[FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput)].func = Task_RotomPhone_SmallStartMenu_CloseForSafari;
        }
    }
    else
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        CreateTask(Task_RotomPhone_LargeStartMenu_WaitFadeIn, 0);
    }
}

static void RotomPhone_StartMenu_SelectedFunc_FullScreen(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
    {
        if (!gPaletteFade.active)
            RotomPhone_SmallStartMenu_DoCleanUpAndChangeTaskFunc(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput), Task_OpenRotomPhone_LargeStartMenu);
    }
    else
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        CreateTask(Task_RotomPhone_LargeStartMenu_WaitFadeIn, 0);
    }
}

static void RotomPhone_StartMenu_SelectedFunc_DexNav(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeTaskFunc(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput), Task_OpenDexNavFromStartMenu);
    else
    {
        RotomPhone_LargeStartMenu_DoCleanUpAndCreateTask(Task_OpenDexNavFromStartMenu, 0);
    }
}

static void RotomPhone_StartMenu_SelectedFunc_Clock(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
    {
        u8 taskId = FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput);
        if (taskId == TASK_NONE)
            return;
        
        u8 time[24];
        RtcCalcLocalTime();
        FormatDecimalTimeWithoutSeconds(time, gLocalTime.hours, gLocalTime.minutes, RP_24_HOUR_MODE);
        u8 fontId = GetFontIdToFit(time, ReturnNormalTextFont(), 0, sWindowTemplate_FlipPhone.width * 8);
        FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdFlipPhone, PIXEL_FILL(FLIP_PHONE_BG_COLOUR));
        AddTextPrinterParameterized4(sRotomPhone_SmallStartMenu->windowIdFlipPhone, fontId,
        GetStringCenterAlignXOffset(fontId, time, sWindowTemplate_FlipPhone.width * 8),
        ROTOM_SPEECH_BOTTOM_ROW_Y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_FLIP_PHONE], TEXT_SKIP_DRAW, time);
        CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdFlipPhone, COPYWIN_GFX);
        tRotomMessageSoundEffect = SE_BALL_TRAY_EXIT;
        tRotomUpdateTimer = RP_MESSAGE_UPDATE_TIMER;
        sRotomPhone_SmallStartMenu->isLoading = FALSE;
    }
    else
    {
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_ViewWallClock);
    }
}

static void RotomPhone_StartMenu_SelectedFunc_Daycare(void)
{
    #define MON_ONE 0
    #define MON_TWO 1

    #define MON_ICON_Y 220
    #define EGG_COMPATABILITY_ICON_Y 210
    #define MON_ICON_ONE_X 60
    #define MON_ICON_TWO_X 180
    #define MON_COMPATABILITY_ICON_X (MON_ICON_ONE_X + MON_ICON_TWO_X) / 2
    #define MON_ICON_PAL_SLOT_COMPATABILITY_ICON 0

    #define WIN_WIDTH 6
    #define WIN_HEIGHT 5
    #define WIN_TOP 21

    #define TEXT_LINE_SPACE 14
    
    u8 windowId;

    if (RotomPhone_StartMenu_IsFullScreen() && sRotomPhone_LargeStartMenu->panelIsOpen == FALSE)
    {
        if (GetDaycareState() == DAYCARE_NO_MONS)
            return;

        struct DaycareMon *daycareMonOne = &gSaveBlock1Ptr->daycare.mons[MON_ONE];
        struct DaycareMon *daycareMonTwo = &gSaveBlock1Ptr->daycare.mons[MON_TWO];
        struct BoxPokemon *daycareBoxMonOne = &daycareMonOne->mon;
        struct BoxPokemon *daycareBoxMonTwo = &daycareMonTwo->mon;
        u16 speciesOne = GetBoxMonData(daycareBoxMonOne, MON_DATA_SPECIES);
        u16 speciesTwo = GetBoxMonData(daycareBoxMonTwo, MON_DATA_SPECIES);
        u8 levelGain[2];
        u8 level[3];
        u8 nickname[POKEMON_NAME_LENGTH + 1];
        u8 textBuffer[24];
        u8 fontId;
        u8 animId;
        u8 y;

        LoadMonIconPalettes();

        if (GetDaycareState() != DAYCARE_NO_MONS)
        {
            u32 monOneLevel = GetLevelAfterDaycareSteps(daycareBoxMonOne, daycareMonOne->steps);
            u32 monOneLevelGain = GetNumLevelsGainedFromSteps(daycareMonOne);
            GetBoxMonData(daycareBoxMonOne, MON_DATA_NICKNAME, nickname);

            sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_ONE] =
                CreateMonIcon(speciesOne, SpriteCB_MonIcon_FlippedHorizontal, MON_ICON_ONE_X, MON_ICON_Y, 4, 0);
            gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_ONE]].oam.priority = 0;

            ConvertIntToDecimalStringN(level, monOneLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
            ConvertIntToDecimalStringN(levelGain, monOneLevelGain, STR_CONV_MODE_LEFT_ALIGN, 2);

            struct WindowTemplate winTemplate = CreateWindowTemplate(
                2,
                3,
                WIN_TOP,
                WIN_WIDTH,
                WIN_HEIGHT,
                15,
                ROTOM_FULL_SCREEN_NEXT_WIN_BASE_BLOCK
            );
            sRotomPhone_LargeStartMenu->panelWindowIds[RP_PANEL_WIN_ONE] = AddWindow(&winTemplate);
            windowId = sRotomPhone_LargeStartMenu->panelWindowIds[RP_PANEL_WIN_ONE];
            FillWindowPixelBuffer(windowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
            PutWindowTilemap(windowId);

            y = 0;
            StringCopy(textBuffer, nickname);
            fontId = GetFontIdToFit(textBuffer, ReturnNormalTextFont(), 0, winTemplate.width * 8);
            AddTextPrinterParameterized4(windowId, fontId,
                0,
                y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_WHITE], TEXT_SKIP_DRAW, textBuffer
            );

            y += TEXT_LINE_SPACE;
            StringCopy(textBuffer, COMPOUND_STRING("Level: "));
            StringAppend(textBuffer, level);
            fontId = GetFontIdToFit(textBuffer, ReturnNormalTextFont(), 0, winTemplate.width * 8);
            AddTextPrinterParameterized4(windowId, fontId,
                0,
                y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_WHITE], TEXT_SKIP_DRAW, textBuffer
            );

            y += TEXT_LINE_SPACE - 1;
            StringCopy(textBuffer, COMPOUND_STRING("Gain: +"));
            StringAppend(textBuffer, levelGain);
            fontId = GetFontIdToFit(textBuffer, ReturnNormalTextFont(), 0, winTemplate.width * 8);
            AddTextPrinterParameterized4(windowId, fontId,
                0,
                y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_WHITE], TEXT_SKIP_DRAW, textBuffer
            );

            CopyWindowToVram(windowId, COPYWIN_FULL);
        }

        if (GetDaycareState() != DAYCARE_NO_MONS && GetDaycareState() != DAYCARE_ONE_MON)
        {
            u32 monTwoLevel = GetLevelAfterDaycareSteps(daycareBoxMonTwo, daycareMonTwo->steps);
            u32 monTwoLevelGain = GetNumLevelsGainedFromSteps(daycareMonTwo);
            GetBoxMonData(daycareBoxMonTwo, MON_DATA_NICKNAME, nickname);

            sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_TWO] =
                CreateMonIcon(speciesTwo, SpriteCB_MonIcon, MON_ICON_TWO_X, MON_ICON_Y, 4, 0);
            gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_TWO]].oam.priority = 0;

            ConvertIntToDecimalStringN(level, monTwoLevel, STR_CONV_MODE_LEFT_ALIGN, 3);
            ConvertIntToDecimalStringN(levelGain, monTwoLevelGain, STR_CONV_MODE_LEFT_ALIGN, 2);

            struct WindowTemplate winTemplate = CreateWindowTemplate(
                2,
                21,
                WIN_TOP,
                WIN_WIDTH,
                WIN_HEIGHT,
                15,
                ROTOM_FULL_SCREEN_NEXT_WIN_BASE_BLOCK + (WIN_WIDTH * WIN_HEIGHT)
            );
            sRotomPhone_LargeStartMenu->panelWindowIds[RP_PANEL_WIN_TWO] = AddWindow(&winTemplate);
            windowId = sRotomPhone_LargeStartMenu->panelWindowIds[RP_PANEL_WIN_TWO];
            FillWindowPixelBuffer(windowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
            PutWindowTilemap(windowId);

            y = 0;
            StringCopy(textBuffer, nickname);
            fontId = GetFontIdToFit(textBuffer, ReturnNormalTextFont(), 0, winTemplate.width * 8);
            AddTextPrinterParameterized4(windowId, fontId,
                GetStringRightAlignXOffset(fontId, textBuffer, WIN_WIDTH * 8),
                y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_WHITE], TEXT_SKIP_DRAW, textBuffer
            );

            y += TEXT_LINE_SPACE;
            StringCopy(textBuffer, COMPOUND_STRING("Level: "));
            StringAppend(textBuffer, level);
            fontId = GetFontIdToFit(textBuffer, ReturnNormalTextFont(), 0, winTemplate.width * 8);
            AddTextPrinterParameterized4(windowId, fontId,
                GetStringRightAlignXOffset(fontId, textBuffer, WIN_WIDTH * 8),
                y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_WHITE], TEXT_SKIP_DRAW, textBuffer
            );

            y += TEXT_LINE_SPACE - 1;
            StringCopy(textBuffer, COMPOUND_STRING("Gain: +"));
            StringAppend(textBuffer, levelGain);
            fontId = GetFontIdToFit(textBuffer, ReturnNormalTextFont(), 0, winTemplate.width * 8);
            AddTextPrinterParameterized4(windowId, fontId,
                GetStringRightAlignXOffset(fontId, textBuffer, WIN_WIDTH * 8),
                y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_WHITE], TEXT_SKIP_DRAW, textBuffer
            );

            CopyWindowToVram(windowId, COPYWIN_FULL);
        }

        if (GetDaycareState() == DAYCARE_TWO_MONS)
        {
            struct SpritePalette iconCompatatbilityPal =
            {
                .data = sRotomPhone_DaycareCompatability_Pal,
                .tag = gMonIconPaletteTable[MON_ICON_PAL_SLOT_COMPATABILITY_ICON].tag,
            };

            struct SpriteTemplate iconCompatatbility_SpriteTemplate =
            {
                .tileTag = TAG_PHONE_ICON_GFX,
                .paletteTag = gMonIconPaletteTable[MON_ICON_PAL_SLOT_COMPATABILITY_ICON].tag,
                .oam = &sOam_IconDaycareCompatatbility,
                .callback = SpriteCallbackDummy,
                .anims = sAnims_IconDaycareCompatatbility,
                .affineAnims = gDummySpriteAffineAnimTable,
            };

            LoadCompressedSpriteSheet(&sSpriteSheet_CompatabilityIcon);
            LoadSpritePalette(&iconCompatatbilityPal);
            sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_THREE] =
                CreateSprite(&iconCompatatbility_SpriteTemplate, MON_COMPATABILITY_ICON_X, EGG_COMPATABILITY_ICON_Y, 0);
            gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_THREE]].oam.priority = 0;

            switch (GetDaycareCompatibilityScore(&gSaveBlock1Ptr->daycare))
            {
            case PARENTS_INCOMPATIBLE:
                animId = RP_DAYCARE_COMPATABILITY_ANIM_NON;
                break;
            
            case PARENTS_LOW_COMPATIBILITY:
                animId = RP_DAYCARE_COMPATABILITY_ANIM_LOW;
                break;
            
            case PARENTS_MED_COMPATIBILITY:
                animId = RP_DAYCARE_COMPATABILITY_ANIM_MED;
                break;
            
            case PARENTS_MAX_COMPATIBILITY:
            default:
                animId = RP_DAYCARE_COMPATABILITY_ANIM_MAX;
                break;
            }
            StartSpriteAnim(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_THREE]], animId);
        }

        if (GetDaycareState() == DAYCARE_EGG_WAITING)
        {
            void (*spriteCB)(struct Sprite *sprite);
            if (Random() % 2 == TRUE)
                spriteCB = SpriteCB_MonIcon;
            else
                spriteCB = SpriteCB_MonIcon_FlippedHorizontal;

            sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_THREE] =
                CreateMonIcon(SPECIES_EGG, spriteCB, MON_COMPATABILITY_ICON_X, EGG_COMPATABILITY_ICON_Y, 4, 0);
            gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_THREE]].oam.priority = 0;
        }
    }
    else if (RotomPhone_StartMenu_IsFullScreen() && sRotomPhone_LargeStartMenu->panelIsOpen == TRUE)
    {
        if (GetDaycareState() == DAYCARE_NO_MONS)
            return;

        FreeMonIconPalettes();

        if (GetDaycareState() != DAYCARE_NO_MONS)
        {
            DestroySpriteAndFreeResources(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_ONE]]);
            sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_ONE] = SPRITE_NONE;

            windowId = sRotomPhone_LargeStartMenu->panelWindowIds[RP_PANEL_WIN_ONE];
            FillWindowPixelBuffer(windowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
            CopyWindowToVram(windowId, COPYWIN_FULL);
            RemoveWindow(windowId);
            sRotomPhone_LargeStartMenu->panelWindowIds[RP_PANEL_WIN_ONE] = WINDOW_NONE;
        }

        if (GetDaycareState() != DAYCARE_NO_MONS && GetDaycareState() != DAYCARE_ONE_MON)
        {
            DestroySpriteAndFreeResources(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_TWO]]);
            sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_TWO] = SPRITE_NONE;

            windowId = sRotomPhone_LargeStartMenu->panelWindowIds[RP_PANEL_WIN_TWO];
            FillWindowPixelBuffer(windowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
            CopyWindowToVram(windowId, COPYWIN_FULL);
            RemoveWindow(windowId);
            sRotomPhone_LargeStartMenu->panelWindowIds[RP_PANEL_WIN_TWO] = WINDOW_NONE;
        }

        if (GetDaycareState() == DAYCARE_TWO_MONS || GetDaycareState() == DAYCARE_EGG_WAITING)
        {
            DestroySpriteAndFreeResources(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_THREE]]);
            sRotomPhone_LargeStartMenu->panelSpriteIds[RP_PANEL_SPRITE_THREE] = SPRITE_NONE;
        }
    }
    #undef MON_ONE
    #undef MON_TWO

    #undef MON_ICON_Y
    #undef EGG_COMPATABILITY_ICON_Y
    #undef MON_ICON_ONE_X
    #undef MON_ICON_TWO_X
    #undef MON_ICON_PAL_SLOT_COMPATABILITY_ICON
    
    #undef WIN_WIDTH
    #undef WIN_HEIGHT
    #undef WIN_TOP

    #undef TEXT_LINE_SPACE
}
#undef PHONE_OFFSCREEN_Y
#undef TAG_ROTOM_FACE_GFX
#undef TAG_PHONE_ICON_GFX
#undef TAG_ROTOM_FACE_ICON_PAL
#undef PHONE_BG_PAL_SLOT
#undef PHONE_BASE_COLOUR_INDEX

#undef PHONE_COMFY_SLIDE_DURATION
#undef FACE_ICON_COMFY_SPRING_MASS
#undef FACE_ICON_COMFY_SPRING_TENSION
#undef FACE_ICON_COMFY_SPRING_FRICTION
#undef FACE_ICON_COMFY_SPRING_CLAMP_AFTER

#undef FADE_COLOUR_MAX
#undef FADE_COLOUR_MID
#undef FADE_COLOUR_MIN

#undef RP_MENU_FIRST_OPTION
#undef RP_MENU_LAST_OPTION

#undef RP_FACE_LOOK_UP_ANIMS

#undef ROTOM_SPEECH_WINDOW_WIDTH
#undef ROTOM_SPEECH_WINDOW_WIDTH_PXL
#undef ROTOM_SPEECH_WINDOW_HEIGHT
#undef ROTOM_SPEECH_WINDOW_LEFT
#undef ROTOM_SPEECH_WINDOW_TOP
#undef PHONE_STARTING_BASE_BLOCK

#undef ROTOM_FULL_SCREEN_NEXT_WIN_BASE_BLOCK

#undef FLIP_PHONE_BG_COLOUR
#undef FLIP_PHONE_TEXT_FG_COLOUR
#undef FLIP_PHONE_TEXT_BG_COLOUR
#undef ROTOM_PHONE_BG_COLOUR
#undef ROTOM_PHONE_TEXT_FG_COLOUR
#undef ROTOM_PHONE_TEXT_BG_COLOUR

#undef ROTOM_SPEECH_TOP_ROW_Y
#undef ROTOM_SPEECH_BOTTOM_ROW_Y

#undef tRotomUpdateTimer
#undef tRotomUpdateMessage
#undef tRotomMessageSoundEffect
#undef tRotomPanelComfyAnimId
#undef tRotomPanelLastY
#undef tPhoneY
#undef tPhoneCloseToSave
#undef tPhoneHighlightComfyAnimId
#undef sFrameNumComfyAnimId
