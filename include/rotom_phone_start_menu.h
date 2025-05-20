#ifndef GUARD_ROTOM_PHONE_START_H
#define GUARD_ROTOM_PHONE_START_H

#include "global.h"

/* ENUMs */
enum RotomPhoneMenuItems
{
    ROTOM_PHONE_MENU_POKEDEX,
    ROTOM_PHONE_MENU_PARTY,
    ROTOM_PHONE_MENU_BAG,
    ROTOM_PHONE_MENU_POKENAV,
    ROTOM_PHONE_MENU_TRAINER_CARD,
    ROTOM_PHONE_MENU_SAVE,
    ROTOM_PHONE_MENU_OPTIONS,
    ROTOM_PHONE_MENU_FLAG,
    ROTOM_PHONE_MENU_COUNT,
};

struct RotomPhoneMenuOptions
{
    const u8 *menuName;
};

void RotomPhone_StartMenu_Init(void);
void GoToHandleInput(void);

#endif // GUARD_ROTOM_PHONE_START_H