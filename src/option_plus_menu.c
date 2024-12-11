#include "global.h"
#include "option_plus_menu.h"
#include "main.h"
#include "menu.h"
#include "scanline_effect.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "malloc.h"
#include "bg.h"
#include "gpu_regs.h"
#include "window.h"
#include "text.h"
#include "text_window.h"
#include "international_string_util.h"
#include "strings.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"
#include "menu_helpers.h"
#include "decompress.h"
#include "ikigai_scrolling_background.h"
#include "constants/battle.h"

enum
{
    MENU_MAIN,
    MENU_OVERWORLD,
    MENU_BATTLE,
    MENU_COUNT,
};

// Menu items
enum
{
    MENUITEM_MAIN_TEXTSPEED,
    MENUITEM_MAIN_FONT,
    MENUITEM_MAIN_BUTTONMODE,
    MENUITEM_MAIN_SOUND,
    MENUITEM_MAIN_UNIT_SYSTEM,
    MENUITEM_MAIN_CLOCK_MODE,
    MENUITEM_MAIN_FRAMETYPE,
    MENUITEM_MAIN_TITLE_SCREEN,
    MENUITEM_MAIN_CANCEL,
    MENUITEM_MAIN_COUNT,
};

enum
{
    MENUITEM_OVERWORLD_BIKE_MUSIC,
    MENUITEM_OVERWORLD_SURF_MUSIC,
    MENUITEM_OVERWORLD_NPC_MUG,
    MENUITEM_OVERWORLD_FOLLOWER_MUG,
    MENUITEM_OVERWORLD_MATCHCALL,
    MENUITEM_OVERWORLD_CANCEL,
    MENUITEM_OVERWORLD_COUNT,
};

enum
{
    MENUITEM_BATTLE_BATTLESTYLE,
    MENUITEM_BATTLE_WILD_SPEED,
    MENUITEM_BATTLE_TRAINER_SPEED,
    MENUITEM_BATTLE_BATTLESCENE,
    MENUITEM_BATTLE_DAMAGE_NUMBERS,
    MENUITEM_BATTLE_CANCEL,
    MENUITEM_BATTLE_COUNT,
};

// Window Ids
enum
{
    WIN_TOPBAR,
    WIN_OPTIONS,
    WIN_DESCRIPTION
};

static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    {//WIN_TOPBAR
        .bg = 1,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    {//WIN_OPTIONS
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 3,
        .width = 26,
        .height = 10,
        .paletteNum = 1,
        .baseBlock = 62
    },
    {//WIN_DESCRIPTION
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 1,
        .baseBlock = 500
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
    {
       .bg = 0,
       .charBaseIndex = 1,
       .mapBaseIndex = 30,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
    },
    {
       .bg = 1,
       .charBaseIndex = 1,
       .mapBaseIndex = 31,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 0,
    },
    {
       .bg = 2,
       .charBaseIndex = 0,
       .mapBaseIndex = 29,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 1,
    },
    {
       .bg = 3,
       .charBaseIndex = 3,
       .mapBaseIndex = 27,
       .screenSize = 0,
       .paletteMode = 0,
       .priority = 2,
    },
};

struct OptionMenu
{
    u8 submenu;
    u8 sel[MENUITEM_MAIN_COUNT];
    u8 sel_overworld[MENUITEM_OVERWORLD_COUNT];
    u8 sel_battle[MENUITEM_BATTLE_COUNT];
    int menuCursor[MENU_COUNT];
    int visibleCursor[MENU_COUNT];
    u8 arrowTaskId;
    u8 gfxLoadState;
};

#define Y_DIFF 16 // Difference in pixels between items.
#define OPTIONS_ON_SCREEN 5
#define NUM_OPTIONS_FROM_BORDER 1
#define OPTIONS_TEXT_OFFSET 16

// local functions
static void MainCB2(void);
static void VBlankCB(void);
static void DrawTopBarText(void); //top Option text
static void DrawLeftSideOptionText(int selection, int y);
static void DrawRightSideChoiceText(const u8 *str, int x, int y, bool8 choosen, bool8 active);
static void DrawOptionMenuTexts(void); //left side text;
static void DrawChoices(u32 id, int y); //right side draw function
static void HighlightOptionMenuItem(void);
static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void ScrollMenu(int direction);
static void ScrollAll(int direction); // to bottom or top
static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3);
static int XOptions_ProcessInput(int x, int selection);
static int ProcessInput_Options_Two(int selection);
static int ProcessInput_Options_Three(int selection);
static int ProcessInput_Options_Four(int selection);
static int ProcessInput_Options_Eleven(int selection);
static int ProcessInput_Sound(int selection);
static int ProcessInput_FrameType(int selection);
static int ProcessInput_Interface(int selection);
static const u8 *const OptionTextDescription(void);
static const u8 *const OptionTextRight(u8 menuItem);
static u8 MenuItemCount(void);
static u8 MenuItemCancel(void);
static void DrawDescriptionText(void);
static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style, bool8 active);
static void DrawChoices_Options_Four(const u8 *const *const strings, int selection, int y, bool8 active);
static void ReDrawAll(void);
static void DrawChoices_TextSpeed(int selection, int y);
static void DrawChoices_BattleScene(int selection, int y);
static void DrawChoices_BattleStyle(int selection, int y);
static void DrawChoices_Sound(int selection, int y);
static void DrawChoices_ButtonMode(int selection, int y);
static void DrawChoices_BarSpeed(int selection, int y); //HP and EXP
static void DrawChoices_BattleSpeed(int selection, int y);
static void DrawChoices_UnitSystem(int selection, int y);
static void DrawChoices_ClockMode(int selection, int y);
static void DrawChoices_Font(int selection, int y);
static void DrawChoices_FrameType(int selection, int y);
static void DrawChoices_Interface(int selection, int y);
static void DrawChoices_MatchCall(int selection, int y);
static void DrawChoices_BikeMusic(int selection, int y);
static void DrawChoices_SurfMusic(int selection, int y);
static void DrawChoices_MugshotsNPC(int selection, int y);
static void DrawChoices_MugshotsFollower(int selection, int y);
static void DrawChoices_TitleScreen(int selection, int y);
static void DrawChoices_DamageNumbers(int selection, int y);
static void DrawBgWindowFrames(void);

// EWRAM vars
EWRAM_DATA static struct OptionMenu *sOptions = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg3TilemapBuffer = NULL;

// const data
static const u8 sEqualSignGfx[] = INCBIN_U8("graphics/interface/option_menu_equals_sign.4bpp"); // note: this is only used in the Japanese release
static const u16 sOptionMenuBg_Pal[] = {RGB_IKIGAI_BLUE};
static const u16 sOptionMenuText_Pal[] = INCBIN_U16("graphics/interface/option_menu_text_custom.gbapal");

static const u32 sOptionsPlusTiles[] = INCBIN_U32("graphics/ui_options_plus/options_plus_tiles.4bpp.lz");
static const u32 sOptionsPlusTilemap[] = INCBIN_U32("graphics/ui_options_plus/options_plus_tiles.bin.lz");


#define TEXT_COLOR_OPTIONS_WHITE                1
#define TEXT_COLOR_OPTIONS_GRAY_FG              2
#define TEXT_COLOR_OPTIONS_GRAY_SHADOW          3
#define TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG        4
#define TEXT_COLOR_OPTIONS_ORANGE_FG            5
#define TEXT_COLOR_OPTIONS_ORANGE_SHADOW        6
#define TEXT_COLOR_OPTIONS_RED_FG               7
#define TEXT_COLOR_OPTIONS_RED_SHADOW           8
#define TEXT_COLOR_OPTIONS_GREEN_FG             9
#define TEXT_COLOR_OPTIONS_GREEN_SHADOW         10
#define TEXT_COLOR_OPTIONS_GREEN_DARK_FG        11
#define TEXT_COLOR_OPTIONS_GREEN_DARK_SHADOW    12
#define TEXT_COLOR_OPTIONS_RED_DARK_FG          13
#define TEXT_COLOR_OPTIONS_RED_DARK_SHADOW      14

