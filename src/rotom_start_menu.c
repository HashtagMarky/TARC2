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


#define ROTOM_PHONE_UPDATE_CLOCK_DISPLAY    TRUE
#define ROTOM_PHONE_SHORTENED_NAME          FALSE
#define ROTOM_PHONE_24_HOUR_MODE            gSaveBlock2Ptr->optionsClockMode
#define ROTOM_PHONE_NUM_MINUTES_TO_UPDATE   1
#define ROTOM_PHONE_MESSAGE_UPDATE_TIMER    (30 * 60) / FakeRtc_GetSecondsRatio() * ROTOM_PHONE_NUM_MINUTES_TO_UPDATE
#define ROTOM_PHONE_MESSAGE_SHUTDOWN_TIME   60
#define ROTOM_PHONE_UPDATE_MESSAGE          TRUE


static void SpriteCB_RotomPhoneSmall_IconPoketch(struct Sprite* sprite);
static void SpriteCB_RotomPhoneSmall_IconPokedex(struct Sprite* sprite);
static void SpriteCB_RotomPhoneSmall_IconParty(struct Sprite* sprite);
static void SpriteCB_RotomPhoneSmall_IconBag(struct Sprite* sprite);
static void SpriteCB_RotomPhoneSmall_IconTrainerCard(struct Sprite* sprite);
static void SpriteCB_RotomPhoneSmall_IconSave(struct Sprite* sprite);
static void SpriteCB_RotomPhoneSmall_IconOptions(struct Sprite* sprite);
static void SpriteCB_RotomPhoneSmall_IconFlag(struct Sprite* sprite);
static void SpriteCB_RotomPhoneSmall_IconFullScreen(struct Sprite* sprite);
static void SpriteCB_RotomPhoneSmall_IconDexNav(struct Sprite* sprite);

static void Task_RotomPhone_SmallStartMenu_HandleMainInput(u8 taskId);
static void Task_RotomPhone_SmallStartMenu_RotomShutdown(u8 taskId);
static void Task_RotomPhone_HandleSave(u8 taskId);

static void RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(MainCallback callback);
static u8 RotomPhone_SmallStartMenu_DoCleanUpAndCreateTask(TaskFunc func, u8 priority);
static void RotomPhone_SmallStartMenu_DoCleanUpAndChangeTaskFunc(u8 taskId, TaskFunc func);

