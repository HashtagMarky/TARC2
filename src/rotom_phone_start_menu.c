#include "option_menu.h"
#include "rotom_phone_start_menu.h"
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

/* CONFIGS */
#define ROTOM_PHONE_UPDATE_CLOCK_DISPLAY    TRUE
#define ROTOM_PHONE_SHORTENED_NAME          FALSE
#define ROTOM_PHONE_24_HOUR_MODE            TRUE

/* CALLBACKS */
static void SpriteCB_IconPoketch(struct Sprite* sprite);
static void SpriteCB_IconPokedex(struct Sprite* sprite);
static void SpriteCB_IconParty(struct Sprite* sprite);
static void SpriteCB_IconBag(struct Sprite* sprite);
static void SpriteCB_IconTrainerCard(struct Sprite* sprite);
static void SpriteCB_IconSave(struct Sprite* sprite);
static void SpriteCB_IconOptions(struct Sprite* sprite);
static void SpriteCB_IconFlag(struct Sprite* sprite);

/* TASKs */
static void Task_RotomPhone_SmallStartMenu_HandleMainInput(u8 taskId);
static void Task_HandleSave(u8 taskId);

/* UNLOCKED FUNC */
static bool32 RotomPhone_UnlockedFunc_Unlocked(void);
static bool32 RotomPhone_UnlockedFunc_Pokedex(void);
static bool32 RotomPhone_UnlockedFunc_Pokemon(void);
static bool32 RotomPhone_UnlockedFunc_PokeNav(void);
static bool32 RotomPhone_UnlockedFunc_Save(void);
static bool32 RotomPhone_UnlockedFunc_SafariFlag(void);

/* SELECTED FUNC */
static void RotomPhone_SelectedFunc_Pokedex(void);
static void RotomPhone_SelectedFunc_Pokemon(void);
static void RotomPhone_SelectedFunc_Bag(void);
static void RotomPhone_SelectedFunc_PokeNav(void);
static void RotomPhone_SelectedFunc_Trainer(void);
static void RotomPhone_SelectedFunc_Save(void);
static void RotomPhone_SelectedFunc_Settings(void);
static void RotomPhone_SelectedFunc_SafariFlag(void);

/* OTHER FUNCTIONS */
static void RotomPhone_SmallStartMenu_LoadSprites(void);
static void RotomPhone_SmallStartMenu_CreateAllSprites(void);
static void RotomPhone_SmallStartMenu_LoadBgGfx(void);
static void RotomPhone_SmallStartMenu_ShowTimeWindow(void);
static void RotomPhone_SmallStartMenu_PrintClockDisplay(void);
static void RotomPhone_SmallStartMenu_UpdateMenuName(void);

/* ENUMs */
enum RotomPhoneMenuItems
{
    ROTOM_PHONE_MENU_FLAG,
    ROTOM_PHONE_MENU_POKEDEX,
    ROTOM_PHONE_MENU_PARTY,
    ROTOM_PHONE_MENU_BAG,
    ROTOM_PHONE_MENU_POKENAV,
    ROTOM_PHONE_MENU_TRAINER_CARD,
    ROTOM_PHONE_MENU_SAVE,
    ROTOM_PHONE_MENU_OPTIONS,
    ROTOM_PHONE_MENU_COUNT,
};
#define ROTOM_PHONE_MENU_FIRST_OPTION ROTOM_PHONE_MENU_COUNT - ROTOM_PHONE_MENU_COUNT

enum RotomPhoneSmallOptions
{
    ROTOM_PHONE_SMALL_OPTION_1,
    ROTOM_PHONE_SMALL_OPTION_2,
    ROTOM_PHONE_SMALL_OPTION_3,
    ROTOM_PHONE_SMALL_OPTION_4,
    ROTOM_PHONE_SMALL_OPTION_5,
    ROTOM_PHONE_SMALL_OPTION_6,
    ROTOM_PHONE_SMALL_OPTION_7,
    ROTOM_PHONE_SMALL_OPTION_COUNT,
};

