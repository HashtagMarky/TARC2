#ifndef CALENDAR_UI_H
#define CALENDAR_UI_H

#include "gba/types.h"
#include "main.h"

#define tIsWarp data[0]
#define tFinishedLoading data[1]

extern const u8 EventScript_SetupCalendarWarpHome[];

void Task_OpenCalendarUI(u8 taskId);
void Ikigai_CheckBedtime(void);

#endif // CALENDAR_UI_H