static void RotomPhone_SmallStartMenu_LoadSprites(void);
static void RotomPhone_SmallStartMenu_CreateAllSprites(void);
static void RotomPhone_SmallStartMenu_LoadBgGfx(void);
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
static void Task_RotomPhone_LargeStartMenu_WaitFadeAndBail(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_WaitFadeAndExitGracefully(u8 taskId);
static void Task_RotomPhone_LargeStartMenu_WaitFadeForSelection(u8 taskId);


static void RotomPhone_LargeStartMenu_ResetGpuRegsAndBgs(void);
static bool8 RotomPhone_LargeStartMenu_InitBgs(void);
static void RotomPhone_LargeStartMenu_FadeAndBail(void);
static bool8 RotomPhone_LargeStartMenu_LoadGraphics(void);
static void RotomPhone_LargeStartMenu_InitWindows(void);
static void RotomPhone_LargeStartMenu_PrintUiButtonHints(void);
static void RotomPhone_LargeStartMenu_CreateRegionButtons(void);
static void RotomPhone_LargeStartMenu_StartRegionButtonAnim(enum Region region);
static void RotomPhone_LargeStartMenu_StopRegionButtonAnim(enum Region region);
static void RotomPhone_LargeStartMenu_FreeResources(void);

static void RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(MainCallback callback);
static u8 RotomPhone_LargeStartMenu_DoCleanUpAndCreateTask(TaskFunc func, u8 priority);
static void RotomPhone_LargeStartMenu_DoCleanUpAndChangeTaskFunc(u8 taskId, TaskFunc func);


static bool32 RotomPhone_StartMenu_UnlockedFunc_Unlocked(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_Pokedex(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_Pokemon(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_PokeNav(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_Save(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_SafariFlag(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_FullScreen(void);
static bool32 RotomPhone_StartMenu_UnlockedFunc_DexNav(void);

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


enum RotomPhoneMenuItems
{
    ROTOM_PHONE_MENU_FLAG,
    ROTOM_PHONE_MENU_FULL_SCREEN,
    ROTOM_PHONE_MENU_PARTY,
    ROTOM_PHONE_MENU_DEXNAV,
    ROTOM_PHONE_MENU_POKEDEX,
    ROTOM_PHONE_MENU_BAG,
    ROTOM_PHONE_MENU_POKENAV,
    ROTOM_PHONE_MENU_TRAINER_CARD,
    ROTOM_PHONE_MENU_SAVE,
    ROTOM_PHONE_MENU_OPTIONS,
    ROTOM_PHONE_MENU_COUNT,
};
#define ROTOM_PHONE_MENU_FIRST_OPTION ROTOM_PHONE_MENU_COUNT - ROTOM_PHONE_MENU_COUNT
#define ROTOM_PHONE_MENU_LAST_OPTION  ROTOM_PHONE_MENU_COUNT - 1

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
    const struct SpriteTemplate *iconTemplateSmall;
};

struct RotomPhone_StartMenu
{
    MainCallback savedCallback;
    bool32 isLoading;
    bool32 spriteFlag; // some bool32 holding values for controlling the sprite anims and lifetime
    enum RotomPhoneMenuItems menuSmallOptions[ROTOM_PHONE_SMALL_OPTION_COUNT];
    u32 menuSmallSpriteId[ROTOM_PHONE_SMALL_OPTION_COUNT];
    u32 windowIdRotomSpeech_Top;
    u32 windowIdRotomSpeech_Bottom;
    u32 windowIdFlipPhone;
    u32 windowIdSaveInfo;
};

static EWRAM_DATA struct RotomPhone_StartMenu *sRotomPhone_SmallStartMenu = NULL;
static EWRAM_DATA enum RotomPhoneMenuItems menuSelectedSmall; // Separate memory allocation so it persist between destroying of menu.
static EWRAM_DATA enum RotomPhoneMenuItems menuSelectedLarge; // Separate memory allocation so it persist between destroying of menu.
EWRAM_DATA bool32 openedFullScreenRotomPhone;


// --BG-GFX--
static const u32 sSmallRotomTiles[] = INCBIN_U32("graphics/rotom_start_menu/rotom_phone_tiles.4bpp.lz");
static const u32 sSmallRotomTilemap[] = INCBIN_U32("graphics/rotom_start_menu/rotom_phone_tiles.bin.lz");
static const u32 sFlipTiles[] = INCBIN_U32("graphics/rotom_start_menu/flip_phone_tiles.4bpp.lz");
static const u32 sFlipTilemap[] = INCBIN_U32("graphics/rotom_start_menu/flip_phone_tiles.bin.lz");
static const u16 sStartMenuPalette[] = INCBIN_U16("graphics/rotom_start_menu/rotom_phone_tiles.gbapal");

//--SPRITE-GFX--
#define TAG_ICON_GFX 1234
#define TAG_ICON_PAL 0x4654 | BLEND_IMMUNE_FLAG
#define ICON_COORD_X 184
#define ICON_COORD_Y 50

static const u32 sIconGfx[] = INCBIN_U32("graphics/rotom_start_menu/icons.4bpp.lz");
static const u16 sIconPal[] = INCBIN_U16("graphics/rotom_start_menu/icons.gbapal");

static const struct WindowTemplate sSaveInfoWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 14,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 8
};

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
  .paletteNum = 15,
  .baseBlock = 0x30
};

static const struct WindowTemplate sWindowTemplate_RotomSpeech_Bottom = {
    .bg = 0, 
    .tilemapLeft = ROTOM_SPEECH_WINDOW_LEFT, 
    .tilemapTop = ROTOM_SPEECH_WINDOW_TOP + 2, 
    .width = ROTOM_SPEECH_WINDOW_WIDTH, 
    .height = ROTOM_SPEECH_WINDOW_HEIGHT, 
    .paletteNum = 15,
    .baseBlock = 0x30 + (ROTOM_SPEECH_WINDOW_WIDTH*ROTOM_SPEECH_WINDOW_WIDTH)
};

static const struct WindowTemplate sWindowTemplate_FlipPhone = {
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 16,
    .width = 7,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = (0x30 + (ROTOM_SPEECH_WINDOW_WIDTH*ROTOM_SPEECH_WINDOW_WIDTH)) + (ROTOM_SPEECH_WINDOW_WIDTH*ROTOM_SPEECH_WINDOW_WIDTH)
};

static const struct SpritePalette sSpritePal_Icon[] =
{
    {sIconPal, TAG_ICON_PAL},
    {NULL},
};

static const struct CompressedSpriteSheet sSpriteSheet_Icon[] = 
{
    {sIconGfx, 16*320/2 , TAG_ICON_GFX},
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

static const union AnimCmd gAnimCmdPoketch_NotSelected[] = {
    ANIMCMD_FRAME(28, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdPoketch_Selected[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconPoketchAnim[] = {
    gAnimCmdPoketch_NotSelected,
    gAnimCmdPoketch_Selected,
};

static const union AnimCmd gAnimCmdPokedex_NotSelected[] = {
    ANIMCMD_FRAME(32, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdPokedex_Selected[] = {
    ANIMCMD_FRAME(4, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconPokedexAnim[] = {
    gAnimCmdPokedex_NotSelected,
    gAnimCmdPokedex_Selected,
};

static const union AnimCmd gAnimCmdParty_NotSelected[] = {
    ANIMCMD_FRAME(36, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdParty_Selected[] = {
    ANIMCMD_FRAME(8, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconPartyAnim[] = {
    gAnimCmdParty_NotSelected,
    gAnimCmdParty_Selected,
};

static const union AnimCmd gAnimCmdBag_NotSelected[] = {
    ANIMCMD_FRAME(40, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdBag_Selected[] = {
    ANIMCMD_FRAME(12, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconBagAnim[] = {
    gAnimCmdBag_NotSelected,
    gAnimCmdBag_Selected,
};

static const union AnimCmd gAnimCmdTrainerCard_NotSelected[] = {
    ANIMCMD_FRAME(44, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdTrainerCard_Selected[] = {
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconTrainerCardAnim[] = {
    gAnimCmdTrainerCard_NotSelected,
    gAnimCmdTrainerCard_Selected,
};

static const union AnimCmd gAnimCmdSave_NotSelected[] = {
    ANIMCMD_FRAME(48, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdSave_Selected[] = {
    ANIMCMD_FRAME(20, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconSaveAnim[] = {
    gAnimCmdSave_NotSelected,
    gAnimCmdSave_Selected,
};

static const union AnimCmd gAnimCmdOptions_NotSelected[] = {
    ANIMCMD_FRAME(52, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdOptions_Selected[] = {
    ANIMCMD_FRAME(24, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconOptionsAnim[] = {
    gAnimCmdOptions_NotSelected,
    gAnimCmdOptions_Selected,
};

static const union AnimCmd gAnimCmdFlag_NotSelected[] = {
    ANIMCMD_FRAME(60, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdFlag_Selected[] = {
    ANIMCMD_FRAME(56, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconFlagAnim[] = {
    gAnimCmdFlag_NotSelected,
    gAnimCmdFlag_Selected,
};

static const union AnimCmd gAnimCmdFullScreen_NotSelected[] = {
    ANIMCMD_FRAME(68, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdFullScreen_Selected[] = {
    ANIMCMD_FRAME(64, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconFullScreenAnim[] = {
    gAnimCmdFullScreen_NotSelected,
    gAnimCmdFullScreen_Selected,
};

static const union AnimCmd gAnimCmdDexNav_NotSelected[] = {
    ANIMCMD_FRAME(76, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd gAnimCmdDexNav_Selected[] = {
    ANIMCMD_FRAME(72, 0),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const gIconDexNavAnim[] = {
    gAnimCmdDexNav_NotSelected,
    gAnimCmdDexNav_Selected,
};

static const union AffineAnimCmd sAffineAnimIcon_NoAnim[] =
{
    AFFINEANIMCMD_FRAME(0,0, 0, 60),
    AFFINEANIMCMD_END,
};

static const union AffineAnimCmd sAffineAnimIcon_Anim[] =
{
    AFFINEANIMCMD_FRAME(20, 20, 0, 5),    // Scale big
    AFFINEANIMCMD_FRAME(-10, -10, 0, 10), // Scale smol
    AFFINEANIMCMD_FRAME(0, 0, 1, 4),      // Begin rotating

    AFFINEANIMCMD_FRAME(0, 0, -1, 4),     // Loop starts from here ; Rotate/Tilt left 
    AFFINEANIMCMD_FRAME(0, 0, 0, 2),
    AFFINEANIMCMD_FRAME(0, 0, -1, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 2),
    AFFINEANIMCMD_FRAME(0, 0, -1, 4),

    AFFINEANIMCMD_FRAME(0, 0, 1, 4),      // Rotate/Tilt Right
    AFFINEANIMCMD_FRAME(0, 0, 0, 2),
    AFFINEANIMCMD_FRAME(0, 0, 1, 4),
    AFFINEANIMCMD_FRAME(0, 0, 0, 2),
    AFFINEANIMCMD_FRAME(0, 0, 1, 4),

    AFFINEANIMCMD_JUMP(3),
};

static const union AffineAnimCmd *const sAffineAnimsIcon[] =
{   
    sAffineAnimIcon_NoAnim,
    sAffineAnimIcon_Anim,
};

static const struct SpriteTemplate gSpriteIconPoketch = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconPoketchAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconPoketch,
};

static const struct SpriteTemplate gSpriteIconPokedex = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconPokedexAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconPokedex,
};

static const struct SpriteTemplate gSpriteIconParty = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconPartyAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconParty,
};

static const struct SpriteTemplate gSpriteIconBag = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconBagAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconBag,
};

static const struct SpriteTemplate gSpriteIconTrainerCard = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconTrainerCardAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconTrainerCard,
};

static const struct SpriteTemplate gSpriteIconSave = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconSaveAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconSave,
};

static const struct SpriteTemplate gSpriteIconOptions = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconOptionsAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconOptions,
};

static const struct SpriteTemplate gSpriteIconFlag = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconFlagAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconFlag,
};

static const struct SpriteTemplate gSpriteIconFullScreen = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconFullScreenAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconFullScreen,
};

static const struct SpriteTemplate gSpriteIconDexNav = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconDexNavAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_RotomPhoneSmall_IconDexNav,
};

static void SpriteCB_RotomPhoneSmall_IconPoketch(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_POKENAV && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_POKENAV)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    }
}

static void SpriteCB_RotomPhoneSmall_IconPokedex(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_POKEDEX && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_POKEDEX)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    }
}

static void SpriteCB_RotomPhoneSmall_IconParty(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_PARTY && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_PARTY)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    }
}

static void SpriteCB_RotomPhoneSmall_IconBag(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_BAG && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_BAG)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_RotomPhoneSmall_IconTrainerCard(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_TRAINER_CARD && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_TRAINER_CARD)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_RotomPhoneSmall_IconSave(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_SAVE && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_SAVE)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_RotomPhoneSmall_IconOptions(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_OPTIONS && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_OPTIONS)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_RotomPhoneSmall_IconFlag(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_FLAG && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_FLAG)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_RotomPhoneSmall_IconFullScreen(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_FULL_SCREEN && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_FULL_SCREEN)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_RotomPhoneSmall_IconDexNav(struct Sprite* sprite)
{
    if (menuSelectedSmall == ROTOM_PHONE_MENU_DEXNAV && sRotomPhone_SmallStartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_SmallStartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelectedSmall != ROTOM_PHONE_MENU_DEXNAV)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    }
}


static struct RotomPhoneMenuOptions sRotomPhoneOptions[ROTOM_PHONE_MENU_COUNT] =
{
    [ROTOM_PHONE_MENU_POKEDEX] =
    {
        .menuName = COMPOUND_STRING("Pokédex"),
        .rotomAction = COMPOUND_STRING("to open the Pokédex?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Pokedex,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Pokedex,
        .iconTemplateSmall = &gSpriteIconPokedex,
    },
    [ROTOM_PHONE_MENU_DEXNAV] =
    {
        .menuName = COMPOUND_STRING("DexNav"),
        .rotomAction = COMPOUND_STRING("to use the DexNav?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_DexNav,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_DexNav,
        .iconTemplateSmall = &gSpriteIconDexNav,
    },
    [ROTOM_PHONE_MENU_PARTY] =
    {
        .menuName = COMPOUND_STRING("Party"),
        .rotomAction = COMPOUND_STRING("to view your Party?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Pokemon,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Pokemon,
        .iconTemplateSmall = &gSpriteIconParty,
    },
    [ROTOM_PHONE_MENU_BAG] =
    {
        .menuName = COMPOUND_STRING("Bag"),
        .rotomAction = COMPOUND_STRING("to check your Storage?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Bag,
        .iconTemplateSmall = &gSpriteIconBag,
    },
    [ROTOM_PHONE_MENU_POKENAV] =
    {
        .menuName = COMPOUND_STRING("PokéNav"),
        .rotomAction = COMPOUND_STRING("to browse the PokéNav?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_PokeNav,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_PokeNav,
        .iconTemplateSmall = &gSpriteIconPoketch,
    },
    [ROTOM_PHONE_MENU_TRAINER_CARD] =
    {
        .menuName = COMPOUND_STRING("Trainer"),
        .rotomAction = COMPOUND_STRING("to view your ID Card?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Trainer,
        .iconTemplateSmall = &gSpriteIconTrainerCard,
    },
    [ROTOM_PHONE_MENU_SAVE] =
    {
        .menuName = COMPOUND_STRING("Save"),
        .rotomAction = COMPOUND_STRING("to write in your Journal?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Save,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Save,
        .iconTemplateSmall = &gSpriteIconSave,
    },
    [ROTOM_PHONE_MENU_OPTIONS] =
    {
        .menuName = COMPOUND_STRING("Settings"),
        .rotomAction = COMPOUND_STRING("to change the Settings?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_Settings,
        .iconTemplateSmall = &gSpriteIconOptions,
    },
    [ROTOM_PHONE_MENU_FLAG] =
    {
        .menuName = COMPOUND_STRING("Retire"),
        .rotomAction = COMPOUND_STRING("to end the Safari?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_SafariFlag,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_SafariFlag,
        .iconTemplateSmall = &gSpriteIconFlag,
    },
    [ROTOM_PHONE_MENU_FULL_SCREEN] =
    {
        .menuName = COMPOUND_STRING("Full Screen"),
        .rotomAction = COMPOUND_STRING("to enter Full Screen?"),
        .unlockedFunc = RotomPhone_StartMenu_UnlockedFunc_FullScreen,
        .selectedFunc = RotomPhone_StartMenu_SelectedFunc_FullScreen,
        .iconTemplateSmall = &gSpriteIconFullScreen,
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
    for (enum RotomPhoneMenuItems menuOption = ROTOM_PHONE_MENU_FIRST_OPTION; menuOption < ROTOM_PHONE_MENU_COUNT; menuOption++)
    {
        if (sRotomPhoneOptions[menuOption].unlockedFunc && sRotomPhoneOptions[menuOption].unlockedFunc())
        {
            return menuOption;
        }
    }

    return ROTOM_PHONE_MENU_PARTY;
}

#define tRotomUpdateTimer gTasks[taskId].data[0]
#define tRotomUpdateMessage gTasks[taskId].data[1]
#define tRotomMessageSoundEffect gTasks[taskId].data[2]
void RotomPhone_SmallStartMenu_Init(bool32 printGreeting)
{
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

    if (FlagGet(FLAG_SYS_POKEDEX_GET))
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x80);

    sRotomPhone_SmallStartMenu->savedCallback = CB2_ReturnToFieldWithOpenMenu;
    sRotomPhone_SmallStartMenu->isLoading = FALSE;
    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top = 0;
    sRotomPhone_SmallStartMenu->spriteFlag = FALSE;
    openedFullScreenRotomPhone = FALSE;

    RotomPhone_SmallStartMenu_LoadSprites();
    RotomPhone_SmallStartMenu_CreateAllSprites();
    RotomPhone_SmallStartMenu_LoadBgGfx();
    RotomPhone_SmallStartMenu_CreateSpeechWindows();
    RotomPhone_SmallStartMenu_CreateFlipPhoneWindow();

    if (!sRotomPhoneOptions[menuSelectedSmall].unlockedFunc || !sRotomPhoneOptions[menuSelectedSmall].unlockedFunc())
        menuSelectedSmall = RotomPhone_SetFirstSelectedMenu();

    u8 taskId = CreateTask(Task_RotomPhone_SmallStartMenu_HandleMainInput, 0);
    tRotomUpdateTimer = ROTOM_PHONE_MESSAGE_UPDATE_TIMER;
    tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_TIME;

    if (GetSafariZoneFlag())
        tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_SAFARI;

    if (printGreeting)
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
}

static void RotomPhone_SmallStartMenu_CreateSprite(enum RotomPhoneMenuItems menuItem, enum RotomPhoneSmallOptions spriteId)
{
    s32 x = ICON_COORD_X;
    s32 y = ICON_COORD_Y;
    u32 iconRow;
    u32 iconColumn;

    if (!FlagGet(FLAG_SYS_POKEDEX_GET))
        y += 19;

    iconColumn = spriteId % 2;
    iconRow = spriteId / 2;

    sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId] = CreateSprite(
        sRotomPhoneOptions[menuItem].iconTemplateSmall,
        x + (iconColumn * 24),
        y + (iconRow * 24),
        0
    );
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
        sRotomPhone_SmallStartMenu->menuSmallSpriteId[drawn] = SPRITE_NONE;
    }
}

static void RotomPhone_SmallStartMenu_LoadBgGfx(void)
{
    u8* buf = GetBgTilemapBuffer(0); 
    LoadBgTilemap(0, 0, 0, 0);
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
    {
        DecompressAndCopyTileDataToVram(0, sSmallRotomTiles, 0, 0, 0);
        DecompressDataWithHeaderWram(sSmallRotomTilemap, buf);
    }
    else
    {
        DecompressAndCopyTileDataToVram(0, sFlipTiles, 0, 0, 0);
        DecompressDataWithHeaderWram(sFlipTilemap, buf);
    }
    
    LoadPalette(gStandardMenuPalette, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
    LoadPalette(sStartMenuPalette, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
    ScheduleBgCopyTilemapToVram(0);
}
#define ROTOM_SPEECH_TOP_ROW_Y      1
#define ROTOM_SPEECH_BOTTOM_ROW_Y   1
static void RotomPhone_SmallStartMenu_CreateSpeechWindows(void)
{
    if (!FlagGet(FLAG_SYS_POKEDEX_GET))
        return;

    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top = AddWindow(&sWindowTemplate_RotomSpeech_Top);
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top);

    sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom = AddWindow(&sWindowTemplate_RotomSpeech_Bottom);
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom);
}

static void RotomPhone_SmallStartMenu_CreateFlipPhoneWindow(void)
{
    if (FlagGet(FLAG_SYS_POKEDEX_GET))
        return;
    
    sRotomPhone_SmallStartMenu->windowIdFlipPhone = AddWindow(&sWindowTemplate_FlipPhone);
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdFlipPhone, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sRotomPhone_SmallStartMenu->windowIdFlipPhone);
}

static const u8 sText_ClearWindow[] = COMPOUND_STRING("{CLEAR_TO 190}");
static void RotomPhone_SmallStartMenu_PrintRotomSpeech(u8 textBuffer[80], bool32 top, bool32 copy)
{
    u8 fontId = GetFontIdToFit(textBuffer, ReturnNormalTextFont(), 0, ROTOM_SPEECH_WINDOW_WIDTH_PXL);
    u32 windowId;
    windowId = (top == TRUE) ? sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top : sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom;

    AddTextPrinterParameterized(windowId, fontId,
        sText_ClearWindow, 0, ROTOM_SPEECH_TOP_ROW_Y, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(windowId, fontId, textBuffer,
        GetStringCenterAlignXOffset(fontId, textBuffer, ROTOM_SPEECH_WINDOW_WIDTH_PXL),
        ROTOM_SPEECH_TOP_ROW_Y, TEXT_SKIP_DRAW, NULL);
    
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
        if (Random() % 2 == TRUE)
            StringCopy(textBuffer, COMPOUND_STRING("Would you like "));
        else
            StringCopy(textBuffer, COMPOUND_STRING("Do you want "));
        StringAppend(textBuffer, sRotomPhoneOptions[menuSelectedSmall].rotomAction);
        RotomPhone_SmallStartMenu_PrintRotomSpeech(textBuffer, FALSE, TRUE);
    }
    else
    {
        fontId = GetFontIdToFit(sRotomPhoneOptions[menuSelectedSmall].menuName, ReturnNormalTextFont(), 0, sWindowTemplate_FlipPhone.width * 8);
        AddTextPrinterParameterized(sRotomPhone_SmallStartMenu->windowIdFlipPhone, fontId,
            sText_ClearWindow, 0, ROTOM_SPEECH_BOTTOM_ROW_Y, TEXT_SKIP_DRAW, NULL);
        AddTextPrinterParameterized(sRotomPhone_SmallStartMenu->windowIdFlipPhone, fontId, sRotomPhoneOptions[menuSelectedSmall].menuName,
            GetStringCenterAlignXOffset(fontId, sRotomPhoneOptions[menuSelectedSmall].menuName, sWindowTemplate_FlipPhone.width * 8),
            ROTOM_SPEECH_BOTTOM_ROW_Y, TEXT_SKIP_DRAW, NULL);
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

static void RotomPhone_SmallStartMenu_ExitAndClearTilemap(void)
{
    u32 i;
    u8 *buf = GetBgTilemapBuffer(0);
    
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    ClearWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom);
    ClearWindowTilemap(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top);

    CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom, COPYWIN_GFX);
    CopyWindowToVram(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top, COPYWIN_GFX);

    RemoveWindow(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Top);
    RemoveWindow(sRotomPhone_SmallStartMenu->windowIdRotomSpeech_Bottom);

    for (i=0; i<2048; i++)
    {
        buf[i] = 0;
    }
    ScheduleBgCopyTilemapToVram(0);

    for (enum RotomPhoneSmallOptions spriteId = ROTOM_PHONE_SMALL_OPTION_1; spriteId < ROTOM_PHONE_SMALL_OPTION_COUNT; spriteId++)
    {
        if (sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId] != SPRITE_NONE)
        {
            FreeSpriteOamMatrix(&gSprites[sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId]]);
            DestroySprite(&gSprites[sRotomPhone_SmallStartMenu->menuSmallSpriteId[spriteId]]);
        }
    }

    if (sRotomPhone_SmallStartMenu != NULL)
    {
        FreeSpriteTilesByTag(TAG_ICON_GFX);  
        Free(sRotomPhone_SmallStartMenu);
        sRotomPhone_SmallStartMenu = NULL;
    }

    ScriptUnfreezeObjectEvents();  
    UnlockPlayerFieldControls();
}

static void RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(MainCallback callback)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput));
        PlayRainStoppingSoundEffect();
        RotomPhone_SmallStartMenu_ExitAndClearTilemap();
        CleanupOverworldWindowsAndTilemaps();
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
            if (!openedFullScreenRotomPhone)
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
            if (!openedFullScreenRotomPhone)
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
            if (menuSelectedSmall != ROTOM_PHONE_MENU_SAVE && menuSelectedSmall != ROTOM_PHONE_MENU_FLAG)
                FadeScreen(FADE_TO_BLACK, 0);
            
            sRotomPhone_SmallStartMenu->isLoading = TRUE;
        }
    }
    else if (JOY_NEW(B_BUTTON) && sRotomPhone_SmallStartMenu->isLoading == FALSE)
    {
        if (FlagGet(FLAG_SYS_POKEDEX_GET))
        {
            PlaySE(PMD_EVENT_MOTION_HARAHERI);
            tRotomUpdateTimer = FALSE;
            tRotomUpdateMessage = ROTOM_PHONE_MESSAGE_GOODBYE;
            RotomPhone_SmallStartMenu_CheckUpdateMessage(taskId);
            gTasks[taskId].func = Task_RotomPhone_SmallStartMenu_RotomShutdown;
        }
        else
        {
            PlaySE(SE_BALL_TRAY_ENTER);
            RotomPhone_SmallStartMenu_ExitAndClearTilemap();  
            DestroyTask(taskId);
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

static void Task_RotomPhone_SmallStartMenu_RotomShutdown(u8 taskId)
{
    tRotomUpdateTimer++;
    if (tRotomUpdateTimer == ROTOM_PHONE_MESSAGE_UPDATE_TIMER + ROTOM_PHONE_MESSAGE_SHUTDOWN_TIME)
    {
        RotomPhone_SmallStartMenu_ExitAndClearTilemap();  
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 256);
        DestroyTask(taskId);
    }
}
#undef tRotomUpdateTimer
#undef tRotomUpdateMessage
#undef tRotomMessageSoundEffect




/*
 * The code in this file assumes you have read and understood everything in `sample_ui_start_here.c'. The comments here
 * will only cover new/changed code.
 */

/*
 * Define dex regions here. We'll use an enum since that way we can use them to index into various arrays.
 * Recall that enums automatically assign starting from 0, so REGION_KANTO=0, REGION_JOHTO=1, etc.
 */
// enum Region A copy now exists in an include/constants/regions.h
// {
//     REGION_KANTO,
//     REGION_JOHTO,
//     REGION_HOENN,
//     REGION_SINNOH,
//     REGION_UNOVA,
//     REGION_KALOS,
//     REGION_NONE = 0xFF
// };

struct RotomPhone_LargeStartMenuState
{
    MainCallback savedCallback;
    u8 loadState;
    u8 mode;

    // Save the current dex region
    enum Region region;
    // This will store not the current dex region, but which region button is "hovering" in the panel
    enum Region selectedRegion;

    // Store the Y coordinate for the panel (for scrolling), and a flag if the panel is open or not
    u8 panelY;
    bool8 panelIsOpen;

    /*
     * Save the sprite IDs for each button so we can manipulate their positions and change their attributes through the
     * global sprite table. The array is indexed by the region defines below, so for e.g. Kanto is index 0, Sinnoh is
     * index 3, etc.
     */
    u8 regionButtonSpriteIds[REGIONS_COUNT];
};

enum WindowIds
{
    WIN_UI_HINTS,
    WIN_MON_INFO
};

static EWRAM_DATA struct RotomPhone_LargeStartMenuState *sRotomPhone_LargeStartMenu = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
// We'll have an additional tilemap buffer for the sliding panel, which will live on BG2
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;

#define MON_ICON_X     39
#define MON_ICON_Y     36
// Define the dex number start/end of each region for easy reference
static const u16 sDexRanges[REGIONS_COUNT][2] = {
    // Kanto goes from Bulbasaur to Mew
    [REGION_KANTO]   = {NATIONAL_DEX_BULBASAUR, NATIONAL_DEX_MEW},
    // Johto goes from Chikorita to Celebi
    [REGION_JOHTO]   = {NATIONAL_DEX_CHIKORITA, NATIONAL_DEX_CELEBI},
    // Etc.
    [REGION_HOENN]   = {NATIONAL_DEX_TREECKO, NATIONAL_DEX_DEOXYS},
    [REGION_SINNOH]  = {NATIONAL_DEX_TURTWIG, NATIONAL_DEX_ARCEUS},
    [REGION_UNOVA]   = {NATIONAL_DEX_VICTINI, NATIONAL_DEX_GENESECT},
    [REGION_KALOS]   = {NATIONAL_DEX_CHESPIN, NATIONAL_DEX_VOLCANION}
};
static const u8 sRegionNameKanto[] =  _("Kanto");
static const u8 sRegionNameJohto[] =  _("Johto");
static const u8 sRegionNameHoenn[] =  _("Hoenn");
static const u8 sRegionNameSinnoh[] = _("Sinnoh");
static const u8 sRegionNameUnova[] =  _("Unova");
static const u8 sRegionNameKalos[] =  _("Kalos");
static const u8 *const sRegionNames[REGIONS_COUNT] = {
    [REGION_KANTO]   = sRegionNameKanto,
    [REGION_JOHTO]   = sRegionNameJohto,
    [REGION_HOENN]   = sRegionNameHoenn,
    [REGION_SINNOH]  = sRegionNameSinnoh,
    [REGION_UNOVA]   = sRegionNameUnova,
    [REGION_KALOS]   = sRegionNameKalos
};
/*
 * Define some colors for the main bg, we will use the palette loading function to hotswap them when the user changes
 * regions. These colors are encoded using BGR555 encoding. If you'd like to change these colors, you may find this
 * online BGR555 color picker helpful. Make sure to use Big Endian mode:
 * https://orangeglo.github.io/BGR555/
 */
static const u16 sRegionBgColors[] = {
    [REGION_KANTO]   = 0x6a93,
    [REGION_JOHTO]   = 0x527a,
    [REGION_HOENN]   = 0x4f55,
    [REGION_SINNOH]  = 0x4b7c,
    [REGION_UNOVA]   = 0x5ef7,
    [REGION_KALOS]   = 0x76fb
};
/*
 * We'll use this struct to define the button relationships. I.e. which button is above, to the left of, etc. Putting
 * these into an array like below will make it much easier to handle user input later.
 */
struct RegionSelection
{
    u8 upRegion;
    u8 downRegion;
    u8 leftRegion;
    u8 rightRegion;
};
// The actual button relationships
static const struct RegionSelection sRegionSelections[] =
{
    /*
     * E.g. the Kanto button will be in the top left, so there is no region above it, Sinnoh will be below it, there is
     * no region to the left of it, and Johto will be right of it.
     */
    [REGION_KANTO] = {
        .upRegion    = REGION_NONE,
        .downRegion  = REGION_SINNOH,
        .leftRegion  = REGION_NONE,
        .rightRegion = REGION_JOHTO
    },
    [REGION_JOHTO] = {
        .upRegion    = REGION_NONE,
        .downRegion  = REGION_UNOVA,
        .leftRegion  = REGION_KANTO,
        .rightRegion = REGION_HOENN
    },
    [REGION_HOENN] = {
        .upRegion    = REGION_NONE,
        .downRegion  = REGION_KALOS,
        .leftRegion  = REGION_JOHTO,
        .rightRegion = REGION_NONE
    },
    [REGION_SINNOH] = {
        .upRegion    = REGION_KANTO,
        .downRegion  = REGION_NONE,
        .leftRegion  = REGION_NONE,
        .rightRegion = REGION_UNOVA
    },
    [REGION_UNOVA] = {
        .upRegion    = REGION_JOHTO,
        .downRegion  = REGION_NONE,
        .leftRegion  = REGION_SINNOH,
        .rightRegion = REGION_KALOS
    },
    [REGION_KALOS] = {
        .upRegion    = REGION_HOENN,
        .downRegion  = REGION_NONE,
        .leftRegion  = REGION_UNOVA,
        .rightRegion = REGION_NONE
    }
};

#define MODE_INFO   0
#define MODE_STATS  1
#define MODE_OTHER  2
static const u8 sModeNameInfo[] =  _("Info");
static const u8 sModeNameStats[] =  _("Stats");
static const u8 sModeNameOther[] =  _("Other");
static const u8 *const sModeNames[3] = {
    [MODE_INFO]   = sModeNameInfo,
    [MODE_STATS]  = sModeNameStats,
    [MODE_OTHER]  = sModeNameOther
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
    [WIN_UI_HINTS] =
    {
        .bg = 0,
        .tilemapLeft = 10,
        .tilemapTop = 0,
        .width = 20,
        .height = 7,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_MON_INFO] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 9,
        .width = 26,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 1 + (20 * 7)
    },
    DUMMY_WIN_TEMPLATE
};

static const u32 sRotomPhone_LargeStartMenuTiles[] = INCBIN_U32("graphics/rotom_start_menu/rotom_phone_tiles.4bpp.lz");

// New graphics for the buttons. Create these from 4bpp indexed PNGs, just like before.
static const u32 sRotomPhone_LargeStartMenuKantoButton[] = INCBIN_U32("graphics/sample_ui/kanto.4bpp");
static const u32 sRotomPhone_LargeStartMenuJohtoButton[] = INCBIN_U32("graphics/sample_ui/johto.4bpp");
static const u32 sRotomPhone_LargeStartMenuHoennButton[] = INCBIN_U32("graphics/sample_ui/hoenn.4bpp");
static const u32 sRotomPhone_LargeStartMenuSinnohButton[] = INCBIN_U32("graphics/sample_ui/sinnoh.4bpp");
static const u32 sRotomPhone_LargeStartMenuUnovaButton[] = INCBIN_U32("graphics/sample_ui/unova.4bpp");
static const u32 sRotomPhone_LargeStartMenuKalosButton[] = INCBIN_U32("graphics/sample_ui/kalos.4bpp");

static const u32 sRotomPhone_LargeStartMenuTilemap[] = INCBIN_U32("graphics/rotom_start_menu/full_screen.bin.lz");
static const u32 sRotomPhone_LargeStartMenuPanelTilemap[] = INCBIN_U32("graphics/rotom_start_menu/full_screen_panel.bin.lz");

static const u16 sRotomPhone_LargeStartMenuPalette[] = INCBIN_U16("graphics/rotom_start_menu/rotom_phone_tiles.gbapal");
static const u16 sRotomPhone_LargeStartMenu_KantoJohtoHoennPalette[] = INCBIN_U16("graphics/sample_ui/kanto_johto_hoenn.gbapal");
static const u16 sRotomPhone_LargeStartMenu_SinnohUnovaKalosPalette[] = INCBIN_U16("graphics/sample_ui/sinnoh_unova_kalos.gbapal");

/*
 * Define some palette tags for our buttons. We have two different button palettes, each being used for 3 of the region
 * buttons. The palette tagging system helps the game keep track of which sprite palette slots are in use. Think of it
 * almost like a reference counter. The game uses the SpritePalette structure to associate a tag with a given palette.
 * Then, using the special `LoadSpritePalette' function, you can load the palette into VRAM and the game will only
 * perform the load if that palette tag is not already `active'. We'll see how to connect your sprites up with your
 * SpritePalettes a little further down.
 */
/*
 * Also note: these tags MUST be unique within the given context. In the case of our sample UI here, there are only two
 * palette tags so I just picked some numbers at random. If you are modifying one of the existing game screens/contexts,
 * you'll need to make sure you don't accidentally duplicate a palette tag value, else the game will start to mix up
 * the various sprite palettes and your sprites will look incorrect.
 */
#define PALETTE_TAG_KANTO_JOHTO_HOENN 0x1000
#define PALETTE_TAG_SINNOH_UNOVA_KALOS 0x1001
static const struct SpritePalette sKantoJohtoHoennButtonsSpritePalette =
{
    .data = sRotomPhone_LargeStartMenu_KantoJohtoHoennPalette,
    .tag = PALETTE_TAG_KANTO_JOHTO_HOENN
};
static  const struct SpritePalette sSinnohUnovaKalosButtonsSpritePalette =
{
    .data = sRotomPhone_LargeStartMenu_SinnohUnovaKalosPalette,
    .tag = PALETTE_TAG_SINNOH_UNOVA_KALOS
};

// Indices for the anim arrays
#define DEFAULT_ANIM  0
#define SELECTED_ANIM 0

/*
 * The AnimCmd structure is a quick way to define how you want your sprite to be animated. It uses a clever macro/union
 * system to allow for maximum flexibility. If you are unsure of how unions work, check here:
 *      https://beej.us/guide/bgc/html/split/structs-ii-more-fun-with-structs.html#unions
 * The long-short of it is that the below AnimCmd array will instruct the sprite system to use the sprite's 0th frame
 * for a duration of 30 before ending the animation (I am not exactly sure if this means 30 game-frames, but the upshot
 * is you can change this value to make a sprite frame last longer or shorter). In our case, the buttons are static so
 * this animation doesn't actually do anything. You can change the duration value to no effect.
 *
 * Below, we'll see what exactly is meant by `sprite's 0th frame'.
 */
static const union AnimCmd sButtonDefaultAnim[] =
{
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_END
};
/*
 * Sprites can have multiple animations. So we'll store an array of AnimCmd arrays. In our case, we only need one button
 * anim so this array has one element. However, you could store other AnimCmd arrays here and change which sprite anim
 * plays based on game events.
 */
static const union AnimCmd *const sButtonAnims[] =
{
    [DEFAULT_ANIM] = sButtonDefaultAnim
};
/*
 * Sprites can also have affine anims, which allow for some cool effects like scaling and rotation. We'll be using this
 * affine anim when a button is currently selected so that it slowly grows and shrinks. This AffineAnimCmd array is
 * saying to scale up by a small amount (2) for a duration of 15, then scale back down by a small amount for a duration
 * of 15, then jump to the beginning and start again.
 */
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
/*
 * We'll be storing our sprite frames using the pic table format. The game provides a few different ways to store our
 * sprites. One way is using a spritesheet and a tile tag. We won't be covering that in this tutorial. Instead, we'll
 * use the SpriteFrameImage structure to bring in the graphics of our buttons. In this format, each sprite frame is
 * expected as a separate graphics file, and you lay them out in an array (pic table) like below. Now the `frame' value
 * in our AnimCmds should make sense. The `sprite's 0th frame' refers to the 0th index in this array. If you wanted a
 * multi-framed sprite, you'd need to add each frame graphic to this table, and then set up your AnimCmd to reference
 * which frames you want and in which order.
 */
static const struct SpriteFrameImage sKantoButtonPicTable[] =
{
    /*
     * `obj_frame_tiles' is a macro that auto-magically creates a SpriteFrameImage structure with the data set to our
     * graphics data defined in the `sRotomPhone_LargeStartMenuKantoButton' array and the size properly set to the size of this array.
     */
    obj_frame_tiles(sRotomPhone_LargeStartMenuKantoButton)
};

/*
 * Define some template OAM (Object Attribute Memory) data for our sprite button. The `0' values here aren't actually
 * 0s, they just mean those pieces of data are either unused or are going to be filled in by other sprite code. For
 * example, `paletteNum = 0' does not actually mean use palette 0. Instead, we'll be using the palette defined by our
 * palette tag, and the `gflib/sprite.c' code will fill the correct palette number into the OAM data for us.
 */
static const struct OamData sKantoButtonOam =
{
    // y=0 here because we will set y-coord later when we actually create the sprite
    .y = 0,
    // Default to affine off. We will turn the affine anim on when the button is selected using a special function.
    .affineMode = ST_OAM_AFFINE_OFF,
    /*
     * We are a normal sprite object. There are also window and blend sprite objects, but those are outside the scope
     * of this tutorial.
     */
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    // This sprite is 4bpp, very rarely does the game use an 8bpp sprite
    .bpp = ST_OAM_4BPP,
    /*
     * Our button sprites use the 2:1 length:height sprite shape, so specify that here. Typically, you'll just want your
     * sprite shape to match your sprite size. So if you are making a 64x32 pixel sprite, specify that as your sprite
     * shape.
     */
    .shape = SPRITE_SHAPE(64x32),
    // x=0 here because we will set x-coord later when we actually create the sprite
    .x = 0,
    /*
     * The matrix number has to do with the sprite's affine transformation matrix. We set it to 0 because the gflib
     * sprite code will handle it for us. For more on affine matrices:
     *      https://www.coranac.com/tonc/text/affine.htm
     */
    .matrixNum = 0,
    // Our button sprites are 64x32 pixels
    .size = SPRITE_SIZE(64x32),
    /*
     * We don't need to set this since we are using a pic table, very rarely would you ever manually change this value,
     * typically you'd let the sprite library code update this for you
     */
    .tileNum = 0,
    // Set priority to 0 so our button sprite draws on top of everything
    .priority = 0,
    // We don't set this since we are going to use a SpritePalette and a palette tag to handle palette tracking
    .paletteNum = 0,
    // Used for the affine matrix, no need to set it here
    .affineParam = 0,
    /*
     * If you want more details on how the .shape and .size parameters affect the ultimate on-screen appearance of your
     * sprite, check out the Object Attribute section of the TONC sprite tutorial, specifically table 8.4:
     *      https://www.coranac.com/tonc/text/regobj.htm#sec-oam-entry
     */
};

/*
 * The SpriteTemplate is a structure provided by `gflib/sprite.h' that allows us to easily group and define some basic
 * sprite properties that we can instantiate into actual sprites later. Here, we define a template for our Kanto button.
 */
static const struct SpriteTemplate sKantoButtonSpriteTemplate =
{
    /*
     * We'll set the tile tag to TAG_NONE since we will be using a pic table for this sprite. If you were using a
     * spritesheet, you would want to set the tag here to match the tag you gave the SpriteSheet structure.
     */
    .tileTag = TAG_NONE,
    // Here we will use the palette tag that matches the SpritePalette for the Kanto button.
    .paletteTag = PALETTE_TAG_KANTO_JOHTO_HOENN,
    // Set OAM to our define OAM structure
    .oam = &sKantoButtonOam,
    // Set anims to our defined anims
    .anims = sButtonAnims,
    // Set the pic table for this sprite. If you were using a spritesheet, then this would be NULL
    .images = sKantoButtonPicTable,
    // Set affineAnims to our defined affine anims
    .affineAnims = sButtonAffineAnims,
    /*
     * Sprite callbacks are called as part of `AnimateSprites()', which we are calling in our Main CB. You can use them
     * to perform additional movements and animation updates that aren't handled by your AnimCmds. We don't actually
     * need a special callback for our button sprites so just set the callback to the provided dummy. Check out the code
     * in `AnimateSprites()' in `gflib/sprite.c' to see how this callback is used.
     */
    .callback = SpriteCallbackDummy,
};

/*
 * Now define the necessary objects for the rest of the buttons. All this code is pretty similar to the Kanto button
 * definition code above.
 */

static const struct SpriteFrameImage sJohtoButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuJohtoButton)
};
static const struct OamData sJohtoButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sJohtoButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_KANTO_JOHTO_HOENN,
    .oam = &sJohtoButtonOam,
    .anims = sButtonAnims,
    .images = sJohtoButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteFrameImage sHoennButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuHoennButton)
};
static const struct OamData sHoennButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sHoennButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_KANTO_JOHTO_HOENN,
    .oam = &sHoennButtonOam,
    .anims = sButtonAnims,
    .images = sHoennButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteFrameImage sSinnohButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuSinnohButton)
};
static const struct OamData sSinnohButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sSinnohButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_SINNOH_UNOVA_KALOS,
    .oam = &sSinnohButtonOam,
    .anims = sButtonAnims,
    .images = sSinnohButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteFrameImage sUnovaButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuUnovaButton)
};
static const struct OamData sUnovaButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sUnovaButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_SINNOH_UNOVA_KALOS,
    .oam = &sUnovaButtonOam,
    .anims = sButtonAnims,
    .images = sUnovaButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

static const struct SpriteFrameImage sKalosButtonPicTable[] =
{
    obj_frame_tiles(sRotomPhone_LargeStartMenuKalosButton)
};
static const struct OamData sKalosButtonOam =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};
static const struct SpriteTemplate sKalosButtonSpriteTemplate =
{
    .tileTag = TAG_NONE,
    .paletteTag = PALETTE_TAG_SINNOH_UNOVA_KALOS,
    .oam = &sKalosButtonOam,
    .anims = sButtonAnims,
    .images = sKalosButtonPicTable,
    .affineAnims = sButtonAffineAnims,
    .callback = SpriteCallbackDummy,
};

enum FontColor
{
    FONT_BLACK,
    FONT_WHITE,
    FONT_RED,
    FONT_BLUE,
};
static const u8 sRotomPhone_LargeStartMenuWindowFontColors[][3] =
{
    [FONT_BLACK]  = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY},
    [FONT_WHITE]  = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE,      TEXT_COLOR_DARK_GRAY},
    [FONT_RED]    = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_GRAY},
    [FONT_BLUE]   = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_GRAY},
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
        openedFullScreenRotomPhone = FALSE;
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
        sRotomPhone_LargeStartMenu->region = REGION_KANTO;
        sRotomPhone_LargeStartMenu->selectedRegion = REGION_KANTO;

        if (!sRotomPhoneOptions[menuSelectedLarge].unlockedFunc || !sRotomPhoneOptions[menuSelectedLarge].unlockedFunc())
            menuSelectedLarge = RotomPhone_SetFirstSelectedMenu();

        RotomPhone_LargeStartMenu_PrintUiButtonHints();

        sRotomPhone_LargeStartMenu->panelY = 0;
        sRotomPhone_LargeStartMenu->panelIsOpen = FALSE;

        // Create sprites for the region buttons off screen
        RotomPhone_LargeStartMenu_CreateRegionButtons();
        // Start button select animation for the current region, i.e. Kanto
        RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_LargeStartMenu->selectedRegion);

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
    if (JOY_NEW(DPAD_LEFT))
    {
        PlaySE(SE_SELECT);
        do {
            if (menuSelectedLarge == ROTOM_PHONE_MENU_FIRST_OPTION)
                menuSelectedLarge = ROTOM_PHONE_MENU_LAST_OPTION;
            else
                menuSelectedLarge--;
        } while (!sRotomPhoneOptions[menuSelectedLarge].unlockedFunc());
        RotomPhone_LargeStartMenu_PrintUiButtonHints();
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
        RotomPhone_LargeStartMenu_PrintUiButtonHints();
    }
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_WaitFadeForSelection;
    }
    if (JOY_NEW(START_BUTTON))
    {
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelSlide;
        PlaySE(SE_SELECT);
    }
}

static void Task_RotomPhone_LargeStartMenu_PanelInput(u8 taskId)
{
    // Exit panel when Start or B is pressed
    if (JOY_NEW(START_BUTTON | B_BUTTON))
    {
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelSlide;
        PlaySE(SE_SELECT);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        sRotomPhone_LargeStartMenu->region = sRotomPhone_LargeStartMenu->selectedRegion;
        // Sneakily swap out color 2 in BG1's palette for the new region-specific color
        LoadPalette(&sRegionBgColors[sRotomPhone_LargeStartMenu->region], BG_PLTT_ID(0) + 6, 2);
        RotomPhone_LargeStartMenu_PrintUiButtonHints();
        PlaySE(SE_SELECT);
        gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelSlide;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (sRegionSelections[sRotomPhone_LargeStartMenu->selectedRegion].upRegion != REGION_NONE)
        {
            RotomPhone_LargeStartMenu_StopRegionButtonAnim(sRotomPhone_LargeStartMenu->selectedRegion);
            sRotomPhone_LargeStartMenu->selectedRegion = sRegionSelections[sRotomPhone_LargeStartMenu->selectedRegion].upRegion;
            RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_LargeStartMenu->selectedRegion);
            PlaySE(SE_SELECT);
        }
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (sRegionSelections[sRotomPhone_LargeStartMenu->selectedRegion].downRegion != REGION_NONE)
        {
            RotomPhone_LargeStartMenu_StopRegionButtonAnim(sRotomPhone_LargeStartMenu->selectedRegion);
            sRotomPhone_LargeStartMenu->selectedRegion = sRegionSelections[sRotomPhone_LargeStartMenu->selectedRegion].downRegion;
            RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_LargeStartMenu->selectedRegion);
            PlaySE(SE_SELECT);
        }
    }
    else if (JOY_NEW(DPAD_LEFT))
    {
        if (sRegionSelections[sRotomPhone_LargeStartMenu->selectedRegion].leftRegion != REGION_NONE)
        {
            RotomPhone_LargeStartMenu_StopRegionButtonAnim(sRotomPhone_LargeStartMenu->selectedRegion);
            sRotomPhone_LargeStartMenu->selectedRegion = sRegionSelections[sRotomPhone_LargeStartMenu->selectedRegion].leftRegion;
            RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_LargeStartMenu->selectedRegion);
            PlaySE(SE_SELECT);
        }
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        if (sRegionSelections[sRotomPhone_LargeStartMenu->selectedRegion].rightRegion != REGION_NONE)
        {
            RotomPhone_LargeStartMenu_StopRegionButtonAnim(sRotomPhone_LargeStartMenu->selectedRegion);
            sRotomPhone_LargeStartMenu->selectedRegion = sRegionSelections[sRotomPhone_LargeStartMenu->selectedRegion].rightRegion;
            RotomPhone_LargeStartMenu_StartRegionButtonAnim(sRotomPhone_LargeStartMenu->selectedRegion);
            PlaySE(SE_SELECT);
        }
    }
}

static void Task_RotomPhone_LargeStartMenu_PanelSlide(u8 taskId)
{
    #define PANEL_MAX_Y 95
    /*
     * Register BG2VOFS controls the vertical offset of background 2. Our sliding panel lives on BG2, so by setting the
     * value of this register we can change the starting Y position of the background. We increase it a bit each frame
     * to make the BG look like it is sliding into view.
     */
    SetGpuReg(REG_OFFSET_BG2VOFS, sRotomPhone_LargeStartMenu->panelY);

    // Panel is open, so slide it down out of view
    if (sRotomPhone_LargeStartMenu->panelIsOpen)
    {
        if (sRotomPhone_LargeStartMenu->panelY > 0)
        {
            sRotomPhone_LargeStartMenu->panelY -= 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_KANTO]].y += 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_JOHTO]].y += 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_HOENN]].y += 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_SINNOH]].y += 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_UNOVA]].y += 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_KALOS]].y += 5;
        }
        else if (sRotomPhone_LargeStartMenu->panelY == 0)
        {
            // Panel is done closing, so set state to closed and change task to read main input
            sRotomPhone_LargeStartMenu->panelIsOpen = FALSE;
            gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_MainInput;
        }
    }
    // Panel is closed, so slide it up into view
    else
    {
        if (sRotomPhone_LargeStartMenu->panelY < PANEL_MAX_Y)
        {
            sRotomPhone_LargeStartMenu->panelY += 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_KANTO]].y -= 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_JOHTO]].y -= 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_HOENN]].y -= 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_SINNOH]].y -= 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_UNOVA]].y -= 5;
            gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_KALOS]].y -= 5;
        }
        else if (sRotomPhone_LargeStartMenu->panelY == PANEL_MAX_Y)
        {
            // Panel is done opening, so set state to open and change task to read panel input
            sRotomPhone_LargeStartMenu->panelIsOpen = TRUE;
            gTasks[taskId].func = Task_RotomPhone_LargeStartMenu_PanelInput;
        }
    }
    #undef PANEL_MAX_Y
}