enum RotomPhoneSpriteAnims
{
    SPRITE_INACTIVE,
    SPRITE_ACTIVE,
};

/* STRUCTs */
struct RotomPhoneMenuOptions
{
    const u8 *menuName;
    bool32 (*unlockedFunc)(void);
    void (*selectedFunc)(void);
    const struct SpriteTemplate *iconTemplate;
    s32 yOffset;
};

struct RotomPhone_StartMenu
{
    MainCallback savedCallback;
    bool32 isLoading;
    bool32 spriteFlag; // some bool32 holding values for controlling the sprite anims and lifetime
    enum RotomPhoneMenuItems menuSmallOptions[ROTOM_PHONE_SMALL_OPTION_COUNT];
    u32 menuSmallSpriteId[ROTOM_PHONE_SMALL_OPTION_COUNT];
    u32 windowIdClock;
    u32 windowIdMenuName;
    u32 windowIdSafariBalls;
    u32 windowIdSaveInfo;
};

static EWRAM_DATA struct RotomPhone_StartMenu *sRotomPhone_StartMenu = NULL;
static EWRAM_DATA enum RotomPhoneMenuItems menuSelected; // Separate memory allocation so it persist between destroying of menu.

// --BG-GFX--
static const u32 sStartMenuTiles[] = INCBIN_U32("graphics/rotom_phone_start_menu/bg.4bpp.lz");
static const u32 sStartMenuTilemap[] = INCBIN_U32("graphics/rotom_phone_start_menu/bg.bin.lz");
static const u32 sStartMenuTilemapSafari[] = INCBIN_U32("graphics/rotom_phone_start_menu/bg_safari.bin.lz");
static const u16 sStartMenuPalette[] = INCBIN_U16("graphics/rotom_phone_start_menu/bg.gbapal");

//--SPRITE-GFX--
#define TAG_ICON_GFX 1234
#define TAG_ICON_PAL 0x4654 | BLEND_IMMUNE_FLAG
#define ICON_COORD_X 224
#define ICON_COORD_Y_TOP 17
#define ICON_HEIGHT 32
#define ICON_GAP 3

static const u32 sIconGfx[] = INCBIN_U32("graphics/rotom_phone_start_menu/icons.4bpp.lz");
static const u16 sIconPal[] = INCBIN_U16("graphics/rotom_phone_start_menu/icons.gbapal");

static const struct WindowTemplate sSaveInfoWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 14,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 8
};

static const struct WindowTemplate sWindowTemplate_StartClock = {
  .bg = 0, 
  .tilemapLeft = 2, 
  .tilemapTop = 17, 
  .width = 12,
  .height = 2, 
  .paletteNum = 15,
  .baseBlock = 0x30
};

static const struct WindowTemplate sWindowTemplate_MenuName = {
    .bg = 0, 
    .tilemapLeft = 16, 
    .tilemapTop = 17, 
    .width = 7, 
    .height = 2, 
    .paletteNum = 15,
    .baseBlock = 0x30 + (12*2)
};

static const struct WindowTemplate sWindowTemplate_SafariBalls = {
    .bg = 0,
    .tilemapLeft = 2,
    .tilemapTop = 1,
    .width = 7,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = (0x30 + (12*2)) + (7*2)
};

static const struct SpritePalette sSpritePal_Icon[] =
{
    {sIconPal, TAG_ICON_PAL},
    {NULL},
};

static const struct CompressedSpriteSheet sSpriteSheet_Icon[] = 
{
    {sIconGfx, 16*256/2 , TAG_ICON_GFX},
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
    .callback = SpriteCB_IconPoketch,
};

static const struct SpriteTemplate gSpriteIconPokedex = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconPokedexAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconPokedex,
};

static const struct SpriteTemplate gSpriteIconParty = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconPartyAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconParty,
};

