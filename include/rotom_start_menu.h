#ifndef GUARD_ROTOM_START_MENU_H
#define GUARD_ROTOM_START_MENU_H

#include "global.h"

void RotomPhone_SmallStartMenu_Init(void);
void RotomPhone_LargeStartMenu_Init(void);
void Task_OpenRotomPhone_LargeStartMenu(u8 taskId);

extern EWRAM_DATA bool32 openedFullScreenRotomPhone;

#endif // GUARD_ROTOM_START_MENU_H