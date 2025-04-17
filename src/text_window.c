#include "global.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "palette.h"
#include "constants/pokemon.h"
#include "bg.h"
#include "graphics.h"
#include "menu.h"

const u8 gTextWindowFrame_Gfx[] = INCBIN_U8("graphics/text_window/window_frame.4bpp");
const u16 gTextWindowFrame_EmeraldPal[] = INCBIN_U16("graphics/text_window/window_frame_emerald.gbapal");
static const u16 gTextWindowFrame_TornadusPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/tornadus.gbapal");
static const u16 gTextWindowFrame_ThundurusPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/thundurus.gbapal");
static const u16 gTextWindowFrame_LandorusPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/landorus.gbapal");
static const u16 gTextWindowFrame_EnamorusPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/enamorus.gbapal");

static const u16 gTextWindowFrame_NormalPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/normal.gbapal");
static const u16 gTextWindowFrame_FightingPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/fighting.gbapal");
static const u16 gTextWindowFrame_FlyingPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/flying.gbapal");
static const u16 gTextWindowFrame_PoisonPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/poison.gbapal");
static const u16 gTextWindowFrame_GroundPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/ground.gbapal");
static const u16 gTextWindowFrame_RockPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/rock.gbapal");
static const u16 gTextWindowFrame_BugPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/bug.gbapal");
static const u16 gTextWindowFrame_GhostPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/ghost.gbapal");
static const u16 gTextWindowFrame_SteelPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/steel.gbapal");
static const u16 gTextWindowFrame_FirePal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/fire.gbapal");
static const u16 gTextWindowFrame_WaterPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/water.gbapal");
static const u16 gTextWindowFrame_GrassPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/grass.gbapal");
static const u16 gTextWindowFrame_ElectricPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/electric.gbapal");
static const u16 gTextWindowFrame_PsychicPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/psychic.gbapal");
static const u16 gTextWindowFrame_IcePal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/ice.gbapal");
static const u16 gTextWindowFrame_DragonPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/dragon.gbapal");
static const u16 gTextWindowFrame_DarkPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/dark.gbapal");
static const u16 gTextWindowFrame_FairyPal[] = INCBIN_U16("graphics/text_window/ikigai_palettes/window_frame/fairy.gbapal");

static const u16 sTextWindowPalettes[IKIGAI_TEXT_BOX_FRAMES_COUNT][16] = // Ikigai Type Window Pals Here
{
    INCBIN_U16("graphics/text_window/message_box_emerald.gbapal"),
    INCBIN_U16("graphics/text_window/text_window_pal1.gbapal"),
    INCBIN_U16("graphics/text_window/text_window_pal2.gbapal"),
    INCBIN_U16("graphics/text_window/text_window_pal3.gbapal"),
    INCBIN_U16("graphics/text_window/text_window_pal4.gbapal"),
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/tornadus.gbapal"),     // IKIGAI_INTERFACE_BLUE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/thundurus.gbapal"),    // IKIGAI_INTERFACE_GREEN + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/landorus.gbapal"),     // IKIGAI_INTERFACE_ORANGE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/enamorus.gbapal"),     // IKIGAI_INTERFACE_PINK + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/message_box_emerald.gbapal"),                      // TYPE_NONE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/normal.gbapal"),       // TYPE_NORMAL + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/fighting.gbapal"),     // TYPE_FIGHTING + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/flying.gbapal"),       // TYPE_FLYING + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/poison.gbapal"),       // TYPE_POISON + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/ground.gbapal"),       // TYPE_GROUND + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/rock.gbapal"),         // TYPE_ROCK + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/bug.gbapal"),          // TYPE_BUG + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/ghost.gbapal"),        // TYPE_GHOST + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/steel.gbapal"),        // TYPE_STEEL + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/message_box_emerald.gbapal"),                      // TYPE_MYSTERY + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/fire.gbapal"),         // TYPE_FIRE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/water.gbapal"),        // TYPE_WATER + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/grass.gbapal"),        // TYPE_GRASS + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/electric.gbapal"),     // TYPE_ELECTRIC + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/psychic.gbapal"),      // TYPE_PSYCHIC + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/ice.gbapal"),          // TYPE_ICE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/dragon.gbapal"),       // TYPE_DRAGON + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/dark.gbapal"),         // TYPE_DARK + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/ikigai_palettes/message_box/fairy.gbapal"),        // TYPE_FAIRY + DEFAULT_TEXT_BOX_FRAME_PALETTES
};

