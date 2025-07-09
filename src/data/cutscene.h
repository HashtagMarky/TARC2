#ifndef GUARD_IKIGAI_DATA_GUARD_CUTSCENE_H
#define GUARD_IKIGAI_DATA_GUARD_CUTSCENE_H

#include "global.h"
#include "gba/gba.h"
#include "constants/cutscene.h"
#include "constants/flags.h"
#include "main.h"

extern const u8 EventScript_PlayerHouse_PlayerWakeUp_Skip[];

const struct CutscenesSkips sCutsceneSkipScripts[] =
{
    [PLAYER_HOUSE_WAKE_UP] =
    {
        .scriptCutsceneSkipPtr = EventScript_PlayerHouse_PlayerWakeUp_Skip,
#if CUTSCENE_FLAG_TRACKING == TRUE
        .flag = FLAG_TEMP_1,
#endif
    },
};

#endif // GUARD_IKIGAI_DATA_GUARD_CUTSCENE_H