static void Task_RotomPhone_LargeStartMenu_WaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        openedFullScreenRotomPhone = FALSE;
        SetMainCallback2(sRotomPhone_LargeStartMenu->savedCallback);
        RotomPhone_LargeStartMenu_FreeResources();
        DestroyTask(taskId);
    }
}

static void Task_RotomPhone_LargeStartMenu_WaitFadeAndExitGracefully(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        openedFullScreenRotomPhone = FALSE;
        SetMainCallback2(sRotomPhone_LargeStartMenu->savedCallback);
        RotomPhone_LargeStartMenu_FreeResources();
        DestroyTask(taskId);
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
        LoadPalette(&sRegionBgColors[REGION_KANTO], BG_PLTT_ID(0) + 6, 2);
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
    FillWindowPixelBuffer(WIN_UI_HINTS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(WIN_MON_INFO, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    PutWindowTilemap(WIN_UI_HINTS);
    PutWindowTilemap(WIN_MON_INFO);
    CopyWindowToVram(WIN_UI_HINTS, COPYWIN_FULL);
    CopyWindowToVram(WIN_MON_INFO, COPYWIN_FULL);
}

static const u8 sText_RotomPhone_LargeStartMenuButtonHint1[] = _("{DPAD_UPDOWN}Change POKéMON");
static const u8 sText_RotomPhone_LargeStartMenuButtonHint2[] = _("{A_BUTTON}Mode: {STR_VAR_1}");
static const u8 sText_RotomPhone_LargeStartMenuButtonHint3[] = _("{START_BUTTON}Region");
static const u8 sText_RotomPhone_LargeStartMenuButtonHint4[] = _("{B_BUTTON}Exit");
static void RotomPhone_LargeStartMenu_PrintUiButtonHints(void)
{
    FillWindowPixelBuffer(WIN_UI_HINTS, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    StringCopy(gStringVar1, sModeNames[sRotomPhone_LargeStartMenu->mode]);
    StringExpandPlaceholders(gStringVar2, sText_RotomPhone_LargeStartMenuButtonHint2);

    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_NORMAL, 0, 3, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sRotomPhoneOptions[menuSelectedLarge].menuName);
    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_SMALL, 79, 0, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sText_RotomPhone_LargeStartMenuButtonHint1);
    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_SMALL, 79, 10, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, gStringVar2);
    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_SMALL, 79, 20, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sText_RotomPhone_LargeStartMenuButtonHint3);
    AddTextPrinterParameterized4(WIN_UI_HINTS, FONT_SMALL, 79, 30, 0, 0,
        sRotomPhone_LargeStartMenuWindowFontColors[FONT_WHITE], TEXT_SKIP_DRAW, sText_RotomPhone_LargeStartMenuButtonHint4);

    CopyWindowToVram(WIN_UI_HINTS, COPYWIN_GFX);
}

