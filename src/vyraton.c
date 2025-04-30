#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "vyraton.h"
#include "data/vyraton_randomised_metatiles.h"
#include "fieldmap.h"
#include "field_camera.h"
#include "field_player_avatar.h"
#include "global.fieldmap.h"
#include "random.h"
#include "sound.h"
#include "task.h"
#include "constants/metatile_labels.h"
#include "constants/region_map_sections.h"
#include "constants/songs.h"


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

void gTileset_SSPathfinder_Cabin_Bathroom_ReplacementFunc_Toilet(s32 x, s32 y)
{
    enum gTileset_SSPathfinder_Cabin_Bathroom_Toilets toilet = Random() % TOILET_COUNT;

    if (MapGridGetMetatileIdAt(x, y) != METATILE_SSPathfinder_Cabin_Bathroom_ToiletBottom_Open
    || MapGridGetMetatileIdAt(x, y - 1) != METATILE_SSPathfinder_Cabin_Bathroom_ToiletMiddle_Open
    || MapGridGetMetatileIdAt(x, y - 2) != METATILE_SSPathfinder_Cabin_Bathroom_ToiletTop_Open
    || toilet == TOILET_OPEN)
        return;

    if (toilet == TOILET_CLOSED)
    {
        MapGridSetMetatileIdAt(x, y, METATILE_SSPathfinder_Cabin_Bathroom_ToiletBottom_Closed | MAPGRID_COLLISION_MASK);
        MapGridSetMetatileIdAt(x, y - 1, METATILE_SSPathfinder_Cabin_Bathroom_ToiletMiddle_Closed | MAPGRID_COLLISION_MASK);
        MapGridSetMetatileIdAt(x, y - 2, METATILE_SSPathfinder_Cabin_Bathroom_ToiletTop_Closed | MAPGRID_COLLISION_MASK);
    }
}

void gTileset_SSPathfinder_Cabin_Bathroom_ReplacementFunc_Bath(s32 x, s32 y)
{
    enum gTileset_SSPathfinder_Cabin_Bathroom_Baths bath = Random() % BATH_COUNT;

    if (MapGridGetMetatileIdAt(x, y) != METATILE_SSPathfinder_Cabin_Bathroom_BathLeft_Full
    || MapGridGetMetatileIdAt(x + 1, y) != METATILE_SSPathfinder_Cabin_Bathroom_BathRight_Full
    || bath == BATH_FULL)
        return;

    
    if (bath == BATH_DUCK)
    {
        MapGridSetMetatileIdAt(x, y, METATILE_SSPathfinder_Cabin_Bathroom_BathLeft_Duck | MAPGRID_COLLISION_MASK);
        MapGridSetMetatileIdAt(x + 1, y, METATILE_SSPathfinder_Cabin_Bathroom_BathRight_Duck | MAPGRID_COLLISION_MASK);
    }
    else if (bath == BATH_EMPTY)
    {
        MapGridSetMetatileIdAt(x, y, METATILE_SSPathfinder_Cabin_Bathroom_BathLeft_Empty | MAPGRID_COLLISION_MASK);
        MapGridSetMetatileIdAt(x + 1, y, METATILE_SSPathfinder_Cabin_Bathroom_BathRight_Empty | MAPGRID_COLLISION_MASK);
    }
}

