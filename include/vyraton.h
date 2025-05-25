#ifndef GUARD_VYRATON_H
#define GUARD_VYRATON_H

#include "rtc.h"


void Ikigai_TimeAdvanceFunctions(void);


struct IkigaiRandomWeather
{
    u32 weather;
    u32 chance;
};

bool32 Ikigai_ShouldLoadVyratonWeather(void);
void Ikigai_SetVyratonWeather(void);
u8 Ikigai_GetCurrentVyratonWeather(enum TimeOfDay time);


#define INVALID_METATILE            NUM_METATILES_TOTAL
#define NUM_REPLACEMENT_METATILES   3

extern const struct Tileset gTileset_IkigaiOffice;
extern const struct Tileset gTileset_SSPathfinder_Cabin_Bathroom;
extern const struct Tileset gTileset_IkigaiOutdoors;

extern const u16 gTilesetPalettes_IkigaiOutdoors[][16];
extern const u16 gTilesetPalettes_IkigaiOutdoors_Summer[][16];
extern const u16 gTilesetPalettes_IkigaiOutdoors_Autumn[][16];
extern const u16 gTilesetPalettes_IkigaiOutdoors_Winter[][16];
#define PAL_IKIGAI_OUTDOORS_TILESET_FOLIAGE 4
#define PAL_IKIGAI_OUTDOORS_TILESET_GRASS_PATH 3

struct IkigaiTilesetReplacementMetatiles
{
    u16 metatileIdKey;
    void (*metatileReplacementFunc)(s32 x, s32 y);
    u16 metatileIdReplacements[NUM_REPLACEMENT_METATILES];
    u16 maskCollision;
};

struct IkigaiTilesetReplacements
{
    const struct Tileset *tileset;
    const struct IkigaiTilesetReplacementMetatiles *replacementTiles;
};


void VyratonTilesets_DrawRandomisedMetatiles(void);
void gTileset_IkigaiOffice_ReplacementFunc_Printer(s32 x, s32 y);
void gTileset_IkigaiOffice_ReplacementFunc_Whiteboard(s32 x, s32 y);
void gTileset_SSPathfinder_Cabin_Bathroom_ReplacementFunc_Toilet(s32 x, s32 y);
void gTileset_SSPathfinder_Cabin_Bathroom_ReplacementFunc_Bath(s32 x, s32 y);
void gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater(s32 x, s32 y);


#endif // GUARD_VYRATON_H
