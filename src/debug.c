//CREDITS
//TheXaman:             https://github.com/TheXaman/pokeemerald/tree/tx_debug_system
//CODE USED FROM:
//ketsuban:             https://github.com/pret/pokeemerald/wiki/Add-a-debug-menu
//Pyredrid:             https://github.com/Pyredrid/pokeemerald/tree/debugmenu
//AsparagusEduardo:     https://github.com/AsparagusEduardo/pokeemerald/tree/InfusedEmerald_v2
//Ghoulslash:           https://github.com/ghoulslash/pokeemerald
//Jaizu:                https://jaizu.moe/
//AND OTHER RHH POKEEMERALD-EXPANSION CONTRIBUTORS
#include "global.h"
#include "battle.h"
#include "battle_setup.h"
#include "berry.h"
#include "clock.h"
#include "coins.h"
#include "credits.h"
#include "data.h"
#include "daycare.h"
#include "debug.h"
#include "decoration.h"
#include "decoration_inventory.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "field_message_box.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "follower_npc.h"
#include "international_string_util.h"
#include "item.h"
#include "item_icon.h"
#include "list_menu.h"
#include "m4a.h"
#include "main.h"
#include "main_menu.h"
#include "malloc.h"
#include "map_name_popup.h"
#include "menu.h"
#include "money.h"
#include "naming_screen.h"
#include "new_game.h"
#include "overworld.h"
#include "palette.h"
#include "party_menu.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "region_map.h"
#include "rtc.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "pokemon_summary_screen.h"
#include "wild_encounter.h"
#include "constants/abilities.h"
#include "constants/battle_ai.h"
#include "constants/battle_frontier.h"
#include "constants/coins.h"
#include "constants/decorations.h"
#include "constants/event_objects.h"
#include "constants/expansion.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/map_groups.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "constants/weather.h"
#include "siirtc.h"
#include "rtc.h"
#include "fake_rtc.h"
#include "save.h"

#include "calendar.h"
#include "dynamic_music.h"
#include "ikigai_characters.h"
#include "safari_zone.h"
#include "start_menu.h"
#include "ui_samuel_case.h"
#include "vyraton.h"

#define FLAG_DEBUG_SOUND_OVERWORLD_PLAY TRUE

enum FollowerNPCCreateDebugMenu
{
    DEBUG_FNPC_BRENDAN,
    DEBUG_FNPC_MAY,
    DEBUG_FNPC_STEVEN,
    DEBUG_FNPC_WALLY,
    DEBUG_FNPC_RED,
    DEBUG_FNPC_LEAF,
    DEBUG_FNPC_COUNT,
};

enum FlagsVarsDebugMenu
{
    DEBUG_FLAGVAR_MENU_ITEM_FLAGS,
    DEBUG_FLAGVAR_MENU_ITEM_VARS,
    DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_ALL,
    DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_RESET,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKEDEX,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_NATDEX,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKENAV,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_MATCH_CALL,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_RUN_SHOES,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BICYCLE,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_LOCATIONS,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BADGES_ALL,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_GAME_CLEAR,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_FRONTIER_PASS,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_COLLISION,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_ENCOUNTER,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_TRAINER_SEE,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BAG_USE,
    DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_CATCHING,
};

enum DebugBattleType
{
    DEBUG_BATTLE_0_MENU_ITEM_WILD,
    DEBUG_BATTLE_0_MENU_ITEM_WILD_DOUBLE,
    DEBUG_BATTLE_0_MENU_ITEM_SINGLE,
    DEBUG_BATTLE_0_MENU_ITEM_DOUBLE,
    DEBUG_BATTLE_0_MENU_ITEM_MULTI,
};

enum DebugBattleAIFlags
{
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_00,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_01,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_02,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_03,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_04,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_05,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_06,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_07,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_08,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_09,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_10,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_11,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_12,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_13,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_14,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_15,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_16,
    DEBUG_BATTLE_1_MENU_ITEM_AI_FLAG_17,
    DEBUG_BATTLE_1_MENU_ITEM_CONTINUE,
};

enum DebugBattleEnvironment
{
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_0,
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_1,
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_2,
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_3,
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_4,
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_5,
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_6,
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_7,
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_8,
    DEBUG_BATTLE_2_MENU_ITEM_ENVIRONMENT_9,
};

// *******************************
// Constants
#define DEBUG_MENU_FONT FONT_NORMAL

#define DEBUG_MENU_WIDTH_MAIN 17
#define DEBUG_MENU_HEIGHT_MAIN 9

#define DEBUG_MENU_WIDTH_EXTRA 10
#define DEBUG_MENU_HEIGHT_EXTRA 4

#define DEBUG_MENU_WIDTH_GYM_TYPE 12

#define DEBUG_MENU_WIDTH_WEATHER 15
#define DEBUG_MENU_HEIGHT_WEATHER 3

#define DEBUG_MENU_WIDTH_SOUND 25
#define DEBUG_MENU_HEIGHT_SOUND 6

#define DEBUG_MENU_WIDTH_FLAGVAR 4
#define DEBUG_MENU_HEIGHT_FLAGVAR 2

#define DEBUG_MENU_WIDTH_SEASON 10
#define DEBUG_MENU_HEIGHT_SEASON 3

#define DEBUG_NUMBER_DIGITS_FLAGS 4
#define DEBUG_NUMBER_DIGITS_VARIABLES 5
#define DEBUG_NUMBER_DIGITS_VARIABLE_VALUE 5
#define DEBUG_NUMBER_DIGITS_ITEMS 4
#define DEBUG_NUMBER_DIGITS_ITEM_QUANTITY 3
#define DEBUG_NUMBER_DIGITS_POKEMON_TYPE 2

#define DEBUG_NUMBER_ICON_X 210
#define DEBUG_NUMBER_ICON_Y 50

#define DEBUG_MAX_MENU_ITEMS 20
#define DEBUG_MAX_SUB_MENU_LEVELS 6

// *******************************
struct DebugMenuOption;

typedef void (*DebugFunc)(u8 taskId);
typedef void (*DebugSubmenuFunc)(u8 taskId, const struct DebugMenuOption *items);

struct DebugMenuOption
{
    const u8 *text;
    const void *action;
    const void *actionParams;
};

struct DebugMonData
{
    u16 species;
    u8 level;
    bool8 isShiny:1;
    u8 nature:5;
    u8 abilityNum:2;
    u8 monIVs[NUM_STATS];
    u16 monMoves[MAX_MON_MOVES];
    u8 monEVs[NUM_STATS];
    u8 teraType;
    u8 dynamaxLevel:7;
    u8 gmaxFactor:1;
};

struct DebugMenuListData
{
    const struct DebugMenuOption *subMenuItems[DEBUG_MAX_SUB_MENU_LEVELS];
    struct ListMenuItem listItems[DEBUG_MAX_MENU_ITEMS + 1];
    u8 itemNames[DEBUG_MAX_MENU_ITEMS + 1][26];
    u8 listId;
};

// EWRAM
static EWRAM_DATA struct DebugMonData *sDebugMonData = NULL;
static EWRAM_DATA struct DebugMenuListData *sDebugMenuListData = NULL;
EWRAM_DATA bool8 gIsDebugBattle = FALSE;
EWRAM_DATA u64 gDebugAIFlags = 0;
static EWRAM_DATA u32 sDynamicMusicInput;

// *******************************
// Define functions
static void Debug_ShowMenu(DebugFunc HandleInput, const struct DebugMenuOption *items);
static u8 Debug_GenerateListMenuNames(void);
static void Debug_DestroyMenu(u8 taskId);
static void DebugAction_Cancel(u8 taskId);
static void DebugAction_DestroyExtraWindow(u8 taskId);
static void Debug_RefreshListMenu(u8 taskId);

static void DebugAction_OpenSubMenu(u8 taskId, const struct DebugMenuOption *items);
static void DebugAction_OpenSubMenuFlagsVars(u8 taskId, const struct DebugMenuOption *items);
static void DebugAction_OpenSubMenuFakeRTC(u8 taskId, const struct DebugMenuOption *items);
static void DebugAction_OpenSubMenuCreateFollowerNPC(u8 taskId, const struct DebugMenuOption *items);
static void DebugAction_ExecuteScript(u8 taskId, const u8 *script);
static void DebugAction_ToggleFlag(u8 taskId);

static void DebugTask_HandleMenuInput_General(u8 taskId);

static void DebugAction_Util_Fly(u8 taskId);
static void DebugAction_Util_Warp_Warp(u8 taskId);
static void DebugAction_Util_Warp_SelectMapGroup(u8 taskId);
static void DebugAction_Util_Warp_SelectMap(u8 taskId);
static void DebugAction_Util_Warp_SelectWarp(u8 taskId);
static void DebugAction_Util_Weather(u8 taskId);
static void DebugAction_Util_Weather_SelectId(u8 taskId);
static void DebugAction_Util_WatchCredits(u8 taskId);
static void DebugAction_Util_CheatStart(u8 taskId);

static void DebugAction_TimeMenu_ChangeTimeOfDay(u8 taskId);
static void DebugAction_TimeMenu_ChangeWeekdays(u8 taskId);

static void DebugAction_CreateFollowerNPC(u8 taskId);
static void DebugAction_DestroyFollowerNPC(u8 taskId);

static void DebugAction_PCBag_Fill_PCBoxes_Fast(u8 taskId);
static void DebugAction_PCBag_Fill_PCBoxes_Slow(u8 taskId);
static void DebugAction_PCBag_Fill_PCItemStorage(u8 taskId);
static void DebugAction_PCBag_Fill_PocketItems(u8 taskId);
static void DebugAction_PCBag_Fill_PocketPokeBalls(u8 taskId);
static void DebugAction_PCBag_Fill_PocketTMHM(u8 taskId);
static void DebugAction_PCBag_Fill_PocketBerries(u8 taskId);
static void DebugAction_PCBag_Fill_PocketKeyItems(u8 taskId);
static void DebugAction_PCBag_ClearBag(u8 taskId);
static void DebugAction_PCBag_ClearBoxes(u8 taskId);

static void DebugAction_Party_HealParty(u8 taskId);
static void DebugAction_Party_ClearParty(u8 taskId);
static void DebugAction_Party_SetParty(u8 taskId);
static void DebugAction_Party_BattleSingle(u8 taskId);

static void DebugAction_FlagsVars_Flags(u8 taskId);
static void DebugAction_FlagsVars_FlagsSelect(u8 taskId);
static void DebugAction_FlagsVars_Vars(u8 taskId);
static void DebugAction_FlagsVars_Select(u8 taskId);
static void DebugAction_FlagsVars_SetValue(u8 taskId);
static void DebugAction_FlagsVars_PokedexFlags_All(u8 taskId);
static void DebugAction_FlagsVars_PokedexFlags_Reset(u8 taskId);
static void DebugAction_FlagsVars_SwitchDex(u8 taskId);
static void DebugAction_FlagsVars_SwitchNatDex(u8 taskId);
static void DebugAction_FlagsVars_SwitchPokeNav(u8 taskId);
static void DebugAction_FlagsVars_SwitchMatchCall(u8 taskId);
static void DebugAction_FlagsVars_ToggleFlyFlags(u8 taskId);
static void DebugAction_FlagsVars_ToggleBadgeFlags(u8 taskId);
static void DebugAction_FlagsVars_ToggleGameClear(u8 taskId);
static void DebugAction_FlagsVars_ToggleFrontierPass(u8 taskId);
static void DebugAction_FlagsVars_CollisionOnOff(u8 taskId);
static void DebugAction_FlagsVars_EncounterOnOff(u8 taskId);
static void DebugAction_FlagsVars_TrainerSeeOnOff(u8 taskId);
static void DebugAction_FlagsVars_BagUseOnOff(u8 taskId);
static void DebugAction_FlagsVars_CatchingOnOff(u8 taskId);
static void DebugAction_FlagsVars_RunningShoes(u8 taskId);
static void DebugAction_FlagsVars_Bicycle(u8 taskId);

static void DebugAction_Give_Item(u8 taskId);
static void DebugAction_Give_Item_SelectId(u8 taskId);
static void DebugAction_Give_Item_SelectQuantity(u8 taskId);
static void DebugAction_Give_PokemonSimple(u8 taskId);
static void DebugAction_Give_PokemonComplex(u8 taskId);
static void DebugAction_Give_Pokemon_SelectId(u8 taskId);
static void DebugAction_Give_Pokemon_SelectLevel(u8 taskId);
static void DebugAction_Give_Pokemon_SelectShiny(u8 taskId);
static void DebugAction_Give_Pokemon_SelectNature(u8 taskId);
static void DebugAction_Give_Pokemon_SelectAbility(u8 taskId);
static void DebugAction_Give_Pokemon_SelectTeraType(u8 taskId);
static void DebugAction_Give_Pokemon_SelectDynamaxLevel(u8 taskId);
static void DebugAction_Give_Pokemon_SelectGigantamaxFactor(u8 taskId);
static void DebugAction_Give_Pokemon_SelectIVs(u8 taskId);
static void DebugAction_Give_Pokemon_SelectEVs(u8 taskId);
static void DebugAction_Give_Pokemon_ComplexCreateMon(u8 taskId);
static void DebugAction_Give_Pokemon_Move(u8 taskId);
static void DebugAction_Give_Decoration(u8 taskId);
static void DebugAction_Give_Decoration_SelectId(u8 taskId);
static void DebugAction_Give_MaxMoney(u8 taskId);
static void DebugAction_Give_MaxCoins(u8 taskId);
static void DebugAction_Give_MaxBattlePoints(u8 taskId);
static void DebugAction_Give_DayCareEgg(u8 taskId);

static void DebugAction_Sound_SE(u8 taskId);
static void DebugAction_Sound_SE_SelectId(u8 taskId);
static void DebugAction_Sound_MUS(u8 taskId);
static void DebugAction_Sound_MUS_SelectId(u8 taskId);

static void DebugAction_BerryFunctions_ClearAll(u8 taskId);
static void DebugAction_BerryFunctions_Ready(u8 taskId);
static void DebugAction_BerryFunctions_NextStage(u8 taskId);
static void DebugAction_BerryFunctions_Pests(u8 taskId);
static void DebugAction_BerryFunctions_Weeds(u8 taskId);

static void DebugAction_Player_Name(u8 taskId);
static void DebugAction_Player_Gender(u8 taskId);
static void DebugAction_Player_Id(u8 taskId);

// Ikigai Declarations
static const struct DebugMenuOption sDebugMenu_Actions_Ikigai[];
static void DebugAction_Ikigai_GymType(u8 taskId);
static void DebugAction_Player_Nickname(u8 taskId);
static void DebugAction_Ikigai_MeetAllCharacter(u8 taskId);
static void DebugAction_Ikigai_Season(u8 taskId);
static void DebugAction_Ikigai_CalendarWarp(u8 taskId);
static void DebugAction_Ikigai_MUS_Expansion(u8 taskId);
static void DebugAction_Ikigai_PokemonCries(u8 taskId);
static void DebugAction_OpenSubMenuDynamicMusic(u8 taskId, const struct DebugMenuOption *items);
static void DebugAction_DynamicMusic_InstrumentRemove(u8 taskId);
static void DebugAction_DynamicMusic_InstrumentRestore(u8 taskId);
static void DebugAction_DynamicMusic_InstrumentPlayOnly(u8 taskId);
static void DebugAction_DynamicMusic_TrackRemove(u8 taskId);
static void DebugAction_DynamicMusic_TrackRestore(u8 taskId);
static void DebugAction_DynamicMusic_TrackPlayOnly(u8 taskId);
static void DebugAction_DynamicMusic_Movement(u8 taskId);
static void DebugAction_OpenSubMenuRotomStartMenu(u8 taskId, const struct DebugMenuOption *items);
static void DebugAction_Ikigai_TogglePokemonFlag(u8 taskId);
static void DebugAction_Ikigai_ToggleSafariFlag(u8 taskId);
static void DebugAction_Ikigai_ToggleDexNavFlag(u8 taskId);
static void DebugAction_Ikigai_DefaultStartMenu(u8 taskId);
extern const u8 Route117_PokemonDayCare_EventScript_DaycareWoman[];

extern const u8 Debug_FlagsNotSetOverworldConfigMessage[];
extern const u8 Debug_FlagsNotSetBattleConfigMessage[];
extern const u8 Debug_FlagsAndVarNotSetBattleConfigMessage[];
extern const u8 Debug_EventScript_FontTest[];
extern const u8 Debug_EventScript_CheckEVs[];
extern const u8 Debug_EventScript_CheckIVs[];
extern const u8 Debug_EventScript_InflictStatus1[];
extern const u8 Debug_EventScript_SetHiddenNature[];
extern const u8 Debug_EventScript_Script_1[];
extern const u8 Debug_EventScript_Script_2[];
extern const u8 Debug_EventScript_Script_3[];
extern const u8 Debug_EventScript_Script_4[];
extern const u8 Debug_EventScript_Script_5[];
extern const u8 Debug_EventScript_Script_6[];
extern const u8 Debug_EventScript_Script_7[];
extern const u8 Debug_EventScript_Script_8[];
extern const u8 DebugScript_DaycareMonsNotCompatible[];
extern const u8 DebugScript_OneDaycareMons[];
extern const u8 DebugScript_ZeroDaycareMons[];

extern const u8 Debug_ShowFieldMessageStringVar4[];
extern const u8 Debug_OpenPokeSphere[];
extern const u8 Debug_OpenCalendar[];
extern const u8 Debug_CheatStart[];
extern const u8 Debug_HatchAnEgg[];
extern const u8 PlayersHouse_2F_EventScript_SetWallClock[];
extern const u8 PlayersHouse_2F_EventScript_CheckWallClock[];
extern const u8 Debug_CheckSaveBlock[];
extern const u8 Debug_CheckROMSpace[];
extern const u8 Debug_BoxFilledMessage[];
extern const u8 Debug_ShowExpansionVersion[];
extern const u8 Debug_EventScript_EWRAMCounters[];
extern const u8 Debug_Follower_NPC_Event_Script[];
extern const u8 Debug_Follower_NPC_Not_Enabled[];
extern const u8 Debug_EventScript_Steven_Multi[];
extern const u8 Debug_EventScript_PrintTimeOfDay[];
extern const u8 Debug_EventScript_TellTheTime[];
extern const u8 Debug_EventScript_FakeRTCNotEnabled[];

extern const u8 Debug_BerryPestsDisabled[];
extern const u8 Debug_BerryWeedsDisabled[];

extern const u8 FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon[];

#include "data/map_group_count.h"

// Text
// General
static const u8 sDebugText_Arrow[] =          _("{CLEAR_TO 110}{RIGHT_ARROW}");
static const u8 sDebugText_True[] =          _("TRUE");
static const u8 sDebugText_False[] =         _("FALSE");
static const u8 sDebugText_Colored_True[] =  _("{COLOR GREEN}TRUE");
static const u8 sDebugText_Colored_False[] = _("{COLOR RED}FALSE");
static const u8 sDebugText_Dashes[] =        _("---");
static const u8 sDebugText_Empty[] =         _("");
static const u8 sDebugText_Continue[] =      _("Continue…");
// Util Menu
static const u8 sDebugText_Util_WarpToMap_SelectMapGroup[] = _("Group: {STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n\n{STR_VAR_3}{CLEAR_TO 90}");
static const u8 sDebugText_Util_WarpToMap_SelectMap[] =      _("Map: {STR_VAR_1}{CLEAR_TO 90}\nMapSec:{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}\n{STR_VAR_3}{CLEAR_TO 90}");
static const u8 sDebugText_Util_WarpToMap_SelectWarp[] =     _("Warp:{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_3}{CLEAR_TO 90}");
static const u8 sDebugText_Util_WarpToMap_SelMax[] =         _("{STR_VAR_1} / {STR_VAR_2}");
static const u8 sDebugText_Util_Weather_ID[] =               _("Weather ID: {STR_VAR_3}\n{STR_VAR_1}\n{STR_VAR_2}");

//Time Menu

static const u8 *const gDayNameStringsTable[WEEKDAY_COUNT] = {
    COMPOUND_STRING("Sunday"),
    COMPOUND_STRING("Monday"),
    COMPOUND_STRING("Tuesday"),
    COMPOUND_STRING("Wednesday"),
    COMPOUND_STRING("Thursday"),
    COMPOUND_STRING("Friday"),
    COMPOUND_STRING("Saturday"),
};

static const u8 *const gTimeOfDayStringsTable[TIMES_OF_DAY_COUNT] = {
    COMPOUND_STRING("Morning"),
    COMPOUND_STRING("Day"),
    COMPOUND_STRING("Evening"),
    COMPOUND_STRING("Night"),
};

// Follower NPC

static const u8 *const gFollowerNPCStringsTable[DEBUG_FNPC_COUNT] = {
    COMPOUND_STRING("Brendan"),
    COMPOUND_STRING("May"),
    COMPOUND_STRING("Steven"),
    COMPOUND_STRING("Wally"),
    COMPOUND_STRING("Red"),
    COMPOUND_STRING("Leaf"),
};

