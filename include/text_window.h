#ifndef GUARD_TEXT_WINDOW_H
#define GUARD_TEXT_WINDOW_H

#define IKIGAI_TEXT_WINDOW_BLUE                 0
#define IKIGAI_TEXT_WINDOW_PINK                 1
#define IKIGAI_TEXT_WINDOW_GYM_TYPE_COLOUR      2
#define IKIGAI_TEXT_WINDOW_GYM_TYPE_COLOUR_DARK 3

#define IKIGAI_DEFAULT_WINDOW_FRAMES_COUNT      2
#define DEFAULT_TEXT_BOX_FRAME_PALETTES         5

#define IKIGAI_WINDOW_FRAMES_COUNT              IKIGAI_DEFAULT_WINDOW_FRAMES_COUNT + NUMBER_OF_MON_TYPES - 1 // = 23 - 1 to account for no Stellar Type Gym.
#define IKIGAI_TEXT_BOX_FRAMES_COUNT            IKIGAI_WINDOW_FRAMES_COUNT + DEFAULT_TEXT_BOX_FRAME_PALETTES // = 27

struct TilesPal
{
    const u8 *tiles;
    const u16 *pal;
};

extern const u8 gTextWindowFrame_Gfx[];
extern const u16 gTextWindowFrame_EmeraldPal[];

const struct TilesPal *GetWindowFrameTilesPal(u8 id);
void LoadMessageBoxGfx(u8 windowId, u16 destOffset, u8 palOffset);
void LoadWindowGfx(u8 windowId, u8 frameId, u16 destOffset, u8 palOffset);
void LoadUserWindowBorderGfx(u8 windowId, u16 destOffset, u8 palOffset);
void LoadUserWindowBorderGfx_(u8 windowId, u16 destOffset, u8 palOffset);
void LoadUserWindowBorderGfxOnBg(u8 bg, u16 destOffset, u8 palOffset);
void DrawTextBorderOuter(u8 windowId, u16 tileNum, u8 palNum);
void DrawTextBorderInner(u8 windowId, u16 tileNum, u8 palNum);
void rbox_fill_rectangle(u8 windowId);
const u16 *GetTextWindowPalette(u8 id);
const u16 *GetOverworldTextboxPalettePtr(void);
const u32 *GetBattleTextboxPalettePtr(void);

#endif // GUARD_TEXT_WINDOW_H
