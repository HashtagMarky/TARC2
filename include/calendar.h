#ifndef CALENDAR_UI_H
#define CALENDAR_UI_H

#include "gba/types.h"
#include "main.h"

#define tIsWarp data[0]
#define tFinishedLoading data[1]

void Task_OpenCalendarUI(u8 taskId);

#define DAYS_IN_SEASON 28

enum Seasons
{
    SEASON_SPRING,
    SEASON_SUMMER,
    SEASON_AUTUMN,
    SEASON_WINTER,
    SEASON_COUNT,
};

extern const u8 gSeasonNames[SEASON_COUNT][7];

#endif // CALENDAR_UI_H
