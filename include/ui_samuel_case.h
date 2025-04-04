#ifndef GUARD_UI_BIRCH_MENU_H
#define GUARD_UI_BIRCH_MENU_H

#include "main.h"

extern const u16 gIkigaiStarters[NUMBER_OF_MON_TYPES][2];

void Task_OpenSamuelCase(u8 taskId);
void SamuelCase_Init(MainCallback callback);
void SetIkigaiStarterPokedexFlags(void);


#endif // GUARD_UI_MENU_H