static const struct TilesPal sWindowFrames[IKIGAI_WINDOW_FRAMES_COUNT] =
{
    {gTextWindowFrame_Gfx, gTextWindowFrame_TornadusPal},       // IKIGAI_INTERFACE_GREEN
    {gTextWindowFrame_Gfx, gTextWindowFrame_ThundurusPal},      // IKIGAI_INTERFACE_BLUE
    {gTextWindowFrame_Gfx, gTextWindowFrame_LandorusPal},       // IKIGAI_INTERFACE_ORANGE
    {gTextWindowFrame_Gfx, gTextWindowFrame_EnamorusPal},       // IKIGAI_INTERFACE_PINK
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},        // TYPE_NONE + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_NormalPal},         // TYPE_NORMAL + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_FightingPal},       // TYPE_FIGHTING + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_FlyingPal},         // TYPE_FLYING + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_PoisonPal},         // TYPE_POISON + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_GroundPal},         // TYPE_GROUND + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_RockPal},           // TYPE_ROCK + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_BugPal},            // TYPE_BUG + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_GhostPal},          // TYPE_GHOST + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_SteelPal},          // TYPE_STEEL + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},        // TYPE_MYSTERY + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_FirePal},           // TYPE_FIRE + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_WaterPal},          // TYPE_WATER + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_GrassPal},          // TYPE_GRASS + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_ElectricPal},       // TYPE_ELECTRIC + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_PsychicPal},        // TYPE_PSYCHIC + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_IcePal},            // TYPE_ICE + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_DragonPal},         // TYPE_DRAGON + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_DarkPal},           // TYPE_DARK + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_FairyPal}           // TYPE_FAIRY + IKIGAI_DEFAULT_INTERFACE_COUNT
};

static const u16 sTextWindowDexNavFrame[] = INCBIN_U16("graphics/text_window/dexnav_pal.gbapal");
static const struct TilesPal sDexNavWindowFrame = {gTextWindowFrame_Gfx, sTextWindowDexNavFrame};