static void RotomPhone_LargeStartMenu_CreateRegionButtons(void)
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
    LoadSpritePalette(&sKantoJohtoHoennButtonsSpritePalette);
    LoadSpritePalette(&sSinnohUnovaKalosButtonsSpritePalette);
    /*
     * Create a sprite for each button at the given X/Y position. We set subpriority to 0 so they draw on top. Sprite
     * subpriorities provide you a way to have multiple different "priorities" within the same layer. So for example,
     * say you want Sprite A and Sprite B to draw on top of a BG with priority 1. You can set the priority of Sprite A
     * and Sprite B to 0 for that. But now suppose you also want Sprite A to draw on top of Sprite B. For that, set
     * Sprite A's subpriority to 0 and Sprite B's subpriority to 1.
     */
    sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_KANTO] =
        CreateSprite(&sKantoButtonSpriteTemplate, BUTTON_START_X, BUTTON_START_Y, 0);
    sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_JOHTO] =
        CreateSprite(&sJohtoButtonSpriteTemplate, BUTTON_START_X + 70, BUTTON_START_Y, 0);
    sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_HOENN] =
        CreateSprite(&sHoennButtonSpriteTemplate, BUTTON_START_X + 2*70, BUTTON_START_Y, 0);
    sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_SINNOH] =
        CreateSprite(&sSinnohButtonSpriteTemplate, BUTTON_START_X, BUTTON_START_Y + 40, 0);
    sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_UNOVA] =
        CreateSprite(&sUnovaButtonSpriteTemplate, BUTTON_START_X + 70, BUTTON_START_Y + 40, 0);
    sRotomPhone_LargeStartMenu->regionButtonSpriteIds[REGION_KALOS] =
        CreateSprite(&sKalosButtonSpriteTemplate, BUTTON_START_X + 2*70, BUTTON_START_Y + 40, 0);
    #undef BUTTON_START_X
    #undef BUTTON_START_Y
}

