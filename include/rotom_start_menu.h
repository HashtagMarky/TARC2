#ifndef GUARD_ROTOM_START_MENU_H
#define GUARD_ROTOM_START_MENU_H

#include "global.h"

#define RP_CONFIG_NOT_FLIP_PHONE          FlagGet(FLAG_SYS_POKEDEX_GET)
#define RP_CONFIG_MONOCHROME_ICONS        FALSE
#define RP_CONFIG_PALETTE_BUFFER          TRUE
#define RP_CONFIG_UPDATE_CLOCK_DISPLAY    TRUE
#define RP_CONFIG_24_HOUR_MODE            gSaveBlock2Ptr->optionsClockMode
#define RP_CONFIG_NUM_MINUTES_TO_UPDATE   1
#define RP_CONFIG_MESSAGE_SHUTDOWN_TIME   0
#define RP_CONFIG_UPDATE_MESSAGE          TRUE
#define RP_CONFIG_UPDATE_MESSAGE_SOUND    TRUE
#define RP_CONFIG_FACE_UPDATE_PERCENT     100

void RotomPhone_OverworldMenu_Init(bool32 firstInit);
void RotomPhone_FullScreenMenu_Init(void);
void Task_RotomPhone_FullScreenMenu_Open(u8 taskId);
bool32 RotomPhone_StartMenu_IsFullScreen(void);

#endif // GUARD_ROTOM_START_MENU_H


/*      --  Credits & Requirements  --

The Rotom Start Menu uses ShantyTown's ComfyAnim Library,
many thanks to them for creating this public resource.
It will need to be added to your project in order to use
this if not already:
https://github.com/huderlem/pokeemerald/tree/comfy_anims

This Rotom Start Menu was built upon two other public
resources. Grant-Lucas' Sample UI (Sliding Panel) and
Vol's Start Menu provided a basis for both aspects of
this start menu to be made. Many thanks, and a lot of
credit goes to them as well.


        --  How to Add a New Option --
*/