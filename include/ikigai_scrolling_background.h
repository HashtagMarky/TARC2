#ifndef GUARD_IKIGAI_SCROLLING_BACKGROUND_H
#define GUARD_IKIGAI_SCROLLING_BACKGROUND_H

#include "global.h"

void StartIkigaiScrollingBackground(u8 background);
extern const u16 *ReturnScrollingBackgroundPalette(void);
extern const u16 *ReturnScrollingBackgroundGymPalette(void);
extern const u16 *ReturnMenuUIPalette(void);
extern const u16 *ReturnMenuUIGymPalette(void);

extern const u32 IkigaiScrollingBgTiles[];
extern const u32 IkigaiScrollingBgTilemap_PalOne[];
extern const u32 IkigaiScrollingBgTilemap_PalTwo[];

#endif // GUARD_IKIGAI_SCROLLING_BACKGROUND_H