static void RotomPhone_LargeStartMenu_StartRegionButtonAnim(enum Region region)
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
    gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[region]].oam.affineMode = ST_OAM_AFFINE_DOUBLE;

    /*
     * Starts the given affine anim on the given sprite. In this case, animate the sprite associated with the given
     * region ID, and use the only affine anim we defined. `SELECTED_ANIM' is just an index into our AffineAnimCmd table
     * we defined way earlier.
     */
    StartSpriteAffineAnim(&gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[region]], SELECTED_ANIM);

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
    CalcCenterToCornerVec(&gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[region]],
        SPRITE_SHAPE(64x32), SPRITE_SIZE(64x32), ST_OAM_AFFINE_DOUBLE);
}

static void RotomPhone_LargeStartMenu_StopRegionButtonAnim(enum Region region)
{
    /*
     * This function works just like the above function, but does everything in reverse.
     */

    // Disable double-size mode
    gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[region]].oam.affineMode = ST_OAM_AFFINE_OFF;
    // Return our sprite to the regular static anim
    StartSpriteAnim(&gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[region]], DEFAULT_ANIM);
    // Recalculate the offset vector for a regular sized sprite
    CalcCenterToCornerVec(&gSprites[sRotomPhone_LargeStartMenu->regionButtonSpriteIds[region]],
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
    ResetSpriteData();
}


