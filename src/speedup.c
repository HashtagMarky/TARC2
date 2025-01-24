#include "speedup.h"
#include "global.h"
#include "gba/gba.h"
#include "event_data.h"
#include "main.h"
#include "battle.h"
#include "pokemon.h"
#include "constants/species.h"
#include "constants/flags.h"
#include "constants/global.h"


// Battle Speed Up (Credit to Pokabbie)
static u8 GetBattleSpeedOption(void);
static bool8 IsBattleImportant(void);

u32 Rogue_GetBattleSpeedScale(bool32 forHealthbar)
{
    u8 battleSpeedOption = GetBattleSpeedOption();

    if (IsBattleImportant())
        return 1;

    // Hold L to slow down
    if(JOY_HELD(L_BUTTON))
        return 1;

    // We want to speed up all anims until input selection starts
    if(InBattleChoosingMoves())
        gBattleStruct->hasBattleInputStarted = TRUE;

    if(gBattleStruct->hasBattleInputStarted)
    {
        // Always run at 1x speed here
        if(InBattleChoosingMoves())
            return 1;
/*
        // When battle anims are turned off, it's a bit too hard to read text, so force running at normal speed
        if(!forHealthbar && battleSpeedOption == OPTIONS_BATTLE_SPEED_DISABLED && InBattleRunningActions())
            return 1;
*/
    }

    // We don't need to speed up health bar anymore as that passively happens now
    switch (battleSpeedOption)
    {
    case OPTIONS_SPEEDUP_NORMAL:
        return forHealthbar ? (OPTIONS_SPEEDUP_NORMAL_EXTRA_ITERATIONS + 1) : (OPTIONS_SPEEDUP_NORMAL_EXTRA_ITERATIONS + 1);

    case OPTIONS_SPEEDUP_2X:
        return forHealthbar ? (OPTIONS_SPEEDUP_NORMAL_EXTRA_ITERATIONS + 1) : (OPTIONS_SPEEDUP_2X_EXTRA_ITERATIONS + 1);

    case OPTIONS_SPEEDUP_4X:
        return forHealthbar ? (OPTIONS_SPEEDUP_NORMAL_EXTRA_ITERATIONS + 1) : (OPTIONS_SPEEDUP_4X_EXTRA_ITERATIONS + 1);

    case OPTIONS_SPEEDUP_8X:
        return forHealthbar ? (OPTIONS_SPEEDUP_NORMAL_EXTRA_ITERATIONS + 1) : (OPTIONS_SPEEDUP_8X_EXTRA_ITERATIONS + 1);
/*
    // Print text at a readable speed still
    case OPTIONS_BATTLE_SPEED_DISABLED:
        if(gBattleStruct->hasBattleInputStarted)
            return forHealthbar ? 10 : 1;
        else
            return 4;
*/
    }

    return 1;
}

static u8 GetBattleSpeedOption(void)
{
    if (IsBattleImportant())
        return OPTIONS_SPEEDUP_NORMAL;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        return gSaveBlock2Ptr->optionsTrainerBattleSpeed;
    else
        return gSaveBlock2Ptr->optionsWildBattleSpeed;
}

static bool8 IsBattleImportant(void)
{
    // First Battle will not be Sped Up
    if (gBattleTypeFlags & BATTLE_TYPE_FIRST_BATTLE)
        return TRUE;

    // Tutorial Battle will not be Sped Up
    if (gBattleTypeFlags & BATTLE_TYPE_WALLY_TUTORIAL)
        return TRUE;

    // Legendary Battles will not be Sped Up
    if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY)  
        return TRUE;

    // Battles against Roaming Pokémon will not be Sped Up
    if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)  
        return TRUE;

    // If a Wild Pokémon in the party is shiny, battles will not be Sped Up
    if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER))
    {
        for (int i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES) != SPECIES_NONE
            && IsMonShiny(&gEnemyParty[i]))
                return TRUE;
        }
    }

    return FALSE;
}

u8 Speedup_AdditionalIterations(u8 speed, bool8 overworld)
{
    if (overworld
        && (JOY_HELD(R_BUTTON)
        || FlagGet(FLAG_SUPPRESS_OVERWORLD_SPEEDUP)
        || FlagGet(DN_FLAG_SEARCHING)))
    {
        return OPTIONS_SPEEDUP_NORMAL_EXTRA_ITERATIONS;
    }

    switch (speed)
    {
    case OPTIONS_SPEEDUP_8X: return OPTIONS_SPEEDUP_8X_EXTRA_ITERATIONS;
    case OPTIONS_SPEEDUP_4X: return OPTIONS_SPEEDUP_4X_EXTRA_ITERATIONS;
    case OPTIONS_SPEEDUP_2X: return OPTIONS_SPEEDUP_2X_EXTRA_ITERATIONS;
    case OPTIONS_SPEEDUP_NORMAL: return OPTIONS_SPEEDUP_NORMAL_EXTRA_ITERATIONS;
    default: return OPTIONS_SPEEDUP_NORMAL_EXTRA_ITERATIONS;
    }
}

