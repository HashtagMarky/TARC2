#ifndef GUARD_ROTOM_START_MENU_H
#define GUARD_ROTOM_START_MENU_H

#include "global.h"

void RotomPhone_SmallStartMenu_Init(bool32 printGreeting);
void RotomPhone_LargeStartMenu_Init(void);
void Task_OpenRotomPhone_LargeStartMenu(u8 taskId);
bool32 RotomPhone_StartMenu_IsFullScreen(void);

#endif // GUARD_ROTOM_START_MENU_H