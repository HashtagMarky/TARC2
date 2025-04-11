#ifndef GUARD_IKIGAI_SCROLLING_BACKGROUND_H
#define GUARD_IKIGAI_SCROLLING_BACKGROUND_H

#include "global.h"
#include "calendar.h"

enum IkigaiBackgroundTypes
{
    IKIGAI_BG_DEFAULT,
    IKIGAI_BG_INTERFACE,
    IKIGAI_BG_GYM,
    IKIGAI_BG_SEASON,
    IKIGAI_BG_TYPE_COUNT,
};

void StartIkigaiScrollingBackground(u8 background);
void IkigaiScrollingBackground_CreateTiles(u8 background);
void IkigaiScrollingBackground_CreateTilemap(u8 paletteSlot, void *dest);
void IkigaiScrollingBackground_LoadPalette(u8 paletteSlot, enum IkigaiBackgroundTypes type);
void IkigaiMenuUI_LoadPalette(u8 paletteSlot, enum IkigaiBackgroundTypes type);

extern const u16 *ReturnScrollingBackgroundPalette(void);
extern const u16 *ReturnScrollingBackgroundGymPalette(void);
extern const u16 *ReturnScrollingBackgroundSeasonPalette(enum Seasons season);
extern const u16 *ReturnMenuUIPalette(void);
extern const u16 *ReturnMenuUIGymPalette(void);

#endif // GUARD_IKIGAI_SCROLLING_BACKGROUND_H