// Menu draw and input functions
struct // MENU_MAIN
{
    void (*drawChoices)(int selection, int y);
    int (*processInput)(int selection);
} static const sItemFunctionsMain[MENUITEM_MAIN_COUNT] =
{
    [MENUITEM_MAIN_TEXTSPEED]       = {DrawChoices_TextSpeed,   ProcessInput_Options_Four},
    [MENUITEM_MAIN_FONT]            = {DrawChoices_Font,        ProcessInput_Options_Two},
    [MENUITEM_MAIN_SOUND]           = {DrawChoices_Sound,       ProcessInput_Sound},
    [MENUITEM_MAIN_BUTTONMODE]      = {DrawChoices_ButtonMode,  ProcessInput_Options_Three},
    [MENUITEM_MAIN_UNIT_SYSTEM]     = {DrawChoices_UnitSystem,  ProcessInput_Options_Two},
    [MENUITEM_MAIN_CLOCK_MODE]      = {DrawChoices_ClockMode,   ProcessInput_Options_Two},
    [MENUITEM_MAIN_FRAMETYPE]       = {DrawChoices_Interface,   ProcessInput_Interface},
    [MENUITEM_MAIN_TITLE_SCREEN]    = {DrawChoices_TitleScreen, ProcessInput_Options_Two},
    [MENUITEM_MAIN_CANCEL]          = {NULL, NULL},
};

struct // MENU_OVERWORLD
{
    void (*drawChoices)(int selection, int y);
    int (*processInput)(int selection);
} static const sItemFunctionsOverworld[MENUITEM_OVERWORLD_COUNT] =
{
    [MENUITEM_OVERWORLD_BIKE_MUSIC]     = {DrawChoices_BikeMusic,           ProcessInput_Options_Two},
    [MENUITEM_OVERWORLD_SURF_MUSIC]     = {DrawChoices_SurfMusic,           ProcessInput_Options_Two},
    [MENUITEM_OVERWORLD_NPC_MUG]        = {DrawChoices_MugshotsNPC,         ProcessInput_Options_Two},
    [MENUITEM_OVERWORLD_FOLLOWER_MUG]   = {DrawChoices_MugshotsFollower,    ProcessInput_Options_Three},
    [MENUITEM_OVERWORLD_MATCHCALL]      = {DrawChoices_MatchCall,           ProcessInput_Options_Two},
    [MENUITEM_OVERWORLD_CANCEL]         = {NULL, NULL},
};

struct // MENU_BATTLE
{
    void (*drawChoices)(int selection, int y);
    int (*processInput)(int selection);
} static const sItemFunctionsBattle[MENUITEM_BATTLE_COUNT] =
{ 
    [MENUITEM_BATTLE_BATTLESCENE]   = {DrawChoices_BattleScene, ProcessInput_Options_Four},
    [MENUITEM_BATTLE_BATTLESTYLE]   = {DrawChoices_BattleStyle, ProcessInput_Options_Two},
    [MENUITEM_BATTLE_WILD_SPEED]    = {DrawChoices_BattleSpeed, ProcessInput_Options_Four},
    [MENUITEM_BATTLE_TRAINER_SPEED] = {DrawChoices_BattleSpeed, ProcessInput_Options_Four},
    [MENUITEM_BATTLE_DAMAGE_NUMBERS] = {DrawChoices_DamageNumbers, ProcessInput_Options_Three},
    [MENUITEM_BATTLE_CANCEL]        = {NULL, NULL},
};

// Menu left side option names text
static const u8 sText_WildSpeed[]           = _("{FONT_GET_NARROW}WILD BATTLE SPEED{FONT_NORMAL}");
static const u8 sText_TrainerSpeed[]        = _("{FONT_GET_NARROW}SHOWDOWN SPEED{FONT_NORMAL}");
static const u8 sText_UnitSystem[]          = _("UNIT SYSTEM");
static const u8 sText_BikeMusic[]           = _("BIKE MUSIC");
static const u8 sText_SurfMusic[]           = _("SURF MUSIC");
static const u8 sText_MugshotNPC[]          = _("{FONT_GET_NARROW}NPC MUGSHOTS{FONT_NORMAL}");
static const u8 sText_MugshotFollower[]     = _("{FONT_GET_NARROW}FOLLOWER MUGSHOTS{FONT_NORMAL}");
static const u8 sText_TitleScreen[]         = _("TITLE SCREEN");
static const u8 *const sOptionMenuItemsNamesMain[MENUITEM_MAIN_COUNT] =
{
    [MENUITEM_MAIN_TEXTSPEED]       = gText_TextSpeed,
    [MENUITEM_MAIN_FONT]            = gText_Font,
    [MENUITEM_MAIN_SOUND]           = gText_Sound,
    [MENUITEM_MAIN_BUTTONMODE]      = gText_ButtonMode,
    [MENUITEM_MAIN_UNIT_SYSTEM]     = sText_UnitSystem,
    [MENUITEM_MAIN_CLOCK_MODE]      = COMPOUND_STRING("CLOCK MODE"),
    [MENUITEM_MAIN_FRAMETYPE]       = COMPOUND_STRING("INTERFACE"),
    [MENUITEM_MAIN_TITLE_SCREEN]    = sText_TitleScreen,
    [MENUITEM_MAIN_CANCEL]          = gText_OptionMenuSave,
};

static const u8 *const sOptionMenuItemsNamesOverworld[MENUITEM_OVERWORLD_COUNT] =
{
    [MENUITEM_OVERWORLD_BIKE_MUSIC]     = sText_BikeMusic,
    [MENUITEM_OVERWORLD_SURF_MUSIC]     = sText_SurfMusic,
    [MENUITEM_OVERWORLD_NPC_MUG]        = sText_MugshotNPC,
    [MENUITEM_OVERWORLD_FOLLOWER_MUG]   = sText_MugshotFollower,
    [MENUITEM_OVERWORLD_MATCHCALL]      = COMPOUND_STRING("{FONT_GET_NARROW}OVERWORLD CALLS"),
    [MENUITEM_OVERWORLD_CANCEL]         = gText_OptionMenuSave,
};

static const u8 *const sOptionMenuItemsNamesBattle[MENUITEM_BATTLE_COUNT] =
{
    [MENUITEM_BATTLE_BATTLESCENE]   = gText_BattleScene,
    [MENUITEM_BATTLE_BATTLESTYLE]   = gText_BattleStyle,
    [MENUITEM_BATTLE_WILD_SPEED]    = sText_WildSpeed,
    [MENUITEM_BATTLE_TRAINER_SPEED] = sText_TrainerSpeed,
    [MENUITEM_BATTLE_DAMAGE_NUMBERS] = COMPOUND_STRING("DAMAGE NUMBERS"),
    [MENUITEM_BATTLE_CANCEL]        = gText_OptionMenuSave,
};

static const u8 *const OptionTextRight(u8 menuItem)
{
    switch (sOptions->submenu)
    {
    case MENU_MAIN:         return sOptionMenuItemsNamesMain[menuItem];
    case MENU_OVERWORLD:    return sOptionMenuItemsNamesOverworld[menuItem];
    case MENU_BATTLE:       return sOptionMenuItemsNamesBattle[menuItem];
    }
    return sOptionMenuItemsNamesMain[menuItem];
}

// Menu left side text conditions
static bool8 CheckConditions(int selection)
{
    switch (sOptions->submenu)
    {
    case MENU_MAIN:
        switch(selection)
        {
        case MENUITEM_MAIN_TEXTSPEED:       return TRUE;
        case MENUITEM_MAIN_FONT:            return TRUE;
        case MENUITEM_MAIN_SOUND:           return TRUE;
        case MENUITEM_MAIN_BUTTONMODE:      return TRUE;
        case MENUITEM_MAIN_UNIT_SYSTEM:     return TRUE;
        case MENUITEM_MAIN_CLOCK_MODE:      return TRUE;
        case MENUITEM_MAIN_FRAMETYPE:       return TRUE;
        case MENUITEM_MAIN_TITLE_SCREEN:    return TRUE;
        case MENUITEM_MAIN_CANCEL:          return TRUE;
        case MENUITEM_MAIN_COUNT:           return TRUE;
        }
    case MENU_OVERWORLD:
        switch (selection)
        {
        case MENUITEM_OVERWORLD_BIKE_MUSIC:     return TRUE;
        case MENUITEM_OVERWORLD_SURF_MUSIC:     return TRUE;
        case MENUITEM_OVERWORLD_NPC_MUG:        return TRUE;
        case MENUITEM_OVERWORLD_FOLLOWER_MUG:   return TRUE;
        case MENUITEM_OVERWORLD_MATCHCALL:      return TRUE;
        case MENUITEM_OVERWORLD_CANCEL:         return TRUE;
        }
    case MENU_BATTLE:
        switch(selection)
        {
        case MENUITEM_BATTLE_BATTLESCENE:   return TRUE;
        case MENUITEM_BATTLE_BATTLESTYLE:   return TRUE;
        case MENUITEM_BATTLE_WILD_SPEED:    return TRUE;
        case MENUITEM_BATTLE_TRAINER_SPEED: return TRUE;
        case MENUITEM_BATTLE_DAMAGE_NUMBERS: return TRUE;
        case MENUITEM_BATTLE_CANCEL:        return TRUE;
        case MENUITEM_BATTLE_COUNT:         return TRUE;
        }
    }
    return FALSE; // Options Menu Changes
}

