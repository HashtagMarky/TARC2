#ifndef GUARD_ROTOM_PHONE_START_H
#define GUARD_ROTOM_PHONE_START_H

#include "global.h"

void RotomPhone_StartMenu_Init(void);
void Task_OpenRotomPhone_LargeStartMenu(u8 taskId);

extern EWRAM_DATA bool32 openedFullScreenRotomPhone;

#endif // GUARD_ROTOM_PHONE_START_H