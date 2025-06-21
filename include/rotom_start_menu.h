#ifndef GUARD_ROTOM_START_MENU_H
#define GUARD_ROTOM_START_MENU_H

#include "global.h"

#define RP_NOT_FLIP_PHONE          FlagGet(FLAG_SYS_POKEDEX_GET)
#define RP_GREY_ICONS              FALSE
#define RP_UPDATE_CLOCK_DISPLAY    TRUE
#define RP_24_HOUR_MODE            gSaveBlock2Ptr->optionsClockMode
#define RP_NUM_MINUTES_TO_UPDATE   1
#define RP_MESSAGE_UPDATE_TIMER    (30 * 60) / FakeRtc_GetSecondsRatio() * RP_NUM_MINUTES_TO_UPDATE
#define RP_MESSAGE_SHUTDOWN_TIME   0
#define RP_UPDATE_MESSAGE          TRUE
#define RP_UPDATE_MESSAGE_SOUND    TRUE
#define RP_FACE_UPDATE_PERCENT     100

void RotomPhone_OverworldMenu_Init(bool32 firstInit);
void RotomPhone_FullScreenMenu_Init(void);
void Task_RotomPhone_FullScreenMenu_Open(u8 taskId);
bool32 RotomPhone_StartMenu_IsFullScreen(void);

#endif // GUARD_ROTOM_START_MENU_H