// Flags/Vars Menu
static const u8 sDebugText_FlagsVars_VariableHex[] =         _("{STR_VAR_1}{CLEAR_TO 90}\n0x{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_FlagsVars_Variable[] =            _("Var: {STR_VAR_1}{CLEAR_TO 90}\nVal: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_2}");
static const u8 sDebugText_FlagsVars_VariableValueSet[] =    _("Var: {STR_VAR_1}{CLEAR_TO 90}\nVal: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_2}");
// Give Menu
static const u8 sDebugText_PokemonShiny[] =             _("Shiny:{CLEAR_TO 90}\n   {STR_VAR_2}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{CLEAR_TO 90}");
static const u8 sDebugText_IVs[] =                      _("IV {STR_VAR_1}:{CLEAR_TO 90}\n    {STR_VAR_3}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
static const u8 sDebugText_EVs[] =                      _("EV {STR_VAR_1}:{CLEAR_TO 90}\n    {STR_VAR_3}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}");
// Sound Menu
static const u8 sDebugText_Sound_SFX_ID[] =             _("SFX ID: {STR_VAR_3}   {START_BUTTON} Stop\n{STR_VAR_1}    {CLEAR_TO 240}\n{STR_VAR_2}");
static const u8 sDebugText_Sound_Music_ID[] =           _("Music ID: {STR_VAR_3}   {START_BUTTON} Stop\n{STR_VAR_1}    {CLEAR_TO 240}\n{STR_VAR_2}");

const u8 *const gText_DigitIndicator[] =
{
    COMPOUND_STRING("{LEFT_ARROW}+1{RIGHT_ARROW}        "),
    COMPOUND_STRING("{LEFT_ARROW}+10{RIGHT_ARROW}       "),
    COMPOUND_STRING("{LEFT_ARROW}+100{RIGHT_ARROW}      "),
    COMPOUND_STRING("{LEFT_ARROW}+1000{RIGHT_ARROW}     "),
    COMPOUND_STRING("{LEFT_ARROW}+10000{RIGHT_ARROW}    "),
    COMPOUND_STRING("{LEFT_ARROW}+100000{RIGHT_ARROW}   "),
    COMPOUND_STRING("{LEFT_ARROW}+1000000{RIGHT_ARROW}  "),
    COMPOUND_STRING("{LEFT_ARROW}+10000000{RIGHT_ARROW} "),
};

static const s32 sPowersOfTen[] =
{
             1,
            10,
           100,
          1000,
         10000,
        100000,
       1000000,
      10000000,
     100000000,
    1000000000,
};

// *******************************
// Menu Actions. Make sure that submenus are defined before the menus that call them.
static const struct DebugMenuOption sDebugMenu_Actions_TimeMenu_TimesOfDay[] =
{
    [TIME_MORNING] = { gTimeOfDayStringsTable[TIME_MORNING], DebugAction_TimeMenu_ChangeTimeOfDay },
    [TIME_DAY]     = { gTimeOfDayStringsTable[TIME_DAY],     DebugAction_TimeMenu_ChangeTimeOfDay },
    [TIME_EVENING] = { gTimeOfDayStringsTable[TIME_EVENING], DebugAction_TimeMenu_ChangeTimeOfDay },
    [TIME_NIGHT]   = { gTimeOfDayStringsTable[TIME_NIGHT],   DebugAction_TimeMenu_ChangeTimeOfDay },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_TimeMenu_Weekdays[] =
{
    [WEEKDAY_SUN] = { gDayNameStringsTable[WEEKDAY_SUN], DebugAction_TimeMenu_ChangeWeekdays },
    [WEEKDAY_MON] = { gDayNameStringsTable[WEEKDAY_MON], DebugAction_TimeMenu_ChangeWeekdays },
    [WEEKDAY_TUE] = { gDayNameStringsTable[WEEKDAY_TUE], DebugAction_TimeMenu_ChangeWeekdays },
    [WEEKDAY_WED] = { gDayNameStringsTable[WEEKDAY_WED], DebugAction_TimeMenu_ChangeWeekdays },
    [WEEKDAY_THU] = { gDayNameStringsTable[WEEKDAY_THU], DebugAction_TimeMenu_ChangeWeekdays },
    [WEEKDAY_FRI] = { gDayNameStringsTable[WEEKDAY_FRI], DebugAction_TimeMenu_ChangeWeekdays },
    [WEEKDAY_SAT] = { gDayNameStringsTable[WEEKDAY_SAT], DebugAction_TimeMenu_ChangeWeekdays },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_FollowerNPCMenu_Create[] =
{
    [DEBUG_FNPC_BRENDAN] = { gFollowerNPCStringsTable[DEBUG_FNPC_BRENDAN], DebugAction_CreateFollowerNPC },
    [DEBUG_FNPC_MAY] =     { gFollowerNPCStringsTable[DEBUG_FNPC_MAY],     DebugAction_CreateFollowerNPC },
    [DEBUG_FNPC_STEVEN] =  { gFollowerNPCStringsTable[DEBUG_FNPC_STEVEN],  DebugAction_CreateFollowerNPC },
    [DEBUG_FNPC_WALLY] =   { gFollowerNPCStringsTable[DEBUG_FNPC_WALLY],   DebugAction_CreateFollowerNPC },
    [DEBUG_FNPC_RED] =     { gFollowerNPCStringsTable[DEBUG_FNPC_RED],     DebugAction_CreateFollowerNPC },
    [DEBUG_FNPC_LEAF] =    { gFollowerNPCStringsTable[DEBUG_FNPC_LEAF],    DebugAction_CreateFollowerNPC },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_TimeMenu[] =
{
    { COMPOUND_STRING("Get time…"),         DebugAction_ExecuteScript, Debug_EventScript_TellTheTime },
    { COMPOUND_STRING("Get time of day…"),  DebugAction_ExecuteScript, Debug_EventScript_PrintTimeOfDay },
    { COMPOUND_STRING("Set time of day…"),  DebugAction_OpenSubMenuFakeRTC, sDebugMenu_Actions_TimeMenu_TimesOfDay },
    { COMPOUND_STRING("Set weekday…"),      DebugAction_OpenSubMenuFakeRTC, sDebugMenu_Actions_TimeMenu_Weekdays },
    { COMPOUND_STRING("Check wall clock…"), DebugAction_ExecuteScript, PlayersHouse_2F_EventScript_CheckWallClock },
    { COMPOUND_STRING("Set wall clock…"),   DebugAction_ExecuteScript, PlayersHouse_2F_EventScript_SetWallClock },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_BerryFunctions[] =
{
    { COMPOUND_STRING("Clear map trees"),      DebugAction_BerryFunctions_ClearAll },
    { COMPOUND_STRING("Ready map trees"),      DebugAction_BerryFunctions_Ready },
    { COMPOUND_STRING("Grow map trees"),       DebugAction_BerryFunctions_NextStage },
    { COMPOUND_STRING("Give map trees pests"), DebugAction_BerryFunctions_Pests },
    { COMPOUND_STRING("Give map trees weeds"), DebugAction_BerryFunctions_Weeds },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_FollowerNPCMenu[] =
{
    { COMPOUND_STRING("Create Follower"),  DebugAction_OpenSubMenuCreateFollowerNPC, sDebugMenu_Actions_FollowerNPCMenu_Create },
    { COMPOUND_STRING("Destroy Follower"), DebugAction_DestroyFollowerNPC },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Utilities[] =
{
    { COMPOUND_STRING("Fly to map…"),       DebugAction_Util_Fly },
    { COMPOUND_STRING("Warp to map warp…"), DebugAction_Util_Warp_Warp },
    { COMPOUND_STRING("Set weather…"),      DebugAction_Util_Weather },
    { COMPOUND_STRING("Font Test…"),        DebugAction_ExecuteScript, Debug_EventScript_FontTest },
    { COMPOUND_STRING("Time Functions…"),   DebugAction_OpenSubMenu, sDebugMenu_Actions_TimeMenu, },
    { COMPOUND_STRING("Watch credits…"),    DebugAction_Util_WatchCredits },
    { COMPOUND_STRING("Cheat start"),       DebugAction_Util_CheatStart },
    { COMPOUND_STRING("Berry Functions…"),  DebugAction_OpenSubMenu, sDebugMenu_Actions_BerryFunctions },
    { COMPOUND_STRING("EWRAM Counters…"),   DebugAction_ExecuteScript, Debug_EventScript_EWRAMCounters },
    { COMPOUND_STRING("Follower NPC…"),     DebugAction_OpenSubMenu, sDebugMenu_Actions_FollowerNPCMenu },
    { COMPOUND_STRING("Steven Multi"),      DebugAction_ExecuteScript, Debug_EventScript_Steven_Multi },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_PCBag_Fill[] =
{
    { COMPOUND_STRING("Fill PC Boxes Fast"),        DebugAction_PCBag_Fill_PCBoxes_Fast },
    { COMPOUND_STRING("Fill PC Boxes Slow (LAG!)"), DebugAction_PCBag_Fill_PCBoxes_Slow },
    { COMPOUND_STRING("Fill PC Items") ,            DebugAction_PCBag_Fill_PCItemStorage },
    { COMPOUND_STRING("Fill Pocket Items"),         DebugAction_PCBag_Fill_PocketItems },
    { COMPOUND_STRING("Fill Pocket Poké Balls"),    DebugAction_PCBag_Fill_PocketPokeBalls },
    { COMPOUND_STRING("Fill Pocket TMHM"),          DebugAction_PCBag_Fill_PocketTMHM },
    { COMPOUND_STRING("Fill Pocket Berries"),       DebugAction_PCBag_Fill_PocketBerries },
    { COMPOUND_STRING("Fill Pocket Key Items"),     DebugAction_PCBag_Fill_PocketKeyItems },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_PCBag[] =
{
    { COMPOUND_STRING("Access PC"),           DebugAction_ExecuteScript, EventScript_PC },
    { COMPOUND_STRING("Fill…"),               DebugAction_OpenSubMenu, sDebugMenu_Actions_PCBag_Fill },
    { COMPOUND_STRING("Clear Bag"),           DebugAction_PCBag_ClearBag },
    { COMPOUND_STRING("Clear Storage Boxes"), DebugAction_PCBag_ClearBoxes },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Party[] =
{
    { COMPOUND_STRING("Move Reminder"),      DebugAction_ExecuteScript, FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon },
    { COMPOUND_STRING("Hatch an Egg"),       DebugAction_ExecuteScript, Debug_HatchAnEgg },
    { COMPOUND_STRING("Heal party"),         DebugAction_Party_HealParty },
    { COMPOUND_STRING("Inflict Status1"),    DebugAction_ExecuteScript, Debug_EventScript_InflictStatus1 },
    { COMPOUND_STRING("Set Hidden Nature"),  DebugAction_ExecuteScript, Debug_EventScript_SetHiddenNature },
    { COMPOUND_STRING("Check EVs"),          DebugAction_ExecuteScript, Debug_EventScript_CheckEVs },
    { COMPOUND_STRING("Check IVs"),          DebugAction_ExecuteScript, Debug_EventScript_CheckIVs },
    { COMPOUND_STRING("Clear Party"),        DebugAction_Party_ClearParty },
    { COMPOUND_STRING("Set Party"),          DebugAction_Party_SetParty },
    { COMPOUND_STRING("Start Debug Battle"), DebugAction_Party_BattleSingle },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Give[] =
{
    { COMPOUND_STRING("Give item XYZ…"),    DebugAction_Give_Item },
    { COMPOUND_STRING("Pokémon (Basic)"),   DebugAction_Give_PokemonSimple },
    { COMPOUND_STRING("Pokémon (Complex)"), DebugAction_Give_PokemonComplex },
    { COMPOUND_STRING("Give Decoration…"),  DebugAction_Give_Decoration },
    { COMPOUND_STRING("Max Money"),         DebugAction_Give_MaxMoney },
    { COMPOUND_STRING("Max Coins"),         DebugAction_Give_MaxCoins },
    { COMPOUND_STRING("Max Battle Points"), DebugAction_Give_MaxBattlePoints },
    { COMPOUND_STRING("Daycare Egg"),       DebugAction_Give_DayCareEgg },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Player[] =
{
    { COMPOUND_STRING("Player name"),    DebugAction_Player_Name },
    { COMPOUND_STRING("Toggle gender"),  DebugAction_Player_Gender },
    { COMPOUND_STRING("New Trainer ID"), DebugAction_Player_Id },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Scripts[] =
{
    { COMPOUND_STRING("Script 1"), DebugAction_ExecuteScript, Debug_EventScript_Script_1 },
    { COMPOUND_STRING("Script 2"), DebugAction_ExecuteScript, Debug_EventScript_Script_2 },
    { COMPOUND_STRING("Script 3"), DebugAction_ExecuteScript, Debug_EventScript_Script_3 },
    { COMPOUND_STRING("Script 4"), DebugAction_ExecuteScript, Debug_EventScript_Script_4 },
    { COMPOUND_STRING("Script 5"), DebugAction_ExecuteScript, Debug_EventScript_Script_5 },
    { COMPOUND_STRING("Script 6"), DebugAction_ExecuteScript, Debug_EventScript_Script_6 },
    { COMPOUND_STRING("Script 7"), DebugAction_ExecuteScript, Debug_EventScript_Script_7 },
    { COMPOUND_STRING("Script 8"), DebugAction_ExecuteScript, Debug_EventScript_Script_8 },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Sound[] =
{
    { COMPOUND_STRING("SFX…"),   DebugAction_Sound_SE },
    { COMPOUND_STRING("Music…"), DebugAction_Sound_MUS },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_ROMInfo2[] =
{
    { COMPOUND_STRING("Save Block space"),  DebugAction_ExecuteScript, Debug_CheckSaveBlock },
    { COMPOUND_STRING("ROM space"),         DebugAction_ExecuteScript, Debug_CheckROMSpace },
    { COMPOUND_STRING("Expansion Version"), DebugAction_ExecuteScript, Debug_ShowExpansionVersion },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Flags[] =
{
    [DEBUG_FLAGVAR_MENU_ITEM_FLAGS]                = { COMPOUND_STRING("Set Flag XYZ…"),                     DebugAction_FlagsVars_Flags },
    [DEBUG_FLAGVAR_MENU_ITEM_VARS]                 = { COMPOUND_STRING("Set Var XYZ…"),                      DebugAction_FlagsVars_Vars },
    [DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_ALL]         = { COMPOUND_STRING("Pokédex Flags All"),                 DebugAction_FlagsVars_PokedexFlags_All },
    [DEBUG_FLAGVAR_MENU_ITEM_DEXFLAGS_RESET]       = { COMPOUND_STRING("Pokédex Flags Reset"),               DebugAction_FlagsVars_PokedexFlags_Reset },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKEDEX]       = { COMPOUND_STRING("Toggle {STR_VAR_1}Pokédex"),         DebugAction_ToggleFlag, DebugAction_FlagsVars_SwitchDex },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_NATDEX]        = { COMPOUND_STRING("Toggle {STR_VAR_1}National Dex"),    DebugAction_ToggleFlag, DebugAction_FlagsVars_SwitchNatDex },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKENAV]       = { COMPOUND_STRING("Toggle {STR_VAR_1}PokéNav"),         DebugAction_ToggleFlag, DebugAction_FlagsVars_SwitchPokeNav },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_MATCH_CALL]    = { COMPOUND_STRING("Toggle {STR_VAR_1}Match Call"),      DebugAction_ToggleFlag, DebugAction_FlagsVars_SwitchMatchCall },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_RUN_SHOES]     = { COMPOUND_STRING("Toggle {STR_VAR_1}Running Shoes"),   DebugAction_ToggleFlag, DebugAction_FlagsVars_RunningShoes },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BICYCLE]       = { COMPOUND_STRING("Toggle {STR_VAR_1}Bicycle"),         DebugAction_ToggleFlag, DebugAction_FlagsVars_Bicycle },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_LOCATIONS]     = { COMPOUND_STRING("Toggle {STR_VAR_1}Fly Flags"),       DebugAction_ToggleFlag, DebugAction_FlagsVars_ToggleFlyFlags },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BADGES_ALL]    = { COMPOUND_STRING("Toggle {STR_VAR_1}All badges"),      DebugAction_ToggleFlag, DebugAction_FlagsVars_ToggleBadgeFlags },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_GAME_CLEAR]    = { COMPOUND_STRING("Toggle {STR_VAR_1}Game clear"),      DebugAction_ToggleFlag, DebugAction_FlagsVars_ToggleGameClear },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_FRONTIER_PASS] = { COMPOUND_STRING("Toggle {STR_VAR_1}Frontier Pass"),   DebugAction_ToggleFlag, DebugAction_FlagsVars_ToggleFrontierPass },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_COLLISION]     = { COMPOUND_STRING("Toggle {STR_VAR_1}Collision OFF"),   DebugAction_ToggleFlag, DebugAction_FlagsVars_CollisionOnOff },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_ENCOUNTER]     = { COMPOUND_STRING("Toggle {STR_VAR_1}Encounter OFF"),   DebugAction_ToggleFlag, DebugAction_FlagsVars_EncounterOnOff },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_TRAINER_SEE]   = { COMPOUND_STRING("Toggle {STR_VAR_1}Trainer See OFF"), DebugAction_ToggleFlag, DebugAction_FlagsVars_TrainerSeeOnOff },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BAG_USE]       = { COMPOUND_STRING("Toggle {STR_VAR_1}Bag Use OFF"),     DebugAction_ToggleFlag, DebugAction_FlagsVars_BagUseOnOff },
    [DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_CATCHING]      = { COMPOUND_STRING("Toggle {STR_VAR_1}Catching OFF"),    DebugAction_ToggleFlag, DebugAction_FlagsVars_CatchingOnOff },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Main[] =
{
    { COMPOUND_STRING("Ikigai Debug…"), DebugAction_OpenSubMenu, sDebugMenu_Actions_Ikigai, },
    { COMPOUND_STRING("Utilities…"),    DebugAction_OpenSubMenu, sDebugMenu_Actions_Utilities, },
    { COMPOUND_STRING("PC/Bag…"),       DebugAction_OpenSubMenu, sDebugMenu_Actions_PCBag, },
    { COMPOUND_STRING("Party…"),        DebugAction_OpenSubMenu, sDebugMenu_Actions_Party, },
    { COMPOUND_STRING("Give X…"),       DebugAction_OpenSubMenu, sDebugMenu_Actions_Give, },
    { COMPOUND_STRING("Player…"),       DebugAction_OpenSubMenu, sDebugMenu_Actions_Player, },
    { COMPOUND_STRING("Scripts…"),      DebugAction_OpenSubMenu, sDebugMenu_Actions_Scripts, },
    { COMPOUND_STRING("Flags & Vars…"), DebugAction_OpenSubMenuFlagsVars, sDebugMenu_Actions_Flags, },
    { COMPOUND_STRING("Sound…"),        DebugAction_OpenSubMenu, sDebugMenu_Actions_Sound, },
    { COMPOUND_STRING("ROM Info…"),     DebugAction_OpenSubMenu, sDebugMenu_Actions_ROMInfo2, },
    { COMPOUND_STRING("Cancel"),        DebugAction_Cancel, },
    { NULL }
};

// *******************************
// Ikigai Debug Menu Actions
static const struct DebugMenuOption sDebugMenu_Actions_Ikigai_Player[] =
{
    { COMPOUND_STRING("Gym Type…"),             DebugAction_Ikigai_GymType },
    { COMPOUND_STRING("Toggle Player Body"),    DebugAction_Player_Gender },
    { COMPOUND_STRING("Player Name"),           DebugAction_Player_Name },
    { COMPOUND_STRING("Player Nickname"),       DebugAction_Player_Nickname },
    { COMPOUND_STRING("Player DynPals…"),       DebugAction_ExecuteScript, DynPal_OpenFullMenu },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Ikigai_Character[] =
{
    { COMPOUND_STRING("Open PokéSphere"),       DebugAction_ExecuteScript, Debug_OpenPokeSphere },
    { COMPOUND_STRING("Meet All Characters"),   DebugAction_Ikigai_MeetAllCharacter },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Ikigai_Temporal[] =
{
    { COMPOUND_STRING("Time Functions…"),   DebugAction_OpenSubMenu, sDebugMenu_Actions_TimeMenu, },
    { COMPOUND_STRING("Set Season…"),       DebugAction_Ikigai_Season },
    { COMPOUND_STRING("Show Calendar"),     DebugAction_ExecuteScript, Debug_OpenCalendar },
    { COMPOUND_STRING("Calendar Warp"),     DebugAction_Ikigai_CalendarWarp },
    { COMPOUND_STRING("Set Weather…"),      DebugAction_Util_Weather },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Ikigai_DynamicMusic_Instruments[] =
{
    { COMPOUND_STRING("Remove Instrument"),     DebugAction_DynamicMusic_InstrumentRemove },
    { COMPOUND_STRING("Restore Instrument"),    DebugAction_DynamicMusic_InstrumentRestore },
    { COMPOUND_STRING("Play Only Instrument"),  DebugAction_DynamicMusic_InstrumentPlayOnly },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks[] =
{
    { COMPOUND_STRING("Remove Track"),     DebugAction_DynamicMusic_TrackRemove },
    { COMPOUND_STRING("Restore Track"),    DebugAction_DynamicMusic_TrackRestore },
    { COMPOUND_STRING("Play Only Track"),  DebugAction_DynamicMusic_TrackPlayOnly },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Ikigai_DynamicMusic_SelectTrack[] =
{
    { COMPOUND_STRING("All Tracks…"),   DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 00…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 01…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 02…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 03…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 04…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 05…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 06…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 07…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 08…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 09…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 10…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { COMPOUND_STRING("Track 11…"),     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Tracks},
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Ikigai_DynamicMusic[] =
{
    [INSTRUMENT_ACCORDION] =    { COMPOUND_STRING("Accordian…"),                DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Instruments },
    [INSTRUMENT_KEYTAR] =       { COMPOUND_STRING("Keytar…"),                   DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Instruments },
    [INSTRUMENT_ERHU] =         { COMPOUND_STRING("Erhu…"),                     DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Instruments },
    [INSTRUMENT_GUITAR] =       { COMPOUND_STRING("Guitar…"),                   DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Instruments },
    [INSTRUMENT_WHISTLE] =      { COMPOUND_STRING("Whistle…"),                  DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Instruments },
    [INSTRUMENT_FLUTE] =        { COMPOUND_STRING("Flute…"),                    DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Instruments },
    [INSTRUMENT_ALL] =          { COMPOUND_STRING("All Instruments…"),          DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Instruments },
    [INSTRUMENT_COUNT] =        { COMPOUND_STRING("Playing Instruments…"),      DebugAction_OpenSubMenuDynamicMusic, sDebugMenu_Actions_Ikigai_DynamicMusic_Instruments },
                                { COMPOUND_STRING("Isolate Tracks…"),           DebugAction_OpenSubMenu, sDebugMenu_Actions_Ikigai_DynamicMusic_SelectTrack },
                                { COMPOUND_STRING("Dynamic Movement Music"),    DebugAction_DynamicMusic_Movement },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Ikigai_Sound[] =
{
    { COMPOUND_STRING("SFX…"),              DebugAction_Sound_SE },
    { COMPOUND_STRING("Music…"),            DebugAction_Sound_MUS },
    { COMPOUND_STRING("Music Expansion…"),  DebugAction_Ikigai_MUS_Expansion },
    { COMPOUND_STRING("Dynamic Music…"),    DebugAction_OpenSubMenu, sDebugMenu_Actions_Ikigai_DynamicMusic},
    { COMPOUND_STRING("Pokémon Cries…"),    DebugAction_Ikigai_PokemonCries },
    { NULL }
};

enum IkigaiStartMenuSubmenu
{
    DEBUG_IKIGAI_START_MENU_OPEN_DEFAULT_START,
    DEBUG_IKIGAI_START_MENU_TOGGLE_SAFARI,
    DEBUG_IKIGAI_START_MENU_TOGGLE_POKEDEX,
    DEBUG_IKIGAI_START_MENU_TOGGLE_POKEMON,
    DEBUG_IKIGAI_START_MENU_TOGGLE_POKENAV,
    DEBUG_IKIGAI_START_MENU_TOGGLE_DEXNAV,
    DEBUG_IKIGAI_START_MENU_DAYCARE,
};

static const struct DebugMenuOption sDebugMenu_Actions_Ikigai_StartMenu[] =
{
    [DEBUG_IKIGAI_START_MENU_OPEN_DEFAULT_START]    = { COMPOUND_STRING("Open Vanilla Start Menu"),     DebugAction_Ikigai_DefaultStartMenu },
    [DEBUG_IKIGAI_START_MENU_TOGGLE_SAFARI]         = { COMPOUND_STRING("Toggle {STR_VAR_1}Safari"),    DebugAction_ToggleFlag, DebugAction_Ikigai_ToggleSafariFlag },
    [DEBUG_IKIGAI_START_MENU_TOGGLE_POKEDEX]        = { COMPOUND_STRING("Toggle {STR_VAR_1}Pokédex"),   DebugAction_ToggleFlag, DebugAction_FlagsVars_SwitchDex },
    [DEBUG_IKIGAI_START_MENU_TOGGLE_POKEMON]        = { COMPOUND_STRING("Toggle {STR_VAR_1}Pokémon"),   DebugAction_ToggleFlag, DebugAction_Ikigai_TogglePokemonFlag },
    [DEBUG_IKIGAI_START_MENU_TOGGLE_POKENAV]        = { COMPOUND_STRING("Toggle {STR_VAR_1}PokeNav"),   DebugAction_ToggleFlag, DebugAction_FlagsVars_SwitchPokeNav },
    [DEBUG_IKIGAI_START_MENU_TOGGLE_DEXNAV]         = { COMPOUND_STRING("Toggle {STR_VAR_1}DexNav"),    DebugAction_ToggleFlag, DebugAction_Ikigai_ToggleDexNavFlag },
    [DEBUG_IKIGAI_START_MENU_DAYCARE]               = { COMPOUND_STRING("Talk to Daycare Woman"),       DebugAction_ExecuteScript,  Route117_PokemonDayCare_EventScript_DaycareWoman },
    { NULL }
};

static const struct DebugMenuOption sDebugMenu_Actions_Ikigai[] =
{
    { COMPOUND_STRING("Player Menu…"),      DebugAction_OpenSubMenu, sDebugMenu_Actions_Ikigai_Player },
    { COMPOUND_STRING("Character Menu…"),   DebugAction_OpenSubMenu, sDebugMenu_Actions_Ikigai_Character },
    { COMPOUND_STRING("Temporal Menu…"),    DebugAction_OpenSubMenu, sDebugMenu_Actions_Ikigai_Temporal },
    { COMPOUND_STRING("Sound Menu…"),       DebugAction_OpenSubMenu, sDebugMenu_Actions_Ikigai_Sound },
    { COMPOUND_STRING("Rotom Start Menu…"), DebugAction_OpenSubMenuRotomStartMenu, sDebugMenu_Actions_Ikigai_StartMenu },
    { NULL }
};

// *******************************
// Windows
static const struct WindowTemplate sDebugMenuWindowTemplateMain =
{
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_MAIN,
    .height = 2 * DEBUG_MENU_HEIGHT_MAIN,
    .paletteNum = 15,
    .baseBlock = 1,
};

static const struct WindowTemplate sDebugMenuWindowTemplateExtra =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_MENU_WIDTH_EXTRA - 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_EXTRA,
    .height = 2 * DEBUG_MENU_HEIGHT_EXTRA,
    .paletteNum = 15,
    .baseBlock = 1,
};

static const struct WindowTemplate sDebugMenuWindowTemplateGymType =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_MENU_WIDTH_GYM_TYPE - 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_GYM_TYPE,
    .height = 2 * DEBUG_MENU_HEIGHT_EXTRA,
    .paletteNum = 15,
    .baseBlock = 1,
};

static const struct WindowTemplate sDebugMenuWindowTemplateWeather =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_MENU_WIDTH_WEATHER - 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_WEATHER,
    .height = 2 * DEBUG_MENU_HEIGHT_WEATHER,
    .paletteNum = 15,
    .baseBlock = 1,
};

static const struct WindowTemplate sDebugMenuWindowTemplateSound =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_MENU_WIDTH_SOUND - 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_SOUND,
    .height = DEBUG_MENU_HEIGHT_SOUND,
    .paletteNum = 15,
    .baseBlock = 1,
};

static const struct WindowTemplate sDebugMenuWindowTemplateSeason =
{
    .bg = 0,
    .tilemapLeft = 30 - DEBUG_MENU_WIDTH_SEASON - 1,
    .tilemapTop = 1,
    .width = DEBUG_MENU_WIDTH_SEASON,
    .height = 2 * DEBUG_MENU_HEIGHT_SEASON,
    .paletteNum = 15,
    .baseBlock = 1,
};

static bool32 Debug_SaveCallbackMenu(struct DebugMenuOption *callbackItems);

// *******************************
// Functions universal
void Debug_ShowMainMenu(void)
{
    sDebugMenuListData = AllocZeroed(sizeof(*sDebugMenuListData));
    sDebugMenuListData->listId = 0;
    Debug_ShowMenu(DebugTask_HandleMenuInput_General, sDebugMenu_Actions_Main);
}

#define tMenuTaskId          data[0]
#define tWindowId            data[1]
#define tSubWindowId         data[2]
#define tInput               data[3]
#define tDigit               data[4]

static bool32 Debug_SaveCallbackMenu(struct DebugMenuOption *callbackItems)
{
    bool32 success = FALSE;
    for (u32 i = 0; i < DEBUG_MAX_SUB_MENU_LEVELS; i++)
    {
        if (sDebugMenuListData->subMenuItems[i] == NULL)
        {
            sDebugMenuListData->subMenuItems[i] = callbackItems;
            success = TRUE;
            break;
        }
    }
    return success;
}

static u32 Debug_RemoveCallbackMenu(void)
{
    u32 freeCallbackLevel = 0;
    for (u32 i = DEBUG_MAX_SUB_MENU_LEVELS; i > 0; i--)
    {
        if (sDebugMenuListData->subMenuItems[i - 1] != NULL)
        {
            sDebugMenuListData->subMenuItems[i - 1] = NULL;
            freeCallbackLevel = i - 1;
            break;
        }
    }
    return freeCallbackLevel;
}

static const struct DebugMenuOption *Debug_GetCurrentCallbackMenu(void)
{
    const struct DebugMenuOption *callbackItems = NULL;
    for (u32 i = DEBUG_MAX_SUB_MENU_LEVELS; i > 0; i--)
    {
        if (sDebugMenuListData->subMenuItems[i - 1] != NULL)
        {
            callbackItems = sDebugMenuListData->subMenuItems[i - 1];
            break;
        }
    }
    return callbackItems;
}

static bool32 IsSubMenuAction(const void *action)
{
    return action == DebugAction_OpenSubMenu
        || action == DebugAction_OpenSubMenuFlagsVars
        || action == DebugAction_OpenSubMenuFakeRTC
        || action == DebugAction_OpenSubMenuCreateFollowerNPC
        || action == DebugAction_OpenSubMenuDynamicMusic
        || action == DebugAction_OpenSubMenuRotomStartMenu;
}

static void Debug_ShowMenu(DebugFunc HandleInput, const struct DebugMenuOption *items)
{
    struct ListMenuTemplate menuTemplate = {0};
    u8 windowId;
    u8 menuTaskId;
    u8 inputTaskId;

    if (items != NULL)
        Debug_SaveCallbackMenu((struct DebugMenuOption *)items);
    else
        items = Debug_GetCurrentCallbackMenu();

    // create window
    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateMain);
    DrawStdWindowFrame(windowId, FALSE);

    u32 totalItems = 0;

    if (sDebugMenuListData->listId != 0)
    {
        totalItems = Debug_GenerateListMenuNames();
    }
    else
    {
        for (u32 i = 0; items[i].text != NULL; i++)
        {
            sDebugMenuListData->listItems[i].id = i;
            StringExpandPlaceholders(gStringVar4, items[i].text);
            if (IsSubMenuAction(items[i].action))
                StringAppend(gStringVar4, sDebugText_Arrow);
            StringCopy(&sDebugMenuListData->itemNames[i][0], gStringVar4);
            sDebugMenuListData->listItems[i].name = &sDebugMenuListData->itemNames[i][0];
            totalItems++;
        }
    }

    // create list menu
    menuTemplate.items = sDebugMenuListData->listItems;
    menuTemplate.moveCursorFunc = ListMenuDefaultCursorMoveFunc;
    menuTemplate.totalItems = totalItems;
    menuTemplate.maxShowed = DEBUG_MENU_HEIGHT_MAIN;
    menuTemplate.windowId = windowId;
    menuTemplate.header_X = 0;
    menuTemplate.item_X = 8;
    menuTemplate.cursor_X = 0;
    menuTemplate.upText_Y = 1;
    menuTemplate.cursorPal = 2;
    menuTemplate.fillValue = 1;
    menuTemplate.cursorShadowPal = 3;
    menuTemplate.lettersSpacing = 1;
    menuTemplate.itemVerticalPadding = 0;
    menuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    menuTemplate.fontId = DEBUG_MENU_FONT;
    menuTemplate.cursorKind = 0;
    menuTaskId = ListMenuInit(&menuTemplate, 0, 0);

    // create input handler task
    inputTaskId = CreateTask(HandleInput, 3);
    gTasks[inputTaskId].tMenuTaskId = menuTaskId;
    gTasks[inputTaskId].tWindowId = windowId;
    gTasks[inputTaskId].tSubWindowId = 0;

    Debug_RefreshListMenu(inputTaskId);

    // draw everything
    CopyWindowToVram(windowId, COPYWIN_FULL);
}

static void Debug_DestroyMenu(u8 taskId)
{
    DestroyListMenuTask(gTasks[taskId].tMenuTaskId, NULL, NULL);
    RemoveWindow(gTasks[taskId].tWindowId);
    DestroyTask(taskId);
}

static void Debug_DestroyMenu_Full(u8 taskId)
{
    if (gTasks[taskId].tSubWindowId != 0)
    {
        ClearStdWindowAndFrame(gTasks[taskId].tSubWindowId, FALSE);
        DebugAction_DestroyExtraWindow(taskId);
    }
    DestroyListMenuTask(gTasks[taskId].tMenuTaskId, NULL, NULL);
    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);
    DestroyTask(taskId);
    UnfreezeObjectEvents();
    Free(sDebugMenuListData);
}

static void Debug_DestroyMenu_Full_Script(u8 taskId, const u8 *script)
{
    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    FreezeObjectEvents();
    ScriptContext_SetupScript(script);
}

static void Debug_HandleInput_Numeric(u8 taskId, s32 min, s32 max, u32 digits)
{
    if (JOY_NEW(DPAD_UP))
    {
        gTasks[taskId].tInput += sPowersOfTen[gTasks[taskId].tDigit];
        if (gTasks[taskId].tInput > max)
            gTasks[taskId].tInput = max;
    }
    if (JOY_NEW(DPAD_DOWN))
    {
        gTasks[taskId].tInput -= sPowersOfTen[gTasks[taskId].tDigit];
        if (gTasks[taskId].tInput < min)
            gTasks[taskId].tInput = min;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (gTasks[taskId].tDigit > 0)
            gTasks[taskId].tDigit -= 1;
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (gTasks[taskId].tDigit < digits - 1)
            gTasks[taskId].tDigit += 1;
    }
}

static void DebugAction_Cancel(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

static void DebugAction_DestroyExtraWindow(u8 taskId)
{
    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    ClearStdWindowAndFrame(gTasks[taskId].tSubWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tSubWindowId);

    DestroyListMenuTask(gTasks[taskId].tMenuTaskId, NULL, NULL);
    DestroyTask(taskId);
    ScriptContext_Enable();
    UnfreezeObjectEvents();
}

static const u16 sLocationFlags[] =
{
    FLAG_VISITED_LITTLEROOT_TOWN,
    FLAG_VISITED_OLDALE_TOWN,
    FLAG_VISITED_DEWFORD_TOWN,
    FLAG_VISITED_LAVARIDGE_TOWN,
    FLAG_VISITED_FALLARBOR_TOWN,
    FLAG_VISITED_VERDANTURF_TOWN,
    FLAG_VISITED_PACIFIDLOG_TOWN,
    FLAG_VISITED_PETALBURG_CITY,
    FLAG_VISITED_SLATEPORT_CITY,
    FLAG_VISITED_MAUVILLE_CITY,
    FLAG_VISITED_RUSTBORO_CITY,
    FLAG_VISITED_FORTREE_CITY,
    FLAG_VISITED_LILYCOVE_CITY,
    FLAG_VISITED_MOSSDEEP_CITY,
    FLAG_VISITED_SOOTOPOLIS_CITY,
    FLAG_VISITED_EVER_GRANDE_CITY,
    FLAG_LANDMARK_POKEMON_LEAGUE,
    FLAG_LANDMARK_BATTLE_FRONTIER,
};

static u8 Debug_CheckToggleFlags(u8 id)
{
    u8 result = FALSE;

    switch (id)
    {
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKEDEX:
            result = FlagGet(FLAG_SYS_POKEDEX_GET);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_NATDEX:
            result = IsNationalPokedexEnabled();
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_POKENAV:
            result = FlagGet(FLAG_SYS_POKENAV_GET);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_MATCH_CALL:
            result = FlagGet(FLAG_ADDED_MATCH_CALL_TO_POKENAV) && FlagGet(FLAG_HAS_MATCH_CALL);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_RUN_SHOES:
            result = FlagGet(FLAG_SYS_B_DASH);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BICYCLE:
            result = FlagGet(FLAG_RECEIVED_BIKE);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_LOCATIONS:
            result = TRUE;
            for (u32 i = 0; i < ARRAY_COUNT(sLocationFlags); i++)
            {
                if (!FlagGet(sLocationFlags[i]))
                {
                    result = FALSE;
                    break;
                }
            }
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BADGES_ALL:
            result = TRUE;
            for (u32 i = 0; i < ARRAY_COUNT(gBadgeFlags); i++)
            {
                if (!FlagGet(gBadgeFlags[i]))
                {
                    result = FALSE;
                    break;
                }
            }
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_GAME_CLEAR:
            result = FlagGet(FLAG_SYS_GAME_CLEAR);
            break;
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_FRONTIER_PASS:
            result = FlagGet(FLAG_SYS_FRONTIER_PASS);
            break;
    #if OW_FLAG_NO_COLLISION != 0
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_COLLISION:
            result = FlagGet(OW_FLAG_NO_COLLISION);
            break;
    #endif
    #if OW_FLAG_NO_ENCOUNTER != 0
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_ENCOUNTER:
            result = FlagGet(OW_FLAG_NO_ENCOUNTER);
            break;
    #endif
    #if OW_FLAG_NO_TRAINER_SEE != 0
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_TRAINER_SEE:
            result = FlagGet(OW_FLAG_NO_TRAINER_SEE);
            break;
    #endif
    #if B_FLAG_NO_BAG_USE != 0
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_BAG_USE:
            result = FlagGet(B_FLAG_NO_BAG_USE);
            break;
    #endif
    #if B_FLAG_NO_CATCHING != 0
        case DEBUG_FLAGVAR_MENU_ITEM_TOGGLE_CATCHING:
            result = FlagGet(B_FLAG_NO_CATCHING);
            break;
    #endif
        default:
            result = 0xFF;
            break;
    }

    return result;
}

static u8 Debug_RotomPhoneFlags(u8 id)
{
    u8 result = FALSE;

    switch (id)
    {
    case DEBUG_IKIGAI_START_MENU_TOGGLE_POKEDEX:
        result = FlagGet(FLAG_SYS_POKEDEX_GET);
        break;
    
    case DEBUG_IKIGAI_START_MENU_TOGGLE_POKEMON:
        result = FlagGet(FLAG_SYS_POKEMON_GET);
        break;
    
    case DEBUG_IKIGAI_START_MENU_TOGGLE_POKENAV:
        result = FlagGet(FLAG_SYS_POKENAV_GET);
        break;
    
    case DEBUG_IKIGAI_START_MENU_TOGGLE_SAFARI:
        result = GetSafariZoneFlag();
        break;

    case DEBUG_IKIGAI_START_MENU_TOGGLE_DEXNAV:
        result = FlagGet(DN_FLAG_DEXNAV_GET);
        break;
    
    default:
        result = 0xFF;
        break;
    }

    return result;
}

static u8 Debug_GenerateListMenuNames(void)
{
    const u8 sColor_Red[] = _("{COLOR RED}");
    const u8 sColor_Green[] = _("{COLOR GREEN}");
    u32 i, flagResult = 0;
    u8 const *name = NULL;

    u8 totalItems = 0;
    if (sDebugMenuListData->listId == 1)
        // Failsafe to prevent memory corruption
        totalItems = min(ARRAY_COUNT(sDebugMenu_Actions_Flags) - 1, DEBUG_MAX_MENU_ITEMS);
    else if (sDebugMenuListData->listId == 2)
        // Failsafe to prevent memory corruption
        totalItems = min(ARRAY_COUNT(sDebugMenu_Actions_Ikigai_StartMenu) - 1, DEBUG_MAX_MENU_ITEMS);

    // Copy item names for all entries but the last (which is Cancel)
    for (i = 0; i < totalItems; i++)
    {
        if (sDebugMenuListData->listId == 1)
        {
            flagResult = Debug_CheckToggleFlags(i);
            name = sDebugMenu_Actions_Flags[i].text;
        }
        else if (sDebugMenuListData->listId == 2)
        {
            // if (i >= sDebugMenu_ListTemplate_Ikigai_StartMenu.totalItems)
            //     return; // Idk why the complier needs this.
            flagResult = Debug_RotomPhoneFlags(i);
            name = sDebugMenu_Actions_Ikigai_StartMenu[i].text;
        }

        if (flagResult == 0xFF)
        {
            StringCopy(&sDebugMenuListData->itemNames[i][0], name);
        }
        else if (flagResult)
        {
            StringCopy(gStringVar1, sColor_Green);
            StringExpandPlaceholders(gStringVar4, name);
            StringCopy(&sDebugMenuListData->itemNames[i][0], gStringVar4);
        }
        else
        {
            StringCopy(gStringVar1, sColor_Red);
            StringExpandPlaceholders(gStringVar4, name);
            StringCopy(&sDebugMenuListData->itemNames[i][0], gStringVar4);
        }

        sDebugMenuListData->listItems[i].name = &sDebugMenuListData->itemNames[i][0];
        sDebugMenuListData->listItems[i].id = i;
    }
    return totalItems;
}

static void Debug_RefreshListMenu(u8 taskId)
{
    u8 totalItems = Debug_GenerateListMenuNames();

    // Set list menu data
    gMultiuseListMenuTemplate.items = sDebugMenuListData->listItems;
    gMultiuseListMenuTemplate.totalItems = totalItems;
    gMultiuseListMenuTemplate.maxShowed = DEBUG_MENU_HEIGHT_MAIN;
    gMultiuseListMenuTemplate.windowId = gTasks[taskId].tWindowId;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.upText_Y = 1;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 1;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.lettersSpacing = 1;
    gMultiuseListMenuTemplate.itemVerticalPadding = 0;
    gMultiuseListMenuTemplate.scrollMultiple = LIST_NO_MULTIPLE_SCROLL;
    gMultiuseListMenuTemplate.fontId = 1;
    gMultiuseListMenuTemplate.cursorKind = 0;
}

static void DebugTask_HandleMenuInput_General(u8 taskId)
{
    const struct DebugMenuOption *options = Debug_GetCurrentCallbackMenu();
    u32 input = ListMenu_ProcessInput(gTasks[taskId].tMenuTaskId);
    struct DebugMenuOption option = options[input];

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (option.action != NULL)
        {
            if (IsSubMenuAction(option.action))
            {
                ((DebugSubmenuFunc)option.action)(taskId, option.actionParams);
            }
            else if (option.action == DebugAction_ExecuteScript)
            {
                Debug_DestroyMenu_Full_Script(taskId, (const u8 *)option.actionParams);
            }
            else if (option.action == DebugAction_ToggleFlag)
            {
                ((DebugFunc)option.actionParams)(taskId);
                DebugAction_ToggleFlag(taskId);
            }
            else
            {
                ((DebugFunc)option.action)(taskId);
            }
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        if (Debug_GetCurrentCallbackMenu() != NULL && Debug_RemoveCallbackMenu() != 0)
        {
            Debug_DestroyMenu(taskId);
            if (sDebugMenuListData->listId != 0)
                sDebugMenuListData->listId = 0;
            Debug_ShowMenu(DebugTask_HandleMenuInput_General, NULL);
        }
        else
        {
            Debug_DestroyMenu_Full(taskId);
            ScriptContext_Enable();
        }
    }
}

static void DebugAction_OpenSubMenuFlagsVars(u8 taskId, const struct DebugMenuOption *items)
{
    Debug_DestroyMenu(taskId);
    sDebugMenuListData->listId = 1;
    Debug_ShowMenu(DebugTask_HandleMenuInput_General, items);
}

static void DebugAction_OpenSubMenu(u8 taskId, const struct DebugMenuOption *items)
{
    Debug_DestroyMenu(taskId);
    sDebugMenuListData->listId = 0;
    Debug_ShowMenu(DebugTask_HandleMenuInput_General, items);
}

static void DebugAction_OpenSubMenuFakeRTC(u8 taskId, const struct DebugMenuOption *items)
{
    if (!OW_USE_FAKE_RTC)
    {
        Debug_DestroyMenu_Full_Script(taskId, Debug_EventScript_FakeRTCNotEnabled);
    }
    else
    {
        Debug_DestroyMenu(taskId);
        Debug_ShowMenu(DebugTask_HandleMenuInput_General, items);
    }
}

static void DebugAction_ExecuteScript(u8 taskId, const u8 *script)
{
    Debug_DestroyMenu_Full_Script(taskId, script);
}

static void DebugAction_ToggleFlag(u8 taskId)
{
    Debug_GenerateListMenuNames();
    RedrawListMenu(gTasks[taskId].tMenuTaskId);
}

static void DebugAction_OpenSubMenuCreateFollowerNPC(u8 taskId, const struct DebugMenuOption *items)
{
    if (FNPC_ENABLE_NPC_FOLLOWERS)
    {
        Debug_DestroyMenu(taskId);
        Debug_ShowMenu(DebugTask_HandleMenuInput_General, items);
    }
    else
    {
        Debug_DestroyMenu_Full_Script(taskId, Debug_Follower_NPC_Not_Enabled);
    }
}

// *******************************
// Actions Utilities

static void DebugAction_Util_Fly(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    SetMainCallback2(CB2_OpenFlyMap);
}

#define tMapGroup  data[5]
#define tMapNum    data[6]
#define tWarp      data[7]

#define LAST_MAP_GROUP (MAP_GROUPS_COUNT - 1)

static void DebugAction_Util_Warp_Warp(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, 3);
    ConvertIntToDecimalStringN(gStringVar2, LAST_MAP_GROUP, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
    StringCopy(gStringVar3, gText_DigitIndicator[0]);
    StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMapGroup);
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

    gTasks[taskId].func = DebugAction_Util_Warp_SelectMapGroup;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = 0;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tMapGroup = 0;
    gTasks[taskId].tMapNum = 0;
    gTasks[taskId].tWarp = 0;
}

static void DebugAction_Util_Warp_SelectMapGroup(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 0, LAST_MAP_GROUP, 3);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, 3);
        ConvertIntToDecimalStringN(gStringVar2, LAST_MAP_GROUP, STR_CONV_MODE_LEADING_ZEROS, 3);
        StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].tDigit]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMapGroup);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].tMapGroup = gTasks[taskId].tInput;
        gTasks[taskId].tInput = 0;
        gTasks[taskId].tDigit = 0;

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, (MAP_GROUP_COUNT[gTasks[taskId].tMapGroup] - 1 >= 100) ? 3 : 2);
        ConvertIntToDecimalStringN(gStringVar2, MAP_GROUP_COUNT[gTasks[taskId].tMapGroup] - 1, STR_CONV_MODE_LEADING_ZEROS, (MAP_GROUP_COUNT[gTasks[taskId].tMapGroup] - 1 >= 100) ? 3 : 2);
        StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
        GetMapName(gStringVar2, Overworld_GetMapHeaderByGroupAndId(gTasks[taskId].tMapGroup, gTasks[taskId].tInput)->regionMapSectionId, 0);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].tDigit]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMap);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

        gTasks[taskId].func = DebugAction_Util_Warp_SelectMap;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Util_Warp_SelectMap(u8 taskId)
{
    u8 max_value = MAP_GROUP_COUNT[gTasks[taskId].tMapGroup]; //maps in the selected map group

    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 0, max_value - 1, 3);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, (max_value >= 100) ? 3 : 2);
        ConvertIntToDecimalStringN(gStringVar2, MAP_GROUP_COUNT[gTasks[taskId].tMapGroup] - 1, STR_CONV_MODE_LEADING_ZEROS, (max_value >= 100) ? 3 : 2);
        StringExpandPlaceholders(gStringVar1, sDebugText_Util_WarpToMap_SelMax);
        GetMapName(gStringVar2, Overworld_GetMapHeaderByGroupAndId(gTasks[taskId].tMapGroup, gTasks[taskId].tInput)->regionMapSectionId, 0);
        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].tDigit]);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectMap);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].tMapNum = gTasks[taskId].tInput;
        gTasks[taskId].tInput = 0;
        gTasks[taskId].tDigit = 0;

        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].tDigit]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, 3);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectWarp);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
        gTasks[taskId].func = DebugAction_Util_Warp_SelectWarp;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Util_Warp_SelectWarp(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].tInput += sPowersOfTen[gTasks[taskId].tDigit];
            if (gTasks[taskId].tInput > 10)
                gTasks[taskId].tInput = 10;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].tInput -= sPowersOfTen[gTasks[taskId].tDigit];
            if (gTasks[taskId].tInput < 0)
                gTasks[taskId].tInput = 0;
        }

        StringCopy(gStringVar3, gText_DigitIndicator[gTasks[taskId].tDigit]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, 3);
        StringExpandPlaceholders(gStringVar4, sDebugText_Util_WarpToMap_SelectWarp);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].tWarp = gTasks[taskId].tInput;
        //If there's no warp with the number available, warp to the center of the map.
        SetWarpDestinationToMapWarp(gTasks[taskId].tMapGroup, gTasks[taskId].tMapNum, gTasks[taskId].tWarp);
        DoWarp();
        ResetInitialPlayerAvatarState();
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