// Descriptions
static const u8 sText_Empty[]                   = _("");
static const u8 sText_Desc_Save[]               = _("Save your settings.");
static const u8 sText_Desc_TextSpeed[]          = _("Choose one of the four text-display\nspeeds.");
static const u8 sText_Desc_BattleScene_Full[]   = _("Show all POKéMON battle animations.");
static const u8 sText_Desc_BattleScene_Low[]    = _("Remove POKéMON KO celebrations and\nreduce held item animations.");
static const u8 sText_Desc_BattleScene_Off[]    = _("Skip the POKéMON battle animations.");
static const u8 sText_Desc_BattleScene_NoIntro[] = _("Skip the POKéMON battle animations\nand streamline the introduction.");
static const u8 sText_Desc_BattleStyle_Shift[]  = _("Get the option to switch your\nPOKéMON after an enemy KO.");
static const u8 sText_Desc_BattleStyle_Set[]    = _("No free switch after an enemy\nPOKéMON KO.");
static const u8 sText_Desc_SoundMono[]          = _("Sound is the same in all speakers.\nRecommended for original hardware.");
static const u8 sText_Desc_SoundStereo[]        = _("Play the left and right audio channel\nseperatly. Great with headphones.");
static const u8 sText_Desc_ButtonMode[]         = _("All buttons work as normal.");
static const u8 sText_Desc_ButtonMode_LR[]      = _("On some screens the L and R buttons\nact as left and right.");
static const u8 sText_Desc_ButtonMode_LA[]      = _("The L button acts as another A\nbutton for one-handed play.");
static const u8 sText_Desc_UnitSystemImperial[] = _("Display weights and sizes in\npounds and inches.");
static const u8 sText_Desc_UnitSystemMetric[]   = _("Display weights and sizes in\nkilograms and meters.");
static const u8 sText_Desc_FrameType[]          = _("Choose the interface styling.");

// Custom Descriptions
static const u8 sText_Desc_WildSpeed[]                  = _("Choose the speed of wild battles.\nImportant battles are not included.");
static const u8 sText_Desc_TrainerSpeed[]               = _("Choose the speed of trainer battles.\nImportant battles are not included.");
static const u8 sText_Desc_SurfOff[]                    = _("Disables the SURF theme\nwhen using SURF.");
static const u8 sText_Desc_SurfOn[]                     = _("Enables the SURF theme\nwhen using SURF.");
static const u8 sText_Desc_BikeOff[]                    = _("Disables the BIKE theme when\nusing the BIKE.");
static const u8 sText_Desc_BikeOn[]                     = _("Enables the BIKE theme when\nusing the BIKE.");
static const u8 sText_Desc_FontTypeCompact[]            = _("Printed text uses a font\nwhich is more compact.");
static const u8 sText_Desc_FontTypeSpread[]             = _("Printed text uses a font\nwhich is more spread.");
static const u8 sText_Desc_OverworldCallsOn[]           = _("TRAINERs will be able to call you,\noffering rematches and info.");
static const u8 sText_Desc_OverworldCallsOff[]          = _("You will not receive calls.\nSpecial events will still occur.");
static const u8 sText_Desc_MugshotNPCOn[]               = _("Show NPC mugshots during dialogue.\nExcludes following POKéMON.");
static const u8 sText_Desc_MugshotNPCOff[]              = _("Hide NPC mugshots during dialogue.\nExcludes following POKéMON.");
static const u8 sText_Desc_MugshotFollowerPlaceholder[] = _("Show a placeholder mugshot when\nfollowing POKéMON do not have one.");
static const u8 sText_Desc_MugshotFollowerOn[]          = _("Show mugshot of following POKéMON if\nthey are available.");
static const u8 sText_Desc_MugshotFollowerOff[]         = _("Hide following POKéMON mugshots.");
static const u8 sText_Desc_TitleScreenMatch[]           = _("Title screen legendary matches choice\nof interface, if available.");
static const u8 sText_Desc_TitleScreenRandom[]          = _("Title screen legendary is randomised.");
static const u8 sText_Desc_DamageNumbers[]              = _("Whether damage numbers are shown in\nbattle and when they appear.");
static const u8 sText_Desc_ClockMode[]                  = _("Choose which mode of clock is used.");

// Disabled Descriptions
static const u8 sText_Desc_Disabled_Textspeed[]     = _("Only active if xyz.");

// Disabled Battle Descriptions
static const u8 sText_Desc_Disabled_BattleHPBar[]   = _("Only active if xyz.");

static const u8 *const sOptionMenuItemDescriptionsMain[MENUITEM_MAIN_COUNT][3] =
{
    [MENUITEM_MAIN_TEXTSPEED]       = {sText_Desc_TextSpeed,            sText_Empty,                    sText_Empty},
    [MENUITEM_MAIN_FONT]            = {sText_Desc_FontTypeCompact,      sText_Desc_FontTypeSpread,      sText_Empty},
    [MENUITEM_MAIN_SOUND]           = {sText_Desc_SoundMono,            sText_Desc_SoundStereo,         sText_Empty},
    [MENUITEM_MAIN_BUTTONMODE]      = {sText_Desc_ButtonMode,           sText_Desc_ButtonMode_LR,       sText_Desc_ButtonMode_LA},
    [MENUITEM_MAIN_UNIT_SYSTEM]     = {sText_Desc_UnitSystemImperial,   sText_Desc_UnitSystemMetric,    sText_Empty},
    [MENUITEM_MAIN_CLOCK_MODE]      = {sText_Desc_ClockMode,            sText_Empty,                    sText_Empty},
    [MENUITEM_MAIN_FRAMETYPE]       = {sText_Desc_FrameType,            sText_Empty,                    sText_Empty},
    [MENUITEM_MAIN_TITLE_SCREEN]    = {sText_Desc_TitleScreenMatch,     sText_Desc_TitleScreenRandom,   sText_Empty},
    [MENUITEM_MAIN_CANCEL]          = {sText_Desc_Save,                 sText_Empty,                    sText_Empty},
};

static const u8 *const sOptionMenuItemDescriptionsOverworld[MENUITEM_OVERWORLD_COUNT][3] =
{
    [MENUITEM_OVERWORLD_BIKE_MUSIC]     = {sText_Desc_BikeOn,                       sText_Desc_BikeOff,             sText_Empty},
    [MENUITEM_OVERWORLD_SURF_MUSIC]     = {sText_Desc_SurfOn,                       sText_Desc_SurfOff,             sText_Empty},
    [MENUITEM_OVERWORLD_NPC_MUG]        = {sText_Desc_MugshotNPCOn,                 sText_Desc_MugshotNPCOff,       sText_Empty},
    [MENUITEM_OVERWORLD_FOLLOWER_MUG]   = {sText_Desc_MugshotFollowerPlaceholder,   sText_Desc_MugshotFollowerOn,   sText_Desc_MugshotFollowerOff},
    [MENUITEM_OVERWORLD_MATCHCALL]      = {sText_Desc_OverworldCallsOn,             sText_Desc_OverworldCallsOff,   sText_Empty},
    [MENUITEM_OVERWORLD_CANCEL]         = {sText_Desc_Save,                         sText_Empty,                    sText_Empty},
};

static const u8 *const sOptionMenuItemDescriptionsBattle[MENUITEM_BATTLE_COUNT][4] =
{
    [MENUITEM_BATTLE_BATTLESCENE]   = {sText_Desc_BattleScene_Full,     sText_Desc_BattleScene_Low,     sText_Desc_BattleScene_Off,     sText_Desc_BattleScene_NoIntro},
    [MENUITEM_BATTLE_BATTLESTYLE]   = {sText_Desc_BattleStyle_Shift,    sText_Desc_BattleStyle_Set,     sText_Empty,                    sText_Empty},
    [MENUITEM_BATTLE_WILD_SPEED]    = {sText_Desc_WildSpeed,            sText_Empty,                    sText_Empty,                    sText_Empty},
    [MENUITEM_BATTLE_TRAINER_SPEED] = {sText_Desc_TrainerSpeed,         sText_Empty,                    sText_Empty,                    sText_Empty},
    [MENUITEM_BATTLE_DAMAGE_NUMBERS] = {sText_Desc_DamageNumbers,        sText_Empty,                    sText_Empty,                    sText_Empty},
    [MENUITEM_BATTLE_CANCEL]        = {sText_Desc_Save,                 sText_Empty,                    sText_Empty,                    sText_Empty},
};

