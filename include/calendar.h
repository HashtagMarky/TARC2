#ifndef CALENDAR_UI_H
#define CALENDAR_UI_H

#include "gba/types.h"
#include "main.h"

#define tWarp data[0]
#define tFinishedLoading data[1]

void Task_OpenCalendarUI(u8 taskId);

enum Seasons
{
    SEASON_SPRING,
    SEASON_SUMMER,
    SEASON_AUTUMN,
    SEASON_WINTER,
    SEASON_COUNT,
};

#endif // CALENDAR_UI_H