#undef tMapGroup
#undef tMapNum
#undef tWarp

void CheckSaveBlock1Size(struct ScriptContext *ctx)
{
    u32 currSb1Size = sizeof(struct SaveBlock1);
    u32 maxSb1Size = SECTOR_DATA_SIZE * (SECTOR_ID_SAVEBLOCK1_END - SECTOR_ID_SAVEBLOCK1_START + 1);
    ConvertIntToDecimalStringN(gStringVar1, currSb1Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar2, maxSb1Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, maxSb1Size - currSb1Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar4, 1, STR_CONV_MODE_LEFT_ALIGN, 6);
}

void CheckSaveBlock2Size(struct ScriptContext *ctx)
{
    u32 currSb2Size = (sizeof(struct SaveBlock2));
    u32 maxSb2Size = SECTOR_DATA_SIZE;
    ConvertIntToDecimalStringN(gStringVar1, currSb2Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar2, maxSb2Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, maxSb2Size - currSb2Size, STR_CONV_MODE_LEFT_ALIGN, 6);
}

void CheckSaveBlock3Size(struct ScriptContext *ctx)
{
    u32 currSb3Size = (sizeof(struct SaveBlock3));
    u32 maxSb3Size = SAVE_BLOCK_3_CHUNK_SIZE * NUM_SECTORS_PER_SLOT;
    ConvertIntToDecimalStringN(gStringVar1, currSb3Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar2, maxSb3Size, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, maxSb3Size - currSb3Size, STR_CONV_MODE_LEFT_ALIGN, 6);
}

void CheckPokemonStorageSize(struct ScriptContext *ctx)
{
    u32 currPkmnStorageSize = sizeof(struct PokemonStorage);
    u32 maxPkmnStorageSize = SECTOR_DATA_SIZE * (SECTOR_ID_PKMN_STORAGE_END - SECTOR_ID_PKMN_STORAGE_START + 1);
    ConvertIntToDecimalStringN(gStringVar1, currPkmnStorageSize, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar2, maxPkmnStorageSize, STR_CONV_MODE_LEFT_ALIGN, 6);
    ConvertIntToDecimalStringN(gStringVar3, maxPkmnStorageSize - currPkmnStorageSize, STR_CONV_MODE_LEFT_ALIGN, 6);
}

enum RoundMode
{
    ROUND_CEILING,
    ROUND_NEAREST,
    ROUND_FLOOR,
};

static u8 *ConvertQ22_10ToDecimalString(u8 *string, u32 q22_10, u32 decimalDigits, enum RoundMode roundMode)
{
    string = ConvertIntToDecimalStringN(string, q22_10 >> 10, STR_CONV_MODE_LEFT_ALIGN, 10);

    if (decimalDigits == 0)
        return string;

    *string++ = CHAR_PERIOD;

    q22_10 &= (1 << 10) - 1;
    while (decimalDigits-- > 1)
    {
        q22_10 *= 10;
        *string++ = CHAR_0 + (q22_10 >> 10);
        q22_10 &= (1 << 10) - 1;
    }

    q22_10 *= 10;
    switch (roundMode)
    {
    case ROUND_CEILING: q22_10 += (1 << 10) - 1; break;
    case ROUND_NEAREST: q22_10 += 1 << (10 - 1); break;
    case ROUND_FLOOR:                            break;
    }
    *string++ = CHAR_0 + (q22_10 >> 10);

    *string++ = EOS;

    return string;
}

void CheckROMSize(struct ScriptContext *ctx)
{
    extern u8 __rom_end[];
    u32 currROMSizeB = __rom_end - (const u8 *)ROM_START;
    u32 currROMSizeKB = (currROMSizeB + 1023) / 1024;
    u32 currROMFreeKB = ((const u8 *)ROM_END - __rom_end) / 1024;
    ConvertQ22_10ToDecimalString(gStringVar1, currROMSizeKB, 2, ROUND_CEILING);
    ConvertQ22_10ToDecimalString(gStringVar2, currROMFreeKB, 2, ROUND_FLOOR);
}

static void DebugAction_Util_Weather(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateWeather);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopyPadded(gStringVar1, GetWeatherName(0), CHAR_SPACE, 30);
    StringExpandPlaceholders(gStringVar4, sDebugText_Util_Weather_ID);
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

    gTasks[taskId].func = DebugAction_Util_Weather_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = 0;
    gTasks[taskId].tDigit = 0;
}

static void DebugAction_Util_Weather_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, WEATHER_NONE, WEATHER_COUNT - 1, 3);

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].tDigit]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, 2);

        StringCopyPadded(gStringVar1, GetWeatherName(gTasks[taskId].tInput), CHAR_SPACE, 30);

        StringExpandPlaceholders(gStringVar4, sDebugText_Util_Weather_ID);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].tInput <= 14 || gTasks[taskId].tInput >= 20)
        {
            gTasks[taskId].data[5] = gTasks[taskId].tInput;
            SetWeather(gTasks[taskId].data[5]);
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Util_WatchCredits(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    SetMainCallback2(CB2_StartCreditsSequence);
}

static void DebugAction_Player_Name(u8 taskId)
{
    DoNamingScreen(NAMING_SCREEN_PLAYER, gSaveBlock2Ptr->playerName, gSaveBlock2Ptr->playerGender, 0, 0, CB2_ReturnToFieldContinueScript);
}

static void DebugAction_Player_Gender(u8 taskId)
{
    if (gSaveBlock2Ptr->playerGender == MALE)
        gSaveBlock2Ptr->playerGender = FEMALE;
    else
        gSaveBlock2Ptr->playerGender = MALE;
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

static void DebugAction_Player_Id(u8 taskId)
{
    u32 trainerId = Random32();
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

static void DebugAction_Util_CheatStart(u8 taskId)
{
    if (!FlagGet(FLAG_SYS_CLOCK_SET))
        RtcInitLocalTimeOffset(0, 0);

    InitTimeBasedEvents();
    Debug_DestroyMenu_Full_Script(taskId, Debug_CheatStart);
}

void BufferExpansionVersion(struct ScriptContext *ctx)
{
    static const u8 sText_Released[] = _("\nRelease Build");
    static const u8 sText_Unreleased[] = _("\nDevelopment Build");
    u8 *string = gStringVar1;
    *string++ = CHAR_v;
    string = ConvertIntToDecimalStringN(string, EXPANSION_VERSION_MAJOR, STR_CONV_MODE_LEFT_ALIGN, 3);
    *string++ = CHAR_PERIOD;
    string = ConvertIntToDecimalStringN(string, EXPANSION_VERSION_MINOR, STR_CONV_MODE_LEFT_ALIGN, 3);
    *string++ = CHAR_PERIOD;
    string = ConvertIntToDecimalStringN(string, EXPANSION_VERSION_PATCH, STR_CONV_MODE_LEFT_ALIGN, 3);
    if (EXPANSION_TAGGED_RELEASE)
        string = StringCopy(string, sText_Released);
    else
        string = StringCopy(string, sText_Unreleased);
}

void DebugMenu_CalculateTime(struct ScriptContext *ctx)
{
    if (OW_USE_FAKE_RTC)
    {
        struct SiiRtcInfo *rtc = FakeRtc_GetCurrentTime();
        StringExpandPlaceholders(gStringVar1, gDayNameStringsTable[rtc->dayOfWeek]);
        ConvertIntToDecimalStringN(gStringVar2, rtc->hour, STR_CONV_MODE_LEFT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar3, rtc->minute, STR_CONV_MODE_LEADING_ZEROS, 2);
    }
    else
    {
        u32 day = ((gLocalTime.days - 1) + 6) % 7 ;
        RtcCalcLocalTime();
        StringExpandPlaceholders(gStringVar1, gDayNameStringsTable[day]);
        ConvertIntToDecimalStringN(gStringVar2, gLocalTime.hours, STR_CONV_MODE_LEFT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar3, gLocalTime.minutes, STR_CONV_MODE_LEADING_ZEROS, 2);
    }
}

void DebugMenu_CalculateTimeOfDay(struct ScriptContext *ctx)
{
    enum TimeOfDay timeOfDay = GetTimeOfDay();
    switch (timeOfDay)
    {
        case TIME_MORNING:
        case TIME_DAY:
        case TIME_EVENING:
        case TIME_NIGHT:
            StringExpandPlaceholders(gStringVar1, gTimeOfDayStringsTable[timeOfDay]);
            break;
        case TIMES_OF_DAY_COUNT:
            break;
    }
}

// *******************************
// Actions Flags and Vars
static void Debug_Display_FlagInfo(u32 flag, u32 digit, u8 windowId)
{
    ConvertIntToDecimalStringN(gStringVar1, flag, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_FLAGS);
    ConvertIntToHexStringN(gStringVar2, flag, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar1, COMPOUND_STRING("{STR_VAR_1}{CLEAR_TO 90}\n0x{STR_VAR_2}{CLEAR_TO 90}"));
    if (FlagGet(flag))
        StringCopyPadded(gStringVar2, sDebugText_True, CHAR_SPACE, 15);
    else
        StringCopyPadded(gStringVar2, sDebugText_False, CHAR_SPACE, 15);
    StringCopy(gStringVar3, gText_DigitIndicator[digit]);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Flag: {STR_VAR_1}{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}\n{STR_VAR_3}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_FlagsVars_Flags(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial flag
    Debug_Display_FlagInfo(FLAG_TEMP_1, 0, windowId);

    gTasks[taskId].func = DebugAction_FlagsVars_FlagsSelect;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = FLAG_TEMP_1;
    gTasks[taskId].tDigit = 0;
}

static void DebugAction_FlagsVars_FlagsSelect(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        FlagToggle(gTasks[taskId].tInput);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        return;
    }

    Debug_HandleInput_Numeric(taskId, 1, FLAGS_COUNT - 1, DEBUG_NUMBER_DIGITS_FLAGS);

    if (JOY_NEW(DPAD_ANY) || JOY_NEW(A_BUTTON))
    {
        Debug_Display_FlagInfo(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }
}

#define tVarValue  data[5]

static void DebugAction_FlagsVars_Vars(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial var
    ConvertIntToDecimalStringN(gStringVar1, VARS_START, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
    ConvertIntToHexStringN(gStringVar2, VARS_START, STR_CONV_MODE_LEFT_ALIGN, 4);
    StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_VariableHex);
    ConvertIntToDecimalStringN(gStringVar3, 0, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
    StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_Variable);
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

    gTasks[taskId].func = DebugAction_FlagsVars_Select;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = VARS_START;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tVarValue = 0;
}

static void DebugAction_FlagsVars_Select(u8 taskId)
{
    Debug_HandleInput_Numeric(taskId, VARS_START, VARS_END, DEBUG_NUMBER_DIGITS_VARIABLES);

    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].tInput, STR_CONV_MODE_LEFT_ALIGN, 4);
        StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_VariableHex);
        if (VarGetIfExist(gTasks[taskId].tInput) == 0xFFFF)
            gTasks[taskId].tVarValue = 0;
        else
            gTasks[taskId].tVarValue = VarGet(gTasks[taskId].tInput);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].tVarValue, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].tDigit]);

        //Combine str's to full window string
        StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_Variable);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].tDigit = 0;

        PlaySE(SE_SELECT);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].tInput, STR_CONV_MODE_LEFT_ALIGN, 4);
        StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_VariableHex);
        if (VarGetIfExist(gTasks[taskId].tInput) == 0xFFFF)
            gTasks[taskId].tVarValue = 0;
        else
            gTasks[taskId].tVarValue = VarGet(gTasks[taskId].tInput);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].tVarValue, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].tDigit]);
        StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_VariableValueSet);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

        gTasks[taskId].data[6] = gTasks[taskId].data[5]; //New value selector
        gTasks[taskId].func = DebugAction_FlagsVars_SetValue;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        return;
    }
}

static void DebugAction_FlagsVars_SetValue(u8 taskId)
{
    if (JOY_NEW(DPAD_UP))
    {
        if (gTasks[taskId].data[6] + sPowersOfTen[gTasks[taskId].tDigit] <= 32000)
            gTasks[taskId].data[6] += sPowersOfTen[gTasks[taskId].tDigit];
        else
            gTasks[taskId].data[6] = 32000 - 1;

        if (gTasks[taskId].data[6] >= 32000)
            gTasks[taskId].data[6] = 32000 - 1;
    }
    if (JOY_NEW(DPAD_DOWN))
    {
        gTasks[taskId].data[6] -= sPowersOfTen[gTasks[taskId].tDigit];
        if (gTasks[taskId].data[6] < 0)
            gTasks[taskId].data[6] = 0;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        gTasks[taskId].tDigit -= 1;
        if (gTasks[taskId].tDigit < 0)
            gTasks[taskId].tDigit = 0;
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        gTasks[taskId].tDigit += 1;
        if (gTasks[taskId].tDigit > 4)
            gTasks[taskId].tDigit = 4;
    }

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        VarSet(gTasks[taskId].tInput, gTasks[taskId].data[6]);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        return;
    }

    if (JOY_NEW(DPAD_ANY) || JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);

        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        ConvertIntToHexStringN(gStringVar2, gTasks[taskId].tInput, STR_CONV_MODE_LEFT_ALIGN, 4);
        StringExpandPlaceholders(gStringVar1, sDebugText_FlagsVars_VariableHex);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].data[6], STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_VARIABLES);
        StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].tDigit]);
        StringExpandPlaceholders(gStringVar4, sDebugText_FlagsVars_VariableValueSet);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }
}

#undef tVarValue

static void DebugAction_FlagsVars_PokedexFlags_All(u8 taskId)
{
    u16 i;
    for (i = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        GetSetPokedexFlag(i + 1, FLAG_SET_CAUGHT);
        GetSetPokedexFlag(i + 1, FLAG_SET_SEEN);
    }
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

static void DebugAction_FlagsVars_PokedexFlags_Reset(u8 taskId)
{
    int boxId, boxPosition, partyId;
    u16 species;

    // Reset Pokedex to emtpy
    memset(&gSaveBlock1Ptr->dexCaught, 0, sizeof(gSaveBlock1Ptr->dexCaught));
    memset(&gSaveBlock1Ptr->dexSeen, 0, sizeof(gSaveBlock1Ptr->dexSeen));

    // Add party Pokemon to Pokedex
    for (partyId = 0; partyId < PARTY_SIZE; partyId++)
    {
        if (GetMonData(&gPlayerParty[partyId], MON_DATA_SANITY_HAS_SPECIES))
        {
            species = GetMonData(&gPlayerParty[partyId], MON_DATA_SPECIES);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT);
            GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_SEEN);
        }
    }

    // Add box Pokemon to Pokedex
    for (boxId = 0; boxId < TOTAL_BOXES_COUNT; boxId++)
    {
        for (boxPosition = 0; boxPosition < IN_BOX_COUNT; boxPosition++)
        {
            if (GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_SANITY_HAS_SPECIES))
            {
                species = GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_SPECIES);
                GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT);
                GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_SEEN);
            }
        }
    }
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

static void DebugAction_FlagsVars_SwitchDex(u8 taskId)
{
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_POKEDEX_GET);
}

static void DebugAction_FlagsVars_SwitchNatDex(u8 taskId)
{
    if (IsNationalPokedexEnabled())
    {
        DisableNationalPokedex();
        PlaySE(SE_PC_OFF);
    }
    else
    {
        EnableNationalPokedex();
        PlaySE(SE_PC_LOGIN);
    }
}

static void DebugAction_FlagsVars_SwitchPokeNav(u8 taskId)
{
    if (FlagGet(FLAG_SYS_POKENAV_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_POKENAV_GET);
}

static void DebugAction_FlagsVars_SwitchMatchCall(u8 taskId)
{
    if (FlagGet(FLAG_ADDED_MATCH_CALL_TO_POKENAV))
    {
        PlaySE(SE_PC_OFF);
        FlagClear(FLAG_ADDED_MATCH_CALL_TO_POKENAV);
        FlagClear(FLAG_HAS_MATCH_CALL);
    }
    else
    {
        PlaySE(SE_PC_LOGIN);
        FlagSet(FLAG_ADDED_MATCH_CALL_TO_POKENAV);
        FlagSet(FLAG_HAS_MATCH_CALL);
    }
}

static void DebugAction_FlagsVars_RunningShoes(u8 taskId)
{
    if (FlagGet(FLAG_SYS_B_DASH))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_B_DASH);
}

static void DebugAction_FlagsVars_Bicycle(u8 taskId)
{
    if (FlagGet(FLAG_RECEIVED_BIKE))
    {
        PlaySE(SE_PC_OFF);
        FlagClear(FLAG_RECEIVED_BIKE);
    }
    else
    {
        PlaySE(SE_PC_LOGIN);
        if (GetFreeSpaceForItemInBag(ITEM_BICYCLE) >= 1 && CheckBagHasItem(ITEM_BICYCLE, 0))
            AddBagItem(ITEM_BICYCLE, 1);
        FlagSet(FLAG_RECEIVED_BIKE);
    }
}

static void DebugAction_FlagsVars_ToggleFlyFlags(u8 taskId)
{
    if (FlagGet(sLocationFlags[ARRAY_COUNT(sLocationFlags) - 1]))
    {
        PlaySE(SE_PC_OFF);
        for (u32 i = 0; i < ARRAY_COUNT(sLocationFlags); i++)
            FlagClear(sLocationFlags[i]);
    }
    else
    {
        PlaySE(SE_PC_LOGIN);
        for (u32 i = 0; i < ARRAY_COUNT(sLocationFlags); i++)
            FlagSet(sLocationFlags[i]);
    }
}

static void DebugAction_FlagsVars_ToggleBadgeFlags(u8 taskId)
{
    if (FlagGet(gBadgeFlags[ARRAY_COUNT(gBadgeFlags) - 1]))
    {
        PlaySE(SE_PC_OFF);
        for (u32 i = 0; i < ARRAY_COUNT(gBadgeFlags); i++)
            FlagClear(gBadgeFlags[i]);
    }
    else
    {
        PlaySE(SE_PC_LOGIN);
        for (u32 i = 0; i < ARRAY_COUNT(gBadgeFlags); i++)
            FlagSet(gBadgeFlags[i]);
    }
}

static void DebugAction_FlagsVars_ToggleGameClear(u8 taskId)
{
    // Sound effect
    if (FlagGet(FLAG_SYS_GAME_CLEAR))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_GAME_CLEAR);
}

static void DebugAction_FlagsVars_ToggleFrontierPass(u8 taskId)
{
    // Sound effect
    if (FlagGet(FLAG_SYS_FRONTIER_PASS))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_FRONTIER_PASS);
}

static void DebugAction_FlagsVars_CollisionOnOff(u8 taskId)
{
#if OW_FLAG_NO_COLLISION == 0
    Debug_DestroyMenu_Full_Script(taskId, Debug_FlagsNotSetOverworldConfigMessage);
#else
    if (FlagGet(OW_FLAG_NO_COLLISION))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(OW_FLAG_NO_COLLISION);
#endif
}

static void DebugAction_FlagsVars_EncounterOnOff(u8 taskId)
{
#if OW_FLAG_NO_ENCOUNTER == 0
    Debug_DestroyMenu_Full_Script(taskId, Debug_FlagsNotSetOverworldConfigMessage);
#else
    if (FlagGet(OW_FLAG_NO_ENCOUNTER))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(OW_FLAG_NO_ENCOUNTER);
#endif
}

static void DebugAction_FlagsVars_TrainerSeeOnOff(u8 taskId)
{
#if OW_FLAG_NO_TRAINER_SEE == 0
    Debug_DestroyMenu_Full_Script(taskId, Debug_FlagsNotSetOverworldConfigMessage);
#else
    if (FlagGet(OW_FLAG_NO_TRAINER_SEE))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(OW_FLAG_NO_TRAINER_SEE);
#endif
}

static void DebugAction_FlagsVars_BagUseOnOff(u8 taskId)
{
#if B_FLAG_NO_BAG_USE == 0
    Debug_DestroyMenu_Full_Script(taskId, Debug_FlagsNotSetBattleConfigMessage);
#else
    if (FlagGet(B_FLAG_NO_BAG_USE))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(B_FLAG_NO_BAG_USE);
#endif
}

static void DebugAction_FlagsVars_CatchingOnOff(u8 taskId)
{
#if B_FLAG_NO_CATCHING == 0
    Debug_DestroyMenu_Full_Script(taskId, Debug_FlagsNotSetBattleConfigMessage);
#else
    if (FlagGet(B_FLAG_NO_CATCHING))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(B_FLAG_NO_CATCHING);
#endif
}

// *******************************
// Actions Give
#define ITEM_TAG 0xFDF3
#define tItemId    data[5]
#define tSpriteId  data[6]

static void Debug_Display_ItemInfo(u32 itemId, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    u8* end = CopyItemName(itemId, gStringVar1);
    WrapFontIdToFit(gStringVar1, end, DEBUG_MENU_FONT, WindowWidthPx(windowId));
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    ConvertIntToDecimalStringN(gStringVar3, itemId, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Item ID: {STR_VAR_3}\n{STR_VAR_1}{CLEAR_TO 90}\n\n{STR_VAR_2}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Item(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial item
    Debug_Display_ItemInfo(1, 0, windowId);

    gTasks[taskId].func = DebugAction_Give_Item_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = 1;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gTasks[taskId].tInput);
    gSprites[gTasks[taskId].tSpriteId].x2 = DEBUG_NUMBER_ICON_X+10;
    gSprites[gTasks[taskId].tSpriteId].y2 = DEBUG_NUMBER_ICON_Y+10;
    gSprites[gTasks[taskId].tSpriteId].oam.priority = 0;
}

static void DestroyItemIcon(u8 taskId)
{
    FreeSpriteTilesByTag(ITEM_TAG);
    FreeSpritePaletteByTag(ITEM_TAG);
    FreeSpriteOamMatrix(&gSprites[gTasks[taskId].tSpriteId]);
    DestroySprite(&gSprites[gTasks[taskId].tSpriteId]);
}

static void Debug_Display_ItemQuantity(u32 quantity, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    ConvertIntToDecimalStringN(gStringVar1, quantity, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEM_QUANTITY);
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Quantity:{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n\n{STR_VAR_2}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Item_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 1, ITEMS_COUNT - 1, DEBUG_NUMBER_DIGITS_ITEMS);
        Debug_Display_ItemInfo(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
        DestroyItemIcon(taskId);
        gTasks[taskId].tSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, gTasks[taskId].tInput);
        gSprites[gTasks[taskId].tSpriteId].x2 = DEBUG_NUMBER_ICON_X+10;
        gSprites[gTasks[taskId].tSpriteId].y2 = DEBUG_NUMBER_ICON_Y+10;
        gSprites[gTasks[taskId].tSpriteId].oam.priority = 0;
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].tItemId = gTasks[taskId].tInput;
        gTasks[taskId].tInput = 1;
        gTasks[taskId].tDigit = 0;
        Debug_Display_ItemQuantity(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
        gTasks[taskId].func = DebugAction_Give_Item_SelectQuantity;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        DestroyItemIcon(taskId);

        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Give_Item_SelectQuantity(u8 taskId)
{
    u32 itemId = gTasks[taskId].tItemId;

    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 1, MAX_BAG_ITEM_CAPACITY, MAX_ITEM_DIGITS);
        Debug_Display_ItemQuantity(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }

    if (JOY_NEW(A_BUTTON))
    {
        DestroyItemIcon(taskId);

        PlaySE(MUS_LEVEL_UP);
        AddBagItem(itemId, gTasks[taskId].tInput);
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        DestroyItemIcon(taskId);

        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

#undef tItemId
#undef tSpriteId

//Pokemon
static void ResetMonDataStruct(struct DebugMonData *sDebugMonData)
{
    sDebugMonData->species          = 1;
    sDebugMonData->level            = MIN_LEVEL;
    sDebugMonData->isShiny          = FALSE;
    sDebugMonData->nature           = 0;
    sDebugMonData->abilityNum       = 0;
    sDebugMonData->teraType         = TYPE_NONE;
    sDebugMonData->dynamaxLevel     = 0;
    sDebugMonData->gmaxFactor       = FALSE;
    for (u32 i = 0; i < NUM_STATS; i++)
    {
        sDebugMonData->monIVs[i] = 0;
        sDebugMonData->monEVs[i] = 0;
    }
}

#define tIsComplex  data[5]
#define tSpriteId   data[6]
#define tIterator   data[7]

static void Debug_Display_IkigaiGymInfo(u32 type, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    u8 *end;
    if (type != TYPE_NONE && type != TYPE_MYSTERY)
        end = StringCopy(gStringVar1, GetSpeciesName(gIkigaiStarters[type][gSaveBlock2Ptr->playerGender]));
    else
        end = StringCopy(gStringVar1, COMPOUND_STRING("No Available Starter"));
    WrapFontIdToFit(gStringVar1, end, DEBUG_MENU_FONT, WindowWidthPx(windowId));
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    if (type != TYPE_NONE && type != TYPE_MYSTERY)
        StringCopy(gStringVar3, gTypesInfo[type].name);
    else
        StringCopy(gStringVar3, COMPOUND_STRING("None"));
    StringAppend(gStringVar3, COMPOUND_STRING("{CLEAR_TO 110}"));
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Gym Type: {STR_VAR_3}\n{STR_VAR_1}{CLEAR_TO 110}\n\n{STR_VAR_2}{CLEAR_TO 110}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Ikigai_GymType_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, TYPE_NORMAL, NUMBER_OF_MON_TYPES - 2, DEBUG_NUMBER_DIGITS_POKEMON_TYPE);
        Debug_Display_IkigaiGymInfo(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
        FreeMonIconPalettes();
        LoadMonIconPalette(gIkigaiStarters[gTasks[taskId].tInput][gSaveBlock2Ptr->playerGender]);
        gTasks[taskId].tSpriteId = CreateMonIcon(gIkigaiStarters[gTasks[taskId].tInput][gSaveBlock2Ptr->playerGender], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0);
        gSprites[gTasks[taskId].tSpriteId].oam.priority = 0;
    }

    if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].tInput == TYPE_MYSTERY)
            gTasks[taskId].tInput = TYPE_NONE;

        gSaveBlock2Ptr->ikigaiGymType = gTasks[taskId].tInput;
        VarSet(VAR_STARTER_MON, gIkigaiStarters[gTasks[taskId].tInput][gSaveBlock2Ptr->playerGender]);

        if (gIkigaiStarters[gTasks[taskId].tInput][gSaveBlock2Ptr->playerGender] == SPECIES_NONE)
        {
            PlaySE(SE_BOO);
            Free(sDebugMonData);
            FreeMonIconPalettes();
            FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
            DebugAction_DestroyExtraWindow(taskId);
        }

        sDebugMonData->species = gIkigaiStarters[gTasks[taskId].tInput][gSaveBlock2Ptr->playerGender];
        gTasks[taskId].tInput = 1;
        gTasks[taskId].tDigit = 0;

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].tDigit]);
        ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, 3);
        StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
        StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Level:{CLEAR_TO 110}\n{STR_VAR_1}{CLEAR_TO 110}\n{CLEAR_TO 110}\n{STR_VAR_2}{CLEAR_TO 110}"));
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectLevel;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Ikigai_GymType(u8 taskId)
{
    u8 windowId;

    //Mon data struct
    sDebugMonData = AllocZeroed(sizeof(struct DebugMonData));
    ResetMonDataStruct(sDebugMonData);

    //Window initialization
    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateGymType);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial Pokémon
    Debug_Display_IkigaiGymInfo(TYPE_NORMAL, 0, windowId);

    //Set task data
    gTasks[taskId].func = DebugAction_Ikigai_GymType_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = TYPE_NORMAL;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tIsComplex = FALSE;

    FreeMonIconPalettes();
    LoadMonIconPalette(gIkigaiStarters[gTasks[taskId].tInput][gSaveBlock2Ptr->playerGender]);
    gTasks[taskId].tSpriteId = CreateMonIcon(gIkigaiStarters[gTasks[taskId].tInput][gSaveBlock2Ptr->playerGender], SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0);
    gSprites[gTasks[taskId].tSpriteId].oam.priority = 0;
}

static void Debug_Display_SpeciesInfo(u32 species, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    u8 *end = StringCopy(gStringVar1, GetSpeciesName(species));
    WrapFontIdToFit(gStringVar1, end, DEBUG_MENU_FONT, WindowWidthPx(windowId));
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    ConvertIntToDecimalStringN(gStringVar3, species, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Species: {STR_VAR_3}\n{STR_VAR_1}{CLEAR_TO 90}\n\n{STR_VAR_2}{CLEAR_TO 90}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_PokemonCries_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 1, NUM_SPECIES - 1, DEBUG_NUMBER_DIGITS_ITEMS);
        Debug_Display_SpeciesInfo(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
        FreeMonIconPalettes();
        LoadMonIconPalette(gTasks[taskId].tInput);
        gTasks[taskId].tSpriteId = CreateMonIcon(gTasks[taskId].tInput, SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0);
        gSprites[gTasks[taskId].tSpriteId].oam.priority = 0;
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->species = gTasks[taskId].tInput;
        PlayCry_Script(sDebugMonData->species, CRY_MODE_NORMAL);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
        DebugAction_DestroyExtraWindow(taskId);
        Overworld_ChangeMusicToDefault();
    }
}

static void DebugAction_Ikigai_PokemonCries(u8 taskId)
{
    u8 windowId;

    StopMapMusic(); //Stop map music to better hear sounds

    //Mon data struct
    sDebugMonData = AllocZeroed(sizeof(struct DebugMonData));
    ResetMonDataStruct(sDebugMonData);

    //Window initialization
    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial Pokémon
    Debug_Display_SpeciesInfo(sDebugMonData->species, 0, windowId);

    //Set task data
    gTasks[taskId].func = DebugAction_PokemonCries_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = sDebugMonData->species;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tIsComplex = FALSE;

    FreeMonIconPalettes();
    LoadMonIconPalette(gTasks[taskId].tInput);
    gTasks[taskId].tSpriteId = CreateMonIcon(gTasks[taskId].tInput, SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0);
    gSprites[gTasks[taskId].tSpriteId].oam.priority = 0;
}

static void DebugAction_Give_PokemonSimple(u8 taskId)
{
    u8 windowId;

    //Mon data struct
    sDebugMonData = AllocZeroed(sizeof(struct DebugMonData));
    ResetMonDataStruct(sDebugMonData);

    //Window initialization
    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial Pokémon
    Debug_Display_SpeciesInfo(sDebugMonData->species, 0, windowId);

    //Set task data
    gTasks[taskId].func = DebugAction_Give_Pokemon_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = sDebugMonData->species;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tIsComplex = FALSE;

    FreeMonIconPalettes();
    LoadMonIconPalette(gTasks[taskId].tInput);
    gTasks[taskId].tSpriteId = CreateMonIcon(gTasks[taskId].tInput, SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0);
    gSprites[gTasks[taskId].tSpriteId].oam.priority = 0;
}

static void DebugAction_Give_PokemonComplex(u8 taskId)
{
    u8 windowId;

    //Mon data struct
    sDebugMonData = AllocZeroed(sizeof(struct DebugMonData));
    ResetMonDataStruct(sDebugMonData);

    //Window initialization
    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial Pokémon
    Debug_Display_SpeciesInfo(sDebugMonData->species, 0, windowId);

    gTasks[taskId].func = DebugAction_Give_Pokemon_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = 1;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tIsComplex = TRUE;

    FreeMonIconPalettes();
    LoadMonIconPalette(gTasks[taskId].tInput);
    gTasks[taskId].tSpriteId = CreateMonIcon(gTasks[taskId].tInput, SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0);
    gSprites[gTasks[taskId].tSpriteId].oam.priority = 0;
    gTasks[taskId].tIterator = 0;
}

