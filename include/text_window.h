#ifndef GUARD_TEXT_WINDOW_H
#define GUARD_TEXT_WINDOW_H

#define DEFAULT_TEXT_BOX_FRAME_PALETTES       5
#define IKIGAI_WINDOW_FRAMES_COUNT            IKIGAI_DEFAULT_INTERFACE_COUNT + NUMBER_OF_MON_TYPES - 1 // = 25 - 1 to account for no Stellar Type Gym.
#define IKIGAI_TEXT_BOX_FRAMES_COUNT          IKIGAI_WINDOW_FRAMES_COUNT + DEFAULT_TEXT_BOX_FRAME_PALETTES // = 29

struct TilesPal
{
    const u8 *tiles;
    const u16 *pal;
};

extern const u8 gTextWindowFrame_Gfx[];
extern const u16 gTextWindowFrame_EmeraldPal[];

const struct TilesPal *GetWindowFrameTilesPal(u8 id);
void LoadMessageBoxGfx(u8 windowId, u16 destOffset, u8 palOffset);
void LoadSignBoxGfx(u8 windowId, u16 destOffset, u8 palOffset);
void LoadWindowGfx(u8 windowId, u8 frameId, u16 destOffset, u8 palOffset);
void LoadUserWindowBorderGfx(u8 windowId, u16 destOffset, u8 palOffset);
void LoadUserWindowBorderGfx_(u8 windowId, u16 destOffset, u8 palOffset);
void LoadUserWindowBorderGfxOnBg(u8 bg, u16 destOffset, u8 palOffset);
void DrawTextBorderOuter(u8 windowId, u16 tileNum, u8 palNum);
void DrawTextBorderInner(u8 windowId, u16 tileNum, u8 palNum);
void rbox_fill_rectangle(u8 windowId);
const u16 *GetTextWindowPalette(u16 id);
const u16 *GetOverworldTextboxPalettePtr(void);
const u16 *GetBattleTextboxPalettePtr(void);
void LoadSignPostWindowFrameGfx(void);
void LoadDexNavWindowGfx(u8 windowId, u16 destOffset, u8 palOffset);

#endif // GUARD_TEXT_WINDOW_H
