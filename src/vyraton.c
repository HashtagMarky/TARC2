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
        const struct IkigaiTilesetReplacements *tilesetEntry = &sTilesetReplacement[entryTileset];
        const struct IkigaiTilesetReplacementMetatiles *tilesetReplacements = tilesetEntry->replacementTiles;

        if (tilesetEntry->tileset != tilesetPrimary && tilesetEntry->tileset != tilesetSecondary)
            continue;

        for (u32 entryReplacement = 0; tilesetReplacements[entryReplacement].metatileIdKey != INVALID_METATILE; entryReplacement++)
        {
            const struct IkigaiTilesetReplacementMetatiles *metatileEntry = &tilesetReplacements[entryReplacement];
            u16 key = metatileEntry->metatileIdKey;

            for (s32 y = 0; y < height; y++)
            {
                for (s32 x = 0; x < width; x++)
                {
                    if (MapGridGetMetatileIdAt(x, y) != key)
                        continue;

                    if (metatileEntry->metatileReplacementFunc != NULL)
                    {
                        metatileEntry->metatileReplacementFunc(x, y);
                    }
                    else
                    {
                        u16 replacement = INVALID_METATILE;
                        while (replacement == INVALID_METATILE)
                        {
                            replacement = metatileEntry->metatileIdReplacements[Random() % NUM_REPLACEMENT_METATILES];
                        }

                        MapGridSetMetatileIdAt(x, y, replacement | metatileEntry->maskCollision);
                    }
                }
            }
        }
    }
}

void gTileset_IkigaiOffice_ReplacementFunc_Printer(s32 x, s32 y)
{
    enum gTileset_IkigaiOffice_Printers printer = Random() % PRINTER_COUNT;

    if (MapGridGetMetatileIdAt(x, y) != METATILE_IkigaiOffice_PrinterFull_Top
    || MapGridGetMetatileIdAt(x, y + 1) != METATILE_IkigaiOffice_PrinterFull_Bottom
    || printer == PRINTER_FULL)
        return;

    if (printer == PRINTER_EMPTY)
    {
        MapGridSetMetatileIdAt(x, y, METATILE_IkigaiOffice_PrinterEmpty_Top | MAPGRID_COLLISION_MASK);
        MapGridSetMetatileIdAt(x, y + 1, METATILE_IkigaiOffice_PrinterEmpty_Bottom | MAPGRID_COLLISION_MASK);
    }
}

void gTileset_IkigaiOffice_ReplacementFunc_Whiteboard(s32 x, s32 y)
{
    enum gTileset_IkigaiOffice_Whiteboards whiteboard = Random() % WHITEBOARD_COUNT;

    if (MapGridGetMetatileIdAt(x, y) != METATILE_IkigaiOffice_WhiteboardBlank_TopLeft
    || MapGridGetMetatileIdAt(x + 1, y) != METATILE_IkigaiOffice_WhiteboardBlank_TopRight
    || MapGridGetMetatileIdAt(x, y + 1) != METATILE_IkigaiOffice_WhiteboardBlank_BottomLeft
    || MapGridGetMetatileIdAt(x + 1, y + 1) != METATILE_IkigaiOffice_WhiteboardBlank_BottomRight
    || whiteboard == WHITEBOARD_BLANK)
        return;

    if (whiteboard == WHITEBOARD_CHART_ONE)
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
