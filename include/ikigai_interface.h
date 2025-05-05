#ifndef GUARD_IKIGAI_SCROLLING_BACKGROUND_H
#define GUARD_IKIGAI_SCROLLING_BACKGROUND_H

#include "global.h"
#include "fake_rtc.h"

enum IkigaiInterfacePalettes
{
    IKIGAI_PAL_DEFAULT,
    IKIGAI_PAL_INTERFACE,
    IKIGAI_PAL_GYM,
    IKIGAI_PAL_SEASON,
    IKIGAI_PAL_TYPE_COUNT,
};

void StartIkigaiScrollingBackground(u8 background);
void IkigaiScrollingBackground_CreateTiles(u8 background);
void IkigaiScrollingBackground_CreateTilemap(u8 paletteSlot, void *dest);
void IkigaiScrollingBackground_LoadPalette(u8 paletteSlot, enum IkigaiInterfacePalettes type);
void IkigaiUI_LoadPalette(u8 paletteSlot, enum IkigaiInterfacePalettes type);

extern const u16 *Ikigai_ReturnScrollingBackgroundPalette(void);
extern const u16 *Ikigai_ReturnScrollingBackgroundGymPalette(void);
extern const u16 *Ikigai_ReturnScrollingBackgroundSeasonPalette(enum Seasons season);
extern const u16 *Ikigai_ReturnUIPalette(void);
extern const u16 *Ikigai_ReturnUIGymPalette(void);

#endif // GUARD_IKIGAI_SCROLLING_BACKGROUND_H
