#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_script_commands.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_player_avatar.h"
#include "global.fieldmap.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "overworld_encounters.h"
#include "pokedex.h"
#include "pokemon.h"
#include "rtc.h"
#include "constants/items.h"
#include "constants/map_types.h"

// This Should Replicate Cmd_handleballthrow in battle_script_commands.c
void GetOverworldSpeciesCatchRate(void)
{
    u16 ballMultiplier = 100;
    s8 ballAddition = 0;
    u16 species = gSpecialVar_0x8004;
    u16 level = gSpecialVar_0x8006;
    u32 odds, i;
    u8 catchRate;
    u16 ball = gSpecialVar_0x8007;
    u16 tileBehavior;
    s16 x, y;
    u8 shakes;
    u8 maxShakes;
    u8 result = 0;

    PlayerGetDestCoords(&x, &y);
    tileBehavior = MapGridGetMetatileBehaviorAt(x, y);

    catchRate = gSpeciesInfo[species].catchRate;

    if (gSpeciesInfo[species].isUltraBeast)
    {
        if (ball == ITEM_BEAST_BALL)
            ballMultiplier = 500;
        else
            ballMultiplier = 10;
    }
    else
    {
        switch (ball)
        {
        case ITEM_ULTRA_BALL:
            ballMultiplier = 200;
            break;
        case ITEM_SPORT_BALL:
            if (B_SPORT_BALL_MODIFIER <= GEN_7)
                ballMultiplier = 150;
            break;
        case ITEM_GREAT_BALL:
            ballMultiplier = 150;
            break;
        case ITEM_SAFARI_BALL:
            if (B_SAFARI_BALL_MODIFIER <= GEN_7)
                ballMultiplier = 150;
            break;
        case ITEM_NET_BALL:
            if (IS_BATTLER_OF_TYPE(species, TYPE_WATER) || IS_BATTLER_OF_TYPE(species, TYPE_BUG))
                ballMultiplier = B_NET_BALL_MODIFIER >= GEN_7 ? 350 : 300;
            break;
        case ITEM_DIVE_BALL:
            if (GetCurrentMapType() == MAP_TYPE_UNDERWATER
                || (B_DIVE_BALL_MODIFIER >= GEN_4 && (MetatileBehavior_IsSurfableWaterOrUnderwater(tileBehavior) == TRUE)))
                ballMultiplier = 350;
            break;
        case ITEM_NEST_BALL:
            if (B_NEST_BALL_MODIFIER >= GEN_6)
            {
                //((41 - Pokémon's level) ÷ 10)× if Pokémon's level is between 1 and 29, 1× otherwise.
                if (level < 30)
                    ballMultiplier = 410 - (level * 10);
            }
            else if (B_NEST_BALL_MODIFIER >= GEN_5)
            {
                //((41 - Pokémon's level) ÷ 10)×, minimum 1×
                if (level < 31)
                    ballMultiplier = 410 - (level * 10);
            }
            else if (level < 40)
            {
                //((40 - Pokémon's level) ÷ 10)×, minimum 1×
                ballMultiplier = 400 - (level * 10);
                if (ballMultiplier <= 90)
                    ballMultiplier = 100;
            }
            break;
        case ITEM_REPEAT_BALL:
            if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
                ballMultiplier = (B_REPEAT_BALL_MODIFIER >= GEN_7 ? 350 : 300);
            break;
        /*
        case ITEM_TIMER_BALL:
            ballMultiplier = 100 + (gBattleResults.battleTurnCounter * (B_TIMER_BALL_MODIFIER >= GEN_5 ? 30 : 10));
            if (ballMultiplier > 400)
                ballMultiplier = 400;
            break;
        */
        case ITEM_DUSK_BALL:
            i = GetTimeOfDay();
            if (i == TIME_EVENING || i == TIME_NIGHT || gMapHeader.cave || gMapHeader.mapType == MAP_TYPE_UNDERGROUND)
                ballMultiplier = (B_DUSK_BALL_MODIFIER >= GEN_7 ? 300 : 350);
            break;
        case ITEM_QUICK_BALL:
            ballMultiplier = (B_QUICK_BALL_MODIFIER >= GEN_5 ? 500 : 400);
            break;
        case ITEM_LEVEL_BALL:
            if (GetMonData(GetFirstLiveMon(), MON_DATA_LEVEL) >= 4 * level)
                ballMultiplier = 800;
            else if (GetMonData(GetFirstLiveMon(), MON_DATA_LEVEL) > 2 * level)
                ballMultiplier = 400;
            else if (GetMonData(GetFirstLiveMon(), MON_DATA_LEVEL) > level)
                ballMultiplier = 200;
            break;
        case ITEM_LURE_BALL:
            if (MetatileBehavior_IsSurfableWaterOrUnderwater(tileBehavior) == TRUE)
            {
                if (B_LURE_BALL_MODIFIER >= GEN_8)
                    ballMultiplier = 400;
                else if (B_LURE_BALL_MODIFIER >= GEN_7)
                    ballMultiplier = 500;
                else
                    ballMultiplier = 300;
            }
            break;
        case ITEM_MOON_BALL:
        {
            const struct Evolution *evolutions = GetSpeciesEvolutions(species);
            if (evolutions == NULL)
                break;
            for (i = 0; evolutions[i].method != EVOLUTIONS_END; i++)
            {
                if (evolutions[i].method == EVO_ITEM
                    && evolutions[i].param == ITEM_MOON_STONE)
                    ballMultiplier = 400;
            }
        }
        break;
        /*
        case ITEM_LOVE_BALL:
            if (GetMonData(GetFirstLiveMon(), MON_DATA_SPECIES == species)
            {
                u8 gender1 = gSpeciesInfo[species].genderRatio;
                    if (gender1 != MON_MALE
                u8 gender2 = GetMonData(GetFirstLiveMon(), MON_DATA_OT_GENDER;

                if (gender1 != gender2 && gender1 != MON_GENDERLESS && gender2 != MON_GENDERLESS)
                    ballMultiplier = 800;
            }
            break;
        */
        case ITEM_FAST_BALL:
            if (gSpeciesInfo[species].baseSpeed >= 100)
                ballMultiplier = 400;
            break;
        case ITEM_HEAVY_BALL:
            i = GetSpeciesWeight(species);
            if (B_HEAVY_BALL_MODIFIER >= GEN_7)
            {
                if (i < 1000)
                    ballAddition = -20;
                else if (i < 2000)
                    ballAddition = 0;
                else if (i < 3000)
                    ballAddition = 20;
                else
                    ballAddition = 30;
            }
            else if (B_HEAVY_BALL_MODIFIER >= GEN_4)
            {
                if (i < 2048)
                    ballAddition = -20;
                else if (i < 3072)
                    ballAddition = 20;
                else if (i < 4096)
                    ballAddition = 30;
                else
                    ballAddition = 40;
            }
            else
            {
                if (i < 1024)
                    ballAddition = -20;
                else if (i < 2048)
                    ballAddition = 0;
                else if (i < 3072)
                    ballAddition = 20;
                else if (i < 4096)
                    ballAddition = 30;
                else
                    ballAddition = 40;
            }
            break;
        /*
        case ITEM_DREAM_BALL:
            if (B_DREAM_BALL_MODIFIER >= GEN_8 && (gBattleMons[gBattlerTarget].status1 & STATUS1_SLEEP || GetBattlerAbility(gBattlerTarget) == ABILITY_COMATOSE))
                ballMultiplier = 400;
            break;
        */
        case ITEM_BEAST_BALL:
            ballMultiplier = 10;
            break;
        }
    }

    // catchRate is unsigned, which means that it may potentially overflow if sum is applied directly.
    if (catchRate < 21 && ballAddition == -20)
        catchRate = 1;
    else
        catchRate = catchRate + ballAddition;

    /*
    odds = (catchRate * ballMultiplier / 100)
        * (gBattleMons[gBattlerTarget].maxHP * 3 - gBattleMons[gBattlerTarget].hp * 2)
        / (3 * gBattleMons[gBattlerTarget].maxHP);
    */
        
    /*
    if (gBattleMons[gBattlerTarget].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
        odds *= 2;
    if (gBattleMons[gBattlerTarget].status1 & (STATUS1_POISON | STATUS1_BURN | STATUS1_PARALYSIS | STATUS1_TOXIC_POISON | STATUS1_FROSTBITE))
        odds = (odds * 15) / 10;

    if (gBattleResults.catchAttempts[gLastUsedItem - FIRST_BALL] < 255)
        gBattleResults.catchAttempts[gLastUsedItem - FIRST_BALL]++;
    */
        
    odds = (catchRate * ballMultiplier / 100) * (3 - 2) / (3);

    if (odds > 254) // mon caught
    {
        gSpecialVar_Result = TRUE;
    }
    else // mon may be caught, calculate shakes
    {   
        if (CriticalCapture(odds))
        {
            maxShakes = BALL_1_SHAKE;  // critical capture doesn't guarantee capture
            result = 2;
        }
        else
        {
            maxShakes = BALL_3_SHAKES_SUCCESS;
        }

        odds = Sqrt(Sqrt(16711680 / odds));
        odds = 1048560 / odds;
        for (shakes = 0; shakes < maxShakes && Random() < odds; shakes++);

        if (ball == ITEM_MASTER_BALL)
            shakes = maxShakes;

        if (shakes == maxShakes)
        {
            result++;
        }
        else
        {
            result = 0;
        }
        gSpecialVar_Result = result;
    }
}