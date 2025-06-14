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


#define ROTOM_PHONE_UPDATE_CLOCK_DISPLAY    TRUE
#define ROTOM_PHONE_24_HOUR_MODE            gSaveBlock2Ptr->optionsClockMode
#define ROTOM_PHONE_NUM_MINUTES_TO_UPDATE   1
#define ROTOM_PHONE_MESSAGE_UPDATE_TIMER    (30 * 60) / FakeRtc_GetSecondsRatio() * ROTOM_PHONE_NUM_MINUTES_TO_UPDATE
#define ROTOM_PHONE_MESSAGE_SHUTDOWN_TIME   60
#define ROTOM_PHONE_UPDATE_MESSAGE          TRUE
#define ROTOM_PHONE_UPDATE_MESSAGE_SOUND    TRUE
#define FLIP_PHONE_OFFSCREEN_Y 96
#define FLIP_PHONE_SLIDE_DURATION 30


static void RotomPhone_SmallStartMenu_ContinueInit(bool32 firstInit);
static void Task_RotomPhone_SmallStartMenu_FlipPhoneOpen(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_FlipPhoneClose(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_HandleMainInput(u8 taskId);
static void RotomPhone_SmallStartMenu_RotomShutdownPreparation(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_RotomShutdown(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_CloseAndSave(u8 taskId);
static void Task_RotomPhone_HandleSave(u8 taskId);

static void RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(MainCallback callback);
static u8 RotomPhone_SmallStartMenu_DoCleanUpAndCreateTask(TaskFunc func, u8 priority);
static void RotomPhone_SmallStartMenu_DoCleanUpAndChangeTaskFunc(u8 taskId, TaskFunc func);
static void RotomPhone_SmallStartMenu_DoCleanUpAndDestroyTask(u8 taskId, bool32 overworldCleanup);

static void RotomPhone_SmallStartMenu_LoadSprites(void);
static void RotomPhone_SmallStartMenu_CreateRotomFaceSprite(void);
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
static void UNUSED RotomPhone_LargeStartMenu_CreateRegionButtons(void);
static void UNUSED RotomPhone_LargeStartMenu_StartRegionButtonAnim(enum Region region);
static void UNUSED RotomPhone_LargeStartMenu_StopRegionButtonAnim(enum Region region);
static void RotomPhone_LargeStartMenu_FreeResources(void);

static void RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(MainCallback callback);
static u8 RotomPhone_LargeStartMenu_DoCleanUpAndCreateTask(TaskFunc func, u8 priority);
static void RotomPhone_LargeStartMenu_DoCleanUpAndChangeTaskFunc(u8 taskId, TaskFunc func);
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


enum RotomPhoneMenuItems
{
    ROTOM_PHONE_MENU_FULL_SCREEN,
    ROTOM_PHONE_MENU_FLAG,
    ROTOM_PHONE_MENU_SHORTCUT,
    ROTOM_PHONE_MENU_CLOCK,
    ROTOM_PHONE_MENU_POKEDEX,
    ROTOM_PHONE_MENU_PARTY,
    ROTOM_PHONE_MENU_DAYCARE,
    ROTOM_PHONE_MENU_DEXNAV,
    ROTOM_PHONE_MENU_BAG,
    ROTOM_PHONE_MENU_POKENAV,
    ROTOM_PHONE_MENU_TRAINER_CARD,
    ROTOM_PHONE_MENU_SAVE,
    ROTOM_PHONE_MENU_OPTIONS,
    ROTOM_PHONE_MENU_COUNT,
};
#define ROTOM_PHONE_MENU_FIRST_OPTION ROTOM_PHONE_MENU_COUNT - ROTOM_PHONE_MENU_COUNT
#define ROTOM_PHONE_MENU_LAST_OPTION  ROTOM_PHONE_MENU_COUNT - 1
static enum RotomPhoneMenuItems RotomPhone_StartMenu_GetShortcutOption(void)
{
    return ROTOM_PHONE_MENU_POKEDEX;
}

enum RotomPhoneSmallOptions
{
    ROTOM_PHONE_SMALL_OPTION_1,
    ROTOM_PHONE_SMALL_OPTION_2,
    ROTOM_PHONE_SMALL_OPTION_3,
    ROTOM_PHONE_SMALL_OPTION_4,
    ROTOM_PHONE_SMALL_OPTION_5,
    ROTOM_PHONE_SMALL_OPTION_6,
    ROTOM_PHONE_SMALL_OPTION_COUNT,
};

enum RotomPhoneSpriteAnims
{
    SPRITE_INACTIVE,
    SPRITE_ACTIVE,
};

enum RotomPhoneMessages
{
    ROTOM_PHONE_MESSAGE_GOODBYE,
    ROTOM_PHONE_MESSAGE_TIME,
    ROTOM_PHONE_MESSAGE_SAFARI,
    ROTOM_PHONE_MESSAGE_DATE_WEATHER,
    ROTOM_PHONE_MESSAGE_PERSONALITY,
    ROTOM_PHONE_MESSAGE_FUN,
    ROTOM_PHONE_MESSAGE_ADVENTURE,
    ROTOM_PHONE_MESSAGE_COUNT,
};

enum RotomPhoneMessages_Greeting
{
    ROTOM_PHONE_MESSAGE_GREETING_GOOD_DAY,
    ROTOM_PHONE_MESSAGE_GREETING_HELLO,
    ROTOM_PHONE_MESSAGE_GREETING_HI,
    ROTOM_PHONE_MESSAGE_GREETING_HOW_ARE_YOU,
    ROTOM_PHONE_MESSAGE_GREETING_COUNT,
};

enum RotomPhoneMessages_Goodbye
{
    ROTOM_PHONE_MESSAGE_GOODBYE_GOODBYE,
    ROTOM_PHONE_MESSAGE_GOODBYE_SEE_YA,
    ROTOM_PHONE_MESSAGE_GOODBYE_LOGGING_OFF,
    ROTOM_PHONE_MESSAGE_GOODBYE_POWERING_DOWN,
    ROTOM_PHONE_MESSAGE_GOODBYE_COUNT,
};

enum RotomPhoneMessages_DateWeather
{
    ROTOM_PHONE_MESSAGE_DATE_WEATHER_DATE,
    ROTOM_PHONE_MESSAGE_DATE_WEATHER_SEASON,
    ROTOM_PHONE_MESSAGE_DATE_WEATHER_CURRENT_WEATHER,
    ROTOM_PHONE_MESSAGE_DATE_WEATHER_NEXT_WEATHER,
    ROTOM_PHONE_MESSAGE_DATE_WEATHER_COUNT,
};

enum RotomPhoneMessages_Personality
{
    ROTOM_PHONE_MESSAGE_PERSONALITY_MEEP_MORP,
    ROTOM_PHONE_MESSAGE_PERSONALITY_HANDS,
    ROTOM_PHONE_MESSAGE_PERSONALITY_RINGTONE,
    ROTOM_PHONE_MESSAGE_PERSONALITY_SCANNING,
    ROTOM_PHONE_MESSAGE_PERSONALITY_COUNT,
};

enum RotomPhoneMessages_Fun
{
    ROTOM_PHONE_MESSAGE_FUN_WINNING_GRINNING,
    ROTOM_PHONE_MESSAGE_FUN_ANYMORE_BATTLES,
    ROTOM_PHONE_MESSAGE_FUN_FUN_DETECTED,
    ROTOM_PHONE_MESSAGE_FUN_SOMETHING_SILLY,
    ROTOM_PHONE_MESSAGE_FUN_COUNT,
};

enum RotomPhoneMessages_Adventure
{
    ROTOM_PHONE_MESSAGE_ADVENTURE_TO_DO,
    ROTOM_PHONE_MESSAGE_ADVENTURE_GET_LOST,
    ROTOM_PHONE_MESSAGE_ADVENTURE_STEP_STORY,
    ROTOM_PHONE_MESSAGE_ADVENTURE_PACK_CURIOSITY,
    ROTOM_PHONE_MESSAGE_ADVENTURE_COUNT,
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
    MainCallback savedCallback;
    bool32 isLoading;
    bool32 spriteFlag; // some bool32 holding values for controlling the sprite anims and lifetime
    enum RotomPhoneMenuItems menuSmallOptions[ROTOM_PHONE_SMALL_OPTION_COUNT];
    u32 menuSmallRotomFaceSpriteId;
    u32 menuSmallRotomFaceFlashSpriteId;
    u32 menuSmallSpriteId[ROTOM_PHONE_SMALL_OPTION_COUNT];
    u32 menuSmallFlashSpriteId[ROTOM_PHONE_SMALL_OPTION_COUNT];
    u32 windowIdRotomSpeech_Top;
    u32 windowIdRotomSpeech_Bottom;
    u32 windowIdFlipPhone;
};

static EWRAM_DATA struct RotomPhone_StartMenu *sRotomPhone_SmallStartMenu = NULL;
static EWRAM_DATA enum RotomPhoneMenuItems menuSelectedSmall; // Separate memory allocation so it persist between destroying of menu.
static EWRAM_DATA enum RotomPhoneMenuItems menuSelectedLarge; // Separate memory allocation so it persist between destroying of menu.
static EWRAM_DATA bool32 menuFullScreen;

bool32 RotomPhone_StartMenu_IsFullScreen(void)
{
    return menuFullScreen;
}

// --BG-GFX--
static const u32 sSmallRotomTiles[] = INCBIN_U32("graphics/rotom_start_menu/rotom_phone_tiles.4bpp.smol");
static const u32 sSmallRotomTilemap[] = INCBIN_U32("graphics/rotom_start_menu/rotom_phone.bin.smolTM");
static const u32 sSmallRotomSpeechTilemap[] = INCBIN_U32("graphics/rotom_start_menu/rotom_phone_speech.bin.smolTM");
static const u32 sFlipPhoneTiles[] = INCBIN_U32("graphics/rotom_start_menu/flip_phone_tiles.4bpp.smol");
static const u32 sFlipPhoneOpenTilemap[] = INCBIN_U32("graphics/rotom_start_menu/flip_phone_open.bin.smolTM");
static const u32 sFlipPhoneClosedTilemap[] = INCBIN_U32("graphics/rotom_start_menu/flip_phone_closed.bin.smolTM");
static const u16 sPhoneMenuPal[] = INCBIN_U16("graphics/rotom_start_menu/phones.gbapal");
static const u32 sRotomPhoneFace[] = INCBIN_U32("graphics/rotom_start_menu/rotom_face.4bpp.smol");

//--SPRITE-GFX--
#define TAG_ICON_GFX 1234
#define TAG_ICON_PAL 0x4654 | BLEND_IMMUNE_FLAG
#define ICON_COORD_X 184
#define ICON_COORD_Y 50

static const u32 sIconGfx[] = INCBIN_U32("graphics/rotom_start_menu/icons.4bpp.smol");
static const u16 sIconPal[] = INCBIN_U16("graphics/rotom_start_menu/icons.gbapal");

#define ROTOM_SPEECH_WINDOW_WIDTH   18
#define ROTOM_SPEECH_WINDOW_WIDTH_PXL ROTOM_SPEECH_WINDOW_WIDTH * 8
#define ROTOM_SPEECH_WINDOW_HEIGHT  2
#define ROTOM_SPEECH_WINDOW_LEFT    1
#define ROTOM_SPEECH_WINDOW_TOP     15
static const struct WindowTemplate sWindowTemplate_RotomSpeech_Top = {
  .bg = 0, 
  .tilemapLeft = ROTOM_SPEECH_WINDOW_LEFT, 
  .tilemapTop = ROTOM_SPEECH_WINDOW_TOP, 
  .width = ROTOM_SPEECH_WINDOW_WIDTH,
  .height = ROTOM_SPEECH_WINDOW_HEIGHT, 
  .paletteNum = 14,
  .baseBlock = 0x50
};

static const struct WindowTemplate sWindowTemplate_RotomSpeech_Bottom = {
    .bg = 0, 
    .tilemapLeft = ROTOM_SPEECH_WINDOW_LEFT, 
    .tilemapTop = ROTOM_SPEECH_WINDOW_TOP + 2, 
    .width = ROTOM_SPEECH_WINDOW_WIDTH, 
    .height = ROTOM_SPEECH_WINDOW_HEIGHT, 
    .paletteNum = 14,
    .baseBlock = 0x50 + (ROTOM_SPEECH_WINDOW_WIDTH*ROTOM_SPEECH_WINDOW_WIDTH)
};

static const struct WindowTemplate sWindowTemplate_FlipPhone = {
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 17,
    .width = 7,
    .height = 2,
    .paletteNum = 14,
    .baseBlock = 0x50
};

static const struct SpritePalette sSpritePal_Icon[] =
{
    {sIconPal, TAG_ICON_PAL},
    {NULL},
};

static const struct CompressedSpriteSheet sSpriteSheet_Icon[] = 
{
    {sIconGfx, 16*352/2 , TAG_ICON_GFX},
    {NULL},
};

static const struct CompressedSpriteSheet sSpriteSheet_RotomFace[] = 
{
    {sRotomPhoneFace, 64*64/2 , TAG_ICON_GFX + 1},
    {NULL},
};

static const struct OamData gOamIcon = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .objMode = 0,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
};

static const struct OamData sRotomFace_Oam = {
    .size = SPRITE_SIZE(64x64),
    .shape = SPRITE_SHAPE(64x64),
    .priority = 0,
};

static const union AnimCmd sAnimCmdPoketch_NotSelected[] = {
    ANIMCMD_FRAME(28, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdPoketch_Selected[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdPokedex_NotSelected[] = {
    ANIMCMD_FRAME(32, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdPokedex_Selected[] = {
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdParty_NotSelected[] = {
    ANIMCMD_FRAME(36, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdParty_Selected[] = {
    ANIMCMD_FRAME(8, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdBag_NotSelected[] = {
    ANIMCMD_FRAME(40, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdBag_Selected[] = {
    ANIMCMD_FRAME(12, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdTrainerCard_NotSelected[] = {
    ANIMCMD_FRAME(44, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdTrainerCard_Selected[] = {
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdSave_NotSelected[] = {
    ANIMCMD_FRAME(48, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdSave_Selected[] = {
    ANIMCMD_FRAME(20, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconSaveAnim[] = {
};

static const union AnimCmd sAnimCmdOptions_NotSelected[] = {
    ANIMCMD_FRAME(52, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdOptions_Selected[] = {
    ANIMCMD_FRAME(24, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdFlag_NotSelected[] = {
    ANIMCMD_FRAME(60, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdFlag_Selected[] = {
    ANIMCMD_FRAME(56, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdFullScreen_NotSelected[] = {
    ANIMCMD_FRAME(68, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdFullScreen_Selected[] = {
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdDexNav_NotSelected[] = {
    ANIMCMD_FRAME(76, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdDexNav_Selected[] = {
    ANIMCMD_FRAME(72, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdClock_NotSelected[] = {
    ANIMCMD_FRAME(84, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sAnimCmdClock_Selected[] = {
    ANIMCMD_FRAME(80, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sSmallIconAnims[ROTOM_PHONE_MENU_COUNT * 2] = {
    sAnimCmdFullScreen_NotSelected,
    sAnimCmdFullScreen_Selected,
    sAnimCmdFlag_NotSelected,
    sAnimCmdFlag_Selected,
    sAnimCmdFullScreen_NotSelected, // Shortcut
    sAnimCmdFullScreen_Selected,
    sAnimCmdClock_NotSelected,
    sAnimCmdClock_Selected,
    sAnimCmdPokedex_NotSelected,
    sAnimCmdPokedex_Selected,
    sAnimCmdParty_NotSelected,
    sAnimCmdParty_Selected,
    sAnimCmdFullScreen_NotSelected, // Daycare
    sAnimCmdFullScreen_Selected,
    sAnimCmdDexNav_NotSelected,
    sAnimCmdDexNav_Selected,
    sAnimCmdBag_NotSelected,
    sAnimCmdBag_Selected,
    sAnimCmdPoketch_NotSelected,
    sAnimCmdPoketch_Selected,
    sAnimCmdTrainerCard_NotSelected,
    sAnimCmdTrainerCard_Selected,
    sAnimCmdSave_NotSelected,
    sAnimCmdSave_Selected,
    sAnimCmdOptions_NotSelected,
    sAnimCmdOptions_Selected,
};

static const struct SpriteTemplate sSpriteTemplate_RotomSmallIcon = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = sSmallIconAnims,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteTemplate sSpriteTemplate_RotomSmallFace = {
    .tileTag = TAG_ICON_GFX + 1,
    .paletteTag = TAG_ICON_PAL,
    .oam = &sRotomFace_Oam,
    .callback = SpriteCallbackDummy,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable,
};


static struct RotomPhoneMenuOptions sRotomPhoneOptions[ROTOM_PHONE_MENU_COUNT] =
{
    [ROTOM_PHONE_MENU_POKEDEX] =
    {
        .menuName = COMPOUND_STRING("Pokédex"),
        .rotomAction = COMPOUND_STRING("to open the Pokédex?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Pokedex,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Pokedex,
    },
    [ROTOM_PHONE_MENU_DEXNAV] =
    {
        .menuName = COMPOUND_STRING("DexNav"),
        .rotomAction = COMPOUND_STRING("to use the DexNav?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_DexNav,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_DexNav,
    },
    [ROTOM_PHONE_MENU_PARTY] =
    {
        .menuName = COMPOUND_STRING("Party"),
        .rotomAction = COMPOUND_STRING("to view your Party?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Pokemon,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Pokemon,
    },
    [ROTOM_PHONE_MENU_BAG] =
    {
        .menuName = COMPOUND_STRING("Bag"),
        .rotomAction = COMPOUND_STRING("to check your Storage?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Bag,
    },
    [ROTOM_PHONE_MENU_POKENAV] =
    {
        .menuName = COMPOUND_STRING("PokéNav"),
        .rotomAction = COMPOUND_STRING("to browse the PokéNav?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_PokeNav,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_PokeNav,
    },
    [ROTOM_PHONE_MENU_TRAINER_CARD] =
    {
        .menuName = COMPOUND_STRING("Trainer"),
        .rotomAction = COMPOUND_STRING("to view your ID Card?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked_FullScreen,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Trainer,
    },
    [ROTOM_PHONE_MENU_SAVE] =
    {
        .menuName = COMPOUND_STRING("Save"),
        .rotomAction = COMPOUND_STRING("to write in your Journal?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Save,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Save,
    },
    [ROTOM_PHONE_MENU_OPTIONS] =
    {
        .menuName = COMPOUND_STRING("Settings"),
        .rotomAction = COMPOUND_STRING("to change the Settings?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Settings,
    },
    [ROTOM_PHONE_MENU_FLAG] =
    {
        .menuName = COMPOUND_STRING("Retire"),
        .rotomAction = COMPOUND_STRING("to end the Safari?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_SafariFlag,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_SafariFlag,
    },
    [ROTOM_PHONE_MENU_FULL_SCREEN] =
    {
        .menuName = COMPOUND_STRING("Full Screen"),
        .rotomAction = COMPOUND_STRING("to enter Full Screen?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_FullScreen,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_FullScreen,
    },
    [ROTOM_PHONE_MENU_CLOCK] =
    {
        .menuName = COMPOUND_STRING("Clock"),
        .rotomAction = COMPOUND_STRING("to check the time?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Clock,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Clock,
    },
    [ROTOM_PHONE_MENU_SHORTCUT] =
    {
        .menuName = COMPOUND_STRING("Shortcut"),
        .rotomAction = COMPOUND_STRING("Shortcut Action"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Shortcut,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Shortcut,
    },
    [ROTOM_PHONE_MENU_DAYCARE] =
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

#define FLIP_PHONE_BG_COLOUR       7
#define FLIP_PHONE_TEXT_FG_COLOUR  3
#define FLIP_PHONE_TEXT_BG_COLOUR  6
#define ROTOM_PHONE_BG_COLOUR       8
#define ROTOM_PHONE_TEXT_FG_COLOUR  3
#define ROTOM_PHONE_TEXT_BG_COLOUR  6
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


static enum RotomPhoneMenuItems RotomPhone_SetFirstSelectedMenu(void)
{
    for (enum RotomPhoneMenuItems menuOption = ROTOM_PHONE_MENU_FIRST_OPTION; menuOption < ROTOM_PHONE_MENU_COUNT; menuOption++)
    {
        if (sRotomPhoneOptions[menuOption].unlockedFunc && sRotomPhoneOptions[menuOption].unlockedFunc())
        {
            return menuOption;
        }
    }

    return ROTOM_PHONE_MENU_OPTIONS;
}

#define tRotomUpdateTimer gTasks[taskId].data[0]
#define tRotomUpdateMessage gTasks[taskId].data[1]
#define tRotomMessageSoundEffect gTasks[taskId].data[2]
#define tRotomPanelComfyAnimId gTasks[taskId].data[3]
#define tRotomPanelLastY gTasks[taskId].data[4]
#define tFlipPhoneY gTasks[taskId].data[5]
#define tFlipComfyAnimId gTasks[taskId].data[6]
#define tRotomPhoneCloseToSave gTasks[taskId].data[7]
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

    if (FlagGet(FLAG_SYS_POKEDEX_GET) && ROTOM_PHONE_UPDATE_MESSAGE_SOUND)
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x80);

    sRotomPhone_SmallStartMenu->savedCallback = CB2_ReturnToFieldWithOpenMenu;
    sRotomPhone_SmallStartMenu->isLoading = FALSE;
    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top = 0;
    sRotomPhone_SmallStartMenu->spriteFlag = FALSE;
    menuFullScreen = FALSE;

    sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId = SPRITE_NONE;
    sRotomPhone_SmallStartMenu->menuSmallRotomFaceFlashSpriteId = SPRITE_NONE;
    for (enum RotomPhoneSmallOptions smallOptions = ROTOM_PHONE_SMALL_OPTION_1; smallOptions < ROTOM_PHONE_SMALL_OPTION_COUNT; smallOptions++)
    {
        sRotomPhone_SmallStartMenu->menuSmallSpriteId[smallOptions] = SPRITE_NONE;
        sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[smallOptions] = SPRITE_NONE;
    }

    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top = WINDOW_NONE;
    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom = WINDOW_NONE;
    sRotomPhone_SmallStartMenu->windowIdFlipPhone = WINDOW_NONE;

    RotomPhone_SmallStartMenu_LoadBgGfx(firstInit);
    RotomPhone_SmallStartMenu_LoadSprites();

    if (!FlagGet(FLAG_SYS_POKEDEX_GET) && firstInit)
    {
        SetGpuReg(REG_OFFSET_BG0VOFS, -FLIP_PHONE_OFFSCREEN_Y);
        taskId = CreateTask(Task_RotomPhone_SmallStartMenu_FlipPhoneOpen, 0);
        tFlipPhoneY = FLIP_PHONE_OFFSCREEN_Y;
        return;
    }

    RotomPhone_SmallStartMenu_ContinueInit(firstInit);
}

static void RotomPhone_SmallStartMenu_ContinueInit(bool32 firstInit)
{
    u8 taskId;

    RotomPhone_SmallStartMenu_CreateAllSprites();
    RotomPhone_SmallStartMenu_CreateRotomFaceSprite();
    RotomPhone_SmallStartMenu_CreateSpeechWindows();
    RotomPhone_SmallStartMenu_CreateFlipPhoneWindow();
    ScheduleBgCopyTilemapToVram(0);

    if (!sRotomPhoneOptions[menuSelectedSmall].unlockedFunc || !sRotomPhoneOptions[menuSelectedSmall].unlockedFunc())
        menuSelectedSmall = RotomPhone_SetFirstSelectedMenu();

    if (!FlagGet(FLAG_SYS_POKEDEX_GET) && firstInit)
    {
        taskId = FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_FlipPhoneOpen);

        gTasks[taskId].func = Task_RotomPhone_SmallStartMenu_HandleMainInput;
    }
    else
    {
        taskId = CreateTask(Task_RotomPhone_SmallStartMenu_HandleMainInput, 0);
    }
    tRotomUpdateTimer = ROTOM_PHONE_MESSAGE_UPDATE_TIMER / ROTOM_PHONE_NUM_MINUTES_TO_UPDATE;
    tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_TIME;

    if (GetSafariZoneFlag())
        tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_SAFARI;

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
    index = IndexOfSpritePaletteTag(TAG_ICON_PAL);
    LoadPalette(sIconPal, OBJ_PLTT_ID(index), PLTT_SIZE_4BPP); 
    LoadCompressedSpriteSheet(sSpriteSheet_Icon);
    LoadCompressedSpriteSheet(sSpriteSheet_RotomFace);
}

static void RotomPhone_SmallStartMenu_CreateRotomFaceSprite(void)
{
    if (!FlagGet(FLAG_SYS_POKEDEX_GET))
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
    s32 x = ICON_COORD_X;
    s32 y = ICON_COORD_Y;
    u32 iconRow;
    u32 iconColumn;
    u32 animNum;
    if (menuItem != ROTOM_PHONE_MENU_SHORTCUT)
        animNum = menuItem;
    else
        animNum = RotomPhone_StartMenu_GetShortcutOption();
    
    animNum *= 2;

    if (!FlagGet(FLAG_SYS_POKEDEX_GET))
        y += 19;

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
        x + (iconColumn * 24),
        y + (iconRow * 24),
        0
    );
    StartSpriteAnim(&gSprites[sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId]], animNum);

    if (flash)
    {
        sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId] = CreateSprite(
            &sSpriteTemplate_RotomSmallIcon,
            x + (iconColumn * 24),
            y + (iconRow * 24),
            0
        );
        gSprites[sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId]].oam.objMode = ST_OAM_OBJ_WINDOW;
        StartSpriteAnim(&gSprites[sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId]], animNum);
    }
}

static void RotomPhone_SmallStartMenu_CreateAllSprites(void)
{
    enum RotomPhoneSmallOptions drawn = ROTOM_PHONE_SMALL_OPTION_1;

    for (enum RotomPhoneMenuItems menuId = ROTOM_PHONE_MENU_FIRST_OPTION; menuId < ROTOM_PHONE_MENU_COUNT && drawn < ROTOM_PHONE_SMALL_OPTION_COUNT; menuId++)
    {
        const struct RotomPhoneMenuOptions *menuOption = &sRotomPhoneOptions[menuId];

        if (menuOption->unlockedFunc && menuOption->unlockedFunc())
        {
            enum RotomPhoneSmallOptions optionSlot = ROTOM_PHONE_SMALL_OPTION_1 + drawn;

            RotomPhone_SmallStartMenu_CreateSprite(menuId, optionSlot);
            sRotomPhone_SmallStartMenu->menuSmallOptions[optionSlot] = menuId;
            drawn++;
        }
    }

    for (; drawn < ROTOM_PHONE_SMALL_OPTION_COUNT; drawn++)
    {
        sRotomPhone_SmallStartMenu->menuSmallOptions[drawn] = ROTOM_PHONE_MENU_COUNT;
    }
}

static void RotomPhone_SmallStartMenu_LoadBgGfx(bool32 firstInit)
{
    u8* buf = GetBgTilemapBuffer(0);
    const u32 *tilemap;
    LoadBgTilemap(0, 0, 0, 0);
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
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
    
    LoadPalette(sPhoneMenuPal, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
    ScheduleBgCopyTilemapToVram(0);
}
#define ROTOM_SPEECH_TOP_ROW_Y      1
#define ROTOM_SPEECH_BOTTOM_ROW_Y   1
static void RotomPhone_SmallStartMenu_CreateSpeechWindows(void)
{
    if (!FlagGet(FLAG_SYS_POKEDEX_GET))
        return;

    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top = AddWindow(&sWindowTemplate_RotomSpeech_Top);
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top, PIXEL_FILL(ROTOM_PHONE_BG_COLOUR));
    PutWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top);

    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom = AddWindow(&sWindowTemplate_RotomSpeech_Bottom);
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom, PIXEL_FILL(ROTOM_PHONE_BG_COLOUR));
    PutWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom);
}

static void RotomPhone_SmallStartMenu_CreateFlipPhoneWindow(void)
{
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
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
    if (FlagGet(!FLAG_SYS_POKEDEX_GET))
        return;
    
    u8 textBuffer[80];
    enum RotomPhoneMessages_Greeting messageRotom = Random() % ROTOM_PHONE_MESSAGE_GREETING_COUNT;

    switch (messageRotom)
    {
    default:
    case ROTOM_PHONE_MESSAGE_GREETING_GOOD_DAY:
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
    
    case ROTOM_PHONE_MESSAGE_GREETING_HELLO:
        StringCopy(textBuffer, COMPOUND_STRING("Hello there, "));
        break;
    
    case ROTOM_PHONE_MESSAGE_GREETING_HI:
        StringCopy(textBuffer, COMPOUND_STRING("Hi, "));
        break;
    
    case ROTOM_PHONE_MESSAGE_GREETING_HOW_ARE_YOU:
        StringCopy(textBuffer, COMPOUND_STRING("How are you, "));
        break;
    }

    StringAppend(textBuffer, gSaveBlock3Ptr->characters.playerNickname);

    if (messageRotom != ROTOM_PHONE_MESSAGE_GREETING_HOW_ARE_YOU)
        StringAppend(textBuffer, COMPOUND_STRING("."));
    else
        StringAppend(textBuffer, COMPOUND_STRING("?"));

    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
}

static enum RotomPhoneMessages RotomPhone_SmallStartMenu_GetRandomMessage(void)
{
    if (!ROTOM_PHONE_UPDATE_MESSAGE)
        return ROTOM_PHONE_MESSAGE_TIME;
    
    enum RotomPhoneMessages messageRandom;
    messageRandom = Random() % ROTOM_PHONE_MESSAGE_COUNT;
    while (messageRandom == ROTOM_PHONE_MESSAGE_GOODBYE
    || messageRandom == ROTOM_PHONE_MESSAGE_TIME
    || messageRandom == ROTOM_PHONE_MESSAGE_SAFARI)
    {
        messageRandom = Random() % ROTOM_PHONE_MESSAGE_COUNT;
    }

    return messageRandom;
}

static void RotomPhone_SmallStartMenu_CheckUpdateMessage(u8 taskId)
{
    if (!tRotomUpdateTimer && FlagGet(FLAG_SYS_POKEDEX_GET))
    {
        switch (tRotomUpdateMessage)
        {
        case ROTOM_PHONE_MESSAGE_GOODBYE:
            RotomPhone_SmallStartMenu_PrintGoodbye(taskId);
            break;
        
        default:
        case ROTOM_PHONE_MESSAGE_TIME:
            RotomPhone_SmallStartMenu_PrintTime(taskId);
            break;

        case ROTOM_PHONE_MESSAGE_SAFARI:
            RotomPhone_SmallStartMenu_PrintSafari(taskId);
            break;
        
        case ROTOM_PHONE_MESSAGE_DATE_WEATHER:
            RotomPhone_SmallStartMenu_PrintDateWeather(taskId);
            break;

        case ROTOM_PHONE_MESSAGE_PERSONALITY:
            RotomPhone_SmallStartMenu_Personality(taskId);
            break;

        case ROTOM_PHONE_MESSAGE_FUN:
            RotomPhone_SmallStartMenu_PrintHaveFun(taskId);
            break;

        case ROTOM_PHONE_MESSAGE_ADVENTURE:
            RotomPhone_SmallStartMenu_PrintAdventure(taskId);
            break;
        }
        tRotomUpdateTimer = ROTOM_PHONE_MESSAGE_UPDATE_TIMER;
        if (!ROTOM_PHONE_UPDATE_MESSAGE && !GetSafariZoneFlag() && tRotomUpdateMessage != ROTOM_PHONE_MESSAGE_GOODBYE)
            tRotomUpdateTimer *= 2;
        
        if (ROTOM_PHONE_UPDATE_MESSAGE_SOUND && tRotomUpdateMessage != ROTOM_PHONE_MESSAGE_GOODBYE)
            tRotomMessageSoundEffect = PMD_EVENT_SIGN_HATENA_02;
    }
}

static void RotomPhone_SmallStartMenu_PrintGoodbye(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_Goodbye messageRotom = Random() % ROTOM_PHONE_MESSAGE_GOODBYE_COUNT;

    switch (messageRotom)
    {
    default:
    case ROTOM_PHONE_MESSAGE_GOODBYE_GOODBYE:
        StringCopy(textBuffer, COMPOUND_STRING("Goodbye, "));
        StringAppend(textBuffer, gSaveBlock3Ptr->characters.playerNickname);
        StringAppend(textBuffer, COMPOUND_STRING("."));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, FALSE);

        StringCopy(textBuffer, COMPOUND_STRING("I'll see you later!"));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, FALSE);
        break;
    
    case ROTOM_PHONE_MESSAGE_GOODBYE_SEE_YA:
        StringCopy(textBuffer, COMPOUND_STRING("See ya later, "));
        StringAppend(textBuffer, gSaveBlock3Ptr->characters.playerNickname);
        StringAppend(textBuffer, COMPOUND_STRING("!"));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, FALSE);

        StringCopy(textBuffer, COMPOUND_STRING("Don't miss me too much."));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, FALSE);
        break;
    
    case ROTOM_PHONE_MESSAGE_GOODBYE_LOGGING_OFF:
        StringCopy(textBuffer, COMPOUND_STRING("Logging off for now…"));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, FALSE);

        StringCopy(textBuffer, COMPOUND_STRING("Catch you later, "));
        StringAppend(textBuffer, gSaveBlock3Ptr->characters.playerNickname);
        StringAppend(textBuffer, COMPOUND_STRING("."));
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, FALSE);
        break;
    
    case ROTOM_PHONE_MESSAGE_GOODBYE_POWERING_DOWN:
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
    FormatDecimalTimeWithoutSeconds(time, gLocalTime.hours, gLocalTime.minutes, ROTOM_PHONE_24_HOUR_MODE);
    StringCopy(textBuffer, COMPOUND_STRING("It is "));
    StringAppend(textBuffer, time);
    StringAppend(textBuffer, COMPOUND_STRING(" on "));
    StringAppend(textBuffer, gDayNameStringsTable[(gLocalTime.days % WEEKDAY_COUNT)]);
    StringAppend(textBuffer, COMPOUND_STRING("."));
    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = RotomPhone_SmallStartMenu_GetRandomMessage();
    if (GetSafariZoneFlag())
        tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_SAFARI;
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
    tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_PrintDateWeather(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_DateWeather messageRotom = Random() % ROTOM_PHONE_MESSAGE_DATE_WEATHER_COUNT;

    if (messageRotom == ROTOM_PHONE_MESSAGE_DATE_WEATHER_SEASON)
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
    else if (messageRotom == ROTOM_PHONE_MESSAGE_DATE_WEATHER_CURRENT_WEATHER)
    {
        StringCopy(textBuffer, COMPOUND_STRING("Looking like it is "));
        StringAppend(textBuffer, GetWeatherAction(GetCurrentWeather()));
        StringAppend(textBuffer, COMPOUND_STRING(" right now."));
    }
    else if (messageRotom == ROTOM_PHONE_MESSAGE_DATE_WEATHER_NEXT_WEATHER && gTimeOfDay < TIME_NIGHT)
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
    tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_Personality(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_Personality messageRotom = Random() % ROTOM_PHONE_MESSAGE_PERSONALITY_COUNT;

    switch (messageRotom)
    {
    default:
    case ROTOM_PHONE_MESSAGE_PERSONALITY_MEEP_MORP:
        StringCopy(textBuffer, COMPOUND_STRING("Meep-morp!"));
        break;
    
    case ROTOM_PHONE_MESSAGE_PERSONALITY_HANDS:
        StringCopy(textBuffer, COMPOUND_STRING("If I had hands, I'd give you a high-five!"));
        break;
    
    case ROTOM_PHONE_MESSAGE_PERSONALITY_RINGTONE:
        StringCopy(textBuffer, COMPOUND_STRING("I've got a new ringtone to show you!"));
        break;
    
    case ROTOM_PHONE_MESSAGE_PERSONALITY_SCANNING:
        StringCopy(textBuffer, COMPOUND_STRING("Scanning… Yup, you're still awesome!"));
        break;
    }
    
    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_PrintHaveFun(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_Fun messageRotom = Random() % ROTOM_PHONE_MESSAGE_FUN_COUNT;

    switch (messageRotom)
    {
    default:
    case ROTOM_PHONE_MESSAGE_FUN_WINNING_GRINNING:
        StringCopy(textBuffer, COMPOUND_STRING("If you're winning, I am grinning!"));
        break;
    
    case ROTOM_PHONE_MESSAGE_FUN_ANYMORE_BATTLES:
        StringCopy(textBuffer, COMPOUND_STRING("Do we have anymore battles today?"));
        break;
    
    case ROTOM_PHONE_MESSAGE_FUN_FUN_DETECTED:
        StringCopy(textBuffer, COMPOUND_STRING("Fun detected, AMPLIFYING!"));
        break;
    
    case ROTOM_PHONE_MESSAGE_FUN_SOMETHING_SILLY:
        StringCopy(textBuffer, COMPOUND_STRING("Wanna do something silly? I am in!"));
        break;
    }

    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_PrintAdventure(u8 taskId)
{
    u8 textBuffer[80];
    enum RotomPhoneMessages_Adventure messageRotom = Random() % ROTOM_PHONE_MESSAGE_ADVENTURE_COUNT;

    switch (messageRotom)
    {
    default:
    case ROTOM_PHONE_MESSAGE_ADVENTURE_TO_DO:
        u8 location[16];
        StringCopy(textBuffer, COMPOUND_STRING("What's there to do in "));
        GetMapName(location, GetCurrentRegionMapSectionId(), 0);
        StringAppend(textBuffer, location);
        StringAppend(textBuffer, COMPOUND_STRING("?"));
        break;
    
    case ROTOM_PHONE_MESSAGE_ADVENTURE_GET_LOST:
        StringCopy(textBuffer, COMPOUND_STRING("Let's get lost, in the best way."));
        break;
    
    case ROTOM_PHONE_MESSAGE_ADVENTURE_STEP_STORY:
        StringCopy(textBuffer, COMPOUND_STRING("One more step, one more story."));
        break;
    
    case ROTOM_PHONE_MESSAGE_ADVENTURE_PACK_CURIOSITY:
        StringCopy(textBuffer, COMPOUND_STRING("Pack your curiosity, we're going exploring."));
        break;
    }

    
    RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, TRUE, TRUE);
    tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_TIME;
}

static void RotomPhone_SmallStartMenu_UpdateMenuPrompt(u8 taskId)
{
    u8 fontId;
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
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
        
        if (menuSelectedSmall == ROTOM_PHONE_MENU_SHORTCUT)
            StringAppend(textBuffer, sRotomPhoneOptions[RotomPhone_StartMenu_GetShortcutOption()].rotomAction);
        else
            StringAppend(textBuffer, sRotomPhoneOptions[menuSelectedSmall].rotomAction);
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, TRUE);
    }
    else
    {
        u8 menuName[24];
        StringCopy(menuName, sRotomPhoneOptions[menuSelectedSmall].menuName);
        if (!StringCompare(menuName, sRotomPhoneOptions[ROTOM_PHONE_MENU_SHORTCUT].menuName))
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
        DestroySpriteAndFreeResources(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceSpriteId]);
    }
    if (sRotomPhone_SmallStartMenu->menuSmallRotomFaceFlashSpriteId != SPRITE_NONE)
    {
        FreeSpriteOamMatrix(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceFlashSpriteId]);
        DestroySpriteAndFreeResources(&gSprites[sRotomPhone_SmallStartMenu->menuSmallRotomFaceFlashSpriteId]);
    }
    for (enum RotomPhoneSmallOptions spriteId = ROTOM_PHONE_SMALL_OPTION_1; spriteId < ROTOM_PHONE_SMALL_OPTION_COUNT; spriteId++)
    {
        if (sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId] != SPRITE_NONE)
        {
            FreeSpriteOamMatrix(&gSprites[sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId]]);
            DestroySpriteAndFreeResources(&gSprites[sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId]]);
        }
        if (sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId] != SPRITE_NONE)
        {
            FreeSpriteOamMatrix(&gSprites[sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId]]);
            DestroySpriteAndFreeResources(&gSprites[sRotomPhone_SmallStartMenu->menuSmallFlashSpriteId[spriteId]]);
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
        FreeSpriteTilesByTag(TAG_ICON_GFX);  
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

static u8 RotomPhone_SmallStartMenu_DoCleanUpAndCreateTask(TaskFunc func, u8 priority)
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

static void Task_RotomPhone_HandleSave(u8 taskId)
{
    switch (RunSaveCallback_Global())
    {
        case SAVE_IN_PROGRESS:
            break;
        case SAVE_SUCCESS:
        case SAVE_CANCELED: // Back to start menu
            ClearDialogWindowAndFrameToTransparent(0, TRUE);
            ScriptUnfreezeObjectEvents();  
            UnlockPlayerFieldControls();
            if (!RotomPhone_StartMenu_IsFullScreen())
            {
                DestroyTask(taskId);
                m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 256);
            }
            else
            {
                RotomPhone_LargeStartMenu_DoCleanUpAndChangeTaskFunc(taskId, Task_OpenRotomPhone_LargeStartMenu);
                // RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_ReturnToField);
            }
            break;
        case SAVE_ERROR:    // Close start menu
            ClearDialogWindowAndFrameToTransparent(0, TRUE);
            ScriptUnfreezeObjectEvents();
            UnlockPlayerFieldControls();
            SoftResetInBattlePyramid();
            if (!RotomPhone_StartMenu_IsFullScreen())
            {
                DestroyTask(taskId);
                m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 256);
            }
            else
            {
                RotomPhone_LargeStartMenu_DoCleanUpAndChangeTaskFunc(taskId, Task_OpenRotomPhone_LargeStartMenu);
                // RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_ReturnToField);
            }
            break;
    }
}

#define STD_WINDOW_BASE_TILE_NUM 0x214
#define STD_WINDOW_PALETTE_NUM 14

static void RotomPhone_SmallStartMenu_HandleInput(u8 taskId)
{
    sRotomPhone_SmallStartMenu->spriteFlag = FALSE;
    enum RotomPhoneSmallOptions optionCurrent = ROTOM_PHONE_SMALL_OPTION_1;
    s32 offset;
    u32 nextIndex;

    for (enum RotomPhoneSmallOptions i = ROTOM_PHONE_SMALL_OPTION_1; i < ROTOM_PHONE_SMALL_OPTION_COUNT; i++)
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
        offset = (optionCurrent % 2 == 1) ? -1 : ROTOM_PHONE_SMALL_OPTION_COUNT;
    else  if (JOY_NEW(DPAD_RIGHT))
        offset = (optionCurrent % 2 == 0) ? 1 : ROTOM_PHONE_SMALL_OPTION_COUNT;
    else
        offset = 2;

    nextIndex = optionCurrent + offset;
    if (nextIndex >= ROTOM_PHONE_SMALL_OPTION_COUNT
        || nextIndex < ROTOM_PHONE_SMALL_OPTION_1
        || sRotomPhone_SmallStartMenu->menuSmallOptions[nextIndex] == ROTOM_PHONE_MENU_COUNT)
    {
        if (FlagGet(FLAG_SYS_POKEDEX_GET))
            tRotomMessageSoundEffect = PMD_EVENT_SIGN_ANGER_02;
        else
            tRotomMessageSoundEffect = SE_CLICK;
        return;
    }

    menuSelectedSmall = sRotomPhone_SmallStartMenu->menuSmallOptions[nextIndex];
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
        tRotomMessageSoundEffect = PMD_EVENT_SIGN_ASE_01;
    else
        tRotomMessageSoundEffect = SE_CLICK;

    RotomPhone_SmallStartMenu_UpdateMenuPrompt(taskId);
}

static void Task_RotomPhone_SmallStartMenu_FlipPhoneOpen(u8 taskId)
{
    TryAdvanceComfyAnim(&gComfyAnims[tFlipComfyAnimId]);
    if (tFlipPhoneY == FLIP_PHONE_OFFSCREEN_Y)
    {
        struct ComfyAnimEasingConfig config;
        InitComfyAnimConfig_Easing(&config);
        config.durationFrames = FLIP_PHONE_SLIDE_DURATION;
        config.from = Q_24_8(FLIP_PHONE_OFFSCREEN_Y);
        config.to = Q_24_8(0);
        config.easingFunc = ComfyAnimEasing_EaseOutQuad;
        tFlipComfyAnimId = CreateComfyAnim_Easing(&config);
        TryAdvanceComfyAnim(&gComfyAnims[tFlipComfyAnimId]);

        tFlipPhoneY = ReadComfyAnimValueSmooth(&gComfyAnims[tFlipComfyAnimId]);
    }
    else if (GetEasingComfyAnim_CurrentFrame(&gComfyAnims[tFlipComfyAnimId]) == FLIP_PHONE_SLIDE_DURATION / 2)
    {
        DecompressDataWithHeaderWram(sFlipPhoneOpenTilemap, GetBgTilemapBuffer(0));
        ScheduleBgCopyTilemapToVram(0);
    }
    else if (tFlipPhoneY > 0)
    {
        tFlipPhoneY = ReadComfyAnimValueSmooth(&gComfyAnims[tFlipComfyAnimId]);
        SetGpuReg(REG_OFFSET_BG0VOFS, -tFlipPhoneY);
    }
    else
    {
        ReleaseComfyAnim(tFlipComfyAnimId);
        RotomPhone_SmallStartMenu_ContinueInit(TRUE);
    }
}

static void Task_RotomPhone_SmallStartMenu_FlipPhoneClose(u8 taskId)
{
    TryAdvanceComfyAnim(&gComfyAnims[tFlipComfyAnimId]);
    if (tFlipPhoneY == FALSE)
    {
        struct ComfyAnimEasingConfig config;
        InitComfyAnimConfig_Easing(&config);
        config.durationFrames = FLIP_PHONE_SLIDE_DURATION;
        config.from = Q_24_8(0);
        config.to = Q_24_8(FLIP_PHONE_OFFSCREEN_Y);
        config.easingFunc = ComfyAnimEasing_EaseOutQuad;
        tFlipComfyAnimId = CreateComfyAnim_Easing(&config);
        TryAdvanceComfyAnim(&gComfyAnims[tFlipComfyAnimId]);

        RotomPhone_SmallStartMenu_RemoveWindows();
        RotomPhone_SmallStartMenu_DestroySprites();
        DecompressDataWithHeaderWram(sFlipPhoneClosedTilemap, GetBgTilemapBuffer(0));
        ScheduleBgCopyTilemapToVram(0);
        tFlipPhoneY = ReadComfyAnimValueSmooth(&gComfyAnims[tFlipComfyAnimId]);
        SetGpuReg(REG_OFFSET_BG0VOFS, -tFlipPhoneY);
    }
    else if (tFlipPhoneY < FLIP_PHONE_OFFSCREEN_Y)
    {
        tFlipPhoneY = ReadComfyAnimValueSmooth(&gComfyAnims[tFlipComfyAnimId]);
        SetGpuReg(REG_OFFSET_BG0VOFS, -tFlipPhoneY);
    }
    else if (tFlipPhoneY == FLIP_PHONE_OFFSCREEN_Y)
    {
        tFlipPhoneY++;
        RotomPhone_SmallStartMenu_ExitAndClearTilemap();
    }
    else
    {
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        ReleaseComfyAnim(tFlipComfyAnimId);
        DestroyTask(taskId);
    }
}

static void Task_RotomPhone_SmallStartMenu_HandleMainInput(u8 taskId)
{
    u32 index;
    if (sRotomPhone_SmallStartMenu->isLoading == FALSE && !gPaletteFade.active)
    {
        index = IndexOfSpritePaletteTag(TAG_ICON_PAL);
        LoadPalette(sIconPal, OBJ_PLTT_ID(index), PLTT_SIZE_4BPP);
    }

    tRotomMessageSoundEffect = MUS_DUMMY;
    RotomPhone_SmallStartMenu_CheckUpdateMessage(taskId);

    if (tRotomUpdateTimer && sRotomPhone_SmallStartMenu->isLoading == FALSE && !gPaletteFade.active)
        tRotomUpdateTimer--;
    
    if (JOY_NEW(A_BUTTON))
    {
        if (sRotomPhone_SmallStartMenu->isLoading == FALSE)
        {
            if (menuSelectedSmall != ROTOM_PHONE_MENU_SAVE && menuSelectedSmall != ROTOM_PHONE_MENU_FLAG && menuSelectedSmall != ROTOM_PHONE_MENU_CLOCK)
                FadeScreen(FADE_TO_BLACK, 0);
            
            sRotomPhone_SmallStartMenu->isLoading = TRUE;
        }
    }
    else if (JOY_NEW(B_BUTTON) && sRotomPhone_SmallStartMenu->isLoading == FALSE)
    {
        if (FlagGet(FLAG_SYS_POKEDEX_GET))
        {
            gTasks[taskId].func = Task_RotomPhone_SmallStartMenu_RotomShutdown;
            RotomPhone_SmallStartMenu_RotomShutdownPreparation(taskId);
        }
        else
        {
            PlaySE(SE_BALL_TRAY_ENTER);
            tFlipPhoneY = FALSE;
            gTasks[taskId].func = Task_RotomPhone_SmallStartMenu_FlipPhoneClose;
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

    if (tRotomMessageSoundEffect)
        PlaySE(tRotomMessageSoundEffect);
}

static void RotomPhone_SmallStartMenu_RotomShutdownPreparation(u8 taskId)
{
    PlaySE(PMD_EVENT_MOTION_HARAHERI);
    tRotomUpdateTimer = FALSE;
    tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_GOODBYE;
    RotomPhone_SmallStartMenu_CheckUpdateMessage(taskId);
}

static void Task_RotomPhone_SmallStartMenu_RotomShutdown(u8 taskId)
{
    tRotomUpdateTimer++;
    if (tRotomUpdateTimer == ROTOM_PHONE_MESSAGE_UPDATE_TIMER + ROTOM_PHONE_MESSAGE_SHUTDOWN_TIME)
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 256);
        RotomPhone_SmallStartMenu_DoCleanUpAndDestroyTask(taskId, FALSE);
    }
}

static void Task_RotomPhone_SmallStartMenu_CloseAndSave(u8 taskId)
{
    TaskFunc func;
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
        func = Task_RotomPhone_SmallStartMenu_RotomShutdown;
    else
        func = Task_RotomPhone_SmallStartMenu_FlipPhoneClose;

    if (!FuncIsActiveTask(func) && tRotomPhoneCloseToSave == FALSE)
    {
        CreateTask(func, 0);
        tRotomPhoneCloseToSave = TRUE;
    }
    else if (!FuncIsActiveTask(func) && tRotomPhoneCloseToSave == TRUE)
    {
        LoadMessageBoxAndBorderGfx();
        InitSave_Global();
        gTasks[taskId].func = Task_RotomPhone_HandleSave;
    }
}


/*
 * The code in this file assumes you have read and understood everything in `sample_ui_start_here.c'. The comments here
 * will only cover new/changed code.
 */

enum SlidingPanelSprites
{
    PANEL_SPRITE_ONE,
    PANEL_SPRITE_TWO,
    PANEL_SPRITE_THREE,
    PANEL_SPRITE_FOUR,
    PANEL_SPRITE_FIVE,
    PANEL_SPRITE_SIX,
    PANEL_SPRITE_COUNT,
};

enum SlidingPanelWindows
{
    PANEL_WIN_ONE,
    PANEL_WIN_TWO,
    PANEL_WIN_THREE,
    PANEL_WIN_FOUR,
    PANEL_WIN_FIVE,
    PANEL_WIN_SIX,
    PANEL_WIN_COUNT,
};

struct RotomPhone_LargeStartMenuState
{
    MainCallback savedCallback;
    u8 loadState;
    u8 mode;

    // Store the Y coordinate for the panel (for scrolling), and a flag if the panel is open or not
    u8 panelY;
    bool8 panelIsOpen;

    u8 panelSpriteIds[PANEL_SPRITE_COUNT];
    u8 panelWindowIds[PANEL_WIN_COUNT];
};

enum WindowIds
{
    WIN_UI_TOP_BAR,
};

static EWRAM_DATA struct RotomPhone_LargeStartMenuState *sRotomPhone_LargeStartMenu = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
// We'll have an additional tilemap buffer for the sliding panel, which will live on BG2
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;

/*
 * Define some colors for the main bg, we will use the palette loading function to hotswap them when the user changes
 * regions. These colors are encoded using BGR555 encoding. If you'd like to change these colors, you may find this
 * online BGR555 color picker helpful. Make sure to use Big Endian mode:
 * https://orangeglo.github.io/BGR555/
 */

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
        /*
         * We'll add another BG for our sliding button panel. For this BG we'll be using the same tileset so we set the
         * charblock to 3, just like BG1. Priority is 0 so the sliding panel draws on top of everything else, including
         * the text in the dex info window.
         */
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

static const u32 sRotomPhone_LargeStartMenuTiles[] = INCBIN_U32("graphics/rotom_start_menu/rotom_phone_tiles.4bpp.smol");

// New graphics for the buttons. Create these from 4bpp indexed PNGs, just like before.
static const u32 sRotomPhone_LargeStartMenuTilemap[] = INCBIN_U32("graphics/rotom_start_menu/full_screen.bin.smolTM");
static const u32 sRotomPhone_LargeStartMenuPanelTilemap[] = INCBIN_U32("graphics/rotom_start_menu/full_screen_panel.bin.smolTM");

static const u16 sRotomPhone_LargeStartMenuPalette[] = INCBIN_U16("graphics/rotom_start_menu/rotom_phone_tiles.gbapal");

static const u32 sRotomPhone_DaycareCompatability_Gfx[] = INCBIN_U32("graphics/rotom_start_menu/panel/daycare_heart.4bpp.smol");
static const u16 sRotomPhone_DaycareCompatability_Pal[] = INCBIN_U16("graphics/rotom_start_menu/panel/daycare_heart.gbapal");

// Indices for the anim arrays
#define DEFAULT_ANIM  0
#define SELECTED_ANIM 0

enum RotomPhoneDaycareCompatabilityAnims
{
    ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_NON,
    ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_LOW,
    ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_MED,
    ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_MAX,
    ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_COUNT,
};

static const union AnimCmd sButtonDefaultAnim[] =
{
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_END
};

static const union AnimCmd *const sButtonAnims[] =
{
    [DEFAULT_ANIM] = sButtonDefaultAnim
};

static const union AffineAnimCmd sButtonSelectedAnim[] =
{
    AFFINEANIMCMD_FRAME(2, 2, 0, 15),
    AFFINEANIMCMD_FRAME(-2, -2, 0, 15),
    AFFINEANIMCMD_JUMP(0)
};
static const union AffineAnimCmd * const sButtonAffineAnims[] =
{
    [SELECTED_ANIM] = sButtonSelectedAnim
};

static const struct CompressedSpriteSheet iconCompatatbilitySheet = {
    .data = sRotomPhone_DaycareCompatability_Gfx,
    .size = 32 * 32 * ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_COUNT / 2,
    .tag = TAG_ICON_GFX,
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
    sRotomPhone_LargeStartMenu->savedCallback = CB2_ReturnToFieldWithOpenMenu;

    SetMainCallback2(RotomPhone_LargeStartMenu_SetupCB);
}

// Credit: Jaizu, pret
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
        if (!sRotomPhoneOptions[menuSelectedLarge].unlockedFunc || !sRotomPhoneOptions[menuSelectedLarge].unlockedFunc())
            menuSelectedLarge = RotomPhone_SetFirstSelectedMenu();

        RotomPhone_LargeStartMenu_PrintTopBar();

        sRotomPhone_LargeStartMenu->panelY = 0;
        sRotomPhone_LargeStartMenu->panelIsOpen = FALSE;

        for (enum SlidingPanelSprites spritePanel = PANEL_SPRITE_ONE; spritePanel < PANEL_SPRITE_COUNT; spritePanel++)
        {
            sRotomPhone_LargeStartMenu->panelSpriteIds[spritePanel] = SPRITE_NONE;
        }

        for (enum SlidingPanelWindows windowPanel = PANEL_WIN_ONE; windowPanel < PANEL_WIN_COUNT; windowPanel++)
        {
            sRotomPhone_LargeStartMenu->panelWindowIds[windowPanel] = SPRITE_NONE;
        }

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
            if (menuSelectedLarge == ROTOM_PHONE_MENU_FIRST_OPTION)
                menuSelectedLarge = ROTOM_PHONE_MENU_LAST_OPTION;
            else
                menuSelectedLarge--;
        } while (!sRotomPhoneOptions[menuSelectedLarge].unlockedFunc());
        RotomPhone_LargeStartMenu_PrintTopBar();
    }
    if (JOY_NEW(DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
        do {
            if (menuSelectedLarge == ROTOM_PHONE_MENU_LAST_OPTION)
                menuSelectedLarge = ROTOM_PHONE_MENU_FIRST_OPTION;
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
    // Exit panel when Start or B is pressed
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
    /*
     * Register BG2VOFS controls the vertical offset of background 2. Our sliding panel lives on BG2, so by setting the
     * value of this register we can change the starting Y position of the background. We increase it a bit each frame
     * to make the BG look like it is sliding into view.
     */
    SetGpuReg(REG_OFFSET_BG2VOFS, sRotomPhone_LargeStartMenu->panelY);

    // Panel is open, so slide it down out of view
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
            if (PANEL_SPRITE_ONE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_ONE]].y += yDifference;
            if (PANEL_SPRITE_TWO != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_TWO]].y += yDifference;
            if (PANEL_SPRITE_THREE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE]].y += yDifference;
            if (PANEL_SPRITE_FOUR != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_FOUR]].y += yDifference;
            if (PANEL_SPRITE_FIVE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_FIVE]].y += yDifference;
            if (PANEL_SPRITE_SIX != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_SIX]].y += yDifference;
        }
        else if (sRotomPhone_LargeStartMenu->panelY == PANEL_MIN_Y)
        {
            // Panel is done closing, so set state to closed and change task to read main input
            if (sRotomPhoneOptions[menuSelectedLarge].selectedFunc)
                sRotomPhoneOptions[menuSelectedLarge].selectedFunc();
            sRotomPhone_LargeStartMenu->panelIsOpen = FALSE;
            ReleaseComfyAnim(tRotomPanelComfyAnimId);
            gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_MainInput;
        }
    }
    // Panel is closed, so slide it up into view
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
            if (PANEL_SPRITE_ONE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_ONE]].y += yDifference;
            if (PANEL_SPRITE_TWO != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_TWO]].y += yDifference;
            if (PANEL_SPRITE_THREE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE]].y += yDifference;
            if (PANEL_SPRITE_FOUR != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_FOUR]].y += yDifference;
            if (PANEL_SPRITE_FIVE != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_FIVE]].y += yDifference;
            if (PANEL_SPRITE_SIX != SPRITE_NONE)
                gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_SIX]].y += yDifference;
        }
        else if (sRotomPhone_LargeStartMenu->panelY == PANEL_MAX_Y)
        {
            // Panel is done opening, so set state to open and change task to read panel input
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
        SetMainCallback2(sRotomPhone_LargeStartMenu->savedCallback);
        RotomPhone_LargeStartMenu_DoCleanUpAndDestroyTask(taskId);
    }
}

static void Task_RotomPhone_LargeStartMenu_WaitFadeAndExitGracefully(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        menuFullScreen = FALSE;
        SetMainCallback2(sRotomPhone_LargeStartMenu->savedCallback);
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

static void RotomPhone_LargeStartMenu_DoCleanUpAndChangeTaskFunc(u8 taskId, TaskFunc func)
{
    RotomPhone_LargeStartMenu_FreeResources();
    gTasks[taskId].func = func;
}

static void RotomPhone_LargeStartMenu_DoCleanUpAndDestroyTask(u8 taskId)
{
    RotomPhone_LargeStartMenu_FreeResources();
    DestroyTask(taskId);
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
    sBg2TilemapBuffer = AllocZeroed(TILEMAP_BUFFER_SIZE);
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
        LoadPalette(sRotomPhone_LargeStartMenuPalette, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
        /*
         * We are going to dynamically change the BG color depending on the region. We set up our tiles so that the UI
         * BG color is stored in Palette 0, slot 2. So we hot swap that to our saved color for Kanto, since the UI
         * starts in Kanto region. We will need to perform this mini-swap each time the user changes regions.
         */
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

static void UNUSED RotomPhone_LargeStartMenu_CreateRegionButtons(void)
{
    #define BUTTON_START_X 50
    #define BUTTON_START_Y 184
    /*
     * Load the palettes for our sprites into VRAM. Note here we are passing the SpritePalette palette templates we
     * defined earlier. If a palette with the tag given in one of these templates is already loaded, the
     * `LoadSpritePalette' method will skip the actual load step. Otherwise, it finds the first free palette slot and
     * loads the palette there. Note that if you have manually loaded other palettes into OBJ palette memory without
     * tagging them in the sprite system, this loading code may clobber your palette. Also, the `LoadSpritePalette'
     * method returns the palette index it ended up using. If it returns 0xFF that means the load failed. So if you want
     * you can check the result to make sure your load was successful and handle the problem if it wasn't. In our case,
     * we just assume the load succeeded.
     */

    // LoadSpritePalette(&sKantoJohtoHoennButtonsSpritePalette);
    // LoadSpritePalette(&sSinnohUnovaKalosButtonsSpritePalette);

    /*
     * Create a sprite for each button at the given X/Y position. We set subpriority to 0 so they draw on top. Sprite
     * subpriorities provide you a way to have multiple different "priorities" within the same layer. So for example,
     * say you want Sprite A and Sprite B to draw on top of a BG with priority 1. You can set the priority of Sprite A
     * and Sprite B to 0 for that. But now suppose you also want Sprite A to draw on top of Sprite B. For that, set
     * Sprite A's subpriority to 0 and Sprite B's subpriority to 1.
     */

    // sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_ONE] =
    //     CreateSprite(&sKantoButtonSpriteTemplate, BUTTON_START_X, BUTTON_START_Y, 0);
    // sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_TWO] =
    //     CreateSprite(&sJohtoButtonSpriteTemplate, BUTTON_START_X + 70, BUTTON_START_Y, 0);
    // sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE] =
    //     CreateSprite(&sHoennButtonSpriteTemplate, BUTTON_START_X + 2*70, BUTTON_START_Y, 0);
    // sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_FOUR] =
    //     CreateSprite(&sSinnohButtonSpriteTemplate, BUTTON_START_X, BUTTON_START_Y + 40, 0);
    // sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_FIVE] =
    //     CreateSprite(&sUnovaButtonSpriteTemplate, BUTTON_START_X + 70, BUTTON_START_Y + 40, 0);
    // sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_SIX] =
    //     CreateSprite(&sKalosButtonSpriteTemplate, BUTTON_START_X + 2*70, BUTTON_START_Y + 40, 0);
    #undef BUTTON_START_X
    #undef BUTTON_START_Y
}

static void UNUSED RotomPhone_LargeStartMenu_StartRegionButtonAnim(enum Region region)
{
    /*
     * To start the button animation, the first thing we want to do is use the double-size affine mode. We need this for
     * our case since we are growing the sprites. Otherwise, the sprite image itself will grow but the bounds will stay
     * the same, and the sprite will look like it's getting cut off. We'll set the sprite to double sized while the
     * affine shrink/grow is running so that there is plenty of room to draw the bigger sprite.
     *
     * For more details on this "clipping artifact", please see the relevant TONC page:
     *      https://www.coranac.com/tonc/text/affobj.htm#sec-artifact
     */
    gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[region]].oam.affineMode = ST_OAM_AFFINE_DOUBLE;

    /*
     * Starts the given affine anim on the given sprite. In this case, animate the sprite associated with the given
     * region ID, and use the only affine anim we defined. `SELECTED_ANIM' is just an index into our AffineAnimCmd table
     * we defined way earlier.
     */
    StartSpriteAffineAnim(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[region]], SELECTED_ANIM);

    /*
     * The `CalcCenterToCornerVec' function is used by the game to calculate a vector that points from the sprite's
     * logical center point to its top-left corner. The game then adds this vector to your provided sprite positions.
     * This creates the illusion that the coordinates you provide for a sprite are based on the center of the sprite,
     * even though the GBA hardware actually uses top-left-corner based sprite coordinates.
     *
     * This last bit is crucial since we activated double-sizing. We need to recalculate the sprite's offset vector now
     * that we have changed its size. Otherwise the sprite position will appear to change. Try commenting out this call
     * and see what happens when you select a button.
     */
    CalcCenterToCornerVec(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[region]],
        SPRITE_SHAPE(64x32), SPRITE_SIZE(64x32), ST_OAM_AFFINE_DOUBLE);
}

static void UNUSED RotomPhone_LargeStartMenu_StopRegionButtonAnim(enum Region region)
{
    /*
     * This function works just like the above function, but does everything in reverse.
     */

    // Disable double-size mode
    gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[region]].oam.affineMode = ST_OAM_AFFINE_OFF;
    // Return our sprite to the regular static anim
    StartSpriteAnim(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[region]], DEFAULT_ANIM);
    // Recalculate the offset vector for a regular sized sprite
    CalcCenterToCornerVec(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[region]],
        SPRITE_SHAPE(64x32), SPRITE_SIZE(64x32), ST_OAM_AFFINE_OFF);
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
        return FlagGet(FLAG_SYS_POKEDEX_GET) && !GetSafariZoneFlag();
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
    if (!FlagGet(FLAG_SYS_POKEDEX_GET) || (RotomPhone_StartMenu_IsFullScreen()))
        return TRUE;
    else
        return FALSE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Shortcut(void)
{
    if (FlagGet(FLAG_SYS_POKEDEX_GET) && (!RotomPhone_StartMenu_IsFullScreen()) && !GetSafariZoneFlag())
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

static void RotomPhone_StartMenu_SelectedFunc_Trainer(void)
{
    if (!RotomPhone_StartMenu_IsFullScreen())
    {
        if (!gPaletteFade.active)
        {
            RotomPhone_SmallStartMenu_DoCleanUpAndDestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput), TRUE);
            if (IsOverworldLinkActive() || InUnionRoom())
                ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
            else if (FlagGet(FLAG_SYS_FRONTIER_PASS))
                ShowFrontierPass(CB2_ReturnToFieldWithOpenMenu); // Display frontier pass
            else
                ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
        }
    }
    else
    {
        RotomPhone_LargeStartMenu_DoCleanUpAndDestroyTask(FindTaskIdByFunc(Task_RotomPhone_LargeStartMenu_MainInput));
        if (IsOverworldLinkActive() || InUnionRoom())
            ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
        else if (FlagGet(FLAG_SYS_FRONTIER_PASS))
            ShowFrontierPass(CB2_ReturnToFieldWithOpenMenu); // Display frontier pass
        else
            ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
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
            tRotomPhoneCloseToSave = FALSE;
            if (FlagGet(FLAG_SYS_POKEDEX_GET))
                RotomPhone_SmallStartMenu_RotomShutdownPreparation(taskId);
        }
    }
    else
    {
        InitSave_Global();
        CreateTask(Task_RotomPhone_HandleSave, 0x80);
        // RotomPhone_LargeStartMenu_DoCleanUpAndCreateTask(Task_RotomPhone_HandleSave, 0x80);
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
            RotomPhone_SmallStartMenu_ExitAndClearTilemap();
            FreezeObjectEvents();
            LockPlayerFieldControls();
            DestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput));
            SafariZoneRetirePrompt();
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
        {
            menuFullScreen = TRUE;
            RotomPhone_SmallStartMenu_DoCleanUpAndChangeTaskFunc(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput), Task_OpenRotomPhone_LargeStartMenu);
        }
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
        FormatDecimalTimeWithoutSeconds(time, gLocalTime.hours, gLocalTime.minutes, ROTOM_PHONE_24_HOUR_MODE);
        u8 fontId = GetFontIdToFit(time, ReturnNormalTextFont(), 0, sWindowTemplate_FlipPhone.width * 8);
        FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdFlipPhone, PIXEL_FILL(FLIP_PHONE_BG_COLOUR));
        AddTextPrinterParameterized4(sRotomPhone_SmallStartMenu->windowIdFlipPhone, fontId,
        GetStringCenterAlignXOffset(fontId, time, sWindowTemplate_FlipPhone.width * 8),
        ROTOM_SPEECH_BOTTOM_ROW_Y, 0, 0, sRotomPhone_StartMenuFontColors[FONT_FLIP_PHONE], TEXT_SKIP_DRAW, time);
        CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdFlipPhone, COPYWIN_GFX);
        tRotomMessageSoundEffect = SE_BALL_TRAY_EXIT;
        tRotomUpdateTimer = ROTOM_PHONE_MESSAGE_UPDATE_TIMER;
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

            sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_ONE] =
                CreateMonIcon(speciesOne, SpriteCB_MonIcon_FlippedHorizontal, MON_ICON_ONE_X, MON_ICON_Y, 4, 0);
            gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_ONE]].oam.priority = 0;

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
            sRotomPhone_LargeStartMenu->panelWindowIds[PANEL_WIN_ONE] = AddWindow(&winTemplate);
            windowId = sRotomPhone_LargeStartMenu->panelWindowIds[PANEL_WIN_ONE];
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

            sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_TWO] =
                CreateMonIcon(speciesTwo, SpriteCB_MonIcon, MON_ICON_TWO_X, MON_ICON_Y, 4, 0);
            gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_TWO]].oam.priority = 0;

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
            sRotomPhone_LargeStartMenu->panelWindowIds[PANEL_WIN_TWO] = AddWindow(&winTemplate);
            windowId = sRotomPhone_LargeStartMenu->panelWindowIds[PANEL_WIN_TWO];
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
                .tileTag = TAG_ICON_GFX,
                .paletteTag = gMonIconPaletteTable[MON_ICON_PAL_SLOT_COMPATABILITY_ICON].tag,
                .oam = &sOam_IconDaycareCompatatbility,
                .callback = SpriteCallbackDummy,
                .anims = sAnims_IconDaycareCompatatbility,
                .affineAnims = gDummySpriteAffineAnimTable,
            };

            LoadCompressedSpriteSheet(&iconCompatatbilitySheet);
            LoadSpritePalette(&iconCompatatbilityPal);
            sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE] =
                CreateSprite(&iconCompatatbility_SpriteTemplate, MON_COMPATABILITY_ICON_X, EGG_COMPATABILITY_ICON_Y, 0);
            gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE]].oam.priority = 0;

            switch (GetDaycareCompatibilityScore(&gSaveBlock1Ptr->daycare))
            {
            case PARENTS_INCOMPATIBLE:
                animId = ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_NON;
                break;
            
            case PARENTS_LOW_COMPATIBILITY:
                animId = ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_LOW;
                break;
            
            case PARENTS_MED_COMPATIBILITY:
                animId = ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_MED;
                break;
            
            case PARENTS_MAX_COMPATIBILITY:
            default:
                animId = ROTOM_PHONE_DAYCARE_COMPATABILITY_ANIM_MAX;
                break;
            }
            StartSpriteAnim(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE]], animId);
        }

        if (GetDaycareState() == DAYCARE_EGG_WAITING)
        {
            void (*spriteCB)(struct Sprite *sprite);
            if (Random() % 2 == TRUE)
                spriteCB = SpriteCB_MonIcon;
            else
                spriteCB = SpriteCB_MonIcon_FlippedHorizontal;

            sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE] =
                CreateMonIcon(SPECIES_EGG, spriteCB, MON_COMPATABILITY_ICON_X, EGG_COMPATABILITY_ICON_Y, 4, 0);
            gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE]].oam.priority = 0;
        }
    }
    else if (RotomPhone_StartMenu_IsFullScreen() && sRotomPhone_LargeStartMenu->panelIsOpen == TRUE)
    {
        if (GetDaycareState() == DAYCARE_NO_MONS)
            return;

        FreeMonIconPalettes();

        if (GetDaycareState() != DAYCARE_NO_MONS)
        {
            DestroySpriteAndFreeResources(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_ONE]]);
            sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_ONE] = SPRITE_NONE;

            windowId = sRotomPhone_LargeStartMenu->panelWindowIds[PANEL_WIN_ONE];
            FillWindowPixelBuffer(windowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
            CopyWindowToVram(windowId, COPYWIN_FULL);
            RemoveWindow(windowId);
            sRotomPhone_LargeStartMenu->panelWindowIds[PANEL_WIN_ONE] = WINDOW_NONE;
        }

        if (GetDaycareState() != DAYCARE_NO_MONS && GetDaycareState() != DAYCARE_ONE_MON)
        {
            DestroySpriteAndFreeResources(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_TWO]]);
            sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_TWO] = SPRITE_NONE;

            windowId = sRotomPhone_LargeStartMenu->panelWindowIds[PANEL_WIN_TWO];
            FillWindowPixelBuffer(windowId, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
            CopyWindowToVram(windowId, COPYWIN_FULL);
            RemoveWindow(windowId);
            sRotomPhone_LargeStartMenu->panelWindowIds[PANEL_WIN_TWO] = WINDOW_NONE;
        }

        if (GetDaycareState() == DAYCARE_TWO_MONS || GetDaycareState() == DAYCARE_EGG_WAITING)
        {
            DestroySpriteAndFreeResources(&gSprites[sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE]]);
            sRotomPhone_LargeStartMenu->panelSpriteIds[PANEL_SPRITE_THREE] = SPRITE_NONE;
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
#undef tRotomUpdateTimer
#undef tRotomUpdateMessage
#undef tRotomMessageSoundEffect
#undef tRotomPanelComfyAnimId
#undef tRotomPanelLastY
#undef tFlipPhoneY
#undef tRotomPhoneCloseToSave
