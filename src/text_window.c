#include "global.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "palette.h"
#include "constants/pokemon.h"
#include "bg.h"
#include "graphics.h"

const u8 gTextWindowFrame_Gfx[] = INCBIN_U8("graphics/text_window/window_frame.4bpp");
const u16 gTextWindowFrame_EmeraldPal[] = INCBIN_U16("graphics/text_window/window_frame_emerald.gbapal");
static const u16 gTextWindowFrame_GreenPal[] = INCBIN_U16("graphics/text_window/window_frame_ikigai_green.gbapal");
static const u16 gTextWindowFrame_BluePal[] = INCBIN_U16("graphics/text_window/window_frame_ikigai_blue.gbapal");
static const u16 gTextWindowFrame_OrangePal[] = INCBIN_U16("graphics/text_window/window_frame_ikigai_orange.gbapal");
static const u16 gTextWindowFrame_PinkPal[] = INCBIN_U16("graphics/text_window/window_frame_ikigai_pink.gbapal");

static const u16 sTextWindowPalettes[IKIGAI_TEXT_BOX_FRAMES_COUNT][16] = // Ikigai Type Window Pals Here
{
    INCBIN_U16("graphics/text_window/message_box_emerald.gbapal"),
    INCBIN_U16("graphics/text_window/text_window_pal1.gbapal"),
    INCBIN_U16("graphics/text_window/text_window_pal2.gbapal"),
    INCBIN_U16("graphics/text_window/text_window_pal3.gbapal"),
    INCBIN_U16("graphics/text_window/text_window_pal4.gbapal"),
    INCBIN_U16("graphics/text_window/message_box_ikigai_green.gbapal"),         // IKIGAI_INTERFACE_BLUE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/message_box_ikigai_blue.gbapal"),          // IKIGAI_INTERFACE_BLUE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/message_box_ikigai_orange.gbapal"),        // IKIGAI_INTERFACE_BLUE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    INCBIN_U16("graphics/text_window/message_box_ikigai_pink.gbapal")           // IKIGAI_INTERFACE_PINK + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_NONE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_NORMAL + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_FIGHTING + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_FLYING + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_POISON + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_GROUND + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_ROCK + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_BUG + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_GHOST + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_STEEL + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_MYSTERY + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_FIRE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_WATER + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_GRASS + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_ELECTRIC + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_PSYCHIC + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_ICE + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_DRAGON + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_DARK + DEFAULT_TEXT_BOX_FRAME_PALETTES
    // TYPE_FAIRY + DEFAULT_TEXT_BOX_FRAME_PALETTES
};

static const struct TilesPal sWindowFrames[IKIGAI_WINDOW_FRAMES_COUNT] =
{
    {gTextWindowFrame_Gfx, gTextWindowFrame_GreenPal},       // IKIGAI_INTERFACE_GREEN
    {gTextWindowFrame_Gfx, gTextWindowFrame_BluePal},        // IKIGAI_INTERFACE_BLUE
    {gTextWindowFrame_Gfx, gTextWindowFrame_OrangePal},      // IKIGAI_INTERFACE_ORANGE
    {gTextWindowFrame_Gfx, gTextWindowFrame_PinkPal},        // IKIGAI_INTERFACE_PINK
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_NONE + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_NORMAL + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_FIGHTING + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_FLYING + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_POISON + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_GROUND + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_ROCK + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_BUG + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_GHOST + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_STEEL + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_MYSTERY + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_FIRE + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_WATER + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_GRASS + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_ELECTRIC + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_PSYCHIC + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_ICE + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_DRAGON + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal},     // TYPE_DARK + IKIGAI_DEFAULT_INTERFACE_COUNT
    {gTextWindowFrame_Gfx, gTextWindowFrame_EmeraldPal}      // TYPE_FAIRY + IKIGAI_DEFAULT_INTERFACE_COUNT
};

// code
const struct TilesPal *GetWindowFrameTilesPal(u8 id)
{
    if (id >= IKIGAI_WINDOW_FRAMES_COUNT)
        return &sWindowFrames[0];
    else
        return &sWindowFrames[id];
}

void LoadMessageBoxGfx(u8 windowId, u16 destOffset, u8 palOffset)
{
    LoadBgTiles(GetWindowAttribute(windowId, WINDOW_BG), gMessageBox_Gfx, 0x280, destOffset);
    LoadPalette(GetOverworldTextboxPalettePtr(), palOffset, PLTT_SIZE_4BPP);
}

void LoadUserWindowBorderGfx_(u8 windowId, u16 destOffset, u8 palOffset)
{
    LoadUserWindowBorderGfx(windowId, destOffset, palOffset);
}

void LoadWindowGfx(u8 windowId, u8 frameId, u16 destOffset, u8 palOffset)
{
    LoadBgTiles(GetWindowAttribute(windowId, WINDOW_BG), sWindowFrames[frameId].tiles, 0x120, destOffset);
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

const u16 *GetTextWindowPalette(u8 id)
{
    switch (id)
    {
    default:
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
    case IKIGAI_INTERFACE_GYM_TYPE_COLOUR:
        return gBattleTextboxPalette_Emerald;
        break;
    case IKIGAI_INTERFACE_PINK:
        return gBattleTextboxPalette_Pink;
        break;
    case IKIGAI_INTERFACE_ORANGE:
        return gBattleTextboxPalette_Orange;
        break;
    case IKIGAI_INTERFACE_BLUE:
        return gBattleTextboxPalette_Blue;
        break;
    default:
    case IKIGAI_INTERFACE_GREEN:
        return gBattleTextboxPalette_Green;
        break;
    }   
}