static const struct SpriteTemplate gSpriteIconBag = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconBagAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconBag,
};

static const struct SpriteTemplate gSpriteIconTrainerCard = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconTrainerCardAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconTrainerCard,
};

static const struct SpriteTemplate gSpriteIconSave = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconSaveAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconSave,
};

static const struct SpriteTemplate gSpriteIconOptions = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconOptionsAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconOptions,
};

static const struct SpriteTemplate gSpriteIconFlag = {
    .tileTag = TAG_ICON_GFX,
    .paletteTag = TAG_ICON_PAL,
    .oam = &gOamIcon,
    .anims = gIconFlagAnim,
    .images = NULL,
    .affineAnims = sAffineAnimsIcon,
    .callback = SpriteCB_IconFlag,
};

static void SpriteCB_IconPoketch(struct Sprite* sprite)
{
    if (menuSelected == ROTOM_PHONE_MENU_POKENAV && sRotomPhone_StartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_StartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelected != ROTOM_PHONE_MENU_POKENAV)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    }
}

static void SpriteCB_IconPokedex(struct Sprite* sprite)
{
    if (menuSelected == ROTOM_PHONE_MENU_POKEDEX && sRotomPhone_StartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_StartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelected != ROTOM_PHONE_MENU_POKEDEX)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    }
}

static void SpriteCB_IconParty(struct Sprite* sprite)
{
    if (menuSelected == ROTOM_PHONE_MENU_PARTY && sRotomPhone_StartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_StartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelected != ROTOM_PHONE_MENU_PARTY)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    }
}

