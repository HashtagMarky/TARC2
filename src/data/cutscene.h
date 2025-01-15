#ifndef GUARD_IKIGAI_DATA_GUARD_CUTSCENE_H
#define GUARD_IKIGAI_DATA_GUARD_CUTSCENE_H

#include "global.h"
#include "gba/gba.h"
#include "constants/cutscene.h"
#include "main.h"

extern const u8 PetalburgCity_EventScript_CutsceneOneEnd[];
extern const u8 PetalburgCity_EventScript_CutsceneTwoEnd[];

const struct CutscenesSkips sCutsceneSkipScripts[] =
{
    [PETALBURG_TEST_ONE] = {PetalburgCity_EventScript_CutsceneOneEnd},
    [PETALBURG_TEST_TWO] = {PetalburgCity_EventScript_CutsceneTwoEnd}
};

#endif // GUARD_IKIGAI_DATA_GUARD_CUTSCENE_H