static void Debug_Display_Level(u32 level, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    ConvertIntToDecimalStringN(gStringVar1, level, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Level:{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Pokemon_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 1, NUM_SPECIES - 1, DEBUG_NUMBER_DIGITS_ITEMS);
        Debug_Display_SpeciesInfo(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
        FreeMonIconPalettes();
        LoadMonIconPalette(gTasks[taskId].tInput);
        gTasks[taskId].tSpriteId = CreateMonIcon(gTasks[taskId].tInput, SpriteCB_MonIcon, DEBUG_NUMBER_ICON_X, DEBUG_NUMBER_ICON_Y, 4, 0);
        gSprites[gTasks[taskId].tSpriteId].oam.priority = 0;
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->species = gTasks[taskId].tInput;
        gTasks[taskId].tInput = 1;
        gTasks[taskId].tDigit = 0;

        Debug_Display_Level(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectLevel;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void Debug_Display_TrueFalse(bool32 value, u8 windowId, const u8 *titleStr)
{
    static const u8 *txtStr;
    txtStr = value ? sDebugText_True : sDebugText_False;
    StringCopyPadded(gStringVar2, txtStr, CHAR_SPACE, 15);
    ConvertIntToDecimalStringN(gStringVar3, value, STR_CONV_MODE_LEADING_ZEROS, 0);
    StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, titleStr);
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Pokemon_SelectLevel(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 1, MAX_LEVEL, 3);
        Debug_Display_Level(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }

    if (JOY_NEW(A_BUTTON))
    {
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
        if (gTasks[taskId].tIsComplex == FALSE)
        {
            PlaySE(MUS_LEVEL_UP);
            ScriptGiveMon(sDebugMonData->species, gTasks[taskId].tInput, ITEM_NONE);
            // Set flag for user convenience
            FlagSet(FLAG_SYS_POKEMON_GET);
            Free(sDebugMonData);
            DebugAction_DestroyExtraWindow(taskId);
        }
        else
        {
            sDebugMonData->level = gTasks[taskId].tInput;
            gTasks[taskId].tInput = 0;
            gTasks[taskId].tDigit = 0;
            Debug_Display_TrueFalse(gTasks[taskId].tInput, gTasks[taskId].tSubWindowId, sDebugText_PokemonShiny);
            gTasks[taskId].func = DebugAction_Give_Pokemon_SelectShiny;
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void Debug_Display_Nature(u32 natureId, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    ConvertIntToDecimalStringN(gStringVar3, natureId, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
    StringCopy(gStringVar1, gNaturesInfo[natureId].name);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Nature ID: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Pokemon_SelectShiny(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].tInput ^= JOY_NEW(DPAD_UP | DPAD_DOWN) > 0;
        Debug_Display_TrueFalse(gTasks[taskId].tInput, gTasks[taskId].tSubWindowId, sDebugText_PokemonShiny);
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->isShiny = gTasks[taskId].tInput;
        gTasks[taskId].tInput = 0;
        gTasks[taskId].tDigit = 0;
        Debug_Display_Nature(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectNature;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void Debug_Display_Ability(u32 abilityId, u32 digit, u8 windowId)//(u32 natureId, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    ConvertIntToDecimalStringN(gStringVar3, abilityId, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
    u8 *end = StringCopy(gStringVar1, gAbilitiesInfo[abilityId].name);
    WrapFontIdToFit(gStringVar1, end, DEBUG_MENU_FONT, WindowWidthPx(windowId));
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Ability Num: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Pokemon_SelectNature(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].tInput += sPowersOfTen[gTasks[taskId].tDigit];
            if (gTasks[taskId].tInput > NUM_NATURES - 1)
                gTasks[taskId].tInput = NUM_NATURES - 1;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].tInput -= sPowersOfTen[gTasks[taskId].tDigit];
            if (gTasks[taskId].tInput < 0)
                gTasks[taskId].tInput = 0;
        }

        Debug_Display_Nature(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->nature = gTasks[taskId].tInput;
        gTasks[taskId].tInput = 0;
        gTasks[taskId].tDigit = 0;

        u32 abilityId = GetAbilityBySpecies(sDebugMonData->species, 0);
        Debug_Display_Ability(abilityId, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectAbility;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void Debug_Display_TeraType(u32 typeId, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    ConvertIntToDecimalStringN(gStringVar3, typeId, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
    StringCopy(gStringVar1, gTypesInfo[typeId].name);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Tera Type: {STR_VAR_3}{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Pokemon_SelectAbility(u8 taskId)
{
    u8 abilityCount = NUM_ABILITY_SLOTS - 1; //-1 for proper iteration
    u8 i = 0;

    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].tInput += sPowersOfTen[gTasks[taskId].tDigit];
            if (gTasks[taskId].tInput > abilityCount)
                gTasks[taskId].tInput = abilityCount;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].tInput -= sPowersOfTen[gTasks[taskId].tDigit];
            if (gTasks[taskId].tInput < 0)
                gTasks[taskId].tInput = 0;
        }

        while (GetAbilityBySpecies(sDebugMonData->species, gTasks[taskId].tInput - i) == ABILITY_NONE && gTasks[taskId].tInput - i < NUM_ABILITY_SLOTS)
        {
            i++;
        }
        u32 abilityId = GetAbilityBySpecies(sDebugMonData->species, gTasks[taskId].tInput - i);
        Debug_Display_Ability(abilityId, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->abilityNum = gTasks[taskId].tInput - i;
        gTasks[taskId].tInput = 0;
        gTasks[taskId].tDigit = 0;

        Debug_Display_TeraType(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectTeraType;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void Debug_Display_DynamaxLevel(u32 level, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    ConvertIntToDecimalStringN(gStringVar1, level, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Dmax Lvl:{CLEAR_TO 90}\n{STR_VAR_1}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{STR_VAR_2}{CLEAR_TO 90}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Pokemon_SelectTeraType(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);

        if (JOY_NEW(DPAD_UP))
        {
            gTasks[taskId].tInput += sPowersOfTen[gTasks[taskId].tDigit];
            if (gTasks[taskId].tInput > NUMBER_OF_MON_TYPES - 1)
                gTasks[taskId].tInput = NUMBER_OF_MON_TYPES - 1;
        }
        if (JOY_NEW(DPAD_DOWN))
        {
            gTasks[taskId].tInput -= sPowersOfTen[gTasks[taskId].tDigit];
            if (gTasks[taskId].tInput < 0)
                gTasks[taskId].tInput = 0;
        }

        Debug_Display_TeraType(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->teraType = gTasks[taskId].tInput;
        gTasks[taskId].tInput = 0;
        gTasks[taskId].tDigit = 0;

        Debug_Display_DynamaxLevel(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);

        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectDynamaxLevel;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void Debug_Display_GigantamaxFactor(u32 input, u8 windowId)
{
    Debug_Display_TrueFalse(input, windowId, COMPOUND_STRING("Gmax Factor:{CLEAR_TO 90}\n   {STR_VAR_2}{CLEAR_TO 90}\n{CLEAR_TO 90}\n{CLEAR_TO 90}"));
}

static void DebugAction_Give_Pokemon_SelectDynamaxLevel(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 0, MAX_DYNAMAX_LEVEL, 2);
        Debug_Display_DynamaxLevel(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->dynamaxLevel = gTasks[taskId].tInput;
        gTasks[taskId].tInput = 0;
        gTasks[taskId].tDigit = 0;
        Debug_Display_GigantamaxFactor(gTasks[taskId].tInput, gTasks[taskId].tSubWindowId);
        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectGigantamaxFactor;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        FreeMonIconPalettes();
        FreeAndDestroyMonIconSprite(&gSprites[gTasks[taskId].tSpriteId]);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void Debug_Display_StatInfo(const u8* text, u32 stat, u32 value, u32 digit, u8 windowId)
{
    StringCopy(gStringVar1, gStatNamesTable[stat]);
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    ConvertIntToDecimalStringN(gStringVar3, value, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopyPadded(gStringVar3, gStringVar3, CHAR_SPACE, 15);
    StringExpandPlaceholders(gStringVar4, text);
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Pokemon_SelectGigantamaxFactor(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].tInput ^= JOY_NEW(DPAD_UP | DPAD_DOWN) > 0;
        Debug_Display_GigantamaxFactor(gTasks[taskId].tInput, gTasks[taskId].tSubWindowId);
    }

    if (JOY_NEW(A_BUTTON))
    {
        sDebugMonData->gmaxFactor = gTasks[taskId].tInput;
        gTasks[taskId].tInput = 0;
        gTasks[taskId].tDigit = 0;
        Debug_Display_StatInfo(sDebugText_IVs, gTasks[taskId].tIterator, gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
        gTasks[taskId].func = DebugAction_Give_Pokemon_SelectIVs;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Give_Pokemon_SelectIVs(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 0, MAX_PER_STAT_IVS, 3);
        Debug_Display_StatInfo(sDebugText_IVs, gTasks[taskId].tIterator, gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }

    //If A or B button
    if (JOY_NEW(A_BUTTON))
    {
        // Set IVs for stat
        sDebugMonData->monIVs[gTasks[taskId].tIterator] = gTasks[taskId].tInput;

        //Check if all IVs set
        if (gTasks[taskId].tIterator != NUM_STATS - 1)
        {
            gTasks[taskId].tIterator++;
            gTasks[taskId].tInput = 0;
            gTasks[taskId].tDigit = 0;

            Debug_Display_StatInfo(sDebugText_IVs, gTasks[taskId].tIterator, gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
            gTasks[taskId].func = DebugAction_Give_Pokemon_SelectIVs;
        }
        else
        {
            gTasks[taskId].tInput = 0;
            gTasks[taskId].tDigit = 0;
            gTasks[taskId].tIterator = 0;

            Debug_Display_StatInfo(sDebugText_EVs, gTasks[taskId].tIterator, gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
            gTasks[taskId].func = DebugAction_Give_Pokemon_SelectEVs;
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static u32 GetDebugPokemonTotalEV(void)
{
    u32 totalEVs = 0;
    for (u32 i = 0; i < NUM_STATS; i++)
        totalEVs += sDebugMonData->monEVs[i];
    return totalEVs;
}

static void Debug_Display_MoveInfo(u32 moveId, u32 iteration, u32 digit, u8 windowId)
{
    // Doesn't expand placeholdes so a 4th dynamic value can be shown.
    u8 *end = StringCopy(gStringVar1, GetMoveName(moveId));
    WrapFontIdToFit(gStringVar1, end, DEBUG_MENU_FONT, WindowWidthPx(windowId));
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    StringCopy(gStringVar4, COMPOUND_STRING("Move "));
    ConvertIntToDecimalStringN(gStringVar3, iteration, STR_CONV_MODE_LEADING_ZEROS, 1);
    StringAppend(gStringVar4, gStringVar3);
    StringAppend(gStringVar4, COMPOUND_STRING(": "));
    ConvertIntToDecimalStringN(gStringVar3, moveId, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringAppend(gStringVar4, gStringVar3);
    StringAppend(gStringVar4, COMPOUND_STRING("{CLEAR_TO 90}\n"));
    StringAppend(gStringVar4, gStringVar1);
    StringAppend(gStringVar4, COMPOUND_STRING("{CLEAR_TO 90}\n{CLEAR_TO 90}\n"));
    StringAppend(gStringVar4, gText_DigitIndicator[digit]);
    StringAppend(gStringVar4, COMPOUND_STRING("{CLEAR_TO 90}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Pokemon_SelectEVs(u8 taskId)
{
    u16 totalEV = GetDebugPokemonTotalEV();

    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 0, MAX_PER_STAT_EVS, 4);
        Debug_Display_StatInfo(sDebugText_EVs, gTasks[taskId].tIterator, gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }

    //If A or B button
    if (JOY_NEW(A_BUTTON))
    {
        // Set EVs for stat
        sDebugMonData->monEVs[gTasks[taskId].tIterator] = gTasks[taskId].tInput;

        //Check if all EVs set
        if (gTasks[taskId].tIterator != NUM_STATS - 1)
        {
            gTasks[taskId].tIterator++;
            gTasks[taskId].tInput = 0;
            gTasks[taskId].tDigit = 0;
            Debug_Display_StatInfo(sDebugText_EVs, gTasks[taskId].tIterator, gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
            gTasks[taskId].func = DebugAction_Give_Pokemon_SelectEVs;
        }
        else
        {
            gTasks[taskId].tInput = 0;
            gTasks[taskId].tDigit = 0;
            gTasks[taskId].tIterator = 0;

            if (totalEV > MAX_TOTAL_EVS)
            {
                for (u32 i = 0; i < NUM_STATS; i++)
                {
                    sDebugMonData->monEVs[i] = 0;
                }

                PlaySE(SE_FAILURE);
                Debug_Display_StatInfo(sDebugText_EVs, gTasks[taskId].tIterator, gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
                gTasks[taskId].func = DebugAction_Give_Pokemon_SelectEVs;
            }
            else
            {
                Debug_Display_MoveInfo(gTasks[taskId].tInput, gTasks[taskId].tIterator, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
                gTasks[taskId].func = DebugAction_Give_Pokemon_Move;
            }
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Give_Pokemon_Move(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 0, MOVES_COUNT - 1, 4);

        Debug_Display_MoveInfo(gTasks[taskId].tInput, gTasks[taskId].tIterator, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
    }

    if (JOY_NEW(A_BUTTON))
    {
        // Set current value
        sDebugMonData->monMoves[gTasks[taskId].tIterator] = gTasks[taskId].tInput;

        // If MOVE_NONE selected, stop asking for additional moves
        if (gTasks[taskId].tInput == MOVE_NONE)
            gTasks[taskId].tIterator = MAX_MON_MOVES;

        //If NOT last move or selected MOVE_NONE ask for next move, else make mon
        if (gTasks[taskId].tIterator < MAX_MON_MOVES - 1)
        {
            gTasks[taskId].tIterator++;
            gTasks[taskId].tInput = 0;
            gTasks[taskId].tDigit = 0;

            Debug_Display_MoveInfo(gTasks[taskId].tInput, gTasks[taskId].tIterator, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
            gTasks[taskId].func = DebugAction_Give_Pokemon_Move;
        }
        else
        {
            gTasks[taskId].tInput = 0;
            gTasks[taskId].tDigit = 0;

            PlaySE(MUS_LEVEL_UP);
            gTasks[taskId].func = DebugAction_Give_Pokemon_ComplexCreateMon;
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        Free(sDebugMonData);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Give_Pokemon_ComplexCreateMon(u8 taskId) //https://github.com/ghoulslash/pokeemerald/tree/custom-givemon
{
    enum NationalDexOrder nationalDexNum;
    int sentToPc;
    struct Pokemon mon;
    u8 i;
    u16 moves[MAX_MON_MOVES];
    u8 IVs[NUM_STATS];
    u8 iv_val;
    u8 EVs[NUM_STATS];
    u8 ev_val;
    u16 species     = sDebugMonData->species;
    u8 level        = sDebugMonData->level;
    bool8 isShiny   = sDebugMonData->isShiny;
    u8 nature       = sDebugMonData->nature;
    u8 abilityNum   = sDebugMonData->abilityNum;
    u32 teraType    = sDebugMonData->teraType;
    u32 dmaxLevel   = sDebugMonData->dynamaxLevel;
    u32 gmaxFactor  = sDebugMonData->gmaxFactor;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        moves[i] = sDebugMonData->monMoves[i];
    }
    for (u32 i = 0; i < NUM_STATS; i++)
    {
        EVs[i] = sDebugMonData->monEVs[i];
        IVs[i] = sDebugMonData->monIVs[i];
    }

    //Nature
    if (nature == NUM_NATURES || nature == 0xFF)
        nature = Random() % NUM_NATURES;
    CreateMonWithNature(&mon, species, level, USE_RANDOM_IVS, nature);

    //Shininess
    SetMonData(&mon, MON_DATA_IS_SHINY, &isShiny);

    // Gigantamax factor
    SetMonData(&mon, MON_DATA_GIGANTAMAX_FACTOR, &gmaxFactor);

    // Dynamax Level
    SetMonData(&mon, MON_DATA_DYNAMAX_LEVEL, &dmaxLevel);

    // tera type
    if (teraType == TYPE_NONE || teraType == TYPE_MYSTERY || teraType >= NUMBER_OF_MON_TYPES)
        teraType = GetTeraTypeFromPersonality(&mon);
    SetMonData(&mon, MON_DATA_TERA_TYPE, &teraType);

    //IVs
    for (i = 0; i < NUM_STATS; i++)
    {
        iv_val = IVs[i];
        if (iv_val != USE_RANDOM_IVS && iv_val != 0xFF)
            SetMonData(&mon, MON_DATA_HP_IV + i, &iv_val);
    }

    //EVs
    for (i = 0; i < NUM_STATS; i++)
    {
        ev_val = EVs[i];
        if (ev_val)
            SetMonData(&mon, MON_DATA_HP_EV + i, &ev_val);
    }

    //Moves
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (moves[i] == MOVE_NONE || moves[i] >= MOVES_COUNT)
            continue;

        SetMonMoveSlot(&mon, moves[i], i);
    }

    //Ability
    if (abilityNum == 0xFF || GetAbilityBySpecies(species, abilityNum) == ABILITY_NONE)
    {
        do {
            abilityNum = Random() % NUM_ABILITY_SLOTS;  // includes hidden abilities
        } while (GetAbilityBySpecies(species, abilityNum) == ABILITY_NONE);
    }

    SetMonData(&mon, MON_DATA_ABILITY_NUM, &abilityNum);

    //Update mon stats before giving it to the player
    CalculateMonStats(&mon);

    // give player the mon
    SetMonData(&mon, MON_DATA_OT_NAME, gSaveBlock2Ptr->playerName);
    SetMonData(&mon, MON_DATA_OT_GENDER, &gSaveBlock2Ptr->playerGender);
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            break;
    }

    if (i >= PARTY_SIZE)
    {
        sentToPc = CopyMonToPC(&mon);
    }
    else
    {
        sentToPc = MON_GIVEN_TO_PARTY;
        CopyMon(&gPlayerParty[i], &mon, sizeof(mon));
        gPlayerPartyCount = i + 1;
    }

    //Pokedex entry
    nationalDexNum = SpeciesToNationalPokedexNum(species);
    switch(sentToPc)
    {
    case MON_GIVEN_TO_PARTY:
    case MON_GIVEN_TO_PC:
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_SEEN);
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_CAUGHT);
        break;
    case MON_CANT_GIVE:
        break;
    }

    // Set flag for user convenience
    FlagSet(FLAG_SYS_POKEMON_GET);

    Free(sDebugMonData);
    DebugAction_DestroyExtraWindow(taskId); //return sentToPc;
}

#undef tIsComplex
#undef tSpriteId
#undef tIterator

//Decoration
#define tSpriteId  data[6]

static void Debug_Display_DecorationInfo(u32 itemId, u32 digit, u8 windowId)
{
    StringCopy(gStringVar2, gText_DigitIndicator[digit]);
    u8* end = StringCopy(gStringVar1, gDecorations[itemId].name);
    WrapFontIdToFit(gStringVar1, end, DEBUG_MENU_FONT, WindowWidthPx(windowId));
    StringCopyPadded(gStringVar1, gStringVar1, CHAR_SPACE, 15);
    ConvertIntToDecimalStringN(gStringVar3, itemId, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringExpandPlaceholders(gStringVar4, COMPOUND_STRING("Decor ID: {STR_VAR_3}\n{STR_VAR_1}{CLEAR_TO 90}\n\n{STR_VAR_2}"));
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
}

static void DebugAction_Give_Decoration(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateExtra);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial decoration
    Debug_Display_DecorationInfo(1, 0, windowId);

    gTasks[taskId].func = DebugAction_Give_Decoration_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = 1;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tSpriteId = AddDecorationIconObject(gTasks[taskId].tInput, DEBUG_NUMBER_ICON_X+8, DEBUG_NUMBER_ICON_Y+10, 0, ITEM_TAG, ITEM_TAG);
}

static void DestroyDecorationIcon(u8 taskId)
{
    FreeSpriteTilesByTag(ITEM_TAG);
    FreeSpritePaletteByTag(ITEM_TAG);
    FreeSpriteOamMatrix(&gSprites[gTasks[taskId].tSpriteId]);
    DestroySprite(&gSprites[gTasks[taskId].tSpriteId]);
}

static void DebugAction_Give_Decoration_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, 1, NUM_DECORATIONS, DEBUG_NUMBER_DIGITS_ITEMS);
        Debug_Display_DecorationInfo(gTasks[taskId].tInput, gTasks[taskId].tDigit, gTasks[taskId].tSubWindowId);
        DestroyDecorationIcon(taskId);
        gTasks[taskId].tSpriteId = AddDecorationIconObject(gTasks[taskId].tInput, DEBUG_NUMBER_ICON_X+8, DEBUG_NUMBER_ICON_Y+10, 0, ITEM_TAG, ITEM_TAG);
    }

    if (JOY_NEW(A_BUTTON))
    {
        DestroyItemIcon(taskId);

        PlaySE(MUS_LEVEL_UP);
        DecorationAdd(gTasks[taskId].tInput);
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        DestroyDecorationIcon(taskId);

        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

#undef tSpriteId

static void DebugAction_Give_MaxMoney(u8 taskId)
{
    SetMoney(&gSaveBlock1Ptr->money, MAX_MONEY);
}

static void DebugAction_Give_MaxCoins(u8 taskId)
{
    SetCoins(MAX_COINS);
}

static void DebugAction_Give_MaxBattlePoints(u8 taskId)
{
    gSaveBlock2Ptr->frontier.battlePoints = MAX_BATTLE_FRONTIER_POINTS;
}

static void DebugAction_Give_DayCareEgg(u8 taskId)
{
    s32 emptySlot = Daycare_FindEmptySpot(&gSaveBlock1Ptr->daycare);
    if (emptySlot == 0) // no daycare mons
        Debug_DestroyMenu_Full_Script(taskId, DebugScript_ZeroDaycareMons);
    else if (emptySlot == 1) // 1 daycare mon
        Debug_DestroyMenu_Full_Script(taskId, DebugScript_OneDaycareMons);
    else if (GetDaycareCompatibilityScore(&gSaveBlock1Ptr->daycare) == PARENTS_INCOMPATIBLE) // not compatible parents
        Debug_DestroyMenu_Full_Script(taskId, DebugScript_DaycareMonsNotCompatible);
    else // 2 pokemon which can have a pokemon baby together
        TriggerPendingDaycareEgg();
}

// *******************************
// Actions TimeMenu

static void DebugAction_TimeMenu_ChangeTimeOfDay(u8 taskId)
{
    u32 input = ListMenu_ProcessInput(gTasks[taskId].tMenuTaskId);

    DebugAction_DestroyExtraWindow(taskId);
    RtcCalcLocalTime();
    switch (input)
    {
        case TIME_MORNING:
            FakeRtc_ForwardTimeTo(Ikigai_GetSeasonalTimeHour(gLocalTime.days, TIME_MORNING, FALSE), 0, 0);
            break;
        case TIME_DAY:
            FakeRtc_ForwardTimeTo(Ikigai_GetSeasonalTimeHour(gLocalTime.days, TIME_DAY, FALSE), 0, 0);
            break;
        case TIME_EVENING:
            FakeRtc_ForwardTimeTo(Ikigai_GetSeasonalTimeHour(gLocalTime.days, TIME_EVENING, FALSE), 0, 0);
            break;
        case TIME_NIGHT:
            FakeRtc_ForwardTimeTo(Ikigai_GetSeasonalTimeHour(gLocalTime.days, TIME_NIGHT, FALSE), 0, 0);
            break;
    }
    Debug_DestroyMenu_Full(taskId);
    SetMainCallback2(CB2_LoadMap);
}

static void DebugAction_TimeMenu_ChangeWeekdays(u8 taskId)
{
    u32 input = ListMenu_ProcessInput(gTasks[taskId].tMenuTaskId);
    struct SiiRtcInfo *rtc = FakeRtc_GetCurrentTime();
    u32 daysToAdd = 0;

    DebugAction_DestroyExtraWindow(taskId);
    daysToAdd = ((input - rtc->dayOfWeek) + WEEKDAY_COUNT) % WEEKDAY_COUNT;
    FakeRtc_AdvanceTimeBy(daysToAdd, 0, 0, 0);
    Debug_DestroyMenu_Full(taskId);
    SetMainCallback2(CB2_LoadMap);
}

// *******************************
// Actions PCBag

static void DebugAction_PCBag_Fill_PCBoxes_Fast(u8 taskId) //Credit: Sierraffinity
{
    int boxId, boxPosition;
    u32 personality;
    struct BoxPokemon boxMon;
    u16 species = SPECIES_BULBASAUR;
    u8 speciesName[POKEMON_NAME_LENGTH + 1];

    personality = Random32();

    CreateBoxMon(&boxMon, species, 100, USE_RANDOM_IVS, FALSE, personality, OT_ID_PLAYER_ID, 0);

    for (boxId = 0; boxId < TOTAL_BOXES_COUNT; boxId++)
    {
        for (boxPosition = 0; boxPosition < IN_BOX_COUNT; boxPosition++, species++)
        {
            if (!GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_SANITY_HAS_SPECIES))
            {
                StringCopy(speciesName, GetSpeciesName(species));
                SetBoxMonData(&boxMon, MON_DATA_NICKNAME, &speciesName);
                SetBoxMonData(&boxMon, MON_DATA_SPECIES, &species);
                GiveBoxMonInitialMoveset(&boxMon);
                gPokemonStoragePtr->boxes[boxId][boxPosition] = boxMon;
            }
        }
    }

    // Set flag for user convenience
    FlagSet(FLAG_SYS_POKEMON_GET);
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

static void DebugAction_PCBag_Fill_PCBoxes_Slow(u8 taskId)
{
    int boxId, boxPosition;
    struct BoxPokemon boxMon;
    u32 species = SPECIES_BULBASAUR;
    bool8 spaceAvailable = FALSE;

    for (boxId = 0; boxId < TOTAL_BOXES_COUNT; boxId++)
    {
        for (boxPosition = 0; boxPosition < IN_BOX_COUNT; boxPosition++)
        {
            if (!GetBoxMonData(&gPokemonStoragePtr->boxes[boxId][boxPosition], MON_DATA_SANITY_HAS_SPECIES))
            {
                if (!spaceAvailable)
                    PlayBGM(MUS_RG_MYSTERY_GIFT);
                CreateBoxMon(&boxMon, species, 100, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
                gPokemonStoragePtr->boxes[boxId][boxPosition] = boxMon;
                species = (species < NUM_SPECIES - 1) ? species + 1 : 1;
                spaceAvailable = TRUE;
            }
        }
    }

    // Set flag for user convenience
    FlagSet(FLAG_SYS_POKEMON_GET);
    if (spaceAvailable)
        PlayBGM(GetCurrentMapMusic());

    Debug_DestroyMenu_Full_Script(taskId, Debug_BoxFilledMessage);
}

static void DebugAction_PCBag_Fill_PCItemStorage(u8 taskId)
{
    u16 itemId;

    for (itemId = 1; itemId < ITEMS_COUNT; itemId++)
    {
        if (!CheckPCHasItem(itemId, MAX_PC_ITEM_CAPACITY))
            AddPCItem(itemId, MAX_PC_ITEM_CAPACITY);
    }
}

static void DebugAction_PCBag_Fill_PocketItems(u8 taskId)
{
    u16 itemId;

    for (itemId = 1; itemId < ITEMS_COUNT; itemId++)
    {
        if (GetItemPocket(itemId) == POCKET_ITEMS && CheckBagHasSpace(itemId, MAX_BAG_ITEM_CAPACITY))
            AddBagItem(itemId, MAX_BAG_ITEM_CAPACITY);
    }
}

static void DebugAction_PCBag_Fill_PocketPokeBalls(u8 taskId)
{
    u16 ballId;

    for (ballId = BALL_STRANGE; ballId < POKEBALL_COUNT; ballId++)
    {
        if (CheckBagHasSpace(ballId, MAX_BAG_ITEM_CAPACITY))
            AddBagItem(ballId, MAX_BAG_ITEM_CAPACITY);
    }
}

static void DebugAction_PCBag_Fill_PocketTMHM(u8 taskId)
{
    u16 index, itemId;

    for (index = 0; index < NUM_ALL_MACHINES; index++)
    {
        itemId = GetTMHMItemId(index + 1);
        if (CheckBagHasSpace(itemId, 1) && ItemIdToBattleMoveId(itemId) != MOVE_NONE)
            AddBagItem(itemId, 1);
    }
}

static void DebugAction_PCBag_Fill_PocketBerries(u8 taskId)
{
    u16 itemId;

    for (itemId = FIRST_BERRY_INDEX; itemId < LAST_BERRY_INDEX; itemId++)
    {
        if (CheckBagHasSpace(itemId, MAX_BAG_ITEM_CAPACITY))
            AddBagItem(itemId, MAX_BAG_ITEM_CAPACITY);
    }
}

static void DebugAction_PCBag_Fill_PocketKeyItems(u8 taskId)
{
    u16 itemId;

    for (itemId = 1; itemId < ITEMS_COUNT; itemId++)
    {
        if (GetItemPocket(itemId) == POCKET_KEY_ITEMS && CheckBagHasSpace(itemId, 1))
            AddBagItem(itemId, 1);
    }
}

static void DebugAction_PCBag_ClearBag(u8 taskId)
{
    PlaySE(MUS_LEVEL_UP);
    ClearBag();
}

static void DebugAction_PCBag_ClearBoxes(u8 taskId)
{
    ResetPokemonStorageSystem();
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

// *******************************
// Actions Sound
static const u8 *const sBGMNames[END_MUS - START_MUS + 1];
static const u8 *const sSENames[END_SE + 1];

#define tCurrentSong  data[5]

static void DebugAction_Sound_SE(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateSound);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial sound effect
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, 1, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringCopyPadded(gStringVar1, sSENames[0], CHAR_SPACE, 35);
    StringExpandPlaceholders(gStringVar4, sDebugText_Sound_SFX_ID);
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

    StopMapMusic(); //Stop map music to better hear sounds

    gTasks[taskId].func = DebugAction_Sound_SE_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = 1;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tCurrentSong = gTasks[taskId].tInput;
}

static void DebugAction_Sound_SE_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        const u8 *seName;
        Debug_HandleInput_Numeric(taskId, 1, END_SE, DEBUG_NUMBER_DIGITS_ITEMS);

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].tDigit]);
        seName = sSENames[gTasks[taskId].tInput - 1];
        if (seName == NULL)
            seName = sDebugText_Dashes;
        StringCopyPadded(gStringVar1, seName, CHAR_SPACE, 35);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
        StringExpandPlaceholders(gStringVar4, sDebugText_Sound_SFX_ID);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        m4aSongNumStop(gTasks[taskId].tCurrentSong);
        gTasks[taskId].tCurrentSong = gTasks[taskId].tInput;
        m4aSongNumStart(gTasks[taskId].tInput);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        m4aSongNumStop(gTasks[taskId].tCurrentSong);
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (JOY_NEW(START_BUTTON))
    {
        m4aSongNumStop(gTasks[taskId].tCurrentSong);
    }
}

static void DebugAction_Sound_MUS(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateSound);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial song
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, START_MUS, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringCopyPadded(gStringVar1, sBGMNames[0], CHAR_SPACE, 35);
    StringExpandPlaceholders(gStringVar4, sDebugText_Sound_Music_ID);
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

    StopMapMusic(); //Stop map music to better hear new music

    gTasks[taskId].func = DebugAction_Sound_MUS_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = START_MUS;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tCurrentSong = gTasks[taskId].tInput;
}

static const u32 gDebugFollowerNPCGraphics[] = 
{
    OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL,
    OBJ_EVENT_GFX_RIVAL_MAY_NORMAL,
    OBJ_EVENT_GFX_STEVEN,
    OBJ_EVENT_GFX_WALLY,
    OBJ_EVENT_GFX_RED,
    OBJ_EVENT_GFX_LEAF,
};

static void DebugAction_CreateFollowerNPC(u8 taskId)
{
    u32 input = ListMenu_ProcessInput(gTasks[taskId].tMenuTaskId);
    u32 gfx = gDebugFollowerNPCGraphics[input];

    Debug_DestroyMenu_Full(taskId);
    LockPlayerFieldControls();
    DestroyFollowerNPC();
    SetFollowerNPCData(FNPC_DATA_BATTLE_PARTNER, PARTNER_STEVEN);
    CreateFollowerNPC(gfx, FNPC_ALL, Debug_Follower_NPC_Event_Script);
    UnlockPlayerFieldControls();
}

static void DebugAction_DestroyFollowerNPC(u8 taskId)
{
    if (FNPC_ENABLE_NPC_FOLLOWERS)
    {
        Debug_DestroyMenu_Full(taskId);
        LockPlayerFieldControls();
        DestroyFollowerNPC();
        UnlockPlayerFieldControls();
    }
    else
    {
        Debug_DestroyMenu_Full_Script(taskId, Debug_Follower_NPC_Not_Enabled);
    }
}

// Start of MUSIC_EXPANSION_GEN5_MUSIC_REDUCE_SIZE == FALSE
#define SOUND_LIST_BGM_GEN_V_UNREDUCED              \
    X(MUS_LITTLEROOT_TEST)          \
    X(MUS_GSC_ROUTE38)              \
    X(MUS_CAUGHT)                   \
    X(MUS_VICTORY_WILD)             \
    X(MUS_VICTORY_GYM_LEADER)       \
    X(MUS_VICTORY_LEAGUE)           \
    X(MUS_C_COMM_CENTER)            \
    X(MUS_GSC_PEWTER)               \
    X(MUS_C_VS_LEGEND_BEAST)        \
    X(MUS_ROUTE101)                 \
    X(MUS_ROUTE110)                 \
    X(MUS_ROUTE120)                 \
    X(MUS_PETALBURG)                \
    X(MUS_OLDALE)                   \
    X(MUS_GYM)                      \
    X(MUS_SURF)                     \
    X(MUS_PETALBURG_WOODS)          \
    X(MUS_LEVEL_UP)                 \
    X(MUS_HEAL)                     \
    X(MUS_OBTAIN_BADGE)             \
    X(MUS_OBTAIN_ITEM)              \
    X(MUS_EVOLVED)                  \
    X(MUS_OBTAIN_TMHM)              \
    X(MUS_LILYCOVE_MUSEUM)          \
    X(MUS_ROUTE122)                 \
    X(MUS_OCEANIC_MUSEUM)           \
    X(MUS_EVOLUTION_INTRO)          \
    X(MUS_EVOLUTION)                \
    X(MUS_MOVE_DELETED)             \
    X(MUS_ENCOUNTER_GIRL)           \
    X(MUS_ENCOUNTER_MALE)           \
    X(MUS_ABANDONED_SHIP)           \
    X(MUS_FORTREE)                  \
    X(MUS_BIRCH_LAB)                \
    X(MUS_B_TOWER_RS)               \
    X(MUS_ENCOUNTER_SWIMMER)        \
    X(MUS_CAVE_OF_ORIGIN)           \
    X(MUS_OBTAIN_BERRY)             \
    X(MUS_AWAKEN_LEGEND)            \
    X(MUS_SLOTS_JACKPOT)            \
    X(MUS_SLOTS_WIN)                \
    X(MUS_TOO_BAD)                  \
    X(MUS_ROULETTE)                 \
    X(MUS_LINK_CONTEST_P1)          \
    X(MUS_LINK_CONTEST_P2)          \
    X(MUS_LINK_CONTEST_P3)          \
    X(MUS_LINK_CONTEST_P4)          \
    X(MUS_ENCOUNTER_RICH)           \
    X(MUS_VERDANTURF)               \
    X(MUS_RUSTBORO)                 \
    X(MUS_POKE_CENTER)              \
    X(MUS_ROUTE104)                 \
    X(MUS_ROUTE119)                 \
    X(MUS_CYCLING)                  \
    X(MUS_POKE_MART)                \
    X(MUS_LITTLEROOT)               \
    X(MUS_MT_CHIMNEY)               \
    X(MUS_ENCOUNTER_FEMALE)         \
    X(MUS_LILYCOVE)                 \
    X(MUS_DESERT)                   \
    X(MUS_HELP)                     \
    X(MUS_UNDERWATER)               \
    X(MUS_VICTORY_TRAINER)          \
    X(MUS_TITLE)                    \
    X(MUS_INTRO)                    \
    X(MUS_ENCOUNTER_MAY)            \
    X(MUS_ENCOUNTER_INTENSE)        \
    X(MUS_ENCOUNTER_COOL)           \
    X(MUS_ROUTE113)                 \
    X(MUS_ENCOUNTER_AQUA)           \
    X(MUS_FOLLOW_ME)                \
    X(MUS_ENCOUNTER_BRENDAN)        \
    X(MUS_EVER_GRANDE)              \
    X(MUS_ENCOUNTER_SUSPICIOUS)     \
    X(MUS_VICTORY_AQUA_MAGMA)       \
    X(MUS_CABLE_CAR)                \
    X(MUS_GAME_CORNER)              \
    X(MUS_DEWFORD)                  \
    X(MUS_SAFARI_ZONE)              \
    X(MUS_VICTORY_ROAD)             \
    X(MUS_AQUA_MAGMA_HIDEOUT)       \
    X(MUS_SAILING)                  \
    X(MUS_MT_PYRE)                  \
    X(MUS_SLATEPORT)                \
    X(MUS_MT_PYRE_EXTERIOR)         \
    X(MUS_SCHOOL)                   \
    X(MUS_HALL_OF_FAME)             \
    X(MUS_FALLARBOR)                \
    X(MUS_SEALED_CHAMBER)           \
    X(MUS_CONTEST_WINNER)           \
    X(MUS_CONTEST)                  \
    X(MUS_ENCOUNTER_MAGMA)          \
    X(MUS_INTRO_BATTLE)             \
    X(MUS_ABNORMAL_WEATHER)         \
    X(MUS_WEATHER_GROUDON)          \
    X(MUS_SOOTOPOLIS)               \
    X(MUS_CONTEST_RESULTS)          \
    X(MUS_HALL_OF_FAME_ROOM)        \
    X(MUS_TRICK_HOUSE)              \
    X(MUS_ENCOUNTER_TWINS)          \
    X(MUS_ENCOUNTER_ELITE_FOUR)     \
    X(MUS_ENCOUNTER_HIKER)          \
    X(MUS_CONTEST_LOBBY)            \
    X(MUS_ENCOUNTER_INTERVIEWER)    \
    X(MUS_ENCOUNTER_CHAMPION)       \
    X(MUS_CREDITS)                  \
    X(MUS_END)                      \
    X(MUS_B_FRONTIER)               \
    X(MUS_B_ARENA)                  \
    X(MUS_OBTAIN_B_POINTS)          \
    X(MUS_REGISTER_MATCH_CALL)      \
    X(MUS_B_PYRAMID)                \
    X(MUS_B_PYRAMID_TOP)            \
    X(MUS_B_PALACE)                 \
    X(MUS_RAYQUAZA_APPEARS)         \
    X(MUS_B_TOWER)                  \
    X(MUS_OBTAIN_SYMBOL)            \
    X(MUS_B_DOME)                   \
    X(MUS_B_PIKE)                   \
    X(MUS_B_FACTORY)                \
    X(MUS_VS_RAYQUAZA)              \
    X(MUS_VS_FRONTIER_BRAIN)        \
    X(MUS_VS_MEW)                   \
    X(MUS_B_DOME_LOBBY)             \
    X(MUS_VS_WILD)                  \
    X(MUS_VS_AQUA_MAGMA)            \
    X(MUS_VS_TRAINER)               \
    X(MUS_VS_GYM_LEADER)            \
    X(MUS_VS_CHAMPION)              \
    X(MUS_VS_REGI)                  \
    X(MUS_VS_KYOGRE_GROUDON)        \
    X(MUS_VS_RIVAL)                 \
    X(MUS_VS_ELITE_FOUR)            \
    X(MUS_VS_AQUA_MAGMA_LEADER)     \
    X(MUS_RG_FOLLOW_ME)             \
    X(MUS_RG_GAME_CORNER)           \
    X(MUS_RG_ROCKET_HIDEOUT)        \
    X(MUS_RG_GYM)                   \
    X(MUS_RG_JIGGLYPUFF)            \
    X(MUS_RG_INTRO_FIGHT)           \
    X(MUS_RG_TITLE)                 \
    X(MUS_RG_CINNABAR)              \
    X(MUS_RG_LAVENDER)              \
    X(MUS_RG_HEAL)                  \
    X(MUS_RG_CYCLING)               \
    X(MUS_RG_ENCOUNTER_ROCKET)      \
    X(MUS_RG_ENCOUNTER_GIRL)        \
    X(MUS_RG_ENCOUNTER_BOY)         \
    X(MUS_RG_HALL_OF_FAME)          \
    X(MUS_RG_VIRIDIAN_FOREST)       \
    X(MUS_RG_MT_MOON)               \
    X(MUS_RG_POKE_MANSION)          \
    X(MUS_RG_CREDITS)               \
    X(MUS_RG_ROUTE1)                \
    X(MUS_RG_ROUTE24)               \
    X(MUS_RG_ROUTE3)                \
    X(MUS_RG_ROUTE11)               \
    X(MUS_RG_VICTORY_ROAD)          \
    X(MUS_RG_VS_GYM_LEADER)         \
    X(MUS_RG_VS_TRAINER)            \
    X(MUS_RG_VS_WILD)               \
    X(MUS_RG_VS_CHAMPION)           \
    X(MUS_RG_PALLET)                \
    X(MUS_RG_OAK_LAB)               \
    X(MUS_RG_OAK)                   \
    X(MUS_RG_POKE_CENTER)           \
    X(MUS_RG_SS_ANNE)               \
    X(MUS_RG_SURF)                  \
    X(MUS_RG_POKE_TOWER)            \
    X(MUS_RG_SILPH)                 \
    X(MUS_RG_FUCHSIA)               \
    X(MUS_RG_CELADON)               \
    X(MUS_RG_VICTORY_TRAINER)       \
    X(MUS_RG_VICTORY_WILD)          \
    X(MUS_RG_VICTORY_GYM_LEADER)    \
    X(MUS_RG_VERMILLION)            \
    X(MUS_RG_PEWTER)                \
    X(MUS_RG_ENCOUNTER_RIVAL)       \
    X(MUS_RG_RIVAL_EXIT)            \
    X(MUS_RG_DEX_RATING)            \
    X(MUS_RG_OBTAIN_KEY_ITEM)       \
    X(MUS_RG_CAUGHT_INTRO)          \
    X(MUS_RG_PHOTO)                 \
    X(MUS_RG_GAME_FREAK)            \
    X(MUS_RG_CAUGHT)                \
    X(MUS_RG_NEW_GAME_INSTRUCT)     \
    X(MUS_RG_NEW_GAME_INTRO)        \
    X(MUS_RG_NEW_GAME_EXIT)         \
    X(MUS_RG_POKE_JUMP)             \
    X(MUS_RG_UNION_ROOM)            \
    X(MUS_RG_NET_CENTER)            \
    X(MUS_RG_MYSTERY_GIFT)          \
    X(MUS_RG_BERRY_PICK)            \
    X(MUS_RG_SEVII_CAVE)            \
    X(MUS_RG_TEACHY_TV_SHOW)        \
    X(MUS_RG_SEVII_ROUTE)           \
    X(MUS_RG_SEVII_DUNGEON)         \
    X(MUS_RG_SEVII_123)             \
    X(MUS_RG_SEVII_45)              \
    X(MUS_RG_SEVII_67)              \
    X(MUS_RG_POKE_FLUTE)            \
    X(MUS_RG_VS_DEOXYS)             \
    X(MUS_RG_VS_MEWTWO)             \
    X(MUS_RG_VS_LEGEND)             \
    X(MUS_RG_ENCOUNTER_GYM_LEADER)  \
    X(MUS_RG_ENCOUNTER_DEOXYS)      \
    X(MUS_RG_TRAINER_TOWER)         \
    X(MUS_RG_SLOW_PALLET)           \
    X(MUS_RG_TEACHY_TV_MENU)        \
    X(MUS_DP_TWINLEAF_DAY) \
    X(MUS_DP_SANDGEM_DAY) \
    X(MUS_DP_FLOAROMA_DAY) \
    X(MUS_DP_SOLACEON_DAY) \
    X(MUS_DP_ROUTE225_DAY) \
    X(MUS_DP_VALOR_LAKEFRONT_DAY) \
    X(MUS_DP_JUBILIFE_DAY) \
    X(MUS_DP_CANALAVE_DAY) \
    X(MUS_DP_OREBURGH_DAY) \
    X(MUS_DP_ETERNA_DAY) \
    X(MUS_DP_HEARTHOME_DAY) \
    X(MUS_DP_VEILSTONE_DAY) \
    X(MUS_DP_SUNYSHORE_DAY) \
    X(MUS_DP_SNOWPOINT_DAY) \
    X(MUS_DP_POKEMON_LEAGUE_DAY) \
    X(MUS_DP_FIGHT_AREA_DAY) \
    X(MUS_DP_ROUTE201_DAY) \
    X(MUS_DP_ROUTE203_DAY) \
    X(MUS_DP_ROUTE205_DAY) \
    X(MUS_DP_ROUTE206_DAY) \
    X(MUS_DP_ROUTE209_DAY) \
    X(MUS_DP_ROUTE210_DAY) \
    X(MUS_DP_ROUTE216_DAY) \
    X(MUS_DP_ROUTE228_DAY) \
    X(MUS_DP_ROWAN) \
    X(MUS_DP_TV_BROADCAST) \
    X(MUS_DP_TWINLEAF_NIGHT) \
    X(MUS_DP_SANDGEM_NIGHT) \
    X(MUS_DP_FLOAROMA_NIGHT) \
    X(MUS_DP_SOLACEON_NIGHT) \
    X(MUS_DP_ROUTE225_NIGHT) \
    X(MUS_DP_VALOR_LAKEFRONT_NIGHT) \
    X(MUS_DP_JUBILIFE_NIGHT) \
    X(MUS_DP_CANALAVE_NIGHT) \
    X(MUS_DP_OREBURGH_NIGHT) \
    X(MUS_DP_ETERNA_NIGHT) \
    X(MUS_DP_HEARTHOME_NIGHT) \
    X(MUS_DP_VEILSTONE_NIGHT) \
    X(MUS_DP_SUNYSHORE_NIGHT) \
    X(MUS_DP_SNOWPOINT_NIGHT) \
    X(MUS_DP_POKEMON_LEAGUE_NIGHT) \
    X(MUS_DP_FIGHT_AREA_NIGHT) \
    X(MUS_DP_ROUTE201_NIGHT) \
    X(MUS_DP_ROUTE203_NIGHT) \
    X(MUS_DP_ROUTE205_NIGHT) \
    X(MUS_DP_ROUTE206_NIGHT) \
    X(MUS_DP_ROUTE209_NIGHT) \
    X(MUS_DP_ROUTE210_NIGHT) \
    X(MUS_DP_ROUTE216_NIGHT) \
    X(MUS_DP_ROUTE228_NIGHT) \
    X(MUS_DP_UNDERGROUND) \
    X(MUS_DP_FLAG_CAPTURED) \
    X(MUS_DP_VICTORY_ROAD) \
    X(MUS_DP_ETERNA_FOREST) \
    X(MUS_DP_OLD_CHATEAU) \
    X(MUS_DP_LAKE_CAVERNS) \
    X(MUS_DP_AMITY_SQUARE) \
    X(MUS_DP_GALACTIC_HQ) \
    X(MUS_DP_GALACTIC_ETERNA_BUILDING) \
    X(MUS_DP_GREAT_MARSH) \
    X(MUS_DP_LAKE) \
    X(MUS_DP_MT_CORONET) \
    X(MUS_DP_SPEAR_PILLAR) \
    X(MUS_DP_STARK_MOUNTAIN) \
    X(MUS_DP_OREBURGH_GATE) \
    X(MUS_DP_OREBURGH_MINE) \
    X(MUS_DP_INSIDE_POKEMON_LEAGUE) \
    X(MUS_DP_HALL_OF_FAME_ROOM) \
    X(MUS_DP_POKE_CENTER_DAY) \
    X(MUS_DP_POKE_CENTER_NIGHT) \
    X(MUS_DP_GYM) \
    X(MUS_DP_ROWAN_LAB) \
    X(MUS_DP_CONTEST_LOBBY) \
    X(MUS_DP_POKE_MART) \
    X(MUS_DP_GAME_CORNER) \
    X(MUS_DP_B_TOWER) \
    X(MUS_DP_TV_STATION) \
    X(MUS_DP_GALACTIC_HQ_BASEMENT) \
    X(MUS_DP_AZURE_FLUTE) \
    X(MUS_DP_HALL_OF_ORIGIN) \
    X(MUS_DP_GTS) \
    X(MUS_DP_ENCOUNTER_BOY) \
    X(MUS_DP_ENCOUNTER_TWINS) \
    X(MUS_DP_ENCOUNTER_INTENSE) \
    X(MUS_DP_ENCOUNTER_GALACTIC) \
    X(MUS_DP_ENCOUNTER_LADY) \
    X(MUS_DP_ENCOUNTER_HIKER) \
    X(MUS_DP_ENCOUNTER_RICH) \
    X(MUS_DP_ENCOUNTER_SAILOR) \
    X(MUS_DP_ENCOUNTER_SUSPICIOUS) \
    X(MUS_DP_ENCOUNTER_ACE_TRAINER) \
    X(MUS_DP_ENCOUNTER_GIRL) \
    X(MUS_DP_ENCOUNTER_CYCLIST) \
    X(MUS_DP_ENCOUNTER_ARTIST) \
    X(MUS_DP_ENCOUNTER_ELITE_FOUR) \
    X(MUS_DP_ENCOUNTER_CHAMPION) \
    X(MUS_DP_VS_WILD) \
    X(MUS_DP_VS_GYM_LEADER) \
    X(MUS_DP_VS_UXIE_MESPRIT_AZELF) \
    X(MUS_DP_VS_TRAINER) \
    X(MUS_DP_VS_GALACTIC_BOSS) \
    X(MUS_DP_VS_DIALGA_PALKIA) \
    X(MUS_DP_VS_CHAMPION) \
    X(MUS_DP_VS_GALACTIC) \
    X(MUS_DP_VS_RIVAL) \
    X(MUS_DP_VS_ARCEUS) \
    X(MUS_DP_VS_LEGEND) \
    X(MUS_DP_VICTORY_WILD) \
    X(MUS_DP_VICTORY_TRAINER) \
    X(MUS_DP_VICTORY_GYM_LEADER) \
    X(MUS_DP_VICTORY_CHAMPION) \
    X(MUS_DP_VICTORY_GALACTIC) \
    X(MUS_DP_VICTORY_ELITE_FOUR) \
    X(MUS_DP_VS_GALACTIC_COMMANDER) \
    X(MUS_DP_CONTEST) \
    X(MUS_DP_VS_ELITE_FOUR) \
    X(MUS_DP_FOLLOW_ME) \
    X(MUS_DP_RIVAL) \
    X(MUS_DP_LAKE_EVENT) \
    X(MUS_DP_EVOLUTION) \
    X(MUS_DP_LUCAS) \
    X(MUS_DP_DAWN) \
    X(MUS_DP_LEGEND_APPEARS) \
    X(MUS_DP_CATASTROPHE) \
    X(MUS_DP_POKE_RADAR) \
    X(MUS_DP_SURF) \
    X(MUS_DP_CYCLING) \
    X(MUS_DP_LETS_GO_TOGETHER) \
    X(MUS_DP_TV_END) \
    X(MUS_DP_LEVEL_UP) \
    X(MUS_DP_EVOLVED) \
    X(MUS_DP_OBTAIN_KEY_ITEM) \
    X(MUS_DP_OBTAIN_ITEM) \
    X(MUS_DP_CAUGHT_INTRO) \
    X(MUS_DP_DEX_RATING) \
    X(MUS_DP_OBTAIN_BADGE) \
    X(MUS_DP_POKETCH) \
    X(MUS_DP_OBTAIN_TMHM) \
    X(MUS_DP_OBTAIN_ACCESSORY) \
    X(MUS_DP_MOVE_DELETED) \
    X(MUS_DP_HEAL) \
    X(MUS_DP_OBTAIN_BERRY) \
    X(MUS_DP_CONTEST_DRESS_UP) \
    X(MUS_DP_HALL_OF_FAME) \
    X(MUS_DP_INTRO) \
    X(MUS_DP_TITLE) \
    X(MUS_DP_MYSTERY_GIFT) \
    X(MUS_DP_WFC) \
    X(MUS_DP_DANCE_EASY) \
    X(MUS_DP_DANCE_DIFFICULT) \
    X(MUS_DP_CONTEST_RESULTS) \
    X(MUS_DP_CONTEST_WINNER) \
    X(MUS_DP_POFFINS) \
    X(MUS_DP_SLOTS_WIN) \
    X(MUS_DP_SLOTS_JACKPOT) \
    X(MUS_DP_CREDITS) \
    X(MUS_DP_SLOTS_UNUSED) \
    X(MUS_PL_FIGHT_AREA_DAY) \
    X(MUS_PL_TV_BROADCAST) \
    X(MUS_PL_TV_END) \
    X(MUS_PL_INTRO) \
    X(MUS_PL_TITLE) \
    X(MUS_PL_DISTORTION_WORLD) \
    X(MUS_PL_B_ARCADE) \
    X(MUS_PL_B_HALL) \
    X(MUS_PL_B_CASTLE) \
    X(MUS_PL_B_FACTORY) \
    X(MUS_PL_GLOBAL_TERMINAL) \
    X(MUS_PL_LILYCOVE_BOSSA_NOVA) \
    X(MUS_PL_LOOKER) \
    X(MUS_PL_VS_GIRATINA) \
    X(MUS_PL_VS_FRONTIER_BRAIN) \
    X(MUS_PL_VICTORY_FRONTIER_BRAIN) \
    X(MUS_PL_VS_REGI) \
    X(MUS_PL_CONTEST_COOL) \
    X(MUS_PL_CONTEST_SMART) \
    X(MUS_PL_CONTEST_CUTE) \
    X(MUS_PL_CONTEST_TOUGH) \
    X(MUS_PL_CONTEST_BEAUTY) \
    X(MUS_PL_SPIN_TRADE) \
    X(MUS_PL_WIFI_MINIGAMES) \
    X(MUS_PL_WIFI_PLAZA) \
    X(MUS_PL_WIFI_PARADE) \
    X(MUS_PL_GIRATINA_APPEARS_1) \
    X(MUS_PL_GIRATINA_APPEARS_2) \
    X(MUS_PL_MYSTERY_GIFT) \
    X(MUS_PL_TWINLEAF_MUSIC_BOX) \
    X(MUS_PL_OBTAIN_ARCADE_POINTS) \
    X(MUS_PL_OBTAIN_CASTLE_POINTS) \
    X(MUS_PL_OBTAIN_B_POINTS) \
    X(MUS_PL_WIN_MINIGAME) \
    X(MUS_HG_INTRO) \
    X(MUS_HG_TITLE) \
    X(MUS_HG_NEW_GAME) \
    X(MUS_HG_EVOLUTION) \
    X(MUS_HG_EVOLUTION_NO_INTRO) \
    X(MUS_HG_CYCLING) \
    X(MUS_HG_SURF) \
    X(MUS_HG_E_DENDOURIRI) \
    X(MUS_HG_CREDITS) \
    X(MUS_HG_END) \
    X(MUS_HG_NEW_BARK) \
    X(MUS_HG_CHERRYGROVE) \
    X(MUS_HG_VIOLET) \
    X(MUS_HG_AZALEA) \
    X(MUS_HG_GOLDENROD) \
    X(MUS_HG_ECRUTEAK) \
    X(MUS_HG_CIANWOOD) \
    X(MUS_HG_ROUTE29) \
    X(MUS_HG_ROUTE30) \
    X(MUS_HG_ROUTE34) \
    X(MUS_HG_ROUTE38) \
    X(MUS_HG_ROUTE42) \
    X(MUS_HG_VERMILION) \
    X(MUS_HG_PEWTER) \
    X(MUS_HG_CERULEAN) \
    X(MUS_HG_LAVENDER) \
    X(MUS_HG_CELADON) \
    X(MUS_HG_PALLET) \
    X(MUS_HG_CINNABAR) \
    X(MUS_HG_ROUTE1) \
    X(MUS_HG_ROUTE3) \
    X(MUS_HG_ROUTE11) \
    X(MUS_HG_ROUTE24) \
    X(MUS_HG_ROUTE26) \
    X(MUS_HG_POKE_CENTER) \
    X(MUS_HG_POKE_MART) \
    X(MUS_HG_GYM) \
    X(MUS_HG_ELM_LAB) \
    X(MUS_HG_OAK) \
    X(MUS_HG_DANCE_THEATER) \
    X(MUS_HG_GAME_CORNER) \
    X(MUS_HG_B_TOWER) \
    X(MUS_HG_B_TOWER_RECEPTION) \
    X(MUS_HG_SPROUT_TOWER) \
    X(MUS_HG_UNION_CAVE) \
    X(MUS_HG_RUINS_OF_ALPH) \
    X(MUS_HG_NATIONAL_PARK) \
    X(MUS_HG_BURNED_TOWER) \
    X(MUS_HG_BELL_TOWER) \
    X(MUS_HG_LIGHTHOUSE) \
    X(MUS_HG_TEAM_ROCKET_HQ) \
    X(MUS_HG_ICE_PATH) \
    X(MUS_HG_DRAGONS_DEN) \
    X(MUS_HG_ROCK_TUNNEL) \
    X(MUS_HG_VIRIDIAN_FOREST) \
    X(MUS_HG_VICTORY_ROAD) \
    X(MUS_HG_POKEMON_LEAGUE) \
    X(MUS_HG_FOLLOW_ME_1) \
    X(MUS_HG_FOLLOW_ME_2) \
    X(MUS_HG_ENCOUNTER_RIVAL) \
    X(MUS_HG_RIVAL_EXIT) \
    X(MUS_HG_BUG_CONTEST_PREP) \
    X(MUS_HG_BUG_CATCHING_CONTEST) \
    X(MUS_HG_RADIO_ROCKET) \
    X(MUS_HG_ROCKET_TAKEOVER) \
    X(MUS_HG_MAGNET_TRAIN) \
    X(MUS_HG_SS_AQUA) \
    X(MUS_HG_MT_MOON_SQUARE) \
    X(MUS_HG_RADIO_JINGLE) \
    X(MUS_HG_RADIO_LULLABY) \
    X(MUS_HG_RADIO_MARCH) \
    X(MUS_HG_RADIO_UNOWN) \
    X(MUS_HG_RADIO_POKE_FLUTE) \
    X(MUS_HG_RADIO_OAK) \
    X(MUS_HG_RADIO_BUENA) \
    X(MUS_HG_EUSINE) \
    X(MUS_HG_CLAIR) \
    X(MUS_HG_ENCOUNTER_GIRL_1) \
    X(MUS_HG_ENCOUNTER_BOY_1) \
    X(MUS_HG_ENCOUNTER_SUSPICIOUS_1) \
    X(MUS_HG_ENCOUNTER_SAGE) \
    X(MUS_HG_ENCOUNTER_KIMONO_GIRL) \
    X(MUS_HG_ENCOUNTER_ROCKET) \
    X(MUS_HG_ENCOUNTER_GIRL_2) \
    X(MUS_HG_ENCOUNTER_BOY_2) \
    X(MUS_HG_ENCOUNTER_SUSPICIOUS_2) \
    X(MUS_HG_VS_WILD) \
    X(MUS_HG_VS_TRAINER) \
    X(MUS_HG_VS_GYM_LEADER) \
    X(MUS_HG_VS_RIVAL) \
    X(MUS_HG_VS_ROCKET) \
    X(MUS_HG_VS_SUICUNE) \
    X(MUS_HG_VS_ENTEI) \
    X(MUS_HG_VS_RAIKOU) \
    X(MUS_HG_VS_CHAMPION) \
    X(MUS_HG_VS_WILD_KANTO) \
    X(MUS_HG_VS_TRAINER_KANTO) \
    X(MUS_HG_VS_GYM_LEADER_KANTO) \
    X(MUS_HG_VICTORY_TRAINER) \
    X(MUS_HG_VICTORY_WILD) \
    X(MUS_HG_CAUGHT) \
    X(MUS_HG_VICTORY_GYM_LEADER) \
    X(MUS_HG_VS_HO_OH) \
    X(MUS_HG_VS_LUGIA) \
    X(MUS_HG_POKEATHLON_LOBBY) \
    X(MUS_HG_POKEATHLON_START) \
    X(MUS_HG_POKEATHLON_BEFORE) \
    X(MUS_HG_POKEATHLON_EVENT) \
    X(MUS_HG_POKEATHLON_FINALS) \
    X(MUS_HG_POKEATHLON_RESULTS) \
    X(MUS_HG_POKEATHLON_END) \
    X(MUS_HG_POKEATHLON_WINNER) \
    X(MUS_HG_B_FACTORY) \
    X(MUS_HG_B_HALL) \
    X(MUS_HG_B_ARCADE) \
    X(MUS_HG_B_CASTLE) \
    X(MUS_HG_VS_FRONTIER_BRAIN) \
    X(MUS_HG_VICTORY_FRONTIER_BRAIN) \
    X(MUS_HG_WFC) \
    X(MUS_HG_MYSTERY_GIFT) \
    X(MUS_HG_WIFI_PLAZA) \
    X(MUS_HG_WIFI_MINIGAMES) \
    X(MUS_HG_WIFI_PARADE) \
    X(MUS_HG_GLOBAL_TERMINAL) \
    X(MUS_HG_SPIN_TRADE) \
    X(MUS_HG_GTS) \
    X(MUS_HG_ROUTE47) \
    X(MUS_HG_SAFARI_ZONE_GATE) \
    X(MUS_HG_SAFARI_ZONE) \
    X(MUS_HG_ETHAN) \
    X(MUS_HG_LYRA) \
    X(MUS_HG_GAME_CORNER_WIN) \
    X(MUS_HG_KIMONO_GIRL_DANCE) \
    X(MUS_HG_KIMONO_GIRL) \
    X(MUS_HG_HO_OH_APPEARS) \
    X(MUS_HG_LUGIA_APPEARS) \
    X(MUS_HG_SPIKY_EARED_PICHU) \
    X(MUS_HG_SINJOU_RUINS) \
    X(MUS_HG_RADIO_ROUTE101) \
    X(MUS_HG_RADIO_ROUTE201) \
    X(MUS_HG_RADIO_TRAINER) \
    X(MUS_HG_RADIO_VARIETY) \
    X(MUS_HG_VS_KYOGRE_GROUDON) \
    X(MUS_HG_POKEWALKER) \
    X(MUS_HG_VS_ARCEUS) \
    X(MUS_HG_HEAL) \
    X(MUS_HG_LEVEL_UP) \
    X(MUS_HG_OBTAIN_ITEM) \
    X(MUS_HG_OBTAIN_KEY_ITEM) \
    X(MUS_HG_EVOLVED) \
    X(MUS_HG_OBTAIN_BADGE) \
    X(MUS_HG_OBTAIN_TMHM) \
    X(MUS_HG_OBTAIN_ACCESSORY) \
    X(MUS_HG_MOVE_DELETED) \
    X(MUS_HG_OBTAIN_BERRY) \
    X(MUS_HG_DEX_RATING_1) \
    X(MUS_HG_DEX_RATING_2) \
    X(MUS_HG_DEX_RATING_3) \
    X(MUS_HG_DEX_RATING_4) \
    X(MUS_HG_DEX_RATING_5) \
    X(MUS_HG_DEX_RATING_6) \
    X(MUS_HG_OBTAIN_EGG) \
    X(MUS_HG_BUG_CONTEST_1ST_PLACE) \
    X(MUS_HG_BUG_CONTEST_2ND_PLACE) \
    X(MUS_HG_BUG_CONTEST_3RD_PLACE) \
    X(MUS_HG_CARD_FLIP) \
    X(MUS_HG_CARD_FLIP_GAME_OVER) \
    X(MUS_HG_POKEGEAR_REGISTERED) \
    X(MUS_HG_LETS_GO_TOGETHER) \
    X(MUS_HG_POKEATHLON_READY) \
    X(MUS_HG_POKEATHLON_1ST_PLACE) \
    X(MUS_HG_RECEIVE_POKEMON) \
    X(MUS_HG_OBTAIN_ARCADE_POINTS) \
    X(MUS_HG_OBTAIN_CASTLE_POINTS) \
    X(MUS_HG_OBTAIN_B_POINTS) \
    X(MUS_HG_WIN_MINIGAME) \
    X(MUS_BW12_001) \
    X(MUS_BW12_002) \
    X(MUS_BW12_003) \
    X(MUS_BW12_004) \
    X(MUS_BW12_005) \
    X(MUS_BW12_006) \
    X(MUS_BW12_007) \
    X(MUS_BW12_008) \
    X(MUS_BW12_009) \
    X(MUS_BW12_VS_ELITE_FOUR) \
    X(MUS_BW12_VS_CHEREN_BIANCA) \
    X(MUS_BW12_VS_WILD_STRONG) \
    X(MUS_BW12_013) \
    X(MUS_BW12_014) \
    X(MUS_BW12_015) \
    X(MUS_BW12_016) \
    X(MUS_BW12_GYM) \
    X(MUS_BW12_UNWAVERING_EMOTIONS) \
    X(MUS_BW12_THE_POKEMON_1) \
    X(MUS_BW12_TITLE_SCREEN) \
    X(MUS_BW12_021) \
    X(MUS_BW12_022) \
    X(MUS_BW12_023) \
    X(MUS_BW12_024) \
    X(MUS_BW12_025) \
    X(MUS_BW12_026) \
    X(MUS_BW12_027) \
    X(MUS_BW12_028) \
    X(MUS_BW12_029) \
    X(MUS_BW12_030) \
    X(MUS_BW12_031) \
    X(MUS_BW12_032) \
    X(MUS_BW12_033) \
    X(MUS_BW12_ROUTE_2) \
    X(MUS_BW12_035) \
    X(MUS_BW12_036) \
    X(MUS_BW12_VS_GYM_LEADER_1) \
    X(MUS_BW12_VS_TRAINER_1) \
    X(MUS_BW12_VS_WILD_1) \
    X(MUS_BW12_VS_ALDER) \
    X(MUS_BW12_041) \
    X(MUS_BW12_042) \
    X(MUS_BW12_043) \
    X(MUS_BW12_POKEMON_CENTER) \
    X(MUS_BW12_045) \
    X(MUS_BW12_046) \
    X(MUS_BW12_047) \
    X(MUS_BW12_048) \
    X(MUS_BW12_049) \
    X(MUS_BW12_050) \
    X(MUS_BW12_051) \
    X(MUS_BW12_052) \
    X(MUS_BW12_053) \
    X(MUS_BW12_054) \
    X(MUS_BW12_055) \
    X(MUS_BW12_056) \
    X(MUS_BW12_057) \
    X(MUS_BW12_058) \
    X(MUS_BW12_059) \
    X(MUS_BW12_GAMEFREAK) \
    X(MUS_BW12_061) \
    X(MUS_BW12_062) \
    X(MUS_BW12_063) \
    X(MUS_BW12_064) \
    X(MUS_BW12_065) \
    X(MUS_BW12_066) \
    X(MUS_BW12_067) \
    X(MUS_BW12_068) \
    X(MUS_BW12_069) \
    X(MUS_BW12_070) \
    X(MUS_BW12_071) \
    X(MUS_BW12_072) \
    X(MUS_BW12_073) \
    X(MUS_BW12_VS_ZEKROM) \
    X(MUS_BW12_VS_RESHIRAM) \
    X(MUS_BW12_VS_LEGENDARY) \
    X(MUS_BW12_077) \
    X(MUS_BW12_078) \
    X(MUS_BW12_VS_PWC_FINAL) \
    X(MUS_BW12_080) \
    X(MUS_BW12_081) \
    X(MUS_BW12_VS_TRAINER_SUBWAY) \
    X(MUS_BW12_VS_N) \
    X(MUS_BW12_VS_GHETSIS_1) \
    X(MUS_BW12_085) \
    X(MUS_BW12_086) \
    X(MUS_BW12_087) \
    X(MUS_BW12_088) \
    X(MUS_BW12_BLACK_CITY) \
    X(MUS_BW12_WHITE_FOREST) \
    X(MUS_BW12_091) \
    X(MUS_BW12_092) \
    X(MUS_BW12_VS_GYM_LEADER_WINNING_1) \
    X(MUS_BW12_094) \
    X(MUS_BW12_095) \
    X(MUS_BW12_096) \
    X(MUS_BW12_097) \
    X(MUS_BW12_098) \
    X(MUS_BW12_099) \
    X(MUS_BW12_100) \
    X(MUS_BW12_101) \
    X(MUS_BW12_102) \
    X(MUS_BW12_103) \
    X(MUS_BW12_104) \
    X(MUS_BW12_105) \
    X(MUS_BW12_106) \
    X(MUS_BW12_107) \
    X(MUS_BW12_VS_IN_DANGER) \
    X(MUS_BW12_109) \
    X(MUS_BW12_110) \
    X(MUS_BW12_111) \
    X(MUS_BW12_112) \
    X(MUS_BW12_113) \
    X(MUS_BW12_114) \
    X(MUS_BW12_VS_N_DECISIVE_BATTLE) \
    X(MUS_BW12_116) \
    X(MUS_BW12_117) \
    X(MUS_BW12_118) \
    X(MUS_BW12_119) \
    X(MUS_BW12_120) \
    X(MUS_BW12_121) \
    X(MUS_BW12_CYNTIA_INTRO) \
    X(MUS_BW12_VS_CYNTIA) \
    X(MUS_BW12_124) \
    X(MUS_BW12_VS_WILD_2) \
    X(MUS_BW12_VS_TRAINER_2) \
    X(MUS_BW12_127) \
    X(MUS_BW12_VS_GYM_LEADER_2) \
    X(MUS_BW12_VS_PLASMA_GRUNT_2) \
    X(MUS_BW12_130) \
    X(MUS_BW12_131) \
    X(MUS_BW12_VS_COLRESS) \
    X(MUS_BW12_VS_HUGH) \
    X(MUS_BW12_134) \
    X(MUS_BW12_135) \
    X(MUS_BW12_ENTRALINK) \
    X(MUS_BW12_137) \
    X(MUS_BW12_OPENING_2) \
    X(MUS_BW12_THE_POKEMON_2) \
    X(MUS_BW12_140) \
    X(MUS_BW12_141) \
    X(MUS_BW12_VS_GHETSIS_2) \
    X(MUS_BW12_VS_BW_KYUREM) \
    X(MUS_BW12_144) \
    X(MUS_BW12_145) \
    X(MUS_BW12_146) \
    X(MUS_BW12_147) \
    X(MUS_BW12_148) \
    X(MUS_BW12_VS_PWT_FINAL) \
    X(MUS_BW12_150) \
    X(MUS_BW12_151) \
    X(MUS_BW12_152) \
    X(MUS_BW12_153) \
    X(MUS_BW12_VS_REGI) \
    X(MUS_BW12_REGI_CHAMBER) \
    X(MUS_BW12_VS_PLASMA_GRUNT_1) \
    X(MUS_BW12_PWT) \
    X(MUS_BW12_VS_PWT_KANTO_LEADER) \
    X(MUS_BW12_VS_PWT_KANTO_CHAMPION) \
    X(MUS_BW12_VS_PWT_JOHTO_LEADER) \
    X(MUS_BW12_VS_PWT_JOHTO_CHAMPION) \
    X(MUS_BW12_VS_PWT_HOENN_LEADER) \
    X(MUS_BW12_VS_PWT_HOENN_CHAMPION) \
    X(MUS_BW12_VS_PWT_SINNOH_LEADER) \
    X(MUS_BW12_VS_PWT_SINNOH_CHAMPION) \
    X(MUS_BW12_VS_IRIS) \
    X(MUS_BW12_167) \
    X(MUS_BW12_168) \
    X(MUS_BW12_169) \
    X(MUS_BW12_VS_GYM_LEADER_WINNING_2) \
    X(MUS_BW12_171) \
    X(MUS_BW12_172) \
    X(MUS_BW12_173) \
    X(MUS_BW12_174) \
    X(MUS_BW12_POKEWOOD) \
    X(MUS_BW12_176) \
    X(MUS_BW12_177) \
    X(MUS_BW12_178) \
    X(MUS_BW12_179) \
    X(MUS_BW12_180) \
    X(MUS_BW12_181) \
    X(MUS_BW12_GYM_MISTRALTON) \
    X(MUS_BW12_GYM_OPELUCID) \
    X(MUS_BW12_ASSAULT_OPELUCID_CITY) \
    X(MUS_BW12_FROZEN_TOWN) \
    X(MUS_BW12_186) \
    X(MUS_BW12_187) \
    X(MUS_BW12_188) \
    X(MUS_BW12_189) \
    X(MUS_BW12_190) \
    X(MUS_BW12_191) \
    X(MUS_BW12_192) \
    X(MUS_BW12_193) \
    X(MUS_BW12_GYM_VIRBANK) \
    X(MUS_BW12_195) \
    X(MUS_BW12_196) \
    X(MUS_BW12_197) \
    X(MUS_BW12_GYM_HUMILAU) \
    X(MUS_BW12_199) \
    X(MUS_BW12_GYM_NIMBASA_RUNAWAY) \
    X(MUS_BW12_GYM_NIMBASA_STAGE) \
    X(MUS_BW12_SPOTTED_BEAUTY) \
    X(MUS_BW12_CREDITS) \
    X(MUS_BW12_CREDITS_END) \
    X(MUS_BW12_PWT_VENUE) \
    X(PMD_HABITAT_HOME01) \
    X(PMD_HABITAT_DARK_01) \
    X(PMD_HABITAT_MYSTIC_01) \
    X(PMD_EVENT_DREAM_01) \
    X(PMD_EVENT_DREAM_02) \
    X(PMD_EVENT_LEGEND_01) \
    X(PMD_HABITAT_HIROBA) \
    X(PMD_HABITAT_HOME_SHINKA) \
    X(PMD_EVENT_FEAR) \
    X(PMD_EVENT_KINPAKU) \
    X(PMD_BOSS01) \
    X(PMD_EVENT_YUME) \
    X(PMD_DUNGEON_GENERAL_01) \
    X(PMD_DUNGEON_DENJIHA_01) \
    X(PMD_DUNGEON_AYASHIIMORI_01) \
    X(PMD_HABITAT_CALMLY_02) \
    X(PMD_SYS_SHOP) \
    X(PMD_SYS_STEAL) \
    X(PMD_EVENT_DISASTER) \
    X(PMD_DUNGEON_KYOUKOKU_01) \
    X(PMD_DUNGEON_GENERAL_02) \
    X(PMD_DUNGEON_TENKUU_01) \
    X(PMD_DUNGEON_TENKUU_02) \
    X(PMD_EVENT_ESCAPE_01) \
    X(PMD_DUNGEON_HONOU_01) \
    X(PMD_DUNGEON_EVENTFLOOR_02) \
    X(PMD_HABITAT_ITADAKI_01) \
    X(PMD_HABITAT_NIJI_01) \
    X(PMD_EVENT_ENV_MYSTIC) \
    X(PMD_HABITAT_KAIRYU_01) \
    X(PMD_HABITAT_KAITEI_01) \
    X(PMD_BOSS02) \
    X(PMD_DUNGEON_HONOU_02) \
    X(PMD_HABITAT_DAICHI_01) \
    X(PMD_HABITAT_NAZO_01) \
    X(PMD_EVENT_ESCAPE_02) \
    X(PMD_EVENT_ENV_SOUL) \
    X(PMD_EVENT_ENV_HONOU) \
    X(PMD_DUNGEON_JUHYOU_02) \
    X(PMD_SYS_TITLE_01) \
    X(PMD_EVENT_SEPARATION_01) \
    X(PMD_EVENT_SEPARATION_02) \
    X(PMD_SYS_TITLE_02) \
    X(PMD_SYS_ENDING_01) \
    X(PMD_SYS_ENDING_02) \
    X(PMD_SYS_TITLE_03) \
    X(PMD_SAVE_01) \
    X(PMD_GAMEOVER) \
    X(PMD_GAMECLEAR) \
    X(PMD_EVENT_ZUKKOKE) \
    X(PMD_EVENT_CALMLY) \
    X(PMD_DUNGEON_GUNJYOU_01) \
    X(PMD_EVENT_CLEARD) \
    X(PMD_DUNGEON_JUHYOU_01) \
    X(PMD_HABITAT_CALMLY_01) \
    X(PMD_HABITAT_MYSTIC_02) \
    X(PMD_HABITAT_CALMLY_03) \
    X(PMD_DUNGEON_CHITEI_01) \
    X(PMD_SYS_TRAINING) \
    X(PMD_DUNGEON_TUTORIAL_01) \
    X(PMD_DUNGEON_RAIMEI_01) \
    X(PMD_HABITAT_MYSTIC_06) \
    X(PMD_DUNGEON_CHINMOKU_01) \
    X(PMD_DUNGEON_EVENTFLOOR_01) \
    X(PMD_DUNGEON_REIHOU_01) \
    X(PMD_HABITAT_MYSTIC_03) \
    X(PMD_HABITAT_DENSETSU_01) \
    X(PMD_HABITAT_MINAMI_01) \
    X(PMD_HABITAT_TOZASARETA_01) \
    X(PMD_DUNGEON_HAGANE_01) \
    X(PMD_HABITAT_MYSTIC_04) \
    X(PMD_HABITAT_SAIGO_01) \
    X(PMD_DUNGEON_REIHOU_02) \
    X(PMD_DUNGEON_CHITEI_02) \
    X(PMD_DUNGEON_CHIISANAMORI_01) \
    X(PMD_DUNGEON_RAIMEI_02) \
    X(PMD_HABITAT_IYASHI_01) \
    X(PMD_SYS_MONSTER) \
    X(PMD_LEVELUP) \
    X(PMD_RANKUP) \
    X(PMD_SEEYOU) \
    X(PMD_REWARD) \
    X(PMD_PARTY) \
    X(PMD_DUNGEON_OPEN) \
    X(PMD_AREA_OPEN) \
    X(PMD_INFORMATION) \
    X(PMD_FANFARE) \
    X(PMD_EVOLUTION) \
    X(PMD_EVOLUTION_02) \
    X(PMD_ITEM_IMPORTANT) \
    X(PMD_NEWS) \
    X(PMD_MUSICBOX) \
    X(PMD_EFF_ORUGORU) \
    X(PMD_WIND_S) \
    X(PMD_WIND_M) \
    X(PMD_WAVE_S) \
    X(PMD_WAVE_M) \
    X(PMD_SYS_01) \
    X(PMD_SYS_02) \
    X(PMD_SYS_03) \
    X(PMD_SYS_04) \
    X(PMD_SYS_05) \
    X(PMD_SYS_06) \
    X(PMD_SYS_07) \
    X(PMD_SYS_08) \
    X(PMD_SYS_09) \
    X(PMD_SYS_10) \
    X(PMD_DS_SYS_01) \
    X(PMD_WAZA) \
    X(PMD_WAZA_CLEAR) \
    X(PMD_ITEM_M) \
    X(PMD_ITEM_M_03) \
    X(PMD_MOTION_MONEY) \
    X(PMD_MOTION_ITEM) \
    X(PMD_MOTION_EAT) \
    X(PMD_MOTION_THROW) \
    X(PMD_MOTION_THROW_02) \
    X(PMD_MOTION_THROW_03) \
    X(PMD_MOTION_HUNGRY) \
    X(PMD_MOTION_STAIRS_UP) \
    X(PMD_MOTION_STAIRS_DOWN) \
    X(PMD_MISS_PLAYER_M) \
    X(PMD_MISS_ENEMY_M) \
    X(PMD_TAIATARI_M) \
    X(PMD_MOTION_BREAK) \
    X(PMD_MOTION_DOOR_GENERAL) \
    X(PMD_MOTION_DOOR_BIG) \
    X(PMD_MOTION_SWITCH) \
    X(PMD_EFF_ANIM_2) \
    X(PMD_EFF_ANIM_8) \
    X(PMD_EFF_ANIM_9) \
    X(PMD_EFF_ANIM_10) \
    X(PMD_EFF_ANIM_11) \
    X(PMD_EFF_ANIM_12) \
    X(PMD_EFF_ANIM_4_SCRATCH) \
    X(PMD_EFF_ANIM_4_LICK) \
    X(PMD_EFF_ANIM_4_GORE) \
    X(PMD_EFF_ANIM_4_ATTACK) \
    X(PMD_EFF_ANIM_4_HEEL_DOWN) \
    X(PMD_EFF_ANIM_4_WAG_TAIL) \
    X(PMD_EFF_ANIM_4_PAWUP) \
    X(PMD_EFF_ANIM_4_ELECTRICITY) \
    X(PMD_EFF_ANIM_4_WING_S) \
    X(PMD_EFF_ANIM_4_WING_M) \
    X(PMD_EFF_ANIM_4_WING_L) \
    X(PMD_EFF_ANIM_4_SHAKE) \
    X(PMD_EFF_ANIM_4_JUMP) \
    X(PMD_EFF_ANIM_4_ATTACK_LIGHT) \
    X(PMD_MOTION_WIND_LARGE) \
    X(PMD_MOTION_WAVE_LARGE) \
    X(PMD_THUNDER_M) \
    X(PMD_WIND_M_2) \
    X(PMD_ESP_M) \
    X(PMD_TRAP_TUMBLE) \
    X(PMD_TRAP_SCISSOR) \
    X(PMD_TRAP_BETOBETA) \
    X(PMD_TRAP_SIREN) \
    X(PMD_TRAP_SUMMON) \
    X(PMD_TRAP_HOLE) \
    X(PMD_TRAP_TURNTABLE) \
    X(PMD_TRAP_SLEEP) \
    X(PMD_TRAP_SEAL) \
    X(PMD_TRAP_MINE) \
    X(PMD_TRAP_BIGMINE) \
    X(PMD_STATE_SHIELD) \
    X(PMD_STATE_QUESTION) \
    X(PMD_STATE_SWING) \
    X(PMD_STATE_STIFF) \
    X(PMD_STATE_INVISIBLE) \
    X(PMD_STATE_BLIND_EYE) \
    X(PMD_STATE_DRY_UP) \
    X(PMD_STATE_PULL) \
    X(PMD_STATE_THROW) \
    X(PMD_STATE_QUAKE) \
    X(PMD_STATE_JUMP) \
    X(PMD_STATE_LV_DOWN) \
    X(PMD_STATE_REVIVE) \
    X(PMD_STATE_ESCAPE) \
    X(PMD_WEATHER_CLOUDY) \
    X(PMD_WEATHER_FOG) \
    X(PMD_EVENT_FLY_01_L) \
    X(PMD_EVENT_FLY_02_L) \
    X(PMD_EVENT_FLY_03) \
    X(PMD_EVENT_EARTHQUAKE_01_L) \
    X(PMD_EVENT_MOTION_APPEAR_01) \
    X(PMD_EVENT_MOTION_JUMP_01) \
    X(PMD_EVENT_MOTION_JUMP_05) \
    X(PMD_EVENT_MOTION_JUMP_02) \
    X(PMD_EVENT_MOTION_JUMP_04) \
    X(PMD_EVENT_FLYAWAY_01) \
    X(PMD_EVENT_FLY_FIRE_01) \
    X(PMD_EVENT_SIGN_LAUGH_01) \
    X(PMD_EVENT_SIGN_LAUGH_01_L) \
    X(PMD_EVENT_SIGN_NOTICE_01) \
    X(PMD_EVENT_SIGN_NOTICE_02) \
    X(PMD_EVENT_SIGN_NOTICE_04) \
    X(PMD_EVENT_SIGN_NOTICE_03) \
    X(PMD_EVENT_SIGN_HATENA_01) \
    X(PMD_EVENT_SIGN_HATENA_02) \
    X(PMD_EVENT_SIGN_HATENA_03) \
    X(PMD_EVENT_SIGN_ASE_01) \
    X(PMD_EVENT_SIGN_ANGER_01) \
    X(PMD_EVENT_SIGN_ANGER_02) \
    X(PMD_EVENT_SIGN_SHOCK_01) \
    X(PMD_EVENT_MOTION_QUACK_01) \
    X(PMD_EVENT_MOTION_FEATHER_01) \
    X(PMD_EVENT_EFF_DIZZINESS_01) \
    X(PMD_EVENT_APPEAR_02) \
    X(PMD_EVENT_MOTION_JUMP_03) \
    X(PMD_EVENT_FLASH_02) \
    X(PMD_EVENT_SNEEZE) \
    X(PMD_EVENT_LIGHT_02) \
    X(PMD_EVENT_SIGN_SHOCK_02) \
    X(PMD_EVENT_FALL_01) \
    X(PMD_EVENT_LIGHT_03_L) \
    X(PMD_EVENT_INTERFERE_01) \
    X(PMD_EVENT_MOTION_HOLEUP_01) \
    X(PMD_EVENT_WHITEOUT_03) \
    X(PMD_EVENT_SHOOTINGSTAR) \
    X(PMD_EVENT_KESSYOU) \
    X(PMD_EVENT_FLY_09) \
    X(PMD_EVENT_FLASH_02_L) \
    X(PMD_EVENT_FLASH_03) \
    X(PMD_EVENT_FLASH_06) \
    X(PMD_EVENT_FLASH_04) \
    X(PMD_EVENT_FREEZE) \
    X(PMD_EVENT_DAMAGE_01) \
    X(PMD_EVENT_EARTHQUAKE_02) \
    X(PMD_EVENT_APPEAR_03) \
    X(PMD_EVENT_TELEPORT) \
    X(PMD_EVENT_POST) \
    X(PMD_EVENT_FLASH_01) \
    X(PMD_EVENT_FOOTSTEP_01) \
    X(PMD_EVENT_FLY_04) \
    X(PMD_EVENT_FOOTSTEP_02) \
    X(PMD_EVENT_WHITEOUT) \
    X(PMD_EVENT_WHITEOUT_04) \
    X(PMD_EVENT_SHOOTING_L) \
    X(PMD_EVENT_FALL_02) \
    X(PMD_EVENT_BOUND) \
    X(PMD_EVENT_LIGHTBALL_01) \
    X(PMD_EVENT_EARTHQUAKE_03_L) \
    X(PMD_EVENT_HAKAIKOUSEN) \
    X(PMD_EVENT_LIGHTBALL_02) \
    X(PMD_EVENT_MOTION_HARAHERI) \
    X(PMD_EVENT_MOTION_EAT_L) \
    X(PMD_EVENT_EXPLOSION_01) \
    X(PMD_EVENT_EFF_RAIN)
// End of MUSIC_EXPANSION_GEN5_MUSIC_REDUCE_SIZE == FALSE

// Start of MUSIC_EXPANSION_GEN5_MUSIC_REDUCE_SIZE == TRUE
#define SOUND_LIST_BGM_GEN_V_REDUCED              \
    X(MUS_LITTLEROOT_TEST)          \
    X(MUS_GSC_ROUTE38)              \
    X(MUS_CAUGHT)                   \
    X(MUS_VICTORY_WILD)             \
    X(MUS_VICTORY_GYM_LEADER)       \
    X(MUS_VICTORY_LEAGUE)           \
    X(MUS_C_COMM_CENTER)            \
    X(MUS_GSC_PEWTER)               \
    X(MUS_C_VS_LEGEND_BEAST)        \
    X(MUS_ROUTE101)                 \
    X(MUS_ROUTE110)                 \
    X(MUS_ROUTE120)                 \
    X(MUS_PETALBURG)                \
    X(MUS_OLDALE)                   \
    X(MUS_GYM)                      \
    X(MUS_SURF)                     \
    X(MUS_PETALBURG_WOODS)          \
    X(MUS_LEVEL_UP)                 \
    X(MUS_HEAL)                     \
    X(MUS_OBTAIN_BADGE)             \
    X(MUS_OBTAIN_ITEM)              \
    X(MUS_EVOLVED)                  \
    X(MUS_OBTAIN_TMHM)              \
    X(MUS_LILYCOVE_MUSEUM)          \
    X(MUS_ROUTE122)                 \
    X(MUS_OCEANIC_MUSEUM)           \
    X(MUS_EVOLUTION_INTRO)          \
    X(MUS_EVOLUTION)                \
    X(MUS_MOVE_DELETED)             \
    X(MUS_ENCOUNTER_GIRL)           \
    X(MUS_ENCOUNTER_MALE)           \
    X(MUS_ABANDONED_SHIP)           \
    X(MUS_FORTREE)                  \
    X(MUS_BIRCH_LAB)                \
    X(MUS_B_TOWER_RS)               \
    X(MUS_ENCOUNTER_SWIMMER)        \
    X(MUS_CAVE_OF_ORIGIN)           \
    X(MUS_OBTAIN_BERRY)             \
    X(MUS_AWAKEN_LEGEND)            \
    X(MUS_SLOTS_JACKPOT)            \
    X(MUS_SLOTS_WIN)                \
    X(MUS_TOO_BAD)                  \
    X(MUS_ROULETTE)                 \
    X(MUS_LINK_CONTEST_P1)          \
    X(MUS_LINK_CONTEST_P2)          \
    X(MUS_LINK_CONTEST_P3)          \
    X(MUS_LINK_CONTEST_P4)          \
    X(MUS_ENCOUNTER_RICH)           \
    X(MUS_VERDANTURF)               \
    X(MUS_RUSTBORO)                 \
    X(MUS_POKE_CENTER)              \
    X(MUS_ROUTE104)                 \
    X(MUS_ROUTE119)                 \
    X(MUS_CYCLING)                  \
    X(MUS_POKE_MART)                \
    X(MUS_LITTLEROOT)               \
    X(MUS_MT_CHIMNEY)               \
    X(MUS_ENCOUNTER_FEMALE)         \
    X(MUS_LILYCOVE)                 \
    X(MUS_DESERT)                   \
    X(MUS_HELP)                     \
    X(MUS_UNDERWATER)               \
    X(MUS_VICTORY_TRAINER)          \
    X(MUS_TITLE)                    \
    X(MUS_INTRO)                    \
    X(MUS_ENCOUNTER_MAY)            \
    X(MUS_ENCOUNTER_INTENSE)        \
    X(MUS_ENCOUNTER_COOL)           \
    X(MUS_ROUTE113)                 \
    X(MUS_ENCOUNTER_AQUA)           \
    X(MUS_FOLLOW_ME)                \
    X(MUS_ENCOUNTER_BRENDAN)        \
    X(MUS_EVER_GRANDE)              \
    X(MUS_ENCOUNTER_SUSPICIOUS)     \
    X(MUS_VICTORY_AQUA_MAGMA)       \
    X(MUS_CABLE_CAR)                \
    X(MUS_GAME_CORNER)              \
    X(MUS_DEWFORD)                  \
    X(MUS_SAFARI_ZONE)              \
    X(MUS_VICTORY_ROAD)             \
    X(MUS_AQUA_MAGMA_HIDEOUT)       \
    X(MUS_SAILING)                  \
    X(MUS_MT_PYRE)                  \
    X(MUS_SLATEPORT)                \
    X(MUS_MT_PYRE_EXTERIOR)         \
    X(MUS_SCHOOL)                   \
    X(MUS_HALL_OF_FAME)             \
    X(MUS_FALLARBOR)                \
    X(MUS_SEALED_CHAMBER)           \
    X(MUS_CONTEST_WINNER)           \
    X(MUS_CONTEST)                  \
    X(MUS_ENCOUNTER_MAGMA)          \
    X(MUS_INTRO_BATTLE)             \
    X(MUS_ABNORMAL_WEATHER)         \
    X(MUS_WEATHER_GROUDON)          \
    X(MUS_SOOTOPOLIS)               \
    X(MUS_CONTEST_RESULTS)          \
    X(MUS_HALL_OF_FAME_ROOM)        \
    X(MUS_TRICK_HOUSE)              \
    X(MUS_ENCOUNTER_TWINS)          \
    X(MUS_ENCOUNTER_ELITE_FOUR)     \
    X(MUS_ENCOUNTER_HIKER)          \
    X(MUS_CONTEST_LOBBY)            \
    X(MUS_ENCOUNTER_INTERVIEWER)    \
    X(MUS_ENCOUNTER_CHAMPION)       \
    X(MUS_CREDITS)                  \
    X(MUS_END)                      \
    X(MUS_B_FRONTIER)               \
    X(MUS_B_ARENA)                  \
    X(MUS_OBTAIN_B_POINTS)          \
    X(MUS_REGISTER_MATCH_CALL)      \
    X(MUS_B_PYRAMID)                \
    X(MUS_B_PYRAMID_TOP)            \
    X(MUS_B_PALACE)                 \
    X(MUS_RAYQUAZA_APPEARS)         \
    X(MUS_B_TOWER)                  \
    X(MUS_OBTAIN_SYMBOL)            \
    X(MUS_B_DOME)                   \
    X(MUS_B_PIKE)                   \
    X(MUS_B_FACTORY)                \
    X(MUS_VS_RAYQUAZA)              \
    X(MUS_VS_FRONTIER_BRAIN)        \
    X(MUS_VS_MEW)                   \
    X(MUS_B_DOME_LOBBY)             \
    X(MUS_VS_WILD)                  \
    X(MUS_VS_AQUA_MAGMA)            \
    X(MUS_VS_TRAINER)               \
    X(MUS_VS_GYM_LEADER)            \
    X(MUS_VS_CHAMPION)              \
    X(MUS_VS_REGI)                  \
    X(MUS_VS_KYOGRE_GROUDON)        \
    X(MUS_VS_RIVAL)                 \
    X(MUS_VS_ELITE_FOUR)            \
    X(MUS_VS_AQUA_MAGMA_LEADER)     \
    X(MUS_RG_FOLLOW_ME)             \
    X(MUS_RG_GAME_CORNER)           \
    X(MUS_RG_ROCKET_HIDEOUT)        \
    X(MUS_RG_GYM)                   \
    X(MUS_RG_JIGGLYPUFF)            \
    X(MUS_RG_INTRO_FIGHT)           \
    X(MUS_RG_TITLE)                 \
    X(MUS_RG_CINNABAR)              \
    X(MUS_RG_LAVENDER)              \
    X(MUS_RG_HEAL)                  \
    X(MUS_RG_CYCLING)               \
    X(MUS_RG_ENCOUNTER_ROCKET)      \
    X(MUS_RG_ENCOUNTER_GIRL)        \
    X(MUS_RG_ENCOUNTER_BOY)         \
    X(MUS_RG_HALL_OF_FAME)          \
    X(MUS_RG_VIRIDIAN_FOREST)       \
    X(MUS_RG_MT_MOON)               \
    X(MUS_RG_POKE_MANSION)          \
    X(MUS_RG_CREDITS)               \
    X(MUS_RG_ROUTE1)                \
    X(MUS_RG_ROUTE24)               \
    X(MUS_RG_ROUTE3)                \
    X(MUS_RG_ROUTE11)               \
    X(MUS_RG_VICTORY_ROAD)          \
    X(MUS_RG_VS_GYM_LEADER)         \
    X(MUS_RG_VS_TRAINER)            \
    X(MUS_RG_VS_WILD)               \
    X(MUS_RG_VS_CHAMPION)           \
    X(MUS_RG_PALLET)                \
    X(MUS_RG_OAK_LAB)               \
    X(MUS_RG_OAK)                   \
    X(MUS_RG_POKE_CENTER)           \
    X(MUS_RG_SS_ANNE)               \
    X(MUS_RG_SURF)                  \
    X(MUS_RG_POKE_TOWER)            \
    X(MUS_RG_SILPH)                 \
    X(MUS_RG_FUCHSIA)               \
    X(MUS_RG_CELADON)               \
    X(MUS_RG_VICTORY_TRAINER)       \
    X(MUS_RG_VICTORY_WILD)          \
    X(MUS_RG_VICTORY_GYM_LEADER)    \
    X(MUS_RG_VERMILLION)            \
    X(MUS_RG_PEWTER)                \
    X(MUS_RG_ENCOUNTER_RIVAL)       \
    X(MUS_RG_RIVAL_EXIT)            \
    X(MUS_RG_DEX_RATING)            \
    X(MUS_RG_OBTAIN_KEY_ITEM)       \
    X(MUS_RG_CAUGHT_INTRO)          \
    X(MUS_RG_PHOTO)                 \
    X(MUS_RG_GAME_FREAK)            \
    X(MUS_RG_CAUGHT)                \
    X(MUS_RG_NEW_GAME_INSTRUCT)     \
    X(MUS_RG_NEW_GAME_INTRO)        \
    X(MUS_RG_NEW_GAME_EXIT)         \
    X(MUS_RG_POKE_JUMP)             \
    X(MUS_RG_UNION_ROOM)            \
    X(MUS_RG_NET_CENTER)            \
    X(MUS_RG_MYSTERY_GIFT)          \
    X(MUS_RG_BERRY_PICK)            \
    X(MUS_RG_SEVII_CAVE)            \
    X(MUS_RG_TEACHY_TV_SHOW)        \
    X(MUS_RG_SEVII_ROUTE)           \
    X(MUS_RG_SEVII_DUNGEON)         \
    X(MUS_RG_SEVII_123)             \
    X(MUS_RG_SEVII_45)              \
    X(MUS_RG_SEVII_67)              \
    X(MUS_RG_POKE_FLUTE)            \
    X(MUS_RG_VS_DEOXYS)             \
    X(MUS_RG_VS_MEWTWO)             \
    X(MUS_RG_VS_LEGEND)             \
    X(MUS_RG_ENCOUNTER_GYM_LEADER)  \
    X(MUS_RG_ENCOUNTER_DEOXYS)      \
    X(MUS_RG_TRAINER_TOWER)         \
    X(MUS_RG_SLOW_PALLET)           \
    X(MUS_RG_TEACHY_TV_MENU)        \
    X(MUS_DP_TWINLEAF_DAY) \
    X(MUS_DP_SANDGEM_DAY) \
    X(MUS_DP_FLOAROMA_DAY) \
    X(MUS_DP_SOLACEON_DAY) \
    X(MUS_DP_ROUTE225_DAY) \
    X(MUS_DP_VALOR_LAKEFRONT_DAY) \
    X(MUS_DP_JUBILIFE_DAY) \
    X(MUS_DP_CANALAVE_DAY) \
    X(MUS_DP_OREBURGH_DAY) \
    X(MUS_DP_ETERNA_DAY) \
    X(MUS_DP_HEARTHOME_DAY) \
    X(MUS_DP_VEILSTONE_DAY) \
    X(MUS_DP_SUNYSHORE_DAY) \
    X(MUS_DP_SNOWPOINT_DAY) \
    X(MUS_DP_POKEMON_LEAGUE_DAY) \
    X(MUS_DP_FIGHT_AREA_DAY) \
    X(MUS_DP_ROUTE201_DAY) \
    X(MUS_DP_ROUTE203_DAY) \
    X(MUS_DP_ROUTE205_DAY) \
    X(MUS_DP_ROUTE206_DAY) \
    X(MUS_DP_ROUTE209_DAY) \
    X(MUS_DP_ROUTE210_DAY) \
    X(MUS_DP_ROUTE216_DAY) \
    X(MUS_DP_ROUTE228_DAY) \
    X(MUS_DP_ROWAN) \
    X(MUS_DP_TV_BROADCAST) \
    X(MUS_DP_TWINLEAF_NIGHT) \
    X(MUS_DP_SANDGEM_NIGHT) \
    X(MUS_DP_FLOAROMA_NIGHT) \
    X(MUS_DP_SOLACEON_NIGHT) \
    X(MUS_DP_ROUTE225_NIGHT) \
    X(MUS_DP_VALOR_LAKEFRONT_NIGHT) \
    X(MUS_DP_JUBILIFE_NIGHT) \
    X(MUS_DP_CANALAVE_NIGHT) \
    X(MUS_DP_OREBURGH_NIGHT) \
    X(MUS_DP_ETERNA_NIGHT) \
    X(MUS_DP_HEARTHOME_NIGHT) \
    X(MUS_DP_VEILSTONE_NIGHT) \
    X(MUS_DP_SUNYSHORE_NIGHT) \
    X(MUS_DP_SNOWPOINT_NIGHT) \
    X(MUS_DP_POKEMON_LEAGUE_NIGHT) \
    X(MUS_DP_FIGHT_AREA_NIGHT) \
    X(MUS_DP_ROUTE201_NIGHT) \
    X(MUS_DP_ROUTE203_NIGHT) \
    X(MUS_DP_ROUTE205_NIGHT) \
    X(MUS_DP_ROUTE206_NIGHT) \
    X(MUS_DP_ROUTE209_NIGHT) \
    X(MUS_DP_ROUTE210_NIGHT) \
    X(MUS_DP_ROUTE216_NIGHT) \
    X(MUS_DP_ROUTE228_NIGHT) \
    X(MUS_DP_UNDERGROUND) \
    X(MUS_DP_FLAG_CAPTURED) \
    X(MUS_DP_VICTORY_ROAD) \
    X(MUS_DP_ETERNA_FOREST) \
    X(MUS_DP_OLD_CHATEAU) \
    X(MUS_DP_LAKE_CAVERNS) \
    X(MUS_DP_AMITY_SQUARE) \
    X(MUS_DP_GALACTIC_HQ) \
    X(MUS_DP_GALACTIC_ETERNA_BUILDING) \
    X(MUS_DP_GREAT_MARSH) \
    X(MUS_DP_LAKE) \
    X(MUS_DP_MT_CORONET) \
    X(MUS_DP_SPEAR_PILLAR) \
    X(MUS_DP_STARK_MOUNTAIN) \
    X(MUS_DP_OREBURGH_GATE) \
    X(MUS_DP_OREBURGH_MINE) \
    X(MUS_DP_INSIDE_POKEMON_LEAGUE) \
    X(MUS_DP_HALL_OF_FAME_ROOM) \
    X(MUS_DP_POKE_CENTER_DAY) \
    X(MUS_DP_POKE_CENTER_NIGHT) \
    X(MUS_DP_GYM) \
    X(MUS_DP_ROWAN_LAB) \
    X(MUS_DP_CONTEST_LOBBY) \
    X(MUS_DP_POKE_MART) \
    X(MUS_DP_GAME_CORNER) \
    X(MUS_DP_B_TOWER) \
    X(MUS_DP_TV_STATION) \
    X(MUS_DP_GALACTIC_HQ_BASEMENT) \
    X(MUS_DP_AZURE_FLUTE) \
    X(MUS_DP_HALL_OF_ORIGIN) \
    X(MUS_DP_GTS) \
    X(MUS_DP_ENCOUNTER_BOY) \
    X(MUS_DP_ENCOUNTER_TWINS) \
    X(MUS_DP_ENCOUNTER_INTENSE) \
    X(MUS_DP_ENCOUNTER_GALACTIC) \
    X(MUS_DP_ENCOUNTER_LADY) \
    X(MUS_DP_ENCOUNTER_HIKER) \
    X(MUS_DP_ENCOUNTER_RICH) \
    X(MUS_DP_ENCOUNTER_SAILOR) \
    X(MUS_DP_ENCOUNTER_SUSPICIOUS) \
    X(MUS_DP_ENCOUNTER_ACE_TRAINER) \
    X(MUS_DP_ENCOUNTER_GIRL) \
    X(MUS_DP_ENCOUNTER_CYCLIST) \
    X(MUS_DP_ENCOUNTER_ARTIST) \
    X(MUS_DP_ENCOUNTER_ELITE_FOUR) \
    X(MUS_DP_ENCOUNTER_CHAMPION) \
    X(MUS_DP_VS_WILD) \
    X(MUS_DP_VS_GYM_LEADER) \
    X(MUS_DP_VS_UXIE_MESPRIT_AZELF) \
    X(MUS_DP_VS_TRAINER) \
    X(MUS_DP_VS_GALACTIC_BOSS) \
    X(MUS_DP_VS_DIALGA_PALKIA) \
    X(MUS_DP_VS_CHAMPION) \
    X(MUS_DP_VS_GALACTIC) \
    X(MUS_DP_VS_RIVAL) \
    X(MUS_DP_VS_ARCEUS) \
    X(MUS_DP_VS_LEGEND) \
    X(MUS_DP_VICTORY_WILD) \
    X(MUS_DP_VICTORY_TRAINER) \
    X(MUS_DP_VICTORY_GYM_LEADER) \
    X(MUS_DP_VICTORY_CHAMPION) \
    X(MUS_DP_VICTORY_GALACTIC) \
    X(MUS_DP_VICTORY_ELITE_FOUR) \
    X(MUS_DP_VS_GALACTIC_COMMANDER) \
    X(MUS_DP_CONTEST) \
    X(MUS_DP_VS_ELITE_FOUR) \
    X(MUS_DP_FOLLOW_ME) \
    X(MUS_DP_RIVAL) \
    X(MUS_DP_LAKE_EVENT) \
    X(MUS_DP_EVOLUTION) \
    X(MUS_DP_LUCAS) \
    X(MUS_DP_DAWN) \
    X(MUS_DP_LEGEND_APPEARS) \
    X(MUS_DP_CATASTROPHE) \
    X(MUS_DP_POKE_RADAR) \
    X(MUS_DP_SURF) \
    X(MUS_DP_CYCLING) \
    X(MUS_DP_LETS_GO_TOGETHER) \
    X(MUS_DP_TV_END) \
    X(MUS_DP_LEVEL_UP) \
    X(MUS_DP_EVOLVED) \
    X(MUS_DP_OBTAIN_KEY_ITEM) \
    X(MUS_DP_OBTAIN_ITEM) \
    X(MUS_DP_CAUGHT_INTRO) \
    X(MUS_DP_DEX_RATING) \
    X(MUS_DP_OBTAIN_BADGE) \
    X(MUS_DP_POKETCH) \
    X(MUS_DP_OBTAIN_TMHM) \
    X(MUS_DP_OBTAIN_ACCESSORY) \
    X(MUS_DP_MOVE_DELETED) \
    X(MUS_DP_HEAL) \
    X(MUS_DP_OBTAIN_BERRY) \
    X(MUS_DP_CONTEST_DRESS_UP) \
    X(MUS_DP_HALL_OF_FAME) \
    X(MUS_DP_INTRO) \
    X(MUS_DP_TITLE) \
    X(MUS_DP_MYSTERY_GIFT) \
    X(MUS_DP_WFC) \
    X(MUS_DP_DANCE_EASY) \
    X(MUS_DP_DANCE_DIFFICULT) \
    X(MUS_DP_CONTEST_RESULTS) \
    X(MUS_DP_CONTEST_WINNER) \
    X(MUS_DP_POFFINS) \
    X(MUS_DP_SLOTS_WIN) \
    X(MUS_DP_SLOTS_JACKPOT) \
    X(MUS_DP_CREDITS) \
    X(MUS_DP_SLOTS_UNUSED) \
    X(MUS_PL_FIGHT_AREA_DAY) \
    X(MUS_PL_TV_BROADCAST) \
    X(MUS_PL_TV_END) \
    X(MUS_PL_INTRO) \
    X(MUS_PL_TITLE) \
    X(MUS_PL_DISTORTION_WORLD) \
    X(MUS_PL_B_ARCADE) \
    X(MUS_PL_B_HALL) \
    X(MUS_PL_B_CASTLE) \
    X(MUS_PL_B_FACTORY) \
    X(MUS_PL_GLOBAL_TERMINAL) \
    X(MUS_PL_LILYCOVE_BOSSA_NOVA) \
    X(MUS_PL_LOOKER) \
    X(MUS_PL_VS_GIRATINA) \
    X(MUS_PL_VS_FRONTIER_BRAIN) \
    X(MUS_PL_VICTORY_FRONTIER_BRAIN) \
    X(MUS_PL_VS_REGI) \
    X(MUS_PL_CONTEST_COOL) \
    X(MUS_PL_CONTEST_SMART) \
    X(MUS_PL_CONTEST_CUTE) \
    X(MUS_PL_CONTEST_TOUGH) \
    X(MUS_PL_CONTEST_BEAUTY) \
    X(MUS_PL_SPIN_TRADE) \
    X(MUS_PL_WIFI_MINIGAMES) \
    X(MUS_PL_WIFI_PLAZA) \
    X(MUS_PL_WIFI_PARADE) \
    X(MUS_PL_GIRATINA_APPEARS_1) \
    X(MUS_PL_GIRATINA_APPEARS_2) \
    X(MUS_PL_MYSTERY_GIFT) \
    X(MUS_PL_TWINLEAF_MUSIC_BOX) \
    X(MUS_PL_OBTAIN_ARCADE_POINTS) \
    X(MUS_PL_OBTAIN_CASTLE_POINTS) \
    X(MUS_PL_OBTAIN_B_POINTS) \
    X(MUS_PL_WIN_MINIGAME) \
    X(MUS_HG_INTRO) \
    X(MUS_HG_TITLE) \
    X(MUS_HG_NEW_GAME) \
    X(MUS_HG_EVOLUTION) \
    X(MUS_HG_EVOLUTION_NO_INTRO) \
    X(MUS_HG_CYCLING) \
    X(MUS_HG_SURF) \
    X(MUS_HG_E_DENDOURIRI) \
    X(MUS_HG_CREDITS) \
    X(MUS_HG_END) \
    X(MUS_HG_NEW_BARK) \
    X(MUS_HG_CHERRYGROVE) \
    X(MUS_HG_VIOLET) \
    X(MUS_HG_AZALEA) \
    X(MUS_HG_GOLDENROD) \
    X(MUS_HG_ECRUTEAK) \
    X(MUS_HG_CIANWOOD) \
    X(MUS_HG_ROUTE29) \
    X(MUS_HG_ROUTE30) \
    X(MUS_HG_ROUTE34) \
    X(MUS_HG_ROUTE38) \
    X(MUS_HG_ROUTE42) \
    X(MUS_HG_VERMILION) \
    X(MUS_HG_PEWTER) \
    X(MUS_HG_CERULEAN) \
    X(MUS_HG_LAVENDER) \
    X(MUS_HG_CELADON) \
    X(MUS_HG_PALLET) \
    X(MUS_HG_CINNABAR) \
    X(MUS_HG_ROUTE1) \
    X(MUS_HG_ROUTE3) \
    X(MUS_HG_ROUTE11) \
    X(MUS_HG_ROUTE24) \
    X(MUS_HG_ROUTE26) \
    X(MUS_HG_POKE_CENTER) \
    X(MUS_HG_POKE_MART) \
    X(MUS_HG_GYM) \
    X(MUS_HG_ELM_LAB) \
    X(MUS_HG_OAK) \
    X(MUS_HG_DANCE_THEATER) \
    X(MUS_HG_GAME_CORNER) \
    X(MUS_HG_B_TOWER) \
    X(MUS_HG_B_TOWER_RECEPTION) \
    X(MUS_HG_SPROUT_TOWER) \
    X(MUS_HG_UNION_CAVE) \
    X(MUS_HG_RUINS_OF_ALPH) \
    X(MUS_HG_NATIONAL_PARK) \
    X(MUS_HG_BURNED_TOWER) \
    X(MUS_HG_BELL_TOWER) \
    X(MUS_HG_LIGHTHOUSE) \
    X(MUS_HG_TEAM_ROCKET_HQ) \
    X(MUS_HG_ICE_PATH) \
    X(MUS_HG_DRAGONS_DEN) \
    X(MUS_HG_ROCK_TUNNEL) \
    X(MUS_HG_VIRIDIAN_FOREST) \
    X(MUS_HG_VICTORY_ROAD) \
    X(MUS_HG_POKEMON_LEAGUE) \
    X(MUS_HG_FOLLOW_ME_1) \
    X(MUS_HG_FOLLOW_ME_2) \
    X(MUS_HG_ENCOUNTER_RIVAL) \
    X(MUS_HG_RIVAL_EXIT) \
    X(MUS_HG_BUG_CONTEST_PREP) \
    X(MUS_HG_BUG_CATCHING_CONTEST) \
    X(MUS_HG_RADIO_ROCKET) \
    X(MUS_HG_ROCKET_TAKEOVER) \
    X(MUS_HG_MAGNET_TRAIN) \
    X(MUS_HG_SS_AQUA) \
    X(MUS_HG_MT_MOON_SQUARE) \
    X(MUS_HG_RADIO_JINGLE) \
    X(MUS_HG_RADIO_LULLABY) \
    X(MUS_HG_RADIO_MARCH) \
    X(MUS_HG_RADIO_UNOWN) \
    X(MUS_HG_RADIO_POKE_FLUTE) \
    X(MUS_HG_RADIO_OAK) \
    X(MUS_HG_RADIO_BUENA) \
    X(MUS_HG_EUSINE) \
    X(MUS_HG_CLAIR) \
    X(MUS_HG_ENCOUNTER_GIRL_1) \
    X(MUS_HG_ENCOUNTER_BOY_1) \
    X(MUS_HG_ENCOUNTER_SUSPICIOUS_1) \
    X(MUS_HG_ENCOUNTER_SAGE) \
    X(MUS_HG_ENCOUNTER_KIMONO_GIRL) \
    X(MUS_HG_ENCOUNTER_ROCKET) \
    X(MUS_HG_ENCOUNTER_GIRL_2) \
    X(MUS_HG_ENCOUNTER_BOY_2) \
    X(MUS_HG_ENCOUNTER_SUSPICIOUS_2) \
    X(MUS_HG_VS_WILD) \
    X(MUS_HG_VS_TRAINER) \
    X(MUS_HG_VS_GYM_LEADER) \
    X(MUS_HG_VS_RIVAL) \
    X(MUS_HG_VS_ROCKET) \
    X(MUS_HG_VS_SUICUNE) \
    X(MUS_HG_VS_ENTEI) \
    X(MUS_HG_VS_RAIKOU) \
    X(MUS_HG_VS_CHAMPION) \
    X(MUS_HG_VS_WILD_KANTO) \
    X(MUS_HG_VS_TRAINER_KANTO) \
    X(MUS_HG_VS_GYM_LEADER_KANTO) \
    X(MUS_HG_VICTORY_TRAINER) \
    X(MUS_HG_VICTORY_WILD) \
    X(MUS_HG_CAUGHT) \
    X(MUS_HG_VICTORY_GYM_LEADER) \
    X(MUS_HG_VS_HO_OH) \
    X(MUS_HG_VS_LUGIA) \
    X(MUS_HG_POKEATHLON_LOBBY) \
    X(MUS_HG_POKEATHLON_START) \
    X(MUS_HG_POKEATHLON_BEFORE) \
    X(MUS_HG_POKEATHLON_EVENT) \
    X(MUS_HG_POKEATHLON_FINALS) \
    X(MUS_HG_POKEATHLON_RESULTS) \
    X(MUS_HG_POKEATHLON_END) \
    X(MUS_HG_POKEATHLON_WINNER) \
    X(MUS_HG_B_FACTORY) \
    X(MUS_HG_B_HALL) \
    X(MUS_HG_B_ARCADE) \
    X(MUS_HG_B_CASTLE) \
    X(MUS_HG_VS_FRONTIER_BRAIN) \
    X(MUS_HG_VICTORY_FRONTIER_BRAIN) \
    X(MUS_HG_WFC) \
    X(MUS_HG_MYSTERY_GIFT) \
    X(MUS_HG_WIFI_PLAZA) \
    X(MUS_HG_WIFI_MINIGAMES) \
    X(MUS_HG_WIFI_PARADE) \
    X(MUS_HG_GLOBAL_TERMINAL) \
    X(MUS_HG_SPIN_TRADE) \
    X(MUS_HG_GTS) \
    X(MUS_HG_ROUTE47) \
    X(MUS_HG_SAFARI_ZONE_GATE) \
    X(MUS_HG_SAFARI_ZONE) \
    X(MUS_HG_ETHAN) \
    X(MUS_HG_LYRA) \
    X(MUS_HG_GAME_CORNER_WIN) \
    X(MUS_HG_KIMONO_GIRL_DANCE) \
    X(MUS_HG_KIMONO_GIRL) \
    X(MUS_HG_HO_OH_APPEARS) \
    X(MUS_HG_LUGIA_APPEARS) \
    X(MUS_HG_SPIKY_EARED_PICHU) \
    X(MUS_HG_SINJOU_RUINS) \
    X(MUS_HG_RADIO_ROUTE101) \
    X(MUS_HG_RADIO_ROUTE201) \
    X(MUS_HG_RADIO_TRAINER) \
    X(MUS_HG_RADIO_VARIETY) \
    X(MUS_HG_VS_KYOGRE_GROUDON) \
    X(MUS_HG_POKEWALKER) \
    X(MUS_HG_VS_ARCEUS) \
    X(MUS_HG_HEAL) \
    X(MUS_HG_LEVEL_UP) \
    X(MUS_HG_OBTAIN_ITEM) \
    X(MUS_HG_OBTAIN_KEY_ITEM) \
    X(MUS_HG_EVOLVED) \
    X(MUS_HG_OBTAIN_BADGE) \
    X(MUS_HG_OBTAIN_TMHM) \
    X(MUS_HG_OBTAIN_ACCESSORY) \
    X(MUS_HG_MOVE_DELETED) \
    X(MUS_HG_OBTAIN_BERRY) \
    X(MUS_HG_DEX_RATING_1) \
    X(MUS_HG_DEX_RATING_2) \
    X(MUS_HG_DEX_RATING_3) \
    X(MUS_HG_DEX_RATING_4) \
    X(MUS_HG_DEX_RATING_5) \
    X(MUS_HG_DEX_RATING_6) \
    X(MUS_HG_OBTAIN_EGG) \
    X(MUS_HG_BUG_CONTEST_1ST_PLACE) \
    X(MUS_HG_BUG_CONTEST_2ND_PLACE) \
    X(MUS_HG_BUG_CONTEST_3RD_PLACE) \
    X(MUS_HG_CARD_FLIP) \
    X(MUS_HG_CARD_FLIP_GAME_OVER) \
    X(MUS_HG_POKEGEAR_REGISTERED) \
    X(MUS_HG_LETS_GO_TOGETHER) \
    X(MUS_HG_POKEATHLON_READY) \
    X(MUS_HG_POKEATHLON_1ST_PLACE) \
    X(MUS_HG_RECEIVE_POKEMON) \
    X(MUS_HG_OBTAIN_ARCADE_POINTS) \
    X(MUS_HG_OBTAIN_CASTLE_POINTS) \
    X(MUS_HG_OBTAIN_B_POINTS) \
    X(MUS_HG_WIN_MINIGAME) \
    X(MUS_BW_GAME_FREAK) \
    X(MUS_BW_INTRO) \
    X(MUS_BW_THE_POKEMON) \
    X(MUS_BW_TITLE) \
    X(MUS_BW_NEW_GAME_1) \
    X(MUS_BW_NEW_GAME_2) \
    X(MUS_BW_BEGINNING) \
    X(MUS_BW_NUVEMA) \
    X(MUS_BW_VS_RIVAL) \
    X(MUS_BW_FOLLOW_ME_1) \
    X(MUS_BW_JUNIPER_1) \
    X(MUS_BW_JUNIPER_LAB) \
    X(MUS_BW_OBTAIN_KEY_ITEM) \
    X(MUS_BW_FRIEND) \
    X(MUS_BW_ROUTE1) \
    X(MUS_BW_VS_WILD) \
    X(MUS_BW_VICTORY_WILD) \
    X(MUS_BW_LEVEL_UP) \
    X(MUS_BW_ACCUMULA) \
    X(MUS_BW_FOLLOW_ME_2) \
    X(MUS_BW_POKE_CENTER) \
    X(MUS_BW_HEAL) \
    X(MUS_BW_DEX_RATING_1) \
    X(MUS_BW_SAGE) \
    X(MUS_BW_ROUTE2_SPRING) \
    X(MUS_BW_ROUTE2_SUMMER) \
    X(MUS_BW_ROUTE2_AUTUMN) \
    X(MUS_BW_ROUTE2_WINTER) \
    X(MUS_BW_XTRAN_CALL) \
    X(MUS_BW_ENCOUNTER_YOUNGSTER) \
    X(MUS_BW_VS_TRAINER) \
    X(MUS_BW_VICTORY_TRAINER) \
    X(MUS_BW_ENCOUNTER_LASS) \
    X(MUS_BW_OBTAIN_ITEM) \
    X(MUS_BW_STRIATON) \
    X(MUS_BW_DEX_RATING_2) \
    X(MUS_BW_DREAMYARD) \
    X(MUS_BW_DREAMYARD_B1F) \
    X(MUS_BW_PLASMA) \
    X(MUS_BW_VS_PLASMA) \
    X(MUS_BW_VICTORY_PLASMA) \
    X(MUS_BW_EVOLUTION) \
    X(MUS_BW_EVOLUTION_NO_INTRO) \
    X(MUS_BW_EVOLVED) \
    X(MUS_BW_ENCOUNTER_TWINS) \
    X(MUS_BW_CHEREN) \
    X(MUS_BW_TROUBLE) \
    X(MUS_BW_NACRENE) \
    X(MUS_BW_DEX_RATING_3) \
    X(MUS_BW_GYM) \
    X(MUS_BW_VS_GYM_LEADER) \
    X(MUS_BW_VS_WINNING) \
    X(MUS_BW_VICTORY_GYM_LEADER) \
    X(MUS_BW_OBTAIN_BADGE) \
    X(MUS_BW_OBTAIN_TMHM) \
    X(MUS_BW_GATE) \
    X(MUS_BW_SKYARROW_BRIDGE) \
    X(MUS_BW_CASTELIA) \
    X(MUS_BW_ENCOUNTER_CLERK) \
    X(MUS_BW_ROUTE4_SPRING) \
    X(MUS_BW_ROUTE4_SUMMER) \
    X(MUS_BW_ROUTE4_AUTUMN) \
    X(MUS_BW_ROUTE4_WINTER) \
    X(MUS_BW_ENCOUNTER_BACKPACKER) \
    X(MUS_BW_NIMBASA) \
    X(MUS_BW_BIANCA) \
    X(MUS_BW_EMOTION) \
    X(MUS_BW_N) \
    X(MUS_BW_VS_N_1) \
    X(MUS_BW12_VS_IN_DANGER) \
    X(MUS_BW_CYCLING) \
    X(MUS_BW_ALDER) \
    X(MUS_BW_DRAWBRIDGE) \
    X(MUS_BW_DRIFTVEIL) \
    X(MUS_BW_COLD_STORAGE) \
    X(MUS_BW_ROUTE6_SPRING) \
    X(MUS_BW_ROUTE6_SUMMER) \
    X(MUS_BW_ROUTE6_AUTUMN) \
    X(MUS_BW_ROUTE6_WINTER) \
    X(MUS_BW_ENCOUNTER_PARASOL) \
    X(MUS_BW_ENCOUNTER_SCIENTIST) \
    X(MUS_BW_CHARGESTONE_CAVE) \
    X(MUS_BW_CHARGESTONE_CAVE_B1F) \
    X(MUS_BW_CHARGESTONE_CAVE_B2F) \
    X(MUS_BW_MISTRALTON) \
    X(MUS_BW_JUNIPER_2) \
    X(MUS_BW_DEX_RATING_4) \
    X(MUS_BW_MOVE_DELETED) \
    X(MUS_BW_ENCOUNTER_PSYCHIC) \
    X(MUS_BW_ICIRRUS) \
    X(MUS_BW_DRAGONSPIRAL_TOWER) \
    X(MUS_BW_ENCOUNTER_PLASMA) \
    X(MUS_BW_DRAGONSPIRAL_TOWER_TOP) \
    X(MUS_BW_RELIC_CASTLE) \
    X(MUS_BW_OBTAIN_STONE) \
    X(MUS_BW_TUBELINE_BRIDGE) \
    X(MUS_BW_POKE_MART) \
    X(MUS_BW_ENCOUNTER_ROUGHNECK) \
    X(MUS_BW_OPELUCID_BLACK) \
    X(MUS_BW_OPELUCID_WHITE) \
    X(MUS_BW_ROUTE10) \
    X(MUS_BW_VICTORY_ROAD) \
    X(MUS_BW_ENCOUNTER_ACE_TRAINER) \
    X(MUS_BW_POKEMON_LEAGUE) \
    X(MUS_BW_VS_ELITE_FOUR) \
    X(MUS_BW_N_CASTLE_APPEARS) \
    X(MUS_BW_N_CASTLE_BRIDGE) \
    X(MUS_BW_N_CASTLE) \
    X(MUS_BW_N_ROOM) \
    X(MUS_BW_LEGEND_COME) \
    X(MUS_BW_LEGEND_BORN) \
    X(MUS_BW_VS_RESHIRAM_ZEKROM) \
    X(MUS_BW_VS_N_2) \
    X(MUS_BW_GHETSIS) \
    X(MUS_BW_VS_GHETSIS) \
    X(MUS_BW_FAREWELL) \
    X(MUS_BW_CREDITS) \
    X(MUS_BW_LOOKER) \
    X(MUS_BW_OBTAIN_EGG) \
    X(MUS_BW_ROYAL_UNOVA) \
    X(MUS_BW_WFC) \
    X(MUS_BW_GLOBAL_TERMINAL) \
    X(MUS_BW_GTS) \
    X(MUS_BW_SPIN_TRADE) \
    X(MUS_BW_UNITY_TOWER) \
    X(MUS_BW_DEX_RATING_5) \
    X(MUS_BW_GEAR_STATION) \
    X(MUS_BW_BATTLE_SUBWAY) \
    X(MUS_BW_VS_SUBWAY_TRAINER) \
    X(MUS_BW_OBTAIN_B_POINTS) \
    X(MUS_BW_MUSICAL_THEATER) \
    X(MUS_BW_OBTAIN_ACCESSORY) \
    X(MUS_BW_MUSICAL_DRESS_UP) \
    X(MUS_BW_MUSICAL_CURTAIN_RAISES) \
    X(MUS_BW_MUSICAL_STARDOM) \
    X(MUS_BW_MUSICAL_FOREST_STROLL) \
    X(MUS_BW_MUSICAL_A_SWEET_SOIREE) \
    X(MUS_BW_MUSICAL_EXCITING_NIMBASA) \
    X(MUS_BW_MUSICAL_DLC_MUNNA) \
    X(MUS_BW_MUSICAL_DLC_SMASH) \
    X(MUS_BW_MUSICAL_DLC_FESTA) \
    X(MUS_BW_MUSICAL_DLC_RELIC) \
    X(MUS_BW_MUSICAL_DLC_RUNPA) \
    X(MUS_BW_FERRIS_WHEEL) \
    X(MUS_BW_FEELING_CHECK) \
    X(MUS_BW_FEELING_CHECK_HIGH) \
    X(MUS_BW_FEELING_CHECK_MEDIUM) \
    X(MUS_BW_FEELING_CHECK_LOW) \
    X(MUS_BW_VS_LEGEND) \
    X(MUS_BW_ANVILLE_TOWN) \
    X(MUS_BW_MARVELOUS_BRIDGE) \
    X(MUS_BW_ROUTE12_SPRING) \
    X(MUS_BW_ROUTE12_SUMMER) \
    X(MUS_BW_ROUTE12_AUTUMN) \
    X(MUS_BW_ROUTE12_WINTER) \
    X(MUS_BW_ENCOUNTER_CYCLIST) \
    X(MUS_BW_POKETRANSFER_LAB) \
    X(MUS_BW_POKETRANSFER_BOX) \
    X(MUS_BW_POKETRANSFER_GAME) \
    X(MUS_BW_ENCOUNTER_POKEFAN) \
    X(MUS_BW_BLACK_CITY) \
    X(MUS_BW_WHITE_FOREST) \
    X(MUS_BW_GAME_SYNC) \
    X(MUS_BW_ENTRALINK) \
    X(MUS_BW_FUNFEST_START) \
    X(MUS_BW_ENTRALINK_VISIT) \
    X(MUS_BW_FUNFEST_CLEAR) \
    X(MUS_BW_FUNFEST_FAILED) \
    X(MUS_BW_ENCOUNTER_GENTLEMAN) \
    X(MUS_BW_UNDELLA_SPRING) \
    X(MUS_BW_UNDELLA_SUMMER) \
    X(MUS_BW_CYNTHIA) \
    X(MUS_BW_VS_CYNTHIA) \
    X(MUS_BW_LOSTLORN_FOREST) \
    X(MUS_BW_VS_WILD_STRONG) \
    X(MUS_BW_SURF) \
    X(MUS_BW_ABYSSAL_RUINS) \
    X(MUS_BW_LACUNOSA) \
    X(MUS_BW_VILLAGE_BRIDGE) \
    X(MUS_BW_MYSTERY_GIFT) \
    X(MUS_BW_BATTLE_COMPETITION) \
    X(MUS_BW_ROCKET) \
    X(MUS_BW_VS_KYUREM) \
    X(MUS_BW_DEX_RATING_6) \
    X(MUS_BW_VS_CHAMPION) \
    X(MUS_BW_VICTORY_CHAMPION) \
    X(MUS_BW_HALL_OF_FAME) \
    X(MUS_BW_RELIC_SONG) \
    X(MUS_BW_VS_WCS_CHAMPION) \
    X(MUS_BW_OBTAIN_BERRY) \
    X(PMD_HABITAT_HOME01) \
    X(PMD_HABITAT_DARK_01) \
    X(PMD_HABITAT_MYSTIC_01) \
    X(PMD_EVENT_DREAM_01) \
    X(PMD_EVENT_DREAM_02) \
    X(PMD_EVENT_LEGEND_01) \
    X(PMD_HABITAT_HIROBA) \
    X(PMD_HABITAT_HOME_SHINKA) \
    X(PMD_EVENT_FEAR) \
    X(PMD_EVENT_KINPAKU) \
    X(PMD_BOSS01) \
    X(PMD_EVENT_YUME) \
    X(PMD_DUNGEON_GENERAL_01) \
    X(PMD_DUNGEON_DENJIHA_01) \
    X(PMD_DUNGEON_AYASHIIMORI_01) \
    X(PMD_HABITAT_CALMLY_02) \
    X(PMD_SYS_SHOP) \
    X(PMD_SYS_STEAL) \
    X(PMD_EVENT_DISASTER) \
    X(PMD_DUNGEON_KYOUKOKU_01) \
    X(PMD_DUNGEON_GENERAL_02) \
    X(PMD_DUNGEON_TENKUU_01) \
    X(PMD_DUNGEON_TENKUU_02) \
    X(PMD_EVENT_ESCAPE_01) \
    X(PMD_DUNGEON_HONOU_01) \
    X(PMD_DUNGEON_EVENTFLOOR_02) \
    X(PMD_HABITAT_ITADAKI_01) \
    X(PMD_HABITAT_NIJI_01) \
    X(PMD_EVENT_ENV_MYSTIC) \
    X(PMD_HABITAT_KAIRYU_01) \
    X(PMD_HABITAT_KAITEI_01) \
    X(PMD_BOSS02) \
    X(PMD_DUNGEON_HONOU_02) \
    X(PMD_HABITAT_DAICHI_01) \
    X(PMD_HABITAT_NAZO_01) \
    X(PMD_EVENT_ESCAPE_02) \
    X(PMD_EVENT_ENV_SOUL) \
    X(PMD_EVENT_ENV_HONOU) \
    X(PMD_DUNGEON_JUHYOU_02) \
    X(PMD_SYS_TITLE_01) \
    X(PMD_EVENT_SEPARATION_01) \
    X(PMD_EVENT_SEPARATION_02) \
    X(PMD_SYS_TITLE_02) \
    X(PMD_SYS_ENDING_01) \
    X(PMD_SYS_ENDING_02) \
    X(PMD_SYS_TITLE_03) \
    X(PMD_SAVE_01) \
    X(PMD_GAMEOVER) \
    X(PMD_GAMECLEAR) \
    X(PMD_EVENT_ZUKKOKE) \
    X(PMD_EVENT_CALMLY) \
    X(PMD_DUNGEON_GUNJYOU_01) \
    X(PMD_EVENT_CLEARD) \
    X(PMD_DUNGEON_JUHYOU_01) \
    X(PMD_HABITAT_CALMLY_01) \
    X(PMD_HABITAT_MYSTIC_02) \
    X(PMD_HABITAT_CALMLY_03) \
    X(PMD_DUNGEON_CHITEI_01) \
    X(PMD_SYS_TRAINING) \
    X(PMD_DUNGEON_TUTORIAL_01) \
    X(PMD_DUNGEON_RAIMEI_01) \
    X(PMD_HABITAT_MYSTIC_06) \
    X(PMD_DUNGEON_CHINMOKU_01) \
    X(PMD_DUNGEON_EVENTFLOOR_01) \
    X(PMD_DUNGEON_REIHOU_01) \
    X(PMD_HABITAT_MYSTIC_03) \
    X(PMD_HABITAT_DENSETSU_01) \
    X(PMD_HABITAT_MINAMI_01) \
    X(PMD_HABITAT_TOZASARETA_01) \
    X(PMD_DUNGEON_HAGANE_01) \
    X(PMD_HABITAT_MYSTIC_04) \
    X(PMD_HABITAT_SAIGO_01) \
    X(PMD_DUNGEON_REIHOU_02) \
    X(PMD_DUNGEON_CHITEI_02) \
    X(PMD_DUNGEON_CHIISANAMORI_01) \
    X(PMD_DUNGEON_RAIMEI_02) \
    X(PMD_HABITAT_IYASHI_01) \
    X(PMD_SYS_MONSTER) \
    X(PMD_LEVELUP) \
    X(PMD_RANKUP) \
    X(PMD_SEEYOU) \
    X(PMD_REWARD) \
    X(PMD_PARTY) \
    X(PMD_DUNGEON_OPEN) \
    X(PMD_AREA_OPEN) \
    X(PMD_INFORMATION) \
    X(PMD_FANFARE) \
    X(PMD_EVOLUTION) \
    X(PMD_EVOLUTION_02) \
    X(PMD_ITEM_IMPORTANT) \
    X(PMD_NEWS) \
    X(PMD_MUSICBOX) \
    X(PMD_EFF_ORUGORU) \
    X(PMD_WIND_S) \
    X(PMD_WIND_M) \
    X(PMD_WAVE_S) \
    X(PMD_WAVE_M) \
    X(PMD_SYS_01) \
    X(PMD_SYS_02) \
    X(PMD_SYS_03) \
    X(PMD_SYS_04) \
    X(PMD_SYS_05) \
    X(PMD_SYS_06) \
    X(PMD_SYS_07) \
    X(PMD_SYS_08) \
    X(PMD_SYS_09) \
    X(PMD_SYS_10) \
    X(PMD_DS_SYS_01) \
    X(PMD_WAZA) \
    X(PMD_WAZA_CLEAR) \
    X(PMD_ITEM_M) \
    X(PMD_ITEM_M_03) \
    X(PMD_MOTION_MONEY) \
    X(PMD_MOTION_ITEM) \
    X(PMD_MOTION_EAT) \
    X(PMD_MOTION_THROW) \
    X(PMD_MOTION_THROW_02) \
    X(PMD_MOTION_THROW_03) \
    X(PMD_MOTION_HUNGRY) \
    X(PMD_MOTION_STAIRS_UP) \
    X(PMD_MOTION_STAIRS_DOWN) \
    X(PMD_MISS_PLAYER_M) \
    X(PMD_MISS_ENEMY_M) \
    X(PMD_TAIATARI_M) \
    X(PMD_MOTION_BREAK) \
    X(PMD_MOTION_DOOR_GENERAL) \
    X(PMD_MOTION_DOOR_BIG) \
    X(PMD_MOTION_SWITCH) \
    X(PMD_EFF_ANIM_2) \
    X(PMD_EFF_ANIM_8) \
    X(PMD_EFF_ANIM_9) \
    X(PMD_EFF_ANIM_10) \
    X(PMD_EFF_ANIM_11) \
    X(PMD_EFF_ANIM_12) \
    X(PMD_EFF_ANIM_4_SCRATCH) \
    X(PMD_EFF_ANIM_4_LICK) \
    X(PMD_EFF_ANIM_4_GORE) \
    X(PMD_EFF_ANIM_4_ATTACK) \
    X(PMD_EFF_ANIM_4_HEEL_DOWN) \
    X(PMD_EFF_ANIM_4_WAG_TAIL) \
    X(PMD_EFF_ANIM_4_PAWUP) \
    X(PMD_EFF_ANIM_4_ELECTRICITY) \
    X(PMD_EFF_ANIM_4_WING_S) \
    X(PMD_EFF_ANIM_4_WING_M) \
    X(PMD_EFF_ANIM_4_WING_L) \
    X(PMD_EFF_ANIM_4_SHAKE) \
    X(PMD_EFF_ANIM_4_JUMP) \
    X(PMD_EFF_ANIM_4_ATTACK_LIGHT) \
    X(PMD_MOTION_WIND_LARGE) \
    X(PMD_MOTION_WAVE_LARGE) \
    X(PMD_THUNDER_M) \
    X(PMD_WIND_M_2) \
    X(PMD_ESP_M) \
    X(PMD_TRAP_TUMBLE) \
    X(PMD_TRAP_SCISSOR) \
    X(PMD_TRAP_BETOBETA) \
    X(PMD_TRAP_SIREN) \
    X(PMD_TRAP_SUMMON) \
    X(PMD_TRAP_HOLE) \
    X(PMD_TRAP_TURNTABLE) \
    X(PMD_TRAP_SLEEP) \
    X(PMD_TRAP_SEAL) \
    X(PMD_TRAP_MINE) \
    X(PMD_TRAP_BIGMINE) \
    X(PMD_STATE_SHIELD) \
    X(PMD_STATE_QUESTION) \
    X(PMD_STATE_SWING) \
    X(PMD_STATE_STIFF) \
    X(PMD_STATE_INVISIBLE) \
    X(PMD_STATE_BLIND_EYE) \
    X(PMD_STATE_DRY_UP) \
    X(PMD_STATE_PULL) \
    X(PMD_STATE_THROW) \
    X(PMD_STATE_QUAKE) \
    X(PMD_STATE_JUMP) \
    X(PMD_STATE_LV_DOWN) \
    X(PMD_STATE_REVIVE) \
    X(PMD_STATE_ESCAPE) \
    X(PMD_WEATHER_CLOUDY) \
    X(PMD_WEATHER_FOG) \
    X(PMD_EVENT_FLY_01_L) \
    X(PMD_EVENT_FLY_02_L) \
    X(PMD_EVENT_FLY_03) \
    X(PMD_EVENT_EARTHQUAKE_01_L) \
    X(PMD_EVENT_MOTION_APPEAR_01) \
    X(PMD_EVENT_MOTION_JUMP_01) \
    X(PMD_EVENT_MOTION_JUMP_05) \
    X(PMD_EVENT_MOTION_JUMP_02) \
    X(PMD_EVENT_MOTION_JUMP_04) \
    X(PMD_EVENT_FLYAWAY_01) \
    X(PMD_EVENT_FLY_FIRE_01) \
    X(PMD_EVENT_SIGN_LAUGH_01) \
    X(PMD_EVENT_SIGN_LAUGH_01_L) \
    X(PMD_EVENT_SIGN_NOTICE_01) \
    X(PMD_EVENT_SIGN_NOTICE_02) \
    X(PMD_EVENT_SIGN_NOTICE_04) \
    X(PMD_EVENT_SIGN_NOTICE_03) \
    X(PMD_EVENT_SIGN_HATENA_01) \
    X(PMD_EVENT_SIGN_HATENA_02) \
    X(PMD_EVENT_SIGN_HATENA_03) \
    X(PMD_EVENT_SIGN_ASE_01) \
    X(PMD_EVENT_SIGN_ANGER_01) \
    X(PMD_EVENT_SIGN_ANGER_02) \
    X(PMD_EVENT_SIGN_SHOCK_01) \
    X(PMD_EVENT_MOTION_QUACK_01) \
    X(PMD_EVENT_MOTION_FEATHER_01) \
    X(PMD_EVENT_EFF_DIZZINESS_01) \
    X(PMD_EVENT_APPEAR_02) \
    X(PMD_EVENT_MOTION_JUMP_03) \
    X(PMD_EVENT_FLASH_02) \
    X(PMD_EVENT_SNEEZE) \
    X(PMD_EVENT_LIGHT_02) \
    X(PMD_EVENT_SIGN_SHOCK_02) \
    X(PMD_EVENT_FALL_01) \
    X(PMD_EVENT_LIGHT_03_L) \
    X(PMD_EVENT_INTERFERE_01) \
    X(PMD_EVENT_MOTION_HOLEUP_01) \
    X(PMD_EVENT_WHITEOUT_03) \
    X(PMD_EVENT_SHOOTINGSTAR) \
    X(PMD_EVENT_KESSYOU) \
    X(PMD_EVENT_FLY_09) \
    X(PMD_EVENT_FLASH_02_L) \
    X(PMD_EVENT_FLASH_03) \
    X(PMD_EVENT_FLASH_06) \
    X(PMD_EVENT_FLASH_04) \
    X(PMD_EVENT_FREEZE) \
    X(PMD_EVENT_DAMAGE_01) \
    X(PMD_EVENT_EARTHQUAKE_02) \
    X(PMD_EVENT_APPEAR_03) \
    X(PMD_EVENT_TELEPORT) \
    X(PMD_EVENT_POST) \
    X(PMD_EVENT_FLASH_01) \
    X(PMD_EVENT_FOOTSTEP_01) \
    X(PMD_EVENT_FLY_04) \
    X(PMD_EVENT_FOOTSTEP_02) \
    X(PMD_EVENT_WHITEOUT) \
    X(PMD_EVENT_WHITEOUT_04) \
    X(PMD_EVENT_SHOOTING_L) \
    X(PMD_EVENT_FALL_02) \
    X(PMD_EVENT_BOUND) \
    X(PMD_EVENT_LIGHTBALL_01) \
    X(PMD_EVENT_EARTHQUAKE_03_L) \
    X(PMD_EVENT_HAKAIKOUSEN) \
    X(PMD_EVENT_LIGHTBALL_02) \
    X(PMD_EVENT_MOTION_HARAHERI) \
    X(PMD_EVENT_MOTION_EAT_L) \
    X(PMD_EVENT_EXPLOSION_01) \
    X(PMD_EVENT_EFF_RAIN)
// End of MUSIC_EXPANSION_GEN5_MUSIC_REDUCE_SIZE == TRUE

#define SOUND_LIST_SE               \
    X(SE_USE_ITEM)                  \
    X(SE_PC_LOGIN)                  \
    X(SE_PC_OFF)                    \
    X(SE_PC_ON)                     \
    X(SE_SELECT)                    \
    X(SE_WIN_OPEN)                  \
    X(SE_WALL_HIT)                  \
    X(SE_DOOR)                      \
    X(SE_EXIT)                      \
    X(SE_LEDGE)                     \
    X(SE_BIKE_BELL)                 \
    X(SE_NOT_EFFECTIVE)             \
    X(SE_EFFECTIVE)                 \
    X(SE_SUPER_EFFECTIVE)           \
    X(SE_BALL_OPEN)                 \
    X(SE_FAINT)                     \
    X(SE_FLEE)                      \
    X(SE_SLIDING_DOOR)              \
    X(SE_SHIP)                      \
    X(SE_BANG)                      \
    X(SE_PIN)                       \
    X(SE_BOO)                       \
    X(SE_BALL)                      \
    X(SE_CONTEST_PLACE)             \
    X(SE_A)                         \
    X(SE_I)                         \
    X(SE_U)                         \
    X(SE_E)                         \
    X(SE_O)                         \
    X(SE_N)                         \
    X(SE_SUCCESS)                   \
    X(SE_FAILURE)                   \
    X(SE_EXP)                       \
    X(SE_BIKE_HOP)                  \
    X(SE_SWITCH)                    \
    X(SE_CLICK)                     \
    X(SE_FU_ZAKU)                   \
    X(SE_CONTEST_CONDITION_LOSE)    \
    X(SE_LAVARIDGE_FALL_WARP)       \
    X(SE_ICE_STAIRS)                \
    X(SE_ICE_BREAK)                 \
    X(SE_ICE_CRACK)                 \
    X(SE_FALL)                      \
    X(SE_UNLOCK)                    \
    X(SE_WARP_IN)                   \
    X(SE_WARP_OUT)                  \
    X(SE_REPEL)                     \
    X(SE_ROTATING_GATE)             \
    X(SE_TRUCK_MOVE)                \
    X(SE_TRUCK_STOP)                \
    X(SE_TRUCK_UNLOAD)              \
    X(SE_TRUCK_DOOR)                \
    X(SE_BERRY_BLENDER)             \
    X(SE_CARD)                      \
    X(SE_SAVE)                      \
    X(SE_BALL_BOUNCE_1)             \
    X(SE_BALL_BOUNCE_2)             \
    X(SE_BALL_BOUNCE_3)             \
    X(SE_BALL_BOUNCE_4)             \
    X(SE_BALL_TRADE)                \
    X(SE_BALL_THROW)                \
    X(SE_NOTE_C)                    \
    X(SE_NOTE_D)                    \
    X(SE_NOTE_E)                    \
    X(SE_NOTE_F)                    \
    X(SE_NOTE_G)                    \
    X(SE_NOTE_A)                    \
    X(SE_NOTE_B)                    \
    X(SE_NOTE_C_HIGH)               \
    X(SE_PUDDLE)                    \
    X(SE_BRIDGE_WALK)               \
    X(SE_ITEMFINDER)                \
    X(SE_DING_DONG)                 \
    X(SE_BALLOON_RED)               \
    X(SE_BALLOON_BLUE)              \
    X(SE_BALLOON_YELLOW)            \
    X(SE_BREAKABLE_DOOR)            \
    X(SE_MUD_BALL)                  \
    X(SE_FIELD_POISON)              \
    X(SE_ESCALATOR)                 \
    X(SE_THUNDERSTORM)              \
    X(SE_THUNDERSTORM_STOP)         \
    X(SE_DOWNPOUR)                  \
    X(SE_DOWNPOUR_STOP)             \
    X(SE_RAIN)                      \
    X(SE_RAIN_STOP)                 \
    X(SE_THUNDER)                   \
    X(SE_THUNDER2)                  \
    X(SE_ELEVATOR)                  \
    X(SE_LOW_HEALTH)                \
    X(SE_EXP_MAX)                   \
    X(SE_ROULETTE_BALL)             \
    X(SE_ROULETTE_BALL2)            \
    X(SE_TAILLOW_WING_FLAP)         \
    X(SE_SHOP)                      \
    X(SE_CONTEST_HEART)             \
    X(SE_CONTEST_CURTAIN_RISE)      \
    X(SE_CONTEST_CURTAIN_FALL)      \
    X(SE_CONTEST_ICON_CHANGE)       \
    X(SE_CONTEST_ICON_CLEAR)        \
    X(SE_CONTEST_MONS_TURN)         \
    X(SE_SHINY)                     \
    X(SE_INTRO_BLAST)               \
    X(SE_MUGSHOT)                   \
    X(SE_APPLAUSE)                  \
    X(SE_VEND)                      \
    X(SE_ORB)                       \
    X(SE_DEX_SCROLL)                \
    X(SE_DEX_PAGE)                  \
    X(SE_POKENAV_ON)                \
    X(SE_POKENAV_OFF)               \
    X(SE_DEX_SEARCH)                \
    X(SE_EGG_HATCH)                 \
    X(SE_BALL_TRAY_ENTER)           \
    X(SE_BALL_TRAY_BALL)            \
    X(SE_BALL_TRAY_EXIT)            \
    X(SE_GLASS_FLUTE)               \
    X(SE_M_THUNDERBOLT)             \
    X(SE_M_THUNDERBOLT2)            \
    X(SE_M_HARDEN)                  \
    X(SE_M_NIGHTMARE)               \
    X(SE_M_VITAL_THROW)             \
    X(SE_M_VITAL_THROW2)            \
    X(SE_M_BUBBLE)                  \
    X(SE_M_BUBBLE2)                 \
    X(SE_M_BUBBLE3)                 \
    X(SE_M_RAIN_DANCE)              \
    X(SE_M_CUT)                     \
    X(SE_M_STRING_SHOT)             \
    X(SE_M_STRING_SHOT2)            \
    X(SE_M_ROCK_THROW)              \
    X(SE_M_GUST)                    \
    X(SE_M_GUST2)                   \
    X(SE_M_DOUBLE_SLAP)             \
    X(SE_M_DOUBLE_TEAM)             \
    X(SE_M_RAZOR_WIND)              \
    X(SE_M_ICY_WIND)                \
    X(SE_M_THUNDER_WAVE)            \
    X(SE_M_COMET_PUNCH)             \
    X(SE_M_MEGA_KICK)               \
    X(SE_M_MEGA_KICK2)              \
    X(SE_M_CRABHAMMER)              \
    X(SE_M_JUMP_KICK)               \
    X(SE_M_FLAME_WHEEL)             \
    X(SE_M_FLAME_WHEEL2)            \
    X(SE_M_FLAMETHROWER)            \
    X(SE_M_FIRE_PUNCH)              \
    X(SE_M_TOXIC)                   \
    X(SE_M_SACRED_FIRE)             \
    X(SE_M_SACRED_FIRE2)            \
    X(SE_M_EMBER)                   \
    X(SE_M_TAKE_DOWN)               \
    X(SE_M_BLIZZARD)                \
    X(SE_M_BLIZZARD2)               \
    X(SE_M_SCRATCH)                 \
    X(SE_M_VICEGRIP)                \
    X(SE_M_WING_ATTACK)             \
    X(SE_M_FLY)                     \
    X(SE_M_SAND_ATTACK)             \
    X(SE_M_RAZOR_WIND2)             \
    X(SE_M_BITE)                    \
    X(SE_M_HEADBUTT)                \
    X(SE_M_SURF)                    \
    X(SE_M_HYDRO_PUMP)              \
    X(SE_M_WHIRLPOOL)               \
    X(SE_M_HORN_ATTACK)             \
    X(SE_M_TAIL_WHIP)               \
    X(SE_M_MIST)                    \
    X(SE_M_POISON_POWDER)           \
    X(SE_M_BIND)                    \
    X(SE_M_DRAGON_RAGE)             \
    X(SE_M_SING)                    \
    X(SE_M_PERISH_SONG)             \
    X(SE_M_PAY_DAY)                 \
    X(SE_M_DIG)                     \
    X(SE_M_DIZZY_PUNCH)             \
    X(SE_M_SELF_DESTRUCT)           \
    X(SE_M_EXPLOSION)               \
    X(SE_M_ABSORB_2)                \
    X(SE_M_ABSORB)                  \
    X(SE_M_SCREECH)                 \
    X(SE_M_BUBBLE_BEAM)             \
    X(SE_M_BUBBLE_BEAM2)            \
    X(SE_M_SUPERSONIC)              \
    X(SE_M_BELLY_DRUM)              \
    X(SE_M_METRONOME)               \
    X(SE_M_BONEMERANG)              \
    X(SE_M_LICK)                    \
    X(SE_M_PSYBEAM)                 \
    X(SE_M_FAINT_ATTACK)            \
    X(SE_M_SWORDS_DANCE)            \
    X(SE_M_LEER)                    \
    X(SE_M_SWAGGER)                 \
    X(SE_M_SWAGGER2)                \
    X(SE_M_HEAL_BELL)               \
    X(SE_M_CONFUSE_RAY)             \
    X(SE_M_SNORE)                   \
    X(SE_M_BRICK_BREAK)             \
    X(SE_M_GIGA_DRAIN)              \
    X(SE_M_PSYBEAM2)                \
    X(SE_M_SOLAR_BEAM)              \
    X(SE_M_PETAL_DANCE)             \
    X(SE_M_TELEPORT)                \
    X(SE_M_MINIMIZE)                \
    X(SE_M_SKETCH)                  \
    X(SE_M_SWIFT)                   \
    X(SE_M_REFLECT)                 \
    X(SE_M_BARRIER)                 \
    X(SE_M_DETECT)                  \
    X(SE_M_LOCK_ON)                 \
    X(SE_M_MOONLIGHT)               \
    X(SE_M_CHARM)                   \
    X(SE_M_CHARGE)                  \
    X(SE_M_STRENGTH)                \
    X(SE_M_HYPER_BEAM)              \
    X(SE_M_WATERFALL)               \
    X(SE_M_REVERSAL)                \
    X(SE_M_ACID_ARMOR)              \
    X(SE_M_SANDSTORM)               \
    X(SE_M_TRI_ATTACK)              \
    X(SE_M_TRI_ATTACK2)             \
    X(SE_M_ENCORE)                  \
    X(SE_M_ENCORE2)                 \
    X(SE_M_BATON_PASS)              \
    X(SE_M_MILK_DRINK)              \
    X(SE_M_ATTRACT)                 \
    X(SE_M_ATTRACT2)                \
    X(SE_M_MORNING_SUN)             \
    X(SE_M_FLATTER)                 \
    X(SE_M_SAND_TOMB)               \
    X(SE_M_GRASSWHISTLE)            \
    X(SE_M_SPIT_UP)                 \
    X(SE_M_DIVE)                    \
    X(SE_M_EARTHQUAKE)              \
    X(SE_M_TWISTER)                 \
    X(SE_M_SWEET_SCENT)             \
    X(SE_M_YAWN)                    \
    X(SE_M_SKY_UPPERCUT)            \
    X(SE_M_STAT_INCREASE)           \
    X(SE_M_HEAT_WAVE)               \
    X(SE_M_UPROAR)                  \
    X(SE_M_HAIL)                    \
    X(SE_M_COSMIC_POWER)            \
    X(SE_M_TEETER_DANCE)            \
    X(SE_M_STAT_DECREASE)           \
    X(SE_M_HAZE)                    \
    X(SE_M_HYPER_BEAM2)             \
    X(SE_RG_DOOR)                   \
    X(SE_RG_CARD_FLIP)              \
    X(SE_RG_CARD_FLIPPING)          \
    X(SE_RG_CARD_OPEN)              \
    X(SE_RG_BAG_CURSOR)             \
    X(SE_RG_BAG_POCKET)             \
    X(SE_RG_BALL_CLICK)             \
    X(SE_RG_SHOP)                   \
    X(SE_RG_SS_ANNE_HORN)           \
    X(SE_RG_HELP_OPEN)              \
    X(SE_RG_HELP_CLOSE)             \
    X(SE_RG_HELP_ERROR)             \
    X(SE_RG_DEOXYS_MOVE)            \
    X(SE_RG_POKE_JUMP_SUCCESS)      \
    X(SE_RG_POKE_JUMP_FAILURE)      \
    X(SE_POKENAV_CALL)              \
    X(SE_POKENAV_HANG_UP)           \
    X(SE_ARENA_TIMEUP1)             \
    X(SE_ARENA_TIMEUP2)             \
    X(SE_PIKE_CURTAIN_CLOSE)        \
    X(SE_PIKE_CURTAIN_OPEN)         \
    X(SE_SUDOWOODO_SHAKE)

// Create BGM list
#if MUSIC_EXPANSION_GEN5_MUSIC_REDUCE_SIZE == FALSE
    #define SOUND_LIST_BGM SOUND_LIST_BGM_GEN_V_UNREDUCED
#else
    #define SOUND_LIST_BGM SOUND_LIST_BGM_GEN_V_REDUCED
#endif

#define X(songId) static const u8 sBGMName_##songId[] = _(#songId);
SOUND_LIST_BGM
#undef X

#define X(songId) [songId - START_MUS] = sBGMName_##songId,
static const u8 *const sBGMNames[END_MUS - START_MUS + 1] =
{
SOUND_LIST_BGM
};
#undef X

// Create SE list
#define X(songId) static const u8 sSEName_##songId[] = _(#songId);
SOUND_LIST_SE
#undef X

#define X(songId) [songId - 1] = sSEName_##songId,
static const u8 *const sSENames[END_SE + 1] =
{
SOUND_LIST_SE
};
#undef X

// *******************************
// Actions BerryFunctions

static void DebugAction_BerryFunctions_ClearAll(u8 taskId)
{
    u8 i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].movementType == MOVEMENT_TYPE_BERRY_TREE_GROWTH)
        {
            RemoveBerryTree(GetObjectEventBerryTreeId(i));
            SetBerryTreeJustPicked(gObjectEvents[i].localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
        }
    }

    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}

static void DebugAction_BerryFunctions_Ready(u8 taskId)
{
    u8 i;
    struct BerryTree *tree;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].movementType == MOVEMENT_TYPE_BERRY_TREE_GROWTH)
        {
            tree = &gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(i)];
            if (tree->stage != BERRY_STAGE_NO_BERRY)
            {
                tree->stage = BERRY_STAGE_BERRIES - 1;
                BerryTreeGrow(tree);
            }
        }
    }

    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}

static void DebugAction_BerryFunctions_NextStage(u8 taskId)
{
    u8 i;
    struct BerryTree *tree;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].movementType == MOVEMENT_TYPE_BERRY_TREE_GROWTH)
        {
            tree = &gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(i)];
            BerryTreeGrow(tree);
        }
    }

    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}

static void DebugAction_BerryFunctions_Pests(u8 taskId)
{
    u8 i;

    if (!OW_BERRY_PESTS)
    {
        Debug_DestroyMenu_Full_Script(taskId, Debug_BerryPestsDisabled);
        return;
    }

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].movementType == MOVEMENT_TYPE_BERRY_TREE_GROWTH)
        {
            if (gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(i)].stage != BERRY_STAGE_PLANTED)
                gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(i)].pests = TRUE;
        }
    }

    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}

static void DebugAction_BerryFunctions_Weeds(u8 taskId)
{
    u8 i;

    if (!OW_BERRY_WEEDS)
    {
        Debug_DestroyMenu_Full_Script(taskId, Debug_BerryWeedsDisabled);
        return;
    }

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].movementType == MOVEMENT_TYPE_BERRY_TREE_GROWTH)
        {
            gSaveBlock1Ptr->berryTrees[GetObjectEventBerryTreeId(i)].weeds = TRUE;
        }
    }

    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}

// *******************************
// Actions Party/Boxes

static void DebugAction_Party_HealParty(u8 taskId)
{
    PlaySE(SE_USE_ITEM);
    HealPlayerParty();
    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}

static void DebugAction_Party_ClearParty(u8 taskId)
{
    ZeroPlayerPartyMons();
    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}

enum DebugTrainerIds
{
    DEBUG_TRAINER_PLAYER,
    DEBUG_TRAINER_AI,
    DEBUG_TRAINERS_COUNT
};

const struct Trainer sDebugTrainers[DIFFICULTY_COUNT][DEBUG_TRAINERS_COUNT] =
{
#include "data/debug_trainers.h"
};

const struct Trainer* GetDebugAiTrainer(void)
{
    return &sDebugTrainers[DIFFICULTY_NORMAL][DEBUG_TRAINER_AI];
}

static void DebugAction_Party_SetParty(u8 taskId)
{
    ZeroPlayerPartyMons();
    CreateNPCTrainerPartyFromTrainer(gPlayerParty, &sDebugTrainers[DIFFICULTY_NORMAL][DEBUG_TRAINER_PLAYER], TRUE, BATTLE_TYPE_TRAINER);
    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}

static void DebugAction_Party_BattleSingle(u8 taskId)
{
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    CreateNPCTrainerPartyFromTrainer(gPlayerParty, &sDebugTrainers[DIFFICULTY_NORMAL][DEBUG_TRAINER_PLAYER], TRUE, BATTLE_TYPE_TRAINER);
    CreateNPCTrainerPartyFromTrainer(gEnemyParty, GetDebugAiTrainer(), FALSE, BATTLE_TYPE_TRAINER);

    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    gDebugAIFlags = sDebugTrainers[DIFFICULTY_NORMAL][DEBUG_TRAINER_AI].aiFlags;
    gIsDebugBattle = TRUE;
    gBattleEnvironment = BattleSetup_GetEnvironmentId();
    CalculateEnemyPartyCount();
    BattleSetup_StartTrainerBattle_Debug();
    Debug_DestroyMenu_Full(taskId);
}

void CheckEWRAMCounters(struct ScriptContext *ctx)
{
    ConvertIntToDecimalStringN(gStringVar1, gFollowerSteps, STR_CONV_MODE_LEFT_ALIGN, 5);
    ConvertIntToDecimalStringN(gStringVar2, gChainFishingDexNavStreak, STR_CONV_MODE_LEFT_ALIGN, 5);
}

// Actions Ikigai Debug
static void DebugAction_Player_Nickname(u8 taskId)
{
    DoNamingScreen(NAMING_SCREEN_PLAYER_NICKNAME, gSaveBlock3Ptr->characters.playerNickname, gSaveBlock2Ptr->playerGender, 0, 0, CB2_ReturnToFieldContinueScript);
}

static void DebugAction_Ikigai_MeetAllCharacter(u8 taskId)
{
    IkigaiCharacter_SetAllMetFlags();
    ScriptContext_Enable();
    Debug_DestroyMenu_Full(taskId);
}

static const u8 sDebugText_Ikigai_Season_ID[] = _("Season ID: {STR_VAR_3}\n{STR_VAR_1}\n{STR_VAR_2}");
static void DebugAction_Ikigai_SeasonsSelect(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        PlaySE(SE_SELECT);
        Debug_HandleInput_Numeric(taskId, SEASON_SPRING, SEASON_COUNT - 1, 1);

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].tDigit]);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, 2);

        StringCopyPadded(gStringVar1, gSeasonNames[gTasks[taskId].tInput], CHAR_SPACE, 30);

        StringExpandPlaceholders(gStringVar4, sDebugText_Ikigai_Season_ID);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        Ikigai_SetToYearOneSeason(gTasks[taskId].tInput);

        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
        RtcCalcLocalTime();
        Ikigai_SetVyratonWeather();
        SetSavedWeatherFromCurrMapHeader();
        SetWeather(GetSavedWeather());
        SetMainCallback2(CB2_LoadMap);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DebugAction_DestroyExtraWindow(taskId);
    }
}

static void DebugAction_Ikigai_Season(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateSeason);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    //Display initial ID
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, SEASON_SPRING, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringCopyPadded(gStringVar1, gSeasonNames[SEASON_SPRING], CHAR_SPACE, 30);
    StringExpandPlaceholders(gStringVar4, sDebugText_Ikigai_Season_ID);
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

    gTasks[taskId].func = DebugAction_Ikigai_SeasonsSelect;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = 0;
    gTasks[taskId].tDigit = 0;
}

static void DebugAction_Ikigai_CalendarWarp(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    DoCalendarWarpHome();
}

static void DebugAction_Sound_MUS_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        const u8 *bgmName;
        Debug_HandleInput_Numeric(taskId, START_MUS, END_MUS, DEBUG_NUMBER_DIGITS_ITEMS);

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].tDigit]);
        bgmName = sBGMNames[gTasks[taskId].tInput - START_MUS];
        if (bgmName == NULL)
            bgmName = sDebugText_Dashes;
        StringCopyPadded(gStringVar1, bgmName, CHAR_SPACE, 35);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
        StringExpandPlaceholders(gStringVar4, sDebugText_Sound_Music_ID);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].tCurrentSong = gTasks[taskId].tInput;
        if (FLAG_DEBUG_SOUND_OVERWORLD_PLAY)
        {
            FadeOutAndPlayNewMapMusic(gTasks[taskId].tInput, 1);
        }
        else
        {
            m4aSongNumStop(gTasks[taskId].tCurrentSong);
            m4aSongNumStart(gTasks[taskId].tInput);
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        // m4aSongNumStop(gTasks[taskId].tCurrentSong);   //Uncomment if music should stop after leaving menu
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (JOY_NEW(START_BUTTON))
    {
        m4aSongNumStop(gTasks[taskId].tCurrentSong);
    }
}