static bool32 RotomPhone_StartMenu_UnlockedFunc_Unlocked(void)
{
    return TRUE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Pokedex(void)
{
    return FlagGet(FLAG_SYS_POKEDEX_GET);
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Pokemon(void)
{
    return FlagGet(FLAG_SYS_POKEMON_GET);
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_PokeNav(void)
{
    return FlagGet(FLAG_SYS_POKENAV_GET) && !GetSafariZoneFlag();
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_Save(void)
{
    if (!openedFullScreenRotomPhone)
        return !GetSafariZoneFlag();
    else
        return TRUE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_SafariFlag(void)
{
    if (!openedFullScreenRotomPhone)
        return GetSafariZoneFlag();
    else
        return FALSE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_FullScreen(void)
{
    if (!openedFullScreenRotomPhone)
        return FlagGet(FLAG_SYS_POKEDEX_GET) && !GetSafariZoneFlag();
    else
        return FALSE;
}

static bool32 RotomPhone_StartMenu_UnlockedFunc_DexNav(void)
{
    return FlagGet(DN_FLAG_DEXNAV_GET);
}


static void RotomPhone_StartMenu_SelectedFunc_Pokedex(void)
{
    if (!openedFullScreenRotomPhone)
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_OpenPokedex);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_OpenPokedex);
}

static void RotomPhone_StartMenu_SelectedFunc_Pokemon(void)
{
    if (!openedFullScreenRotomPhone)
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_PartyMenuFromStartMenu);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_PartyMenuFromStartMenu);
}

static void RotomPhone_StartMenu_SelectedFunc_Bag(void)
{
    if (!openedFullScreenRotomPhone)
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_BagMenuFromStartMenu);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_BagMenuFromStartMenu);
}