static const u8 *const sOptionMenuItemDescriptionsDisabledMain[MENUITEM_MAIN_COUNT] =
{
    [MENUITEM_MAIN_TEXTSPEED]       = sText_Desc_Disabled_Textspeed,
    [MENUITEM_MAIN_FONT]            = sText_Empty,
    [MENUITEM_MAIN_SOUND]           = sText_Empty,
    [MENUITEM_MAIN_BUTTONMODE]      = sText_Empty,
    [MENUITEM_MAIN_UNIT_SYSTEM]     = sText_Empty,
    [MENUITEM_MAIN_CLOCK_MODE]      = sText_Empty,
    [MENUITEM_MAIN_FRAMETYPE]       = sText_Empty,
    [MENUITEM_MAIN_TITLE_SCREEN]    = sText_Empty,
    [MENUITEM_MAIN_CANCEL]          = sText_Empty,
};

// Disabled Overworld
static const u8 *const sOptionMenuItemDescriptionsDisabledOverworld[MENUITEM_OVERWORLD_COUNT] =
{
    [MENUITEM_OVERWORLD_BIKE_MUSIC]     = sText_Empty,
    [MENUITEM_OVERWORLD_SURF_MUSIC]     = sText_Empty,
    [MENUITEM_OVERWORLD_NPC_MUG]        = sText_Empty,
    [MENUITEM_OVERWORLD_FOLLOWER_MUG]   = sText_Empty,
    [MENUITEM_OVERWORLD_MATCHCALL]      = sText_Empty,
    [MENUITEM_OVERWORLD_CANCEL]         = sText_Empty,
};

static const u8 *const sOptionMenuItemDescriptionsDisabledBattle[MENUITEM_BATTLE_COUNT] =
{
    [MENUITEM_BATTLE_BATTLESCENE]   = sText_Empty,
    [MENUITEM_BATTLE_BATTLESTYLE]   = sText_Empty,
    [MENUITEM_BATTLE_WILD_SPEED]    = sText_Empty,
    [MENUITEM_BATTLE_TRAINER_SPEED] = sText_Empty,
    [MENUITEM_BATTLE_DAMAGE_NUMBERS] = sText_Empty,
    [MENUITEM_BATTLE_CANCEL]        = sText_Empty,
};

static const u8 *const OptionTextDescription(void)
{
    u8 menuItem = sOptions->menuCursor[sOptions->submenu];
    u8 selection;

    if (sOptions->submenu == MENU_MAIN && menuItem >= MENUITEM_MAIN_COUNT)
    {
        menuItem = MENUITEM_MAIN_CANCEL;
    }
    else if (sOptions->submenu == MENU_OVERWORLD && menuItem >= MENUITEM_OVERWORLD_COUNT)
    {
        menuItem = MENUITEM_OVERWORLD_CANCEL;
    }
    else if (sOptions->submenu == MENU_BATTLE && menuItem >= MENUITEM_BATTLE_COUNT)
    {
        menuItem = MENUITEM_BATTLE_CANCEL;
    }

    switch (sOptions->submenu)
    {
    default: // Options Menu Changes
    case MENU_MAIN:
        if (!CheckConditions(menuItem))
            return sOptionMenuItemDescriptionsDisabledMain[menuItem];
        selection = sOptions->sel[menuItem];
        if (menuItem == MENUITEM_MAIN_TEXTSPEED || menuItem == MENUITEM_MAIN_FRAMETYPE || menuItem == MENUITEM_MAIN_CLOCK_MODE)
            selection = 0;
        return sOptionMenuItemDescriptionsMain[menuItem][selection];
    case MENU_OVERWORLD:
        if (!CheckConditions(menuItem))
            return sOptionMenuItemDescriptionsDisabledOverworld[menuItem];
        selection = sOptions->sel_overworld[menuItem];
        return sOptionMenuItemDescriptionsOverworld[menuItem][selection];
    case MENU_BATTLE:
        if (!CheckConditions(menuItem))
            return sOptionMenuItemDescriptionsDisabledBattle[menuItem];
        selection = sOptions->sel_battle[menuItem];
        if (menuItem == MENUITEM_BATTLE_WILD_SPEED || menuItem == MENUITEM_BATTLE_TRAINER_SPEED || menuItem == MENUITEM_BATTLE_DAMAGE_NUMBERS)
            selection = 0;
        return sOptionMenuItemDescriptionsBattle[menuItem][selection];
    }
}

static u8 MenuItemCount(void)
{
    switch (sOptions->submenu)
    {
    case MENU_MAIN:      return MENUITEM_MAIN_COUNT;
    case MENU_OVERWORLD: return MENUITEM_OVERWORLD_COUNT;
    case MENU_BATTLE:    return MENUITEM_BATTLE_COUNT;
    }
    return MENUITEM_MAIN_COUNT; // Options Menu Changes
}

static u8 MenuItemCancel(void)
{
    switch (sOptions->submenu)
    {
    case MENU_MAIN:      return MENUITEM_MAIN_CANCEL;
    case MENU_OVERWORLD: return MENUITEM_OVERWORLD_CANCEL;
    case MENU_BATTLE:    return MENUITEM_BATTLE_CANCEL;
    }
    return MENUITEM_BATTLE_CANCEL; // Options Menu Changes
}

// Main code
static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    StartIkigaiScrollingBackground(3);
}

static const u8 sText_TopBar_Main[]             = _("GENERAL");
static const u8 sText_TopBar_Main_Right[]       = _("{R_BUTTON}OVERWORLD");
static const u8 sText_TopBar_Overworld[]        = _("OVERWORLD");
static const u8 sText_TopBar_Overworld_Left[]   = _("{L_BUTTON}GENERAL");
static const u8 sText_TopBar_Overworld_Right[]  = _("{R_BUTTON}BATTLE");
static const u8 sText_TopBar_Battle[]           = _("BATTLE");
static const u8 sText_TopBar_Battle_Left[]      = _("{L_BUTTON}OVERWORLD");
static void DrawTopBarText(void)
{
    const u8 color[3] = { 0, TEXT_COLOR_WHITE, TEXT_COLOR_OPTIONS_GRAY_FG };

    FillWindowPixelBuffer(WIN_TOPBAR, PIXEL_FILL(0));
    switch (sOptions->submenu)
    {
        case MENU_MAIN:
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 105,   1, color, 0, sText_TopBar_Main);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 173,   1, color, 0, sText_TopBar_Main_Right);
            break;
        case MENU_OVERWORLD:
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 5,     1, color, 0, sText_TopBar_Overworld_Left);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 98,    1, color, 0, sText_TopBar_Overworld);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 190,   1, color, 0, sText_TopBar_Overworld_Right);
            break;
        case MENU_BATTLE:
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 5,     1, color, 0, sText_TopBar_Battle_Left);
            AddTextPrinterParameterized3(WIN_TOPBAR, FONT_SMALL, 106,   1, color, 0, sText_TopBar_Battle);
            break;
    }
    PutWindowTilemap(WIN_TOPBAR);
    CopyWindowToVram(WIN_TOPBAR, COPYWIN_FULL);
}

static void DrawOptionMenuTexts(void) //left side text
{
    u8 i;

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(0));
    for (i = 0; i < MenuItemCount(); i++)
        DrawLeftSideOptionText(i, (i * Y_DIFF) + 1);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void DrawDescriptionText(void)
{
    u8 color_gray[3];
    color_gray[0] = TEXT_COLOR_TRANSPARENT;
    color_gray[1] = TEXT_COLOR_OPTIONS_GRAY_FG;
    color_gray[2] = TEXT_COLOR_OPTIONS_GRAY_SHADOW;
        
    FillWindowPixelBuffer(WIN_DESCRIPTION, PIXEL_FILL(1));
    AddTextPrinterParameterized4(WIN_DESCRIPTION, FONT_NORMAL, 8, 1, 0, 0, color_gray, TEXT_SKIP_DRAW, OptionTextDescription());
    CopyWindowToVram(WIN_DESCRIPTION, COPYWIN_FULL);
}

static void DrawLeftSideOptionText(int selection, int y)
{
    u8 color_yellow[3];
    u8 color_gray[3];
    const u16 *selectedColorPal = ReturnMenuUIPalette();
    u16 selectedTextColor = selectedColorPal[1];

    color_yellow[0] = TEXT_COLOR_TRANSPARENT;
    color_yellow[1] = TEXT_COLOR_WHITE;
    color_yellow[2] = TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG;
    color_gray[0] = TEXT_COLOR_TRANSPARENT;
    color_gray[1] = TEXT_COLOR_OPTIONS_ORANGE_SHADOW;
    color_gray[2] = TEXT_COLOR_OPTIONS_GRAY_FG;

    if (CheckConditions(selection))
        AddTextPrinterParameterized4(WIN_OPTIONS, FONT_NORMAL, 8, y, 0, 0, color_yellow, TEXT_SKIP_DRAW, OptionTextRight(selection));
    else
        AddTextPrinterParameterized4(WIN_OPTIONS, FONT_NORMAL, 8, y, 0, 0, color_gray, TEXT_SKIP_DRAW, OptionTextRight(selection));
        LoadPalette(&selectedTextColor, OPTIONS_TEXT_OFFSET + color_gray[1], sizeof(selectedTextColor));
}