static void DebugAction_Ikigai_MUS_Expansion_SelectId(u8 taskId)
{
    if (JOY_NEW(DPAD_ANY))
    {
        Debug_HandleInput_Numeric(taskId, MUSIC_EXPANSION_START + 1, END_MUS, DEBUG_NUMBER_DIGITS_ITEMS);

        StringCopy(gStringVar2, gText_DigitIndicator[gTasks[taskId].tDigit]);
        StringCopyPadded(gStringVar1, sBGMNames[gTasks[taskId].tInput - START_MUS], CHAR_SPACE, 35);
        ConvertIntToDecimalStringN(gStringVar3, gTasks[taskId].tInput, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
        StringExpandPlaceholders(gStringVar4, sDebugText_Sound_Music_ID);
        AddTextPrinterParameterized(gTasks[taskId].tSubWindowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);
    }

    if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].tCurrentSong = gTasks[taskId].tInput;
        if (FLAG_DEBUG_SOUND_OVERWORLD_PLAY)
        {
            FadeOutAndPlayNewMapMusic(gTasks[taskId].tInput, 1);
        }
        else
        {
            m4aSongNumStop(gTasks[taskId].tCurrentSong);
            m4aSongNumStart(gTasks[taskId].tInput);
        }
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        // m4aSongNumStop(gTasks[taskId].tCurrentSong);   //Uncomment if music should stop after leaving menu
        DebugAction_DestroyExtraWindow(taskId);
    }
    else if (JOY_NEW(START_BUTTON))
    {
        m4aSongNumStop(gTasks[taskId].tCurrentSong);
    }
}

