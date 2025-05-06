#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "fieldmap.h"
#include "global.fieldmap.h"
#include "constants/metatile_labels.h"

enum gTileset_IkigaiOffice_Printers
{
    PRINTER_FULL,
    PRINTER_EMPTY,
    PRINTER_COUNT,
};

enum gTileset_IkigaiOffice_Whiteboards
{
    WHITEBOARD_BLANK,
    WHITEBOARD_CHART_ONE,
    WHITEBOARD_CHART_TWO,
    WHITEBOARD_COUNT,
};

enum gTileset_SSPathfinder_Cabin_Bathroom_Toilets
{
    TOILET_OPEN,
    TOILET_CLOSED,
    TOILET_COUNT,
};

enum gTileset_SSPathfinder_Cabin_Bathroom_Baths
{
    BATH_FULL,
    BATH_DUCK,
    BATH_EMPTY,
    BATH_COUNT,
};

const struct IkigaiTilesetReplacementMetatiles gTileset_IkigaiOffice_Replacement[] =
{
    {
        .metatileIdKey = METATILE_IkigaiOffice_ClosedBin,
        .metatileIdReplacements = {METATILE_IkigaiOffice_ClosedBin, METATILE_IkigaiOffice_EmptyBin, METATILE_IkigaiOffice_FullBin},
        .maskCollision = MAPGRID_COLLISION_MASK,
    },
    {
        .metatileIdKey = METATILE_IkigaiOffice_PrinterFull_Top,
        .metatileReplacementFunc = gTileset_IkigaiOffice_ReplacementFunc_Printer,
    },
    {
        .metatileIdKey = METATILE_IkigaiOffice_WhiteboardBlank_TopLeft,
        .metatileReplacementFunc = gTileset_IkigaiOffice_ReplacementFunc_Whiteboard,
    },
    {
        INVALID_METATILE,
    },
};

const struct IkigaiTilesetReplacementMetatiles gTileset_SSPathfinder_Cabin_Bathroom_Replacement[] =
{
    {
        .metatileIdKey = METATILE_SSPathfinder_Cabin_Bathroom_ToiletBottom_Open,
        .metatileReplacementFunc = gTileset_SSPathfinder_Cabin_Bathroom_ReplacementFunc_Toilet,
    },
    {
        .metatileIdKey = METATILE_SSPathfinder_Cabin_Bathroom_BathLeft_Full,
        .metatileReplacementFunc = gTileset_SSPathfinder_Cabin_Bathroom_ReplacementFunc_Bath,
    },
    {
        INVALID_METATILE,
    },
};

const struct IkigaiTilesetReplacementMetatiles gTileset_IkigaiOutdoors_Replacement[] =
{
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_BottomCentre_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_BottomLeft_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_BottomRight_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_CentreLeft_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_CentreRight_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Centre_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Inside_BottomLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Inside_BottomRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Inside_TopLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Inside_TopRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Lilypad_One,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Lilypad_Two,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_BottomCentre,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_BottomLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_BottomRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_CentreLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_CentreRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_TopCentre,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_TopLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_TopLeft_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_TopRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Outside_TopRight_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Rock_One,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Rock_Three,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Rock_Two,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_TopCentre_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Inside_TopLeft_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Inside_TopRight_Wall,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Water,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Weed_One,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Weed_Three,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Pond_Weed_Two,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Inside_BottomLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Inside_BottomRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Inside_TopLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Inside_TopRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Outside_BottomCentre,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Outside_BottomLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Outside_BottomRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Outside_CentreLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Outside_CentreRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Outside_TopCentre,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Outside_TopLeft,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Outside_TopRight,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },
    {
        .metatileIdKey = METATILE_IkigaiOutdoors_Puddle_Water,
        .metatileReplacementFunc = gTileset_IkigaiOutdoors_ReplacementFunc_FrozenWater,
    },    
    {
        INVALID_METATILE,
    },
};

const struct IkigaiTilesetReplacements sTilesetReplacement[] =
{
    {&gTileset_IkigaiOffice, gTileset_IkigaiOffice_Replacement},
    {&gTileset_SSPathfinder_Cabin_Bathroom, gTileset_SSPathfinder_Cabin_Bathroom_Replacement},
    {&gTileset_IkigaiOutdoors, gTileset_IkigaiOutdoors_Replacement},
};