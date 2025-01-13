#ifndef GUARD_SPEEDUP_H
#define GUARD_SPEEDUP_H

#include "gba/gba.h"

#define OPTIONS_SPEEDUP_1X                          0
#define OPTIONS_SPEEDUP_2X                          1
#define OPTIONS_SPEEDUP_4X                          2
#define OPTIONS_SPEEDUP_8X                          3
#define OPTIONS_SPEEDUP_MULTIPLIERS                 4
#define OPTIONS_SPEEDUP_1X_EXTRA_ITERATIONS         0
#define OPTIONS_SPEEDUP_2X_EXTRA_ITERATIONS         1
#define OPTIONS_SPEEDUP_4X_EXTRA_ITERATIONS         3
#define OPTIONS_SPEEDUP_8X_EXTRA_ITERATIONS         7

// Battle Speed Up (Credit to Pokabbie)
u32 Rogue_GetBattleSpeedScale(bool32 forHealthbar);

u8 Speedup_AdditionalIterations(u8 speed);

#endif // GUARD_SPEEDUP_H