static void DebugAction_Ikigai_MUS_Expansion(u8 taskId)
{
    u8 windowId;

    ClearStdWindowAndFrame(gTasks[taskId].tWindowId, TRUE);
    RemoveWindow(gTasks[taskId].tWindowId);

    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    windowId = AddWindow(&sDebugMenuWindowTemplateSound);
    DrawStdWindowFrame(windowId, FALSE);

    CopyWindowToVram(windowId, COPYWIN_FULL);

    // Display initial song
    StringCopy(gStringVar2, gText_DigitIndicator[0]);
    ConvertIntToDecimalStringN(gStringVar3, MUSIC_EXPANSION_START + 1, STR_CONV_MODE_LEADING_ZEROS, DEBUG_NUMBER_DIGITS_ITEMS);
    StringCopyPadded(gStringVar1, sBGMNames[MUSIC_EXPANSION_START - MUS_LITTLEROOT_TEST + 1], CHAR_SPACE, 35);
    StringExpandPlaceholders(gStringVar4, sDebugText_Sound_Music_ID);
    AddTextPrinterParameterized(windowId, DEBUG_MENU_FONT, gStringVar4, 0, 0, 0, NULL);

    StopMapMusic(); //Stop map music to better hear new music

    gTasks[taskId].func = DebugAction_Ikigai_MUS_Expansion_SelectId;
    gTasks[taskId].tSubWindowId = windowId;
    gTasks[taskId].tInput = MUSIC_EXPANSION_START + 1;
    gTasks[taskId].tDigit = 0;
    gTasks[taskId].tCurrentSong = gTasks[taskId].tInput;
}

