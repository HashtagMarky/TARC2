#ifndef GUARD_ROTOM_START_MENU_H
#define GUARD_ROTOM_START_MENU_H

#include "global.h"

#define RP_CONFIG_NOT_FLIP_PHONE          FlagGet(FLAG_SYS_POKEDEX_GET)
#define RP_CONFIG_GREY_ICONS              FALSE
#define RP_CONFIG_PALETTE_BUFFER          TRUE
#define RP_CONFIG_UPDATE_CLOCK_DISPLAY    TRUE
#define RP_CONFIG_24_HOUR_MODE            gSaveBlock2Ptr->optionsClockMode
#define RP_CONFIG_NUM_MINUTES_TO_UPDATE   1
#define RP_CONFIG_MESSAGE_UPDATE_TIMER    (30 * 60) / FakeRtc_GetSecondsRatio() * RP_CONFIG_NUM_MINUTES_TO_UPDATE
#define RP_CONFIG_MESSAGE_SHUTDOWN_TIME   0
#define RP_CONFIG_UPDATE_MESSAGE          TRUE
#define RP_CONFIG_UPDATE_MESSAGE_SOUND    TRUE
#define RP_CONFIG_FACE_UPDATE_PERCENT     100

void RotomPhone_OverworldMenu_Init(bool32 firstInit);
void RotomPhone_FullScreenMenu_Init(void);
void Task_RotomPhone_FullScreenMenu_Open(u8 taskId);
bool32 RotomPhone_StartMenu_IsFullScreen(void);

#endif // GUARD_ROTOM_START_MENU_H