static void DrawRightSideChoiceText(const u8 *text, int x, int y, bool8 choosen, bool8 active)
{
    u8 color_red[3];
    u8 color_gray[3];
    const u16 *selectedColorPal = ReturnMenuUIPalette();
    u16 selectedTextColor = selectedColorPal[1];
    u16 selectedShadowColor = selectedColorPal[2];

    if (active)
    {
        color_red[0]    = TEXT_COLOR_TRANSPARENT;
        color_red[1]    = TEXT_COLOR_OPTIONS_WHITE;
        color_red[2]    = TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG;
        color_gray[0]   = TEXT_COLOR_TRANSPARENT;
        color_gray[1]   = TEXT_COLOR_OPTIONS_ORANGE_FG;
        color_gray[2]   = TEXT_COLOR_OPTIONS_GRAY_FG;
    }
    else
    {
        color_red[0]    = TEXT_COLOR_TRANSPARENT;
        color_red[1]    = TEXT_COLOR_OPTIONS_ORANGE_FG;
        color_red[2]    = TEXT_COLOR_OPTIONS_GRAY_FG;
        color_gray[0]   = TEXT_COLOR_TRANSPARENT;
        color_gray[1]   = TEXT_COLOR_OPTIONS_WHITE;
        color_gray[2]   = TEXT_COLOR_OPTIONS_GRAY_LIGHT_FG;
    }


    if (choosen)
    {
        AddTextPrinterParameterized4(WIN_OPTIONS, FONT_NORMAL, x, y, 0, 0, color_gray, TEXT_SKIP_DRAW, text);
        if (active)
        {
            LoadPalette(&selectedTextColor, OPTIONS_TEXT_OFFSET + color_gray[1], sizeof(selectedTextColor));
            LoadPalette(&selectedShadowColor, OPTIONS_TEXT_OFFSET + color_gray[2], sizeof(selectedShadowColor));
        }
    }
    else
    {
        AddTextPrinterParameterized4(WIN_OPTIONS, FONT_NORMAL, x, y, 0, 0, color_red, TEXT_SKIP_DRAW, text);
        if (!active)
        {
            LoadPalette(&selectedTextColor, OPTIONS_TEXT_OFFSET + color_red[1], sizeof(selectedTextColor));
            LoadPalette(&selectedShadowColor, OPTIONS_TEXT_OFFSET + color_red[2], sizeof(selectedShadowColor));
        }
    }
}

static void DrawChoices(u32 id, int y) //right side draw function
{
    switch (sOptions->submenu)
    {
        case MENU_MAIN:
            if (sItemFunctionsMain[id].drawChoices != NULL)
                sItemFunctionsMain[id].drawChoices(sOptions->sel[id], y);
            break;
        case MENU_OVERWORLD:
            if (sItemFunctionsOverworld[id].drawChoices != NULL)
                sItemFunctionsOverworld[id].drawChoices(sOptions->sel_overworld[id], y);
            break;
        case MENU_BATTLE:
            if (sItemFunctionsBattle[id].drawChoices != NULL)
                sItemFunctionsBattle[id].drawChoices(sOptions->sel_battle[id], y);
            break;
    }
}

static void HighlightOptionMenuItem(void)
{
    int cursor = sOptions->visibleCursor[sOptions->submenu];

    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(8, 232));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(cursor * Y_DIFF + 24, cursor * Y_DIFF + 40));
}
static const u16 IkigaiScrollingBgPal_Default[] = INCBIN_U16("graphics/ikigai_scrolling_background/scroll_tiles.gbapal");
static bool8 OptionsMenu_LoadGraphics(void) // Load all the tilesets, tilemaps, spritesheets, and palettes
{
    switch (sOptions->gfxLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(2, sOptionsPlusTiles, 0, 0, 0);
        sOptions->gfxLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sOptionsPlusTilemap, sBg2TilemapBuffer);
            sOptions->gfxLoadState++;
        }
        break;
    case 2:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(3, IkigaiScrollingBgTiles, 0, 0, 0);
        sOptions->gfxLoadState++;
        break;
    case 3:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(IkigaiScrollingBgTilemap_PalTwo, sBg3TilemapBuffer);
            sOptions->gfxLoadState++;
        }
        break;
    case 4:
        LoadPalette(ReturnMenuUIPalette(), 64, 32);
        LoadPalette(ReturnScrollingBackgroundPalette(), 32, 32);
        sOptions->gfxLoadState++;
        break;
    default:
        sOptions->gfxLoadState = 0;
        return TRUE;
    }
    return FALSE;
}

