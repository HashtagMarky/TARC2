#ifndef GUARD_VYRATON_H
#define GUARD_VYRATON_H


#define INVALID_METATILE            NUM_METATILES_TOTAL
#define NUM_REPLACEMENT_METATILES   3

extern const struct Tileset gTileset_IkigaiOffice;
extern const struct Tileset gTileset_SSPathfinder_Cabin_Bathroom;

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


#endif // GUARD_VYRATON_H
