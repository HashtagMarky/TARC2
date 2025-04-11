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
void IkigaiUI_LoadPalette(u8 paletteSlot, enum IkigaiBackgroundTypes type);

extern const u16 *Ikigai_ReturnScrollingBackgroundPalette(void);
extern const u16 *Ikigai_ReturnScrollingBackgroundGymPalette(void);
extern const u16 *Ikigai_ReturnScrollingBackgroundSeasonPalette(enum Seasons season);
extern const u16 *Ikigai_ReturnUIPalette(void);
extern const u16 *Ikigai_ReturnUIGymPalette(void);

#endif // GUARD_IKIGAI_SCROLLING_BACKGROUND_H