void CB2_InitOptionPlusMenu(void)
{
    u32 i, taskId;
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        ResetVramOamAndBgCntRegs();
        sOptions = AllocZeroed(sizeof(*sOptions));
        FreeAllSpritePalettes();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        ResetBgsAndClearDma3BusyFlags(0);
        ResetBgPositions();
        
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_DARKEN | BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG2);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        
        ResetAllBgsCoordinates();
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, NELEMS(sOptionMenuBgTemplates));
        InitWindows(sOptionMenuWinTemplates);

        sBg2TilemapBuffer = Alloc(0x800);
        memset(sBg2TilemapBuffer, 0, 0x800);
        SetBgTilemapBuffer(2, sBg2TilemapBuffer);
        ScheduleBgCopyTilemapToVram(2);

        sBg3TilemapBuffer = Alloc(0x800);
        memset(sBg3TilemapBuffer, 0, 0x800);
        SetBgTilemapBuffer(3, sBg3TilemapBuffer);
        ScheduleBgCopyTilemapToVram(3);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        gMain.state++;
        sOptions->gfxLoadState = 0;
        break;
    case 3:
        if (OptionsMenu_LoadGraphics() == TRUE)
        {
            gMain.state++;
            LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsInterfaceColor)->tiles, 0x120, 0x1A2);
        }
        break;
    case 4:
        LoadPalette(sOptionMenuBg_Pal, 0, sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsInterfaceColor)->pal, 0x70, 0x20);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sOptionMenuText_Pal, OPTIONS_TEXT_OFFSET, sizeof(sOptionMenuText_Pal));
        gMain.state++;
        break;
    case 6:
        sOptions->sel[MENUITEM_MAIN_TEXTSPEED]                  = gSaveBlock2Ptr->optionsTextSpeed;
        sOptions->sel[MENUITEM_MAIN_FONT]                       = gSaveBlock2Ptr->optionsCurrentFont;
        sOptions->sel[MENUITEM_MAIN_SOUND]                      = gSaveBlock2Ptr->optionsSound;
        sOptions->sel[MENUITEM_MAIN_BUTTONMODE]                 = gSaveBlock2Ptr->optionsButtonMode;
        sOptions->sel[MENUITEM_MAIN_UNIT_SYSTEM]                = gSaveBlock2Ptr->optionsUnitSystem;
        sOptions->sel[MENUITEM_MAIN_CLOCK_MODE]                 = gSaveBlock2Ptr->optionsClockMode;
        sOptions->sel[MENUITEM_MAIN_FRAMETYPE]                  = gSaveBlock2Ptr->optionsInterfaceColor;
        sOptions->sel[MENUITEM_MAIN_TITLE_SCREEN]               = gSaveBlock2Ptr->optionsTitleScreenRandomise;
        
        sOptions->sel_overworld[MENUITEM_OVERWORLD_BIKE_MUSIC]      = gSaveBlock2Ptr->optionsBikeMusic;
        sOptions->sel_overworld[MENUITEM_OVERWORLD_SURF_MUSIC]      = gSaveBlock2Ptr->optionsSurfMusic;
        sOptions->sel_overworld[MENUITEM_OVERWORLD_NPC_MUG]         = gSaveBlock2Ptr->optionsSuppressNPCMugshots;
        sOptions->sel_overworld[MENUITEM_OVERWORLD_FOLLOWER_MUG]    = gSaveBlock2Ptr->optionsFollowerMugshots;
        sOptions->sel_overworld[MENUITEM_OVERWORLD_MATCHCALL]       = gSaveBlock2Ptr->optionsDisableMatchCall;

        sOptions->sel_battle[MENUITEM_BATTLE_BATTLESCENE]       = gSaveBlock2Ptr->optionsBattleScene;
        sOptions->sel_battle[MENUITEM_BATTLE_BATTLESTYLE]       = gSaveBlock2Ptr->optionsBattleStyle;
        sOptions->sel_battle[MENUITEM_BATTLE_WILD_SPEED]        = gSaveBlock2Ptr->optionsWildBattleSpeed;
        sOptions->sel_battle[MENUITEM_BATTLE_TRAINER_SPEED]     = gSaveBlock2Ptr->optionsTrainerBattleSpeed;
        sOptions->sel_battle[MENUITEM_BATTLE_DAMAGE_NUMBERS]    = gSaveBlock2Ptr->optionsDamageNumbers;

        sOptions->submenu = MENU_MAIN;

        gMain.state++;
        break;
    case 7:
        PutWindowTilemap(WIN_TOPBAR);
        DrawTopBarText();
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_DESCRIPTION);
        DrawDescriptionText();
        gMain.state++;
        break;
    case 9:
        PutWindowTilemap(WIN_OPTIONS);
        DrawOptionMenuTexts();
        gMain.state++;
        break;
    case 10:
        taskId = CreateTask(Task_OptionMenuFadeIn, 0);
        
        sOptions->arrowTaskId = AddScrollIndicatorArrowPairParameterized(SCROLL_ARROW_UP, 240 / 2, 20, 110, MENUITEM_MAIN_COUNT - 1, 110, 110, 0);

        for (i = 0; i < min(OPTIONS_ON_SCREEN, MenuItemCount()); i++)
            DrawChoices(i, i * Y_DIFF);

        HighlightOptionMenuItem();

        CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
        gMain.state++;
        break;
    case 11:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 12:
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_OptionMenuProcessInput;
        SetGpuReg(REG_OFFSET_WIN0H, 0); // Idk man Im just trying to stop this stupid graphical bug from happening dont judge me
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG_ALL | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_DARKEN | BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG2);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        HighlightOptionMenuItem();
        return;
    }
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
    int i = 0;
    u8 optionsToDraw = min(OPTIONS_ON_SCREEN , MenuItemCount());
    if (JOY_NEW(A_BUTTON))
    {
        if (sOptions->menuCursor[sOptions->submenu] == MenuItemCancel())
            gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sOptions->visibleCursor[sOptions->submenu] == NUM_OPTIONS_FROM_BORDER) // don't advance visible cursor until scrolled to the bottom
        {
            if (--sOptions->menuCursor[sOptions->submenu] == 0)
                sOptions->visibleCursor[sOptions->submenu]--;
            else
                ScrollMenu(1);
        }
        else
        {
            if (--sOptions->menuCursor[sOptions->submenu] < 0) // Scroll all the way to the bottom.
            {
                sOptions->visibleCursor[sOptions->submenu] = sOptions->menuCursor[sOptions->submenu] = optionsToDraw-2;
                ScrollAll(0);
                sOptions->visibleCursor[sOptions->submenu] = optionsToDraw-1;
                sOptions->menuCursor[sOptions->submenu] = MenuItemCount() - 1;
            }
            else
            {
                sOptions->visibleCursor[sOptions->submenu]--;
            }
        }
        HighlightOptionMenuItem();
        DrawDescriptionText();
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sOptions->visibleCursor[sOptions->submenu] == optionsToDraw-2) // don't advance visible cursor until scrolled to the bottom
        {
            if (++sOptions->menuCursor[sOptions->submenu] == MenuItemCount() - 1)
                sOptions->visibleCursor[sOptions->submenu]++;
            else
                ScrollMenu(0);
        }
        else
        {
            if (++sOptions->menuCursor[sOptions->submenu] >= MenuItemCount()-1) // Scroll all the way to the top.
            {
                sOptions->visibleCursor[sOptions->submenu] = optionsToDraw-2;
                sOptions->menuCursor[sOptions->submenu] = MenuItemCount() - optionsToDraw-1;
                ScrollAll(1);
                sOptions->visibleCursor[sOptions->submenu] = sOptions->menuCursor[sOptions->submenu] = 0;
            }
            else
            {
                sOptions->visibleCursor[sOptions->submenu]++;
            }
        }
        HighlightOptionMenuItem();
        DrawDescriptionText();
    }
    else if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        if (sOptions->submenu == MENU_MAIN)
        {
            int cursor = sOptions->menuCursor[sOptions->submenu];
            u8 previousOption = sOptions->sel[cursor];
            if (CheckConditions(cursor))
            {
                if (sItemFunctionsMain[cursor].processInput != NULL)
                {
                    sOptions->sel[cursor] = sItemFunctionsMain[cursor].processInput(previousOption);
                    ReDrawAll();
                    DrawDescriptionText();
                }

                if (previousOption != sOptions->sel[cursor])
                    DrawChoices(cursor, sOptions->visibleCursor[sOptions->submenu] * Y_DIFF);
            }
        }
        else if (sOptions->submenu == MENU_OVERWORLD)
        {
            int cursor = sOptions->menuCursor[sOptions->submenu];
            u8 previousOption = sOptions->sel_overworld[cursor];
            if (CheckConditions(cursor))
            {
                if (sItemFunctionsOverworld[cursor].processInput != NULL)
                {
                    sOptions->sel_overworld[cursor] = sItemFunctionsOverworld[cursor].processInput(previousOption);
                    ReDrawAll();
                    DrawDescriptionText();
                }

                if (previousOption != sOptions->sel_overworld[cursor])
                    DrawChoices(cursor, sOptions->visibleCursor[sOptions->submenu] * Y_DIFF);
            }
        }
        else if (sOptions->submenu == MENU_BATTLE)
        {
            int cursor = sOptions->menuCursor[sOptions->submenu];
            u8 previousOption = sOptions->sel_battle[cursor];
            if (CheckConditions(cursor))
            {
                if (sItemFunctionsBattle[cursor].processInput != NULL)
                {
                    sOptions->sel_battle[cursor] = sItemFunctionsBattle[cursor].processInput(previousOption);
                    ReDrawAll();
                    DrawDescriptionText();
                }

                if (previousOption != sOptions->sel_battle[cursor])
                    DrawChoices(cursor, sOptions->visibleCursor[sOptions->submenu] * Y_DIFF);
            }
        }
    }
    else if (JOY_NEW(R_BUTTON))
    {
        if (sOptions->submenu != MENU_BATTLE)
            sOptions->submenu++;

        DrawTopBarText();
        ReDrawAll();
        HighlightOptionMenuItem();
        DrawDescriptionText();
    }
    else if (JOY_NEW(L_BUTTON))
    {
        if (sOptions->submenu != MENU_MAIN)
            sOptions->submenu--;
        
        DrawTopBarText();
        ReDrawAll();
        HighlightOptionMenuItem();
        DrawDescriptionText();
    }
}

static void Task_OptionMenuSave(u8 taskId)
{
    gSaveBlock2Ptr->optionsTextSpeed                = sOptions->sel[MENUITEM_MAIN_TEXTSPEED];
    gSaveBlock2Ptr->optionsCurrentFont              = sOptions->sel[MENUITEM_MAIN_FONT];
    gSaveBlock2Ptr->optionsSound                    = sOptions->sel[MENUITEM_MAIN_SOUND];
    gSaveBlock2Ptr->optionsButtonMode               = sOptions->sel[MENUITEM_MAIN_BUTTONMODE];
    gSaveBlock2Ptr->optionsUnitSystem               = sOptions->sel[MENUITEM_MAIN_UNIT_SYSTEM];
    gSaveBlock2Ptr->optionsClockMode                = sOptions->sel[MENUITEM_MAIN_CLOCK_MODE];
    gSaveBlock2Ptr->optionsInterfaceColor           = sOptions->sel[MENUITEM_MAIN_FRAMETYPE];
    gSaveBlock2Ptr->optionsTitleScreenRandomise     = sOptions->sel[MENUITEM_MAIN_TITLE_SCREEN];

    gSaveBlock2Ptr->optionsBikeMusic            = sOptions->sel_overworld[MENUITEM_OVERWORLD_BIKE_MUSIC];
    gSaveBlock2Ptr->optionsSurfMusic            = sOptions->sel_overworld[MENUITEM_OVERWORLD_SURF_MUSIC];
    gSaveBlock2Ptr->optionsSuppressNPCMugshots  = sOptions->sel_overworld[MENUITEM_OVERWORLD_NPC_MUG];
    gSaveBlock2Ptr->optionsFollowerMugshots     = sOptions->sel_overworld[MENUITEM_OVERWORLD_FOLLOWER_MUG];
    gSaveBlock2Ptr->optionsDisableMatchCall     = sOptions->sel_overworld[MENUITEM_OVERWORLD_MATCHCALL];

    gSaveBlock2Ptr->optionsBattleScene          = sOptions->sel_battle[MENUITEM_BATTLE_BATTLESCENE];
    gSaveBlock2Ptr->optionsBattleStyle          = sOptions->sel_battle[MENUITEM_BATTLE_BATTLESTYLE];
    gSaveBlock2Ptr->optionsWildBattleSpeed      = sOptions->sel_battle[MENUITEM_BATTLE_WILD_SPEED];
    gSaveBlock2Ptr->optionsTrainerBattleSpeed   = sOptions->sel_battle[MENUITEM_BATTLE_TRAINER_SPEED];
    gSaveBlock2Ptr->optionsDamageNumbers        = sOptions->sel_battle[MENUITEM_BATTLE_DAMAGE_NUMBERS];

    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

#define try_free(ptr) ({        \
    void ** ptr__ = (void **)&(ptr);   \
    if (*ptr__ != NULL)                \
        Free(*ptr__);                  \
})

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        FREE_AND_SET_NULL(sOptions);
        try_free(sBg2TilemapBuffer);
        try_free(sBg3TilemapBuffer);
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, 0);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        HideBg(2);
        HideBg(3);
        SetMainCallback2(gMain.savedCallback);
    }
}