static void RotomPhone_StartMenu_SelectedFunc_PokeNav(void)
{
    if (!openedFullScreenRotomPhone)
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_InitPokeNav);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_InitPokeNav);
}

static void RotomPhone_StartMenu_SelectedFunc_Trainer(void)
{
    if (!openedFullScreenRotomPhone)
    {
        if (!gPaletteFade.active)
        {
            PlayRainStoppingSoundEffect();
            RotomPhone_SmallStartMenu_ExitAndClearTilemap();
            CleanupOverworldWindowsAndTilemaps();
            if (IsOverworldLinkActive() || InUnionRoom())
            {
                ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
                DestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput));
            }
            else if (FlagGet(FLAG_SYS_FRONTIER_PASS))
            {
                ShowFrontierPass(CB2_ReturnToFieldWithOpenMenu); // Display frontier pass
                DestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput));
            }
            else
            {
                ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
                DestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput));
            }
        }
    }
    else
    {
        RotomPhone_LargeStartMenu_FreeResources();
        if (IsOverworldLinkActive() || InUnionRoom())
        {
            ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
            DestroyTask(FindTaskIdByFunc(Task_RotomPhone_LargeStartMenu_MainInput));
        }
        else if (FlagGet(FLAG_SYS_FRONTIER_PASS))
        {
            ShowFrontierPass(CB2_ReturnToFieldWithOpenMenu); // Display frontier pass
            DestroyTask(FindTaskIdByFunc(Task_RotomPhone_LargeStartMenu_MainInput));
        }
        else
        {
            ShowPlayerTrainerCard(CB2_ReturnToFieldWithOpenMenu); // Display trainer card
            DestroyTask(FindTaskIdByFunc(Task_RotomPhone_LargeStartMenu_MainInput));
        }
    }
}

