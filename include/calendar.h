#ifndef CALENDAR_UI_H
#define CALENDAR_UI_H

#include "gba/types.h"
#include "main.h"

#define tSaved data[13]
#define tShouldSave data[14]
#define tFinishedLoading data[15]

void Task_OpenCalendarUI(u8 taskId);

enum Seasons
{
    SEASON_SPRING,
    SEASON_SUMMER,
    SEASON_AUTUMN,
    SEASON_WINTER,
};

#endif // CALENDAR_UI_H