static void ScrollMenu(int direction)
{
    int menuItem, pos;
    u8 optionsToDraw = min(OPTIONS_ON_SCREEN, MenuItemCount());

    if (direction == 0) // scroll down
        menuItem = sOptions->menuCursor[sOptions->submenu] + NUM_OPTIONS_FROM_BORDER, pos = optionsToDraw - 1;
    else
        menuItem = sOptions->menuCursor[sOptions->submenu] - NUM_OPTIONS_FROM_BORDER, pos = 0;

    // Hide one
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF, PIXEL_FILL(0));
    // Show one
    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(0), 0, Y_DIFF * pos, 26 * 8, Y_DIFF);
    // Print
    DrawChoices(menuItem, pos * Y_DIFF);
    DrawLeftSideOptionText(menuItem, (pos * Y_DIFF) + 1);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}
static void ScrollAll(int direction) // to bottom or top
{
    int i, y, menuItem, pos;
    int scrollCount;
    u8 optionsToDraw = min(OPTIONS_ON_SCREEN, MenuItemCount());

    scrollCount = MenuItemCount() - optionsToDraw;

    // Move items up/down
    ScrollWindow(WIN_OPTIONS, direction, Y_DIFF * scrollCount, PIXEL_FILL(1));

    // Clear moved items
    if (direction == 0)
    {
        y = optionsToDraw - scrollCount;
        if (y < 0)
            y = optionsToDraw;
        y *= Y_DIFF;
    }
    else
    {
        y = 0;
    }

    FillWindowPixelRect(WIN_OPTIONS, PIXEL_FILL(0), 0, y, 26 * 8, Y_DIFF * scrollCount);
    // Print new texts
    for (i = 0; i < scrollCount; i++)
    {
        if (direction == 0) // From top to bottom
            menuItem = MenuItemCount() - 1 - i, pos = optionsToDraw - 1 - i;
        else // From bottom to top
            menuItem = i, pos = i;
        DrawChoices(menuItem, pos * Y_DIFF);
        DrawLeftSideOptionText(menuItem, (pos * Y_DIFF) + 1);
    }
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}

// Process Input functions ****GENERIC****
static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3)
{
    int xMid;
    int widthLeft = GetStringWidth(1, txt1, 0);
    int widthMid = GetStringWidth(1, txt2, 0);
    int widthRight = GetStringWidth(1, txt3, 0);

    widthMid -= (198 - 104);
    xMid = (widthLeft - widthMid - widthRight) / 2 + 104;
    return xMid;
}

static int XOptions_ProcessInput(int x, int selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (++selection > (x - 1))
            selection = 0;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (--selection < 0)
            selection = (x - 1);
    }
    return selection;
}

static int ProcessInput_Options_Two(int selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
        selection ^= 1;

    return selection;
}

static int ProcessInput_Options_Three(int selection)
{
    return XOptions_ProcessInput(3, selection);
}

static int ProcessInput_Options_Four(int selection)
{
    return XOptions_ProcessInput(4, selection);
}

static int ProcessInput_Options_Eleven(int selection)
{
    return XOptions_ProcessInput(11, selection);
}

// Process Input functions ****SPECIFIC****
static int ProcessInput_Sound(int selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        SetPokemonCryStereo(selection);
    }

    return selection;
}

static int ProcessInput_FrameType(int selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection < IKIGAI_WINDOW_FRAMES_COUNT - 1)
            selection++;
        else
            selection = 0;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, 0x70, 0x20);
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = IKIGAI_WINDOW_FRAMES_COUNT - 1;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, 0x70, 0x20);
    }
    return selection;
}

static int ProcessInput_Interface(int selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection < IKIGAI_INTERFACE_GYM_TYPE_COLOUR)
            selection++;
        else
            selection = 0;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = IKIGAI_INTERFACE_GYM_TYPE_COLOUR;
    }
    
    gSaveBlock2Ptr->optionsInterfaceColor = selection;
    while (REG_VCOUNT >= 160);          // Wait until VBlank starts
    while (REG_VCOUNT < 160);           // Wait until VBlank ends
    LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
    LoadPalette(GetWindowFrameTilesPal(selection)->pal, 0x70, 0x20);
    // Reloads Palettes in case of Interface Changes
    LoadPalette(ReturnMenuUIPalette(), 64, 32);
    LoadPalette(ReturnScrollingBackgroundPalette(), 32, 32);
    return selection;
}

// Draw Choices functions ****GENERIC****
static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style, bool8 active)
{
    bool8 choosen = FALSE;
    if (style != 0)
        choosen = TRUE;

    DrawRightSideChoiceText(text, x, y+1, choosen, active);
}

static void DrawChoices_Options_Four(const u8 *const *const strings, int selection, int y, bool8 active)
{
    static const u8 choiceOrders[][3] =
    {
        {0, 1, 2},
        {0, 1, 2},
        {1, 2, 3},
        {1, 2, 3},
    };
    u8 styles[4] = {0};
    int xMid;
    const u8 *order = choiceOrders[selection];

    styles[selection] = 1;
    xMid = GetMiddleX(strings[order[0]], strings[order[1]], strings[order[2]]);

    DrawOptionMenuChoice(strings[order[0]], 104, y, styles[order[0]], active);
    DrawOptionMenuChoice(strings[order[1]], xMid, y, styles[order[1]], active);
    DrawOptionMenuChoice(strings[order[2]], GetStringRightAlignXOffset(1, strings[order[2]], 198), y, styles[order[2]], active);
}

static void ReDrawAll(void)
{
    u8 menuItem = sOptions->menuCursor[sOptions->submenu] - sOptions->visibleCursor[sOptions->submenu];
    u8 i;
    u8 optionsToDraw = min(OPTIONS_ON_SCREEN, MenuItemCount());

    // Check if the number of menu items is less than or equal to the number of options on screen
    if (MenuItemCount() <= OPTIONS_ON_SCREEN)
    {
        // If arrows exist, remove them
        if (sOptions->arrowTaskId != TASK_NONE)
        {
            RemoveScrollIndicatorArrowPair(sOptions->arrowTaskId);
            sOptions->arrowTaskId = TASK_NONE;
        }
    }
    else
    {
        // If no arrows exist, add them
        if (sOptions->arrowTaskId == TASK_NONE)
        {
            sOptions->arrowTaskId = AddScrollIndicatorArrowPairParameterized(
                SCROLL_ARROW_UP, 240 / 2, 20, 110, MenuItemCount() - 1, 110, 110, 0
            );
        }
    }

    // Draw menu options
    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(0));
    for (i = 0; i < optionsToDraw; i++)
    {
        DrawChoices(menuItem + i, i * Y_DIFF);
        DrawLeftSideOptionText(menuItem + i, (i * Y_DIFF) + 1);
    }
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
}

// Process Input functions ****SPECIFIC****
static const u8 sText_Faster[] = _("FASTER");
static const u8 sText_Instant[] = _("INSTANT");
static const u8 *const sTextSpeedStrings[] = {gText_TextSpeedSlow, gText_TextSpeedMid, gText_TextSpeedFast, sText_Instant};
static void DrawChoices_TextSpeed(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_TEXTSPEED);
    DrawChoices_Options_Four(sTextSpeedStrings, selection, y, active);
}