static void RotomPhone_StartMenu_SelectedFunc_Save(void)
{
    if (!openedFullScreenRotomPhone)
    {
        if (!gPaletteFade.active)
        {
            RotomPhone_SmallStartMenu_ExitAndClearTilemap();
            FreezeObjectEvents();
            LoadUserWindowBorderGfx(sRotomPhone_SmallStartMenu->windowIdSaveInfo, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
            LockPlayerFieldControls();
            DestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput));
            InitSave_Global();
            CreateTask(Task_RotomPhone_HandleSave, 0x80);
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
    if (!openedFullScreenRotomPhone)
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeCallback(CB2_InitOptionMenu);
    else
        RotomPhone_LargeStartMenu_DoCleanUpAndChangeCallback(CB2_InitOptionMenu);
}

static void RotomPhone_StartMenu_SelectedFunc_SafariFlag(void)
{
    if (!openedFullScreenRotomPhone)
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
    if (!openedFullScreenRotomPhone)
    {
        if (!gPaletteFade.active)
        {
            openedFullScreenRotomPhone = TRUE;
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
    if (!openedFullScreenRotomPhone)
        RotomPhone_SmallStartMenu_DoCleanUpAndChangeTaskFunc(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput), Task_OpenDexNavFromStartMenu);
    else
    {
        RotomPhone_LargeStartMenu_DoCleanUpAndCreateTask(Task_OpenDexNavFromStartMenu, 0);
    }
}
