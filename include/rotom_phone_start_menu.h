#ifndef GUARD_ROTOM_PHONE_START_H
#define GUARD_ROTOM_PHONE_START_H

#include "global.h"

struct RotomPhoneMenuOptions
{
    const u8 *menuName;
};

void RotomPhone_StartMenu_Init(void);
void GoToHandleInput(void);

#endif // GUARD_ROTOM_PHONE_START_H