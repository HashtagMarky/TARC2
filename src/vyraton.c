#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "vyraton.h"
#include "data/vyraton_randomised_metatiles.h"
#include "fieldmap.h"
#include "global.fieldmap.h"
#include "random.h"
#include "constants/metatile_labels.h"
#include "constants/region_map_sections.h"


void VyratonTilesets_DrawRandomisedMetatiles(void)
{
    if (gMapHeader.regionMapSectionId != MAPSEC_VYRATON)
        return;
    
    const struct Tileset *tilesetPrimary = gMapHeader.mapLayout->primaryTileset;
    const struct Tileset *tilesetSecondary = gMapHeader.mapLayout->secondaryTileset;
    s32 width = gBackupMapLayout.width;
    s32 height = gBackupMapLayout.height;

    for (u32 entryTileset = 0; entryTileset < ARRAY_COUNT(sTilesetReplacement); entryTileset++)
    {
        if (sTilesetReplacement[entryTileset].tileset != tilesetPrimary && sTilesetReplacement[entryTileset].tileset != tilesetSecondary)
            break;

        for (u32 entryMetatile = 0; sTilesetReplacement[entryTileset].replacementTiles[entryMetatile].metatileIdKey != INVALID_METATILE; entryMetatile++)
        {
            u16 metatileIdKey = sTilesetReplacement[entryTileset].replacementTiles[entryMetatile].metatileIdKey;
            for (s32 y = 0; y < height; y++)
            {
                for (s32 x = 0; x < width; x++)
                {
                    if (MapGridGetMetatileIdAt(x, y) == metatileIdKey && sTilesetReplacement[entryTileset].replacementTiles[entryMetatile].metatileReplacementFunc != NULL)
                    // Call Metatile Replacement Function
                    {
                        sTilesetReplacement[entryTileset].replacementTiles[entryMetatile].metatileReplacementFunc(x, y);
                    }
                    
                    else if (MapGridGetMetatileIdAt(x, y) == metatileIdKey && sTilesetReplacement[entryTileset].replacementTiles[entryMetatile].metatileReplacementFunc == NULL)
                    // Replace with Single Random Metatile
                    {
                        u16 metatileIdReplacement = INVALID_METATILE;
                        u16 maskCollision = sTilesetReplacement[entryTileset].replacementTiles[entryMetatile].maskCollision;

                        while (metatileIdReplacement == INVALID_METATILE)
                        {
                            metatileIdReplacement = sTilesetReplacement[entryTileset].replacementTiles[entryMetatile].metatileIdReplacements[Random() % NUM_REPLACEMENT_METATILES];
                        }

                        MapGridSetMetatileIdAt(x, y, metatileIdReplacement | maskCollision);
                    }
                }
            }
        }
    }
}

void gTileset_IkigaiOffice_ReplacementFunc_Printer(s32 x, s32 y)
{
    enum gTileset_IkigaiOffice_Printers printer = Random() % PRINTER_COUNT;

    if (MapGridGetMetatileIdAt(x, y) == METATILE_IkigaiOffice_PrinterFull_Top
    && MapGridGetMetatileIdAt(x, y + 1) == METATILE_IkigaiOffice_PrinterFull_Bottom)
    {
        if (printer == PRINTER_FULL)
        {
            return;
        }
        else if (printer == PRINTER_EMPTY)
        {
            MapGridSetMetatileIdAt(x, y, METATILE_IkigaiOffice_PrinterEmpty_Top | MAPGRID_COLLISION_MASK);
            MapGridSetMetatileIdAt(x, y + 1, METATILE_IkigaiOffice_PrinterEmpty_Bottom | MAPGRID_COLLISION_MASK);
        }
    }
}

void gTileset_IkigaiOffice_ReplacementFunc_Whiteboard(s32 x, s32 y)
{
    enum gTileset_IkigaiOffice_Whiteboards whiteboard = Random() % WHITEBOARD_COUNT;

    if (MapGridGetMetatileIdAt(x, y) == METATILE_IkigaiOffice_WhiteboardBlank_TopLeft
    && MapGridGetMetatileIdAt(x + 1, y) == METATILE_IkigaiOffice_WhiteboardBlank_TopRight
    && MapGridGetMetatileIdAt(x, y + 1) == METATILE_IkigaiOffice_WhiteboardBlank_BottomLeft
    && MapGridGetMetatileIdAt(x + 1, y + 1) == METATILE_IkigaiOffice_WhiteboardBlank_BottomRight)
    {
        if (whiteboard == WHITEBOARD_BLANK)
        {
            return;
        }
        else if (whiteboard == WHITEBOARD_CHART_ONE)
        {
            MapGridSetMetatileIdAt(x, y, METATILE_IkigaiOffice_WhiteboardChartOne_TopLeft | MAPGRID_COLLISION_MASK);
            MapGridSetMetatileIdAt(x + 1, y, METATILE_IkigaiOffice_WhiteboardChartOne_TopRight | MAPGRID_COLLISION_MASK);
            MapGridSetMetatileIdAt(x, y + 1, METATILE_IkigaiOffice_WhiteboardChartOne_BottomLeft | MAPGRID_COLLISION_MASK);
            MapGridSetMetatileIdAt(x + 1, y + 1, METATILE_IkigaiOffice_WhiteboardChartOne_BottomRight | MAPGRID_COLLISION_MASK);
        }
        else if (whiteboard == WHITEBOARD_CHART_TWO)
        {
            MapGridSetMetatileIdAt(x, y, METATILE_IkigaiOffice_WhiteboardChartTwo_TopLeft | MAPGRID_COLLISION_MASK);
            MapGridSetMetatileIdAt(x + 1, y, METATILE_IkigaiOffice_WhiteboardChartTwo_TopRight | MAPGRID_COLLISION_MASK);
            MapGridSetMetatileIdAt(x, y + 1, METATILE_IkigaiOffice_WhiteboardChartTwo_BottomLeft | MAPGRID_COLLISION_MASK);
            MapGridSetMetatileIdAt(x + 1, y + 1, METATILE_IkigaiOffice_WhiteboardChartTwo_BottomRight | MAPGRID_COLLISION_MASK);
        }
    }
}