// Battle Textboxes
static const u32 gBattleTextboxPalette_Normal[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/normal.gbapal.lz");
static const u32 gBattleTextboxPalette_Fighting[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/fighting.gbapal.lz");
static const u32 gBattleTextboxPalette_Flying[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/flying.gbapal.lz");
static const u32 gBattleTextboxPalette_Poison[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/poison.gbapal.lz");
static const u32 gBattleTextboxPalette_Ground[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/ground.gbapal.lz");
static const u32 gBattleTextboxPalette_Rock[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/rock.gbapal.lz");
static const u32 gBattleTextboxPalette_Bug[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/bug.gbapal.lz");
static const u32 gBattleTextboxPalette_Ghost[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/ghost.gbapal.lz");
static const u32 gBattleTextboxPalette_Steel[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/steel.gbapal.lz");
static const u32 gBattleTextboxPalette_Fire[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/fire.gbapal.lz");
static const u32 gBattleTextboxPalette_Water[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/water.gbapal.lz");
static const u32 gBattleTextboxPalette_Grass[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/grass.gbapal.lz");
static const u32 gBattleTextboxPalette_Electric[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/electric.gbapal.lz");
static const u32 gBattleTextboxPalette_Psychic[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/psychic.gbapal.lz");
static const u32 gBattleTextboxPalette_Ice[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/ice.gbapal.lz");
static const u32 gBattleTextboxPalette_Dragon[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/dragon.gbapal.lz");
static const u32 gBattleTextboxPalette_Dark[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/dark.gbapal.lz");
static const u32 gBattleTextboxPalette_Fairy[] = INCBIN_U32("graphics/battle_interface/ikigai_palettes/textbox/fairy.gbapal.lz");

static const u32 *GetBattleTextboxGymPalettePtr(void);

// code
const struct TilesPal *GetWindowFrameTilesPal(u8 id)
{
    if (id >= IKIGAI_WINDOW_FRAMES_COUNT)
        return &sWindowFrames[0];
    else if (id == IKIGAI_INTERFACE_GYM_TYPE_COLOUR)
        return &sWindowFrames[IKIGAI_DEFAULT_INTERFACE_COUNT + gSaveBlock2Ptr->ikigaiGymType];
    else
        return &sWindowFrames[id];
}

void LoadMessageBoxGfx(u8 windowId, u16 destOffset, u8 palOffset)
{
    LoadBgTiles(GetWindowAttribute(windowId, WINDOW_BG), gMessageBox_Gfx, 0x280, destOffset);
    LoadPalette(GetOverworldTextboxPalettePtr(), palOffset, PLTT_SIZE_4BPP);
}

void LoadSignBoxGfx(u8 windowId, u16 destOffset, u8 palOffset)
{
    LoadBgTiles(GetWindowAttribute(windowId, WINDOW_BG), gSignpostWindow_Gfx, 0x1C0, destOffset);
    LoadPalette(GetTextWindowPalette(1), palOffset, PLTT_SIZE_4BPP);
}

void LoadUserWindowBorderGfx_(u8 windowId, u16 destOffset, u8 palOffset)
{
    LoadUserWindowBorderGfx(windowId, destOffset, palOffset);
}

void LoadWindowGfx(u8 windowId, u8 frameId, u16 destOffset, u8 palOffset)
{
    LoadBgTiles(GetWindowAttribute(windowId, WINDOW_BG), sWindowFrames[frameId].tiles, 0x120, destOffset);
    if (frameId == IKIGAI_INTERFACE_GYM_TYPE_COLOUR)
        frameId = IKIGAI_DEFAULT_INTERFACE_COUNT + gSaveBlock2Ptr->ikigaiGymType;
    LoadPalette(sWindowFrames[frameId].pal, palOffset, PLTT_SIZE_4BPP);
}

void LoadUserWindowBorderGfx(u8 windowId, u16 destOffset, u8 palOffset)
{
    LoadWindowGfx(windowId, gSaveBlock2Ptr->optionsInterfaceColor, destOffset, palOffset);
}

void DrawTextBorderOuter(u8 windowId, u16 tileNum, u8 palNum)
{
    u8 bgLayer = GetWindowAttribute(windowId, WINDOW_BG);
    u16 tilemapLeft = GetWindowAttribute(windowId, WINDOW_TILEMAP_LEFT);
    u16 tilemapTop = GetWindowAttribute(windowId, WINDOW_TILEMAP_TOP);
    u16 width = GetWindowAttribute(windowId, WINDOW_WIDTH);
    u16 height = GetWindowAttribute(windowId, WINDOW_HEIGHT);

    FillBgTilemapBufferRect(bgLayer, tileNum + 0, tilemapLeft - 1,      tilemapTop - 1,         1,      1,      palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 1, tilemapLeft,          tilemapTop - 1,         width,  1,      palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 2, tilemapLeft + width,  tilemapTop - 1,         1,      1,      palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 3, tilemapLeft - 1,      tilemapTop,             1,      height, palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 5, tilemapLeft + width,  tilemapTop,             1,      height, palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 6, tilemapLeft - 1,      tilemapTop + height,    1,      1,      palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 7, tilemapLeft,          tilemapTop + height,    width,  1,      palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 8, tilemapLeft + width,  tilemapTop + height,    1,      1,      palNum);
}

void DrawTextBorderInner(u8 windowId, u16 tileNum, u8 palNum)
{
    u8 bgLayer = GetWindowAttribute(windowId, WINDOW_BG);
    u16 tilemapLeft = GetWindowAttribute(windowId, WINDOW_TILEMAP_LEFT);
    u16 tilemapTop = GetWindowAttribute(windowId, WINDOW_TILEMAP_TOP);
    u16 width = GetWindowAttribute(windowId, WINDOW_WIDTH);
    u16 height = GetWindowAttribute(windowId, WINDOW_HEIGHT);

    FillBgTilemapBufferRect(bgLayer, tileNum + 0, tilemapLeft,              tilemapTop,                 1,          1,          palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 1, tilemapLeft + 1,          tilemapTop,                 width - 2,  1,          palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 2, tilemapLeft + width - 1,  tilemapTop,                 1,          1,          palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 3, tilemapLeft,              tilemapTop + 1,             1,          height - 2, palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 5, tilemapLeft + width - 1,  tilemapTop + 1,             1,          height - 2, palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 6, tilemapLeft,              tilemapTop + height - 1,    1,          1,          palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 7, tilemapLeft + 1,          tilemapTop + height - 1,    width -     2,  1,      palNum);
    FillBgTilemapBufferRect(bgLayer, tileNum + 8, tilemapLeft + width - 1,  tilemapTop + height - 1,    1,          1,          palNum);
}

void rbox_fill_rectangle(u8 windowId)
{
    u8 bgLayer = GetWindowAttribute(windowId, WINDOW_BG);
    u16 tilemapLeft = GetWindowAttribute(windowId, WINDOW_TILEMAP_LEFT);
    u16 tilemapTop = GetWindowAttribute(windowId, WINDOW_TILEMAP_TOP);
    u16 width = GetWindowAttribute(windowId, WINDOW_WIDTH);
    u16 height = GetWindowAttribute(windowId, WINDOW_HEIGHT);

    FillBgTilemapBufferRect(bgLayer, 0, tilemapLeft - 1, tilemapTop - 1, width + 2, height + 2, 0x11);
}

const u16 *GetTextWindowPalette(u16 id)
{
    switch (id)
    {
    case 0:
        id = 0x00;
        break;
    case 1:
        id = 0x10;
        break;
    case 2:
        id = 0x20;
        break;
    case 3:
        id = 0x30;
        break;
    case 4:
        id = 0x40;
        break;
    case IKIGAI_INTERFACE_GREEN + DEFAULT_TEXT_BOX_FRAME_PALETTES:
        id = 0x50;
        break;
    case IKIGAI_INTERFACE_BLUE + DEFAULT_TEXT_BOX_FRAME_PALETTES:
        id = 0x60;
        break;
    case IKIGAI_INTERFACE_ORANGE + DEFAULT_TEXT_BOX_FRAME_PALETTES:
        id = 0x70;
        break;
    case IKIGAI_INTERFACE_PINK + DEFAULT_TEXT_BOX_FRAME_PALETTES:
        id = 0x80;
        break;
    default:
        id *= 16;
        break;
    }

    return (const u16 *)(sTextWindowPalettes) + id;
}

const u16 *GetOverworldTextboxPalettePtr(void)
{
    switch (gSaveBlock2Ptr->optionsInterfaceColor)
    {
    case IKIGAI_INTERFACE_GYM_TYPE_COLOUR:
        return GetTextWindowPalette(gSaveBlock2Ptr->ikigaiGymType + DEFAULT_TEXT_BOX_FRAME_PALETTES + IKIGAI_DEFAULT_INTERFACE_COUNT);
        break;
    case IKIGAI_INTERFACE_PINK:
    case IKIGAI_INTERFACE_ORANGE:
    case IKIGAI_INTERFACE_BLUE:
    case IKIGAI_INTERFACE_GREEN:
    default:
        return GetTextWindowPalette(gSaveBlock2Ptr->optionsInterfaceColor + DEFAULT_TEXT_BOX_FRAME_PALETTES);
        break;
    }   
}

// Effectively LoadUserWindowBorderGfx but specifying the bg directly instead of a window from that bg
void LoadUserWindowBorderGfxOnBg(u8 bg, u16 destOffset, u8 palOffset)
{
    LoadBgTiles(bg, sWindowFrames[gSaveBlock2Ptr->optionsInterfaceColor].tiles, 0x120, destOffset);
    LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsInterfaceColor)->pal, palOffset, PLTT_SIZE_4BPP);
    switch (gSaveBlock2Ptr->optionsInterfaceColor)
    {
    case IKIGAI_INTERFACE_GYM_TYPE_COLOUR:
        return LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->ikigaiGymType + IKIGAI_DEFAULT_INTERFACE_COUNT)->pal, palOffset, PLTT_SIZE_4BPP);
        break;
    case IKIGAI_INTERFACE_PINK:
    case IKIGAI_INTERFACE_ORANGE:
    case IKIGAI_INTERFACE_BLUE:
    case IKIGAI_INTERFACE_GREEN:
    default:
        return LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsInterfaceColor)->pal, palOffset, PLTT_SIZE_4BPP);
        break;
    }
}

const u32 *GetBattleTextboxPalettePtr(void)
{
    switch (gSaveBlock2Ptr->optionsInterfaceColor)
    {
    case IKIGAI_INTERFACE_PINK:
        return gBattleTextboxPalette_Enamorus;
        
    case IKIGAI_INTERFACE_ORANGE:
        return gBattleTextboxPalette_Landorus;
        
    case IKIGAI_INTERFACE_BLUE:
        return gBattleTextboxPalette_Thundurus;
        
    case IKIGAI_INTERFACE_GREEN:
        return gBattleTextboxPalette_Tornadus;
    
    default:
        return GetBattleTextboxGymPalettePtr();
    }   
}

const u32 *GetBattleTextboxGymPalettePtr(void)
{
    switch (gSaveBlock2Ptr->ikigaiGymType)
    {
        case TYPE_NORMAL:
            return gBattleTextboxPalette_Normal;
        case TYPE_FIGHTING:
            return gBattleTextboxPalette_Fighting;
        case TYPE_FLYING:
            return gBattleTextboxPalette_Flying;
        case TYPE_POISON:
            return gBattleTextboxPalette_Poison;
        case TYPE_GROUND:
            return gBattleTextboxPalette_Ground;
        case TYPE_ROCK:
            return gBattleTextboxPalette_Rock;
        case TYPE_BUG:
            return gBattleTextboxPalette_Bug;
        case TYPE_GHOST:
            return gBattleTextboxPalette_Ghost;
        case TYPE_STEEL:
            return gBattleTextboxPalette_Steel;
        case TYPE_FIRE:
            return gBattleTextboxPalette_Fire;
        case TYPE_WATER:
            return gBattleTextboxPalette_Water;
        case TYPE_GRASS:
            return gBattleTextboxPalette_Grass;
        case TYPE_ELECTRIC:
            return gBattleTextboxPalette_Electric;
        case TYPE_PSYCHIC:
            return gBattleTextboxPalette_Psychic;
        case TYPE_ICE:
            return gBattleTextboxPalette_Ice;
        case TYPE_DRAGON:
            return gBattleTextboxPalette_Dragon;
        case TYPE_DARK:
            return gBattleTextboxPalette_Dark;
        case TYPE_FAIRY:
            return gBattleTextboxPalette_Fairy;
            
        default:
            return gBattleTextboxPalette_Emerald;
    }   
}

void LoadDexNavWindowGfx(u8 windowId, u16 destOffset, u8 palOffset)
{
    LoadBgTiles(GetWindowAttribute(windowId, WINDOW_BG), sDexNavWindowFrame.tiles, 0x120, destOffset);
    LoadPalette(sDexNavWindowFrame.pal, palOffset, 32);
}
