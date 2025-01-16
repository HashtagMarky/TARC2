#ifndef GUARD_IKIGAI_DATA_GUARD_CUTSCENE_H
#define GUARD_IKIGAI_DATA_GUARD_CUTSCENE_H

#include "global.h"
#include "gba/gba.h"
#include "constants/cutscene.h"
#include "constants/flags.h"
#include "main.h"

extern const u8 PetalburgCity_EventScript_CutsceneOneEnd[];
extern const u8 PetalburgCity_EventScript_CutsceneTwoEnd[];

const struct CutscenesSkips sCutsceneSkipScripts[] =
{
    [PETALBURG_TEST_ONE] =
    {
        .scriptCutsceneSkips = PetalburgCity_EventScript_CutsceneOneEnd,
#if CUTSCENE_FLAG_TRACKING == TRUE
        .flag = FLAG_TEMP_1,
#endif
    },

    [PETALBURG_TEST_TWO] =
    {
        .scriptCutsceneSkips = PetalburgCity_EventScript_CutsceneTwoEnd,
#if CUTSCENE_FLAG_TRACKING == TRUE
        .flag = FLAG_TEMP_2,
#endif
    },
};

#endif // GUARD_IKIGAI_DATA_GUARD_CUTSCENE_H