static void SpriteCB_IconBag(struct Sprite* sprite)
{
    if (menuSelected == ROTOM_PHONE_MENU_BAG && sRotomPhone_StartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_StartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelected != ROTOM_PHONE_MENU_BAG)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_IconTrainerCard(struct Sprite* sprite)
{
    if (menuSelected == ROTOM_PHONE_MENU_TRAINER_CARD && sRotomPhone_StartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_StartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelected != ROTOM_PHONE_MENU_TRAINER_CARD)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_IconSave(struct Sprite* sprite)
{
    if (menuSelected == ROTOM_PHONE_MENU_SAVE && sRotomPhone_StartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_StartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelected != ROTOM_PHONE_MENU_SAVE)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_IconOptions(struct Sprite* sprite)
{
    if (menuSelected == ROTOM_PHONE_MENU_OPTIONS && sRotomPhone_StartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_StartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelected != ROTOM_PHONE_MENU_OPTIONS)
    {
        StartSpriteAnim(sprite, SPRITE_INACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_INACTIVE);
    } 
}

static void SpriteCB_IconFlag(struct Sprite* sprite)
{
    if (menuSelected == ROTOM_PHONE_MENU_FLAG && sRotomPhone_StartMenu->spriteFlag == FALSE)
    {
        sRotomPhone_StartMenu->spriteFlag = TRUE;
        StartSpriteAnim(sprite, SPRITE_ACTIVE);
        StartSpriteAffineAnim(sprite, SPRITE_ACTIVE);
    }
    else if (menuSelected != ROTOM_PHONE_MENU_FLAG)
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
        .unlockedFunc = RotomPhone_UnlockedFunc_Pokedex,
        .selectedFunc = RotomPhone_SelectedFunc_Pokedex,
        .iconTemplate = &gSpriteIconPokedex,
        .yOffset = 7,
    },
    [ROTOM_PHONE_MENU_PARTY] =
    {
        .menuName = COMPOUND_STRING("Party"),
        .unlockedFunc = RotomPhone_UnlockedFunc_Pokemon,
        .selectedFunc = RotomPhone_SelectedFunc_Pokemon,
        .iconTemplate = &gSpriteIconParty,
        .yOffset = 7,
    },
    [ROTOM_PHONE_MENU_BAG] =
    {
        .menuName = COMPOUND_STRING("Bag"),
        .unlockedFunc = RotomPhone_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_SelectedFunc_Bag,
        .iconTemplate = &gSpriteIconBag,
        .yOffset = 4,
    },
    [ROTOM_PHONE_MENU_POKENAV] =
    {
        .menuName = COMPOUND_STRING("PokéNav"),
        .unlockedFunc = RotomPhone_UnlockedFunc_PokeNav,
        .selectedFunc = RotomPhone_SelectedFunc_PokeNav,
        .iconTemplate = &gSpriteIconPoketch,
        .yOffset = 4,
    },
    [ROTOM_PHONE_MENU_TRAINER_CARD] =
    {
        .menuName = COMPOUND_STRING("Trainer"),
        .unlockedFunc = RotomPhone_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_SelectedFunc_Trainer,
        .iconTemplate = &gSpriteIconTrainerCard,
        .yOffset = 8,
    },
    [ROTOM_PHONE_MENU_SAVE] =
    {
        .menuName = COMPOUND_STRING("Save"),
        .unlockedFunc = RotomPhone_UnlockedFunc_Save,
        .selectedFunc = RotomPhone_SelectedFunc_Save,
        .iconTemplate = &gSpriteIconSave,
        .yOffset = 6,
    },
    [ROTOM_PHONE_MENU_OPTIONS] =
    {
        .menuName = COMPOUND_STRING("Settings"),
        .unlockedFunc = RotomPhone_UnlockedFunc_Unlocked,
        .selectedFunc = RotomPhone_SelectedFunc_Settings,
        .iconTemplate = &gSpriteIconOptions,
        .yOffset = 9,
    },
    [ROTOM_PHONE_MENU_FLAG] =
    {
        .menuName = COMPOUND_STRING("Retire"),
        .unlockedFunc = RotomPhone_UnlockedFunc_SafariFlag,
        .selectedFunc = RotomPhone_SelectedFunc_SafariFlag,
        .iconTemplate = &gSpriteIconFlag,
        .yOffset = 5,
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
    COMPOUND_STRING("Friday,"),
    COMPOUND_STRING("Saturday,"),
    COMPOUND_STRING("Sunday,"),
    COMPOUND_STRING("Monday,"),
    COMPOUND_STRING("Tuesday,"),
    COMPOUND_STRING("Wednesday,"),
    COMPOUND_STRING("Thursday,"),
};

static void RotomPhone_SetFirstSelectedMenu(void)
{
    for (enum RotomPhoneMenuItems menuOption = ROTOM_PHONE_MENU_FIRST_OPTION; menuOption < ROTOM_PHONE_MENU_COUNT; menuOption++)
    {
        if (sRotomPhoneOptions[menuOption].unlockedFunc && sRotomPhoneOptions[menuOption].unlockedFunc())
        {
            menuSelected = menuOption;
            return;
        }
    }
}

static void ShowSafariBallsWindow(void)
{
    sRotomPhone_StartMenu->windowIdSafariBalls = AddWindow(&sWindowTemplate_SafariBalls);
    FillWindowPixelBuffer(sRotomPhone_StartMenu->windowIdSafariBalls, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sRotomPhone_StartMenu->windowIdSafariBalls);
    ConvertIntToDecimalStringN(gStringVar1, gNumSafariBalls, STR_CONV_MODE_RIGHT_ALIGN, 2);
    StringExpandPlaceholders(gStringVar4, gText_SafariBallStock);
    AddTextPrinterParameterized(sRotomPhone_StartMenu->windowIdSafariBalls, FONT_NARROW, gStringVar4, 0, 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sRotomPhone_StartMenu->windowIdSafariBalls, COPYWIN_GFX);
}

void RotomPhone_StartMenu_Init(void)
{
    if (!IsOverworldLinkActive())
    {
        FreezeObjectEvents();
        PlayerFreeze();
        StopPlayerAvatar();
    }

    HideMapNamePopUpWindow();
    LockPlayerFieldControls();

    if (sRotomPhone_StartMenu == NULL)
    {
        sRotomPhone_StartMenu = AllocZeroed(sizeof(struct RotomPhone_StartMenu));
    }

    if (sRotomPhone_StartMenu == NULL)
    {
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        return;
    }

    sRotomPhone_StartMenu->savedCallback = CB2_ReturnToFieldWithOpenMenu;
    sRotomPhone_StartMenu->isLoading = FALSE;
    sRotomPhone_StartMenu->windowIdClock = 0;
    sRotomPhone_StartMenu->spriteFlag = FALSE;

    RotomPhone_SmallStartMenu_LoadSprites();
    RotomPhone_SmallStartMenu_CreateAllSprites();
    RotomPhone_SmallStartMenu_LoadBgGfx();
    RotomPhone_SmallStartMenu_ShowTimeWindow();
    sRotomPhone_StartMenu->windowIdMenuName = AddWindow(&sWindowTemplate_MenuName);

    if (!sRotomPhoneOptions[menuSelected].unlockedFunc || !sRotomPhoneOptions[menuSelected].unlockedFunc())
        RotomPhone_SetFirstSelectedMenu();

    CreateTask(Task_RotomPhone_SmallStartMenu_HandleMainInput, 0);

    if (GetSafariZoneFlag())
        ShowSafariBallsWindow();

    RotomPhone_SmallStartMenu_UpdateMenuName();
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
    s32 y = ICON_COORD_Y_TOP;
    s32 yOffset = 0;
    enum RotomPhoneSmallOptions optionSlotPrev = spriteId - 1;

    if (spriteId != ROTOM_PHONE_SMALL_OPTION_1 && sRotomPhone_StartMenu->menuSmallOptions[optionSlotPrev] != ROTOM_PHONE_MENU_COUNT)
        y = ICON_HEIGHT + gSprites[sRotomPhone_StartMenu->menuSmallSpriteId[optionSlotPrev]].y - sRotomPhoneOptions[sRotomPhone_StartMenu->menuSmallOptions[optionSlotPrev]].yOffset;

    yOffset += sRotomPhoneOptions[menuItem].yOffset;

    sRotomPhone_StartMenu->menuSmallSpriteId[spriteId] = CreateSprite(
        sRotomPhoneOptions[menuItem].iconTemplate,
        ICON_COORD_X,
        y - yOffset + ICON_GAP,
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
            sRotomPhone_StartMenu->menuSmallOptions[optionSlot] = menuId;
            drawn++;
        }
    }

    for (; drawn < ROTOM_PHONE_SMALL_OPTION_COUNT; drawn++)
    {
        sRotomPhone_StartMenu->menuSmallOptions[drawn] = ROTOM_PHONE_MENU_COUNT;
        sRotomPhone_StartMenu->menuSmallSpriteId[drawn] = SPRITE_NONE;
    }
}

static void RotomPhone_SmallStartMenu_LoadBgGfx(void)
{
    u8* buf = GetBgTilemapBuffer(0); 
    LoadBgTilemap(0, 0, 0, 0);
    DecompressAndCopyTileDataToVram(0, sStartMenuTiles, 0, 0, 0);
    if (GetSafariZoneFlag() == FALSE)
        LZDecompressWram(sStartMenuTilemap, buf);

    else
        LZDecompressWram(sStartMenuTilemapSafari, buf);
    
    LoadPalette(gStandardMenuPalette, BG_PLTT_ID(15), PLTT_SIZE_4BPP);
    LoadPalette(sStartMenuPalette, BG_PLTT_ID(14), PLTT_SIZE_4BPP);
    ScheduleBgCopyTilemapToVram(0);
}

static void RotomPhone_SmallStartMenu_ShowTimeWindow(void)
{
    sRotomPhone_StartMenu->windowIdClock = AddWindow(&sWindowTemplate_StartClock);
    FillWindowPixelBuffer(sRotomPhone_StartMenu->windowIdClock, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sRotomPhone_StartMenu->windowIdClock);

    RotomPhone_SmallStartMenu_PrintClockDisplay();
}

static void RotomPhone_SmallStartMenu_PrintClockDisplay(void)
{
    const u8 *const *weekdayNames = gDayNameStringsTable;
    u8 weekdayFont = FONT_NARROW;
    u8 yOffset = 1;
    u8 xOffset = 0;

    if (ROTOM_PHONE_SHORTENED_NAME)
    {
        weekdayNames = gDayNameStringsTableShortned;
        weekdayFont = FONT_NORMAL;
        yOffset = 0;
        xOffset = 3;
    }

    if (ROTOM_PHONE_24_HOUR_MODE)
        xOffset = 3;

    StringCopy(gStringVar3, weekdayNames[(gLocalTime.days % WEEKDAY_COUNT)]);
    AddTextPrinterParameterized(sRotomPhone_StartMenu->windowIdClock, weekdayFont,
        weekdayNames[(gLocalTime.days % WEEKDAY_COUNT)], 1 + xOffset, 1 - yOffset, TEXT_SKIP_DRAW, NULL
    );

    u8 time[24];
    RtcCalcLocalTime();
    FormatDecimalTimeWithoutSeconds(time, gLocalTime.hours, gLocalTime.minutes, ROTOM_PHONE_24_HOUR_MODE);
	AddTextPrinterParameterized(sRotomPhone_StartMenu->windowIdClock, FONT_NORMAL, time,
        GetStringRightAlignXOffset(FONT_NORMAL, time, sWindowTemplate_StartClock.width * 8) - 1 - xOffset,
        1, TEXT_SKIP_DRAW, NULL
    );
    CopyWindowToVram(sRotomPhone_StartMenu->windowIdClock, COPYWIN_GFX);
}

static void RotomPhone_SmallStartMenu_UpdateMenuName(void)
{
    FillWindowPixelBuffer(sRotomPhone_StartMenu->windowIdMenuName, PIXEL_FILL(TEXT_COLOR_WHITE));
    PutWindowTilemap(sRotomPhone_StartMenu->windowIdMenuName);
    const u8 *optionName = sRotomPhoneOptions[menuSelected].menuName;
    AddTextPrinterParameterized(sRotomPhone_StartMenu->windowIdMenuName, FONT_NORMAL, optionName,
        GetStringCenterAlignXOffset(FONT_NORMAL, optionName, sWindowTemplate_MenuName.width * 8),
        1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(sRotomPhone_StartMenu->windowIdMenuName, COPYWIN_GFX);
}

static void RotomPhone_SmallStartMenu_ExitAndClearTilemap(void)
{
    u32 i;
    u8 *buf = GetBgTilemapBuffer(0);
    
    FillWindowPixelBuffer(sRotomPhone_StartMenu->windowIdMenuName, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    FillWindowPixelBuffer(sRotomPhone_StartMenu->windowIdClock, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));

    ClearWindowTilemap(sRotomPhone_StartMenu->windowIdMenuName);
    ClearWindowTilemap(sRotomPhone_StartMenu->windowIdClock);

    CopyWindowToVram(sRotomPhone_StartMenu->windowIdMenuName, COPYWIN_GFX);
    CopyWindowToVram(sRotomPhone_StartMenu->windowIdClock, COPYWIN_GFX);

    RemoveWindow(sRotomPhone_StartMenu->windowIdClock);
    RemoveWindow(sRotomPhone_StartMenu->windowIdMenuName);

    if (GetSafariZoneFlag() == TRUE)
    {
        FillWindowPixelBuffer(sRotomPhone_StartMenu->windowIdSafariBalls, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
        ClearWindowTilemap(sRotomPhone_StartMenu->windowIdSafariBalls); 
        CopyWindowToVram(sRotomPhone_StartMenu->windowIdSafariBalls, COPYWIN_GFX);
        RemoveWindow(sRotomPhone_StartMenu->windowIdSafariBalls);
    }

    for (i=0; i<2048; i++)
    {
        buf[i] = 0;
    }
    ScheduleBgCopyTilemapToVram(0);

    for (enum RotomPhoneSmallOptions spriteId = ROTOM_PHONE_SMALL_OPTION_1; spriteId < ROTOM_PHONE_SMALL_OPTION_COUNT; spriteId++)
    {
        if (sRotomPhone_StartMenu->menuSmallSpriteId[spriteId] != SPRITE_NONE)
        {
            FreeSpriteOamMatrix(&gSprites[sRotomPhone_StartMenu->menuSmallSpriteId[spriteId]]);
            DestroySprite(&gSprites[sRotomPhone_StartMenu->menuSmallSpriteId[spriteId]]);
        }
    }

    if (sRotomPhone_StartMenu != NULL)
    {
        FreeSpriteTilesByTag(TAG_ICON_GFX);  
        Free(sRotomPhone_StartMenu);
        sRotomPhone_StartMenu = NULL;
    }

    ScriptUnfreezeObjectEvents();  
    UnlockPlayerFieldControls();
}

static void DoCleanUpAndChangeCallback(MainCallback callback)
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

static void Task_HandleSave(u8 taskId)
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
            DestroyTask(taskId);
            break;
        case SAVE_ERROR:    // Close start menu
            ClearDialogWindowAndFrameToTransparent(0, TRUE);
            ScriptUnfreezeObjectEvents();
            UnlockPlayerFieldControls();
            SoftResetInBattlePyramid();
            DestroyTask(taskId);
            break;
    }
}

#define STD_WINDOW_BASE_TILE_NUM 0x214
#define STD_WINDOW_PALETTE_NUM 14

static bool32 RotomPhone_UnlockedFunc_Unlocked(void)
{
    return TRUE;
}

static bool32 RotomPhone_UnlockedFunc_Pokedex(void)
{
    return FlagGet(FLAG_SYS_POKEDEX_GET);
}

static bool32 RotomPhone_UnlockedFunc_Pokemon(void)
{
    return FlagGet(FLAG_SYS_POKEMON_GET);
}

static bool32 RotomPhone_UnlockedFunc_PokeNav(void)
{
    return FlagGet(FLAG_SYS_POKENAV_GET) && !GetSafariZoneFlag();
}

static bool32 RotomPhone_UnlockedFunc_Save(void)
{
    return !GetSafariZoneFlag();
}

static bool32 RotomPhone_UnlockedFunc_SafariFlag(void)
{
    return GetSafariZoneFlag();
}

static void RotomPhone_SelectedFunc_Pokedex(void)
{
    DoCleanUpAndChangeCallback(CB2_OpenPokedex);
}

static void RotomPhone_SelectedFunc_Pokemon(void)
{
    DoCleanUpAndChangeCallback(CB2_PartyMenuFromStartMenu);
}

static void RotomPhone_SelectedFunc_Bag(void)
{
    DoCleanUpAndChangeCallback(CB2_BagMenuFromStartMenu);
}

static void RotomPhone_SelectedFunc_PokeNav(void)
{
    DoCleanUpAndChangeCallback(CB2_InitPokeNav);
}

static void RotomPhone_SelectedFunc_Trainer(void)
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

static void RotomPhone_SelectedFunc_Save(void)
{
    if (!gPaletteFade.active)
    {
        RotomPhone_SmallStartMenu_ExitAndClearTilemap();
        FreezeObjectEvents();
        LoadUserWindowBorderGfx(sRotomPhone_StartMenu->windowIdSaveInfo, STD_WINDOW_BASE_TILE_NUM, BG_PLTT_ID(STD_WINDOW_PALETTE_NUM));
        LockPlayerFieldControls();
        DestroyTask(FindTaskIdByFunc(Task_RotomPhone_SmallStartMenu_HandleMainInput));
        InitSave_Global();
        CreateTask(Task_HandleSave, 0x80);
    }
}

static void RotomPhone_SelectedFunc_Settings(void)
{
    DoCleanUpAndChangeCallback(CB2_InitOptionMenu);
}

static void RotomPhone_SelectedFunc_SafariFlag(void)
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

void GoToHandleInput(void)
{
    CreateTask(Task_RotomPhone_SmallStartMenu_HandleMainInput, 80);
}

static void RotomPhone_SmallStartMenu_HandleInput(bool32 down)
{
    sRotomPhone_StartMenu->spriteFlag = FALSE;
    enum RotomPhoneSmallOptions optionCurrent = ROTOM_PHONE_SMALL_OPTION_1;
    s32 offset;
    u32 nextIndex;

    offset = down ? 1 : -1;

    for (enum RotomPhoneSmallOptions i = ROTOM_PHONE_SMALL_OPTION_1; i < ROTOM_PHONE_SMALL_OPTION_COUNT; i++)
    {
        if (sRotomPhone_StartMenu->menuSmallOptions[i] == menuSelected)
        {
            optionCurrent = i;
            break;
        }
    }

    nextIndex = optionCurrent + offset;
    if (nextIndex >= ROTOM_PHONE_SMALL_OPTION_COUNT
        || nextIndex < ROTOM_PHONE_SMALL_OPTION_1
        || sRotomPhone_StartMenu->menuSmallOptions[nextIndex] == ROTOM_PHONE_MENU_COUNT)
    {
        PlaySE(SE_WALL_HIT);
        return;
    }

    PlaySE(SE_SELECT);
    menuSelected = sRotomPhone_StartMenu->menuSmallOptions[nextIndex];
    RotomPhone_SmallStartMenu_UpdateMenuName();
}

static void Task_RotomPhone_SmallStartMenu_HandleMainInput(u8 taskId)
{
    u32 index;
    if (sRotomPhone_StartMenu->isLoading == FALSE && !gPaletteFade.active)
    {
        index = IndexOfSpritePaletteTag(TAG_ICON_PAL);
        LoadPalette(sIconPal, OBJ_PLTT_ID(index), PLTT_SIZE_4BPP); 
    }

    if (ROTOM_PHONE_UPDATE_CLOCK_DISPLAY)
        RotomPhone_SmallStartMenu_PrintClockDisplay();
    
    if (JOY_NEW(A_BUTTON))
    {
        if (sRotomPhone_StartMenu->isLoading == FALSE)
        {
            if (menuSelected != ROTOM_PHONE_MENU_SAVE && menuSelected != ROTOM_PHONE_MENU_FLAG)
                FadeScreen(FADE_TO_BLACK, 0);
            
            sRotomPhone_StartMenu->isLoading = TRUE;
        }
    }
    else if (JOY_NEW(B_BUTTON) && sRotomPhone_StartMenu->isLoading == FALSE)
    {
        PlaySE(SE_SELECT);
        RotomPhone_SmallStartMenu_ExitAndClearTilemap();  
        DestroyTask(taskId);
    }
    else if (gMain.newKeys & DPAD_UP && sRotomPhone_StartMenu->isLoading == FALSE)
    {
        RotomPhone_SmallStartMenu_HandleInput(FALSE);
    }
    else if (gMain.newKeys & DPAD_DOWN && sRotomPhone_StartMenu->isLoading == FALSE)
    {
        RotomPhone_SmallStartMenu_HandleInput(TRUE);
    }
    else if (sRotomPhone_StartMenu->isLoading == TRUE)
    {
        sRotomPhoneOptions[menuSelected].selectedFunc();
    }
}
