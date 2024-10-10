#ifndef GUARD_IKIGAI_SCROLLING_BACKGROUND_H
#define GUARD_IKIGAI_SCROLLING_BACKGROUND_H

#include "global.h"

void StartScrollingBackground(u8 background, u8 op);
extern const u16 *ReturnScrollingBackgroundPalette(void);
extern const u16 *ReturnScrollingBackgroundGymPalette(void);

extern const u32 IkigaiScrollingBgTiles[];
extern const u32 IkigaiScrollingBgTilemap_PalOne[];
extern const u32 IkigaiScrollingBgTilemap_PalTwo[];

#endif // GUARD_IKIGAI_SCROLLING_BACKGROUND_H