static void DebugAction_OpenSubMenuDynamicMusic(u8 taskId, const struct DebugMenuOption *items)
{
    sDynamicMusicInput = ListMenu_ProcessInput(gTasks[taskId].tMenuTaskId);
    Debug_DestroyMenu(taskId);
    Debug_ShowMenu(DebugTask_HandleMenuInput_General, items);
}

static void DebugAction_DynamicMusic_InstrumentRemove(u8 taskId)
{
    if (sDynamicMusicInput < INSTRUMENT_COUNT)
        DynamicMusic_RemoveInstrument(sDynamicMusicInput);
    else if (sDynamicMusicInput == INSTRUMENT_COUNT)
        DynamicMusic_RemoveAllInstrumentNotPlaying();

    // Debug_DestroyMenu_Full(taskId);
    // ScriptContext_Enable();
}

static void DebugAction_DynamicMusic_InstrumentRestore(u8 taskId)
{
    if (sDynamicMusicInput < INSTRUMENT_COUNT)
        DynamicMusic_RestoreInstrument(sDynamicMusicInput);
    else if (sDynamicMusicInput == INSTRUMENT_COUNT)
        DynamicMusic_RestoreAllInstrumentPlaying();

    // Debug_DestroyMenu_Full(taskId);
    // ScriptContext_Enable();
}