#define tTaskId       data[1]
#define tTimer        data[2]
#define tCoffeeCoordX data[3]
#define tCoffeeCoordY data[4]
static void Task_gTileset_IkigaiOffice_DoCoffeeMachineEffect(u8 taskId)
{
    s32 x = gTasks[taskId].tCoffeeCoordX;
    s32 y = gTasks[taskId].tCoffeeCoordY;

    if (gTasks[taskId].tTimer == 20)
    {
        PlaySE(SE_THUNDERSTORM_STOP);
        MapGridSetMetatileIdAt(x, y - 1, METATILE_IkigaiOffice_CoffeeMachine_CoffeeCup | MAPGRID_COLLISION_MASK);
        DrawWholeMapView();
    }
    else if (gTasks[taskId].tTimer == 60)
    {
        MapGridSetMetatileIdAt(x, y, METATILE_IkigaiOffice_CoffeeMachine_DirtyTable | MAPGRID_COLLISION_MASK);
        DrawWholeMapView();
    }
    else if (gTasks[taskId].tTimer == 100)
    {
        MapGridSetMetatileIdAt(x, y - 1, METATILE_IkigaiOffice_CoffeeMachine_NoCup | MAPGRID_COLLISION_MASK);
        DrawWholeMapView();
        DestroyTask(gTasks[taskId].tTaskId);
    }
    gTasks[taskId].tTimer++;
}

void gTileset_IkigaiOffice_DoCoffeeMachineEffect(void)
{
    s16 x, y;
    u32 tileInFront;

    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);
    tileInFront = MapGridGetMetatileIdAt(x, y);

    if (FuncIsActiveTask(Task_gTileset_IkigaiOffice_DoCoffeeMachineEffect) != TRUE && tileInFront == METATILE_IkigaiOffice_CoffeeMachine_CleanTable)
    {
        u8 taskId = CreateTask(Task_gTileset_IkigaiOffice_DoCoffeeMachineEffect, 8);
        gTasks[taskId].tTaskId = taskId;
        gTasks[taskId].tTimer = 0;
        gTasks[taskId].tCoffeeCoordX = x;
        gTasks[taskId].tCoffeeCoordY = y;
    }
}
#undef tTaskId
#undef tTimer
#undef tCoffeeCoordX
#undef tCoffeeCoordY

void gTileset_SSPathfinder_Cabin_Bathroom_DoToiletEffect(void)
{
    s16 x, y;
    GetXYCoordsOneStepInFrontOfPlayer(&x, &y);

    if (MapGridGetMetatileIdAt(x, y) == METATILE_SSPathfinder_Cabin_Bathroom_ToiletBottom_Open
    && MapGridGetMetatileIdAt(x, y - 1) == METATILE_SSPathfinder_Cabin_Bathroom_ToiletMiddle_Open
    && MapGridGetMetatileIdAt(x, y - 2) == METATILE_SSPathfinder_Cabin_Bathroom_ToiletTop_Open)
    {
        MapGridSetMetatileIdAt(x, y, METATILE_SSPathfinder_Cabin_Bathroom_ToiletBottom_Closed | MAPGRID_COLLISION_MASK);
        MapGridSetMetatileIdAt(x, y - 1, METATILE_SSPathfinder_Cabin_Bathroom_ToiletMiddle_Closed | MAPGRID_COLLISION_MASK);
        MapGridSetMetatileIdAt(x, y - 2, METATILE_SSPathfinder_Cabin_Bathroom_ToiletTop_Closed | MAPGRID_COLLISION_MASK);
    }
    else if (MapGridGetMetatileIdAt(x, y) == METATILE_SSPathfinder_Cabin_Bathroom_ToiletBottom_Closed
    && MapGridGetMetatileIdAt(x, y - 1) == METATILE_SSPathfinder_Cabin_Bathroom_ToiletMiddle_Closed
    && MapGridGetMetatileIdAt(x, y - 2) == METATILE_SSPathfinder_Cabin_Bathroom_ToiletTop_Closed)
    {
        MapGridSetMetatileIdAt(x, y, METATILE_SSPathfinder_Cabin_Bathroom_ToiletBottom_Open);
        MapGridSetMetatileIdAt(x, y - 1, METATILE_SSPathfinder_Cabin_Bathroom_ToiletMiddle_Open | MAPGRID_COLLISION_MASK);
        MapGridSetMetatileIdAt(x, y - 2, METATILE_SSPathfinder_Cabin_Bathroom_ToiletTop_Open | MAPGRID_COLLISION_MASK);
    }
    
    DrawWholeMapView();
}