static void DrawChoices_BattleScene(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_BATTLE_BATTLESCENE);
    switch (selection)
    {
        default:
        case OPTIONS_BATTLE_SCENE_FULL_ANIMATION:
            DrawOptionMenuChoice(COMPOUND_STRING("CINEMATIC MODE"), 104, y, 1, active);
            break;
        case OPTIONS_BATTLE_SCENE_LOW_ANIMATION:
            DrawOptionMenuChoice(COMPOUND_STRING("LOW ANIMATIONS"), 104, y, 1, active);
            break;
        case OPTIONS_BATTLE_SCENE_NO_ANIMATION:
            DrawOptionMenuChoice(COMPOUND_STRING("STATIC BATTLES"), 104, y, 1, active);
            break;
        case OPTIONS_BATTLE_SCENE_NO_INTRO:
            DrawOptionMenuChoice(COMPOUND_STRING("STREAMLINED INTRO"), 104, y, 1, active);
            break;
    }
}

static void DrawChoices_BattleStyle(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_BATTLE_BATTLESTYLE);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleStyleShift, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_BattleStyleSet, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleStyleSet, 198), y, styles[1], active);
}

static void DrawChoices_Sound(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_SOUND);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_SoundMono, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_SoundStereo, GetStringRightAlignXOffset(FONT_NORMAL, gText_SoundStereo, 198), y, styles[1], active);
}

static void DrawChoices_ButtonMode(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_BUTTONMODE);
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_ButtonTypeNormal, gText_ButtonTypeLR, gText_ButtonTypeLEqualsA);
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_ButtonTypeNormal, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_ButtonTypeLR, xMid, y, styles[1], active);
    DrawOptionMenuChoice(gText_ButtonTypeLEqualsA, GetStringRightAlignXOffset(1, gText_ButtonTypeLEqualsA, 198), y, styles[2], active);
}

static const u8 sText_Normal[] = _("NORMAL");
/*
static void DrawChoices_BarSpeed(int selection, int y) //HP and EXP
{
    bool8 active = CheckConditions(MENUITEM_BATTLE_EXP_BAR);

    if (selection == 0)
         DrawOptionMenuChoice(sText_Normal, 104, y, 1, active);
    else if (selection < 10)
    {
        u8 textPlus[] = _("+1{0x77}{0x77}{0x77}{0x77}{0x77}"); // 0x77 is to clear INSTANT text
        textPlus[1] = CHAR_0 + selection;
        DrawOptionMenuChoice(textPlus, 104, y, 1, active);
    }
    else
        DrawOptionMenuChoice(sText_Instant, 104, y, 1, active);
}
*/
static void DrawChoices_BattleSpeed(int selection, int y) 
{
    bool8 active = CheckConditions(MENUITEM_BATTLE_TRAINER_SPEED);

    if (selection == 0)
        DrawOptionMenuChoice(sText_Normal, 104, y, 0, active);
    else
    {
        u8 textMultipler[] = _("x1{0x77}{0x77}{0x77}{0x77}{0x77}");
        textMultipler[1] = CHAR_1 + selection;
        DrawOptionMenuChoice(textMultipler, 104, y, 1, active);
    }
}

static void DrawChoices_UnitSystem(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_UNIT_SYSTEM);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_UnitSystemMetric, 104, y, styles[1], active);
    DrawOptionMenuChoice(gText_UnitSystemImperial, GetStringRightAlignXOffset(1, gText_UnitSystemImperial, 198), y, styles[0], active);
}

static void DrawChoices_ClockMode(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_CLOCK_MODE);
    u8 styles[2] = {0};
    styles[selection] = 1;

    if (selection == 0)
        DrawOptionMenuChoice(COMPOUND_STRING("12"), 104, y, 1, active);
    else
        DrawOptionMenuChoice(COMPOUND_STRING("24"), 104, y, 1, active);

    DrawOptionMenuChoice(COMPOUND_STRING("HOUR"), 119, y, 0, active);
}

static void DrawChoices_FrameType(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_FRAMETYPE);
    u8 text[16];
    u8 n = selection + 1;
    u16 i;

    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];

    // Convert a number to decimal string
    if (n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = 0x77;
        i++;
    }

    text[i] = EOS;

    DrawOptionMenuChoice(gText_FrameType, 104, y, 0, active);
    DrawOptionMenuChoice(text, 128, y, 1, active);
}

static void DrawChoices_Interface(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_FRAMETYPE);

    switch (selection)
    {
        default:
        case IKIGAI_INTERFACE_GREEN:
            DrawOptionMenuChoice(COMPOUND_STRING("TORNDADUS"), 104, y, 1, active);
            break;
        case IKIGAI_INTERFACE_BLUE:
            DrawOptionMenuChoice(COMPOUND_STRING("THUNDURUS"), 104, y, 1, active);
            break;
        case IKIGAI_INTERFACE_ORANGE:
            DrawOptionMenuChoice(COMPOUND_STRING("LANDORUS"), 104, y, 1, active);
            break;
        case IKIGAI_INTERFACE_PINK:
            DrawOptionMenuChoice(COMPOUND_STRING("ENAMORUS"), 104, y, 1, active);
            break;
        case IKIGAI_INTERFACE_GYM_TYPE_COLOUR:
            DrawOptionMenuChoice(COMPOUND_STRING("GYM TYPE"), 104, y, 1, active);
            break;
    }
}

static void DrawChoices_Font(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_FONT);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_OptionFontCompact, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_OptionFontSpread, GetStringRightAlignXOffset(1, gText_OptionFontSpread, 198), y, styles[1], active);
}

static void DrawChoices_MatchCall(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_OVERWORLD_MATCHCALL);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(1, gText_BattleSceneOff, 198), y, styles[1], active);
}

static void DrawChoices_BikeMusic(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_OVERWORLD_BIKE_MUSIC);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(1, gText_BattleSceneOff, 198), y, styles[1], active);
}

static void DrawChoices_SurfMusic(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_OVERWORLD_SURF_MUSIC);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(1, gText_BattleSceneOff, 198), y, styles[1], active);
}

static void DrawChoices_MugshotsNPC(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_OVERWORLD_NPC_MUG);
    u8 styles[2] = {0};
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOn, 104, y, styles[0], active);
    DrawOptionMenuChoice(gText_BattleSceneOff, GetStringRightAlignXOffset(1, gText_BattleSceneOff, 198), y, styles[1], active);
}

static void DrawChoices_MugshotsFollower(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_OVERWORLD_FOLLOWER_MUG);

    if (selection == 0)
        DrawOptionMenuChoice(COMPOUND_STRING("PLACEHOLDER"), 104, y, 1, active);
    else if (selection < 2)
    {
        u8 textOn[] = _("POKéMON ONLY{0x77}{0x77}{0x77}{0x77}{0x77}");
        DrawOptionMenuChoice(textOn, 104, y, 1, active);
    }
    else
        DrawOptionMenuChoice(gText_BattleSceneOff, 104, y, 0, active);
}

static void DrawChoices_TitleScreen(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_MAIN_TITLE_SCREEN);
    u8 styles[2] = {0};
    styles[selection] = 1;

    if (selection == 0)
        DrawOptionMenuChoice(COMPOUND_STRING("MATCH INTERFACE"), 104, y, styles[0], active);
    else
        DrawOptionMenuChoice(COMPOUND_STRING("RANDOMISE LEGEND"), 104, y, 1, active);
}

static void DrawChoices_DamageNumbers(int selection, int y)
{
    bool8 active = CheckConditions(MENUITEM_BATTLE_DAMAGE_NUMBERS);

    switch (selection)
    {
    default:
    case 0:
        DrawOptionMenuChoice(gText_BattleSceneOff, 104, y, 0, active);
        break;
    
    case 1:
        DrawOptionMenuChoice(COMPOUND_STRING("ATTACK DAMAGE ONLY{0x77}{0x77}{0x77}{0x77}{0x77}"), 104, y, 1, active);
        break;
    
    case 2:
        DrawOptionMenuChoice(COMPOUND_STRING("ALL DAMAGE"), 104, y, 1, active);
        break;
    }
}


// Background tilemap
#define TILE_TOP_CORNER_L 0x1A2 // 418
#define TILE_TOP_EDGE     0x1A3 // 419
#define TILE_TOP_CORNER_R 0x1A4 // 420
#define TILE_LEFT_EDGE    0x1A5 // 421
#define TILE_RIGHT_EDGE   0x1A7 // 423
#define TILE_BOT_CORNER_L 0x1A8 // 424
#define TILE_BOT_EDGE     0x1A9 // 425
#define TILE_BOT_CORNER_R 0x1AA // 426

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Option Texts window
    //FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  2,  1,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  2, 26,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  2,  1,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  3,  1, 16,  7);
    //FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  3,  1, 16,  7);
    //FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 13,  1,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 13, 26,  1,  7);
    //FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 13,  1,  1,  7);

    // Description window
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1, 14,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2, 14, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28, 14,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1, 15,  1,  4,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28, 15,  1,  4,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}