static void DebugAction_DynamicMusic_InstrumentPlayOnly(u8 taskId)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
    if (sDynamicMusicInput < INSTRUMENT_COUNT)
        DynamicMusic_PlayOnlyInstrument(sDynamicMusicInput);
    else if (sDynamicMusicInput == INSTRUMENT_COUNT)
        DynamicMusic_PlayOnlyInstrumentPlaying();

    // Debug_DestroyMenu_Full(taskId);
    // ScriptContext_Enable();
}

static void DebugAction_DynamicMusic_TrackRemove(u8 taskId)
{
    u16 trackBits;
    if (sDynamicMusicInput == 0)
        trackBits = TRACKS_ALL;
    else
        trackBits = 1 << (sDynamicMusicInput - 1);
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, 0);

    // Debug_DestroyMenu_Full(taskId);
    // ScriptContext_Enable();
}

static void DebugAction_DynamicMusic_TrackRestore(u8 taskId)
{
    u16 trackBits;
    if (sDynamicMusicInput == 0)
        trackBits = TRACKS_ALL;
    else
        trackBits = 1 << (sDynamicMusicInput - 1);
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, 0x100);

    // Debug_DestroyMenu_Full(taskId);
    // ScriptContext_Enable();
}

static void DebugAction_DynamicMusic_TrackPlayOnly(u8 taskId)
{
    u16 trackBits;
    if (sDynamicMusicInput == 0)
        trackBits = TRACKS_ALL;
    else
        trackBits = 1 << (sDynamicMusicInput - 1);
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0);
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, 0x100);
    
    // Debug_DestroyMenu_Full(taskId);
    // ScriptContext_Enable();
}

static void DebugAction_DynamicMusic_Movement(u8 taskId)
{
    if (FindTaskIdByFunc(Task_UpdateMovementDynamicMusicWait) == TASK_NONE
        && FindTaskIdByFunc(Task_UpdateMovementDynamicMusic) == TASK_NONE)
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
        UpdateMovementDynamicMusic();
        Debug_DestroyMenu_Full(taskId);
        ScriptContext_Enable();
        return;
    }
    else if (FindTaskIdByFunc(Task_UpdateMovementDynamicMusicWait) != TASK_NONE)
    {
        DestroyTask(FindTaskIdByFunc(Task_UpdateMovementDynamicMusicWait));
    }
    else if (FindTaskIdByFunc(Task_UpdateMovementDynamicMusic) != TASK_NONE)
    {
        DestroyTask(FindTaskIdByFunc(Task_UpdateMovementDynamicMusic));
    }

    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
    Debug_DestroyMenu_Full(taskId);
    ScriptContext_Enable();
}

static void DebugAction_OpenSubMenuRotomStartMenu(u8 taskId, const struct DebugMenuOption *items)
{
    Debug_DestroyMenu(taskId);
    sDebugMenuListData->listId = 2;
    Debug_ShowMenu(DebugTask_HandleMenuInput_General, items);
}

static void DebugAction_Ikigai_TogglePokemonFlag(u8 taskId)
{
    if (FlagGet(FLAG_SYS_POKEMON_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(FLAG_SYS_POKEMON_GET);
}

static void DebugAction_Ikigai_ToggleSafariFlag(u8 taskId)
{
    if (GetSafariZoneFlag())
    {
        PlaySE(SE_PC_OFF);
        ResetSafariZoneFlag();
    }
    else
    {
        PlaySE(SE_PC_LOGIN);
        SetSafariZoneFlag();
    }
}

static void DebugAction_Ikigai_ToggleDexNavFlag(u8 taskId)
{
    if (FlagGet(DN_FLAG_DEXNAV_GET))
        PlaySE(SE_PC_OFF);
    else
        PlaySE(SE_PC_LOGIN);
    FlagToggle(DN_FLAG_DEXNAV_GET);
}

static void DebugAction_Ikigai_DefaultStartMenu(u8 taskId)
{
    Debug_DestroyMenu_Full(taskId);
    ShowStartMenu();
}

#undef tCurrentSong

#undef tMenuTaskId
#undef tWindowId
#undef tSubWindowId
#undef tInput
#undef tDigit