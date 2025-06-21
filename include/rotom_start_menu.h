#ifndef GUARD_ROTOM_START_MENU_H
#define GUARD_ROTOM_START_MENU_H

#include "global.h"

#define ROTOM_PHONE_NOT_FLIP_PHONE          FlagGet(FLAG_SYS_POKEDEX_GET)
#define ROTOM_PHONE_GREY_ICONS              FALSE
#define ROTOM_PHONE_UPDATE_CLOCK_DISPLAY    TRUE
#define ROTOM_PHONE_24_HOUR_MODE            gSaveBlock2Ptr->optionsClockMode
#define ROTOM_PHONE_NUM_MINUTES_TO_UPDATE   1
#define ROTOM_PHONE_MESSAGE_UPDATE_TIMER    (30 * 60) / FakeRtc_GetSecondsRatio() * ROTOM_PHONE_NUM_MINUTES_TO_UPDATE
#define ROTOM_PHONE_MESSAGE_SHUTDOWN_TIME   0
#define ROTOM_PHONE_UPDATE_MESSAGE          TRUE
#define ROTOM_PHONE_UPDATE_MESSAGE_SOUND    TRUE

void RotomPhone_SmallStartMenu_Init(bool32 firstInit);
void RotomPhone_LargeStartMenu_Init(void);
void Task_OpenRotomPhone_LargeStartMenu(u8 taskId);
bool32 RotomPhone_StartMenu_IsFullScreen(void);

#endif // GUARD_ROTOM_START_MENU_H