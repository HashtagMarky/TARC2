#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_script_commands.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "field_player_avatar.h"
#include "global.fieldmap.h"
#include "item.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "overworld_encounters.h"
#include "pokedex.h"
#include "pokemon.h"
#include "rtc.h"
#include "script.h"
#include "script_movement.h"
#include "wild_encounter.h"
#include "constants/abilities.h"
#include "constants/event_objects.h"
#include "constants/items.h"
#include "constants/field_mugshots.h"
#include "constants/hold_effects.h"
#include "constants/map_types.h"
#include "constants/pokemon.h"

#define HEADER_NONE                     0xFFFF                  // Should be same as in wild_encounter.c
#define OBJ_EVENT_GFX_LAST              OBJ_EVENT_GFX_VAR_F     // Last variable object event

#define OVERWORLD_CATCH_RATE_MULTIPLYER gSaveBlock2Ptr->optionsOverworldCatchSuccessMultiplyer + 1
#define SPAWN_ODDS_DENOMINATOR          3                       // Spawn Odds = 1 - 1/SPAWN_ODDS_DENOMINATOR
#define RUN_ODDS_DENOMINATOR            3                       // Run Odds = 1/RUN_ODDS_DENOMINATOR

void GetOverworldMonSpecies(void)
{
    SantizeOverworldMonLevel();
    struct ObjectEvent *objEvent = &gObjectEvents[gSelectedObjectEvent];

    switch (objEvent->graphicsId)
    {
    case OBJ_EVENT_GFX_RAYQUAZA_STILL:
    case OBJ_EVENT_GFX_RAYQUAZA:
        gSpecialVar_0x8004 = SPECIES_RAYQUAZA;
        break;

    case OBJ_EVENT_GFX_UNUSED_NATU_DOLL:
        gSpecialVar_0x8004 = SPECIES_NATU;
        break;

    case OBJ_EVENT_GFX_UNUSED_SQUIRTLE_DOLL:
        gSpecialVar_0x8004 = SPECIES_SQUIRTLE;
        break;

    case OBJ_EVENT_GFX_UNUSED_WOOPER_DOLL:
        gSpecialVar_0x8004 = SPECIES_WOOPER;
        break;

    case OBJ_EVENT_GFX_UNUSED_PIKACHU_DOLL:
    case OBJ_EVENT_GFX_PIKACHU_DOLL:
    case OBJ_EVENT_GFX_PIKACHU:
        gSpecialVar_0x8004 = SPECIES_PIKACHU;
        break;

    case OBJ_EVENT_GFX_UNUSED_PORYGON2_DOLL:
        gSpecialVar_0x8004 = SPECIES_PORYGON2;
        break;

    case OBJ_EVENT_GFX_VIGOROTH_CARRYING_BOX:
    case OBJ_EVENT_GFX_VIGOROTH_FACING_AWAY:
        gSpecialVar_0x8004 = SPECIES_VIGOROTH;
        break;

    case OBJ_EVENT_GFX_ZIGZAGOON_1:
    case OBJ_EVENT_GFX_ZIGZAGOON_2:
        gSpecialVar_0x8004 = SPECIES_ZIGZAGOON;
        break;

    case OBJ_EVENT_GFX_PICHU_DOLL:
        gSpecialVar_0x8004 = SPECIES_PICHU;
        break;

    case OBJ_EVENT_GFX_MARILL_DOLL:
        gSpecialVar_0x8004 = SPECIES_MARILL;
        break;

    case OBJ_EVENT_GFX_TOGEPI_DOLL:
        gSpecialVar_0x8004 = SPECIES_TOGEPI;
        break;

    case OBJ_EVENT_GFX_CYNDAQUIL_DOLL:
        gSpecialVar_0x8004 = SPECIES_CYNDAQUIL;
        break;

    case OBJ_EVENT_GFX_CHIKORITA_DOLL:
        gSpecialVar_0x8004 = SPECIES_CHIKORITA;
        break;

    case OBJ_EVENT_GFX_TOTODILE_DOLL:
        gSpecialVar_0x8004 = SPECIES_TOTODILE;
        break;

    case OBJ_EVENT_GFX_JIGGLYPUFF_DOLL:
        gSpecialVar_0x8004 = SPECIES_JIGGLYPUFF;
        break;

    case OBJ_EVENT_GFX_MEOWTH_DOLL:
        gSpecialVar_0x8004 = SPECIES_MEOWTH;
        break;

    case OBJ_EVENT_GFX_CLEFAIRY_DOLL:
        gSpecialVar_0x8004 = SPECIES_CLEFAIRY;
        break;

    case OBJ_EVENT_GFX_DITTO_DOLL:
        gSpecialVar_0x8004 = SPECIES_DITTO;
        break;

    case OBJ_EVENT_GFX_SMOOCHUM_DOLL:
        gSpecialVar_0x8004 = SPECIES_SMOOCHUM;
        break;

    case OBJ_EVENT_GFX_TREECKO_DOLL:
        gSpecialVar_0x8004 = SPECIES_TREECKO;
        break;

    case OBJ_EVENT_GFX_TORCHIC_DOLL:
        gSpecialVar_0x8004 = SPECIES_TORCHIC;
        break;

    case OBJ_EVENT_GFX_MUDKIP_DOLL:
        gSpecialVar_0x8004 = SPECIES_MUDKIP;
        break;

    case OBJ_EVENT_GFX_DUSKULL_DOLL:
        gSpecialVar_0x8004 = SPECIES_DUSKULL;
        break;

    case OBJ_EVENT_GFX_WYNAUT_DOLL:
        gSpecialVar_0x8004 = SPECIES_WYNAUT;
        break;

    case OBJ_EVENT_GFX_BALTOY_DOLL:
        gSpecialVar_0x8004 = SPECIES_BALTOY;
        break;

    case OBJ_EVENT_GFX_KECLEON_DOLL:
    case OBJ_EVENT_GFX_KECLEON:
    case OBJ_EVENT_GFX_KECLEON_BRIDGE_SHADOW:
        gSpecialVar_0x8004 = SPECIES_KECLEON;
        break;

    case OBJ_EVENT_GFX_AZURILL_DOLL:
        gSpecialVar_0x8004 = SPECIES_AZURILL;
        break;

    case OBJ_EVENT_GFX_SKITTY_DOLL:
    case OBJ_EVENT_GFX_SKITTY:
        gSpecialVar_0x8004 = SPECIES_SKITTY;
        break;

    case OBJ_EVENT_GFX_SWABLU_DOLL:
        gSpecialVar_0x8004 = SPECIES_SWABLU;
        break;

    case OBJ_EVENT_GFX_GULPIN_DOLL:
        gSpecialVar_0x8004 = SPECIES_GULPIN;
        break;

    case OBJ_EVENT_GFX_LOTAD_DOLL:
        gSpecialVar_0x8004 = SPECIES_LOTAD;
        break;

    case OBJ_EVENT_GFX_SEEDOT_DOLL:
        gSpecialVar_0x8004 = SPECIES_SEEDOT;
        break;

    case OBJ_EVENT_GFX_BIG_SNORLAX_DOLL:
        gSpecialVar_0x8004 = SPECIES_SNORLAX;
        break;

    case OBJ_EVENT_GFX_BIG_RHYDON_DOLL:
        gSpecialVar_0x8004 = SPECIES_RHYDON;
        break;

    case OBJ_EVENT_GFX_BIG_LAPRAS_DOLL:
        gSpecialVar_0x8004 = SPECIES_LAPRAS;
        break;

    case OBJ_EVENT_GFX_BIG_VENUSAUR_DOLL:
        gSpecialVar_0x8004 = SPECIES_VENUSAUR;
        break;

    case OBJ_EVENT_GFX_BIG_CHARIZARD_DOLL:
        gSpecialVar_0x8004 = SPECIES_CHARIZARD;
        break;

    case OBJ_EVENT_GFX_BIG_BLASTOISE_DOLL:
        gSpecialVar_0x8004 = SPECIES_BLASTOISE;
        break;

    case OBJ_EVENT_GFX_BIG_WAILMER_DOLL:
        gSpecialVar_0x8004 = SPECIES_WAILMER;
        break;

    case OBJ_EVENT_GFX_BIG_REGIROCK_DOLL:
    case OBJ_EVENT_GFX_REGIROCK:
        gSpecialVar_0x8004 = SPECIES_REGIROCK;
        break;

    case OBJ_EVENT_GFX_BIG_REGICE_DOLL:
    case OBJ_EVENT_GFX_REGICE:
        gSpecialVar_0x8004 = SPECIES_REGICE;
        break;

    case OBJ_EVENT_GFX_BIG_REGISTEEL_DOLL:
    case OBJ_EVENT_GFX_REGISTEEL:
        gSpecialVar_0x8004 = SPECIES_REGISTEEL;
        break;

    case OBJ_EVENT_GFX_LATIAS:
        gSpecialVar_0x8004 = SPECIES_LATIAS;
        break;

    case OBJ_EVENT_GFX_LATIOS:
        gSpecialVar_0x8004 = SPECIES_LATIOS;
        break;

    case OBJ_EVENT_GFX_KYOGRE_FRONT:
    case OBJ_EVENT_GFX_KYOGRE_ASLEEP:
    case OBJ_EVENT_GFX_KYOGRE_SIDE:
        gSpecialVar_0x8004 = SPECIES_KYOGRE;
        break;

    case OBJ_EVENT_GFX_GROUDON_FRONT:
    case OBJ_EVENT_GFX_GROUDON_ASLEEP:
    case OBJ_EVENT_GFX_GROUDON_SIDE:
        gSpecialVar_0x8004 = SPECIES_GROUDON;
        break;

    case OBJ_EVENT_GFX_AZUMARILL:
        gSpecialVar_0x8004 = SPECIES_AZUMARILL;
        break;

    case OBJ_EVENT_GFX_WINGULL:
        gSpecialVar_0x8004 = SPECIES_WINGULL;
        break;

    case OBJ_EVENT_GFX_POOCHYENA:
        gSpecialVar_0x8004 = SPECIES_POOCHYENA;
        break;

    case OBJ_EVENT_GFX_KIRLIA:
        gSpecialVar_0x8004 = SPECIES_KIRLIA;
        break;

    case OBJ_EVENT_GFX_DUSCLOPS:
        gSpecialVar_0x8004 = SPECIES_DUSCLOPS;
        break;

    case OBJ_EVENT_GFX_SUDOWOODO:
        gSpecialVar_0x8004 = SPECIES_SUDOWOODO;
        break;

    case OBJ_EVENT_GFX_MEW:
        gSpecialVar_0x8004 = SPECIES_MEW;
        break;

    case OBJ_EVENT_GFX_DEOXYS:
        gSpecialVar_0x8004 = SPECIES_DEOXYS;
        break;

    case OBJ_EVENT_GFX_LUGIA:
        gSpecialVar_0x8004 = SPECIES_LUGIA;
        break;

    case OBJ_EVENT_GFX_HOOH:
        gSpecialVar_0x8004 = SPECIES_HO_OH;
        break;
    
    default:
        if (IS_OW_MON_OBJ(objEvent))
        {
            gSpecialVar_0x8004 = OW_SPECIES(objEvent);
            gSpecialVar_0x8005 = OW_SHINY(objEvent);
        } 
        else
        {
            gSpecialVar_0x8004 = SPECIES_NONE;
            gSpecialVar_0x8005 = FALSE;
        }
        break;
    }
}

void SantizeOverworldMonLevel(void)
{
    if (VarGet(VAR_OVERWORLD_MON_LEVEL) > 100)
        VarSet(VAR_OVERWORLD_MON_LEVEL, 100);

    if (VarGet(VAR_OVERWORLD_MON_LEVEL) <= 0)
        VarSet(VAR_OVERWORLD_MON_LEVEL, 1);
}

// This Should Replicate Cmd_handleballthrow in battle_script_commands.c
void GetOverworldSpeciesCatchRate(void)
{
    u16 ballMultiplier = 100;
    s8 ballAddition = 0;
    u16 species = gSpecialVar_0x8004;
    u16 level = VarGet(VAR_OVERWORLD_MON_LEVEL);
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
        
    odds = (catchRate * ballMultiplier / 100) * (3 - 2) / (3); // Full HP calculation.
    odds *= OVERWORLD_CATCH_RATE_MULTIPLYER;
    if (gSpecialVar_0x8006 == MUGSHOT_EMOTE_SURPRISED)
        odds *= 2;

    if (odds > 254) // mon caught
    {
        gSpecialVar_Result = TRUE;
    }
    else // mon may be caught, calculate shakes
    {   
        if (IsCatchCritical(odds))
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

// This Should Replicate IsRunningFromBattleImpossible in battle_main.c
// It also combines IsAbilityPreventingEscape and CanBattlerEscape from battle_util.c
u8 OverworldTrappedInBattle(void)
{
    u32 holdEffect;
    u16 heldItem = GetMonData(GetFirstLiveMon(), MON_DATA_HELD_ITEM);
    u16 leadMonSpecies = GetMonData(GetFirstLiveMon(), MON_DATA_SPECIES);
    u16 enemyMonSpecies = gSpecialVar_0x8004;
    u8 leadMonAbilityNum = GetMonData(GetFirstLiveMon(), MON_DATA_ABILITY_NUM);
    u8 i;

    if (heldItem == ITEM_NONE || heldItem == ITEM_ENIGMA_BERRY_E_READER)
        holdEffect = HOLD_EFFECT_NONE;
    else
        holdEffect = GetItemHoldEffect(heldItem);

    if (holdEffect == HOLD_EFFECT_CAN_ALWAYS_RUN || holdEffect == HOLD_EFFECT_SHED_SHELL)
        return BATTLE_RUN_SUCCESS;
    if (B_GHOSTS_ESCAPE >= GEN_6 &&
        (gSpeciesInfo[leadMonSpecies].types[0] == TYPE_GHOST
        || gSpeciesInfo[leadMonSpecies].types[1] == TYPE_GHOST))
        return BATTLE_RUN_SUCCESS;
    if (gSpeciesInfo[leadMonSpecies].abilities[leadMonAbilityNum] == ABILITY_RUN_AWAY)
        return BATTLE_RUN_SUCCESS;

    for (i = 0; i < NUM_ABILITY_SLOTS; i++)
    {
        if (gSpeciesInfo[enemyMonSpecies].abilities[i] == ABILITY_SHADOW_TAG
            && (!(B_SHADOW_TAG_ESCAPE >= GEN_4
            && gSpeciesInfo[leadMonSpecies].abilities[leadMonAbilityNum] == ABILITY_SHADOW_TAG))
            && Random() % NUM_ABILITY_SLOTS == BATTLE_RUN_FAILURE)
            return BATTLE_RUN_FAILURE;
        
        if (gSpeciesInfo[enemyMonSpecies].abilities[i] == ABILITY_ARENA_TRAP
            && (holdEffect != HOLD_EFFECT_AIR_BALLOON
            || holdEffect == HOLD_EFFECT_IRON_BALL
            || (gSpeciesInfo[leadMonSpecies].types[0] != TYPE_FLYING
            && gSpeciesInfo[leadMonSpecies].types[1] != TYPE_FLYING))
            && Random() % NUM_ABILITY_SLOTS == BATTLE_RUN_FAILURE)
            return BATTLE_RUN_FAILURE;

        if (gSpeciesInfo[enemyMonSpecies].abilities[i] == ABILITY_MAGNET_PULL
            && (gSpeciesInfo[leadMonSpecies].types[0] == TYPE_STEEL
            || gSpeciesInfo[leadMonSpecies].types[1] == TYPE_STEEL)
            && Random() % NUM_ABILITY_SLOTS == BATTLE_RUN_FAILURE)
            return BATTLE_RUN_FAILURE;
    }

    if (gSpeciesInfo[leadMonSpecies].baseSpeed >= gSpeciesInfo[enemyMonSpecies].baseSpeed)
        return BATTLE_RUN_SUCCESS;
    else
        return BATTLE_RUN_FAILURE;
}

// Can be updated to include run away function above.
bool8 WillOverworldEncounterRun(void)
{
    // u32 holdEffect;
    u16 leadMonSpecies = GetMonData(GetFirstLiveMon(), MON_DATA_SPECIES);
    u16 enemyMonSpecies = gSpecialVar_0x8004;
    // u16 commonHeldItem = gSpeciesInfo[enemyMonSpecies].itemCommon;
    // u16 rareHeldItem = gSpeciesInfo[enemyMonSpecies].itemRare;
    // u8 leadMonAbilityNum = GetMonData(GetFirstLiveMon(), MON_DATA_ABILITY_NUM);
    // u8 i;

    if (((gSpeciesInfo[leadMonSpecies].baseAttack >= gSpeciesInfo[leadMonSpecies].baseSpAttack
        ? gSpeciesInfo[leadMonSpecies].baseAttack : gSpeciesInfo[leadMonSpecies].baseSpAttack)
        >= (gSpeciesInfo[enemyMonSpecies].baseDefense >= gSpeciesInfo[enemyMonSpecies].baseSpDefense
        ? gSpeciesInfo[enemyMonSpecies].baseDefense : gSpeciesInfo[enemyMonSpecies].baseSpDefense))
        && gSpeciesInfo[enemyMonSpecies].baseSpeed > gSpeciesInfo[enemyMonSpecies].baseHP * 3 / 2
        && !(Random() % RUN_ODDS_DENOMINATOR))
        return TRUE;
    else
        return FALSE;
}

bool8 ScrCmd_SetObjectAsWildEncounter(struct ScriptContext *ctx)
{
    u16 localId = VarGet(ScriptReadHalfword(ctx));
    u8 encounterType = ScriptReadByte(ctx);
    // u32 encounterRate;
    u16 headerId = GetCurrentMapWildMonHeaderId();
    u16 graphicsId = GetObjectEventGraphicsIdByLocalIdAndMap(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    u16 variableOffset = (graphicsId >= OBJ_EVENT_GFX_VAR_0) ? graphicsId - OBJ_EVENT_GFX_VAR_0 : 0;
    u16 objectEventVariable = VAR_OBJ_GFX_ID_0 + variableOffset;

    Script_RequestEffects(SCREFF_V1 | SCREFF_HARDWARE);
    Script_RequestWriteVar(objectEventVariable);

    if (!(graphicsId >= OBJ_EVENT_GFX_VARS
        && graphicsId <= OBJ_EVENT_GFX_LAST))
    {
        return FALSE;
    }

    encounterType = (encounterType < ENCOUNTER_TYPES) ? encounterType : ENCOUNTER_LAND;

    if (headerId == HEADER_NONE || encounterType == ENCOUNTER_FIXED)
    {
        VarSet(objectEventVariable, ReturnFixedSpeciesEncounter());
        return FALSE;
    }

/*
    switch (encounterType)
    {
    case ENCOUNTER_SURF:
        encounterRate = gWildMonHeaders[headerId].waterMonsInfo->encounterRate;
        break;
    
    case ENCOUNTER_LAND:
    default:
        encounterRate = gWildMonHeaders[headerId].landMonsInfo->encounterRate;
        break;
    }
*/

    if (Random() % SPAWN_ODDS_DENOMINATOR) // (WillWildEncounterSpawn(encounterRate, TRUE))
    {
        FlagSet(ReturnObjectEventFlagIdByLocalIdAndMap(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup));
    }
    else
    {
        VarSet(objectEventVariable, ReturnHeaderSpeciesEncounter(encounterType, headerId));
    }

    return FALSE;
}

u16 ReturnFixedSpeciesEncounter(void)
{
    u16 shinyTag = GeneratedOverworldMonShinyRoll() ? OBJ_EVENT_MON_SHINY : 0;
    u16 species = SPECIES_NONE;

    species = SPECIES_PIKACHU;
    
    return species + OBJ_EVENT_GFX_SPECIES(NONE) + shinyTag;
}

u16 ReturnHeaderSpeciesEncounter(u8 encounterType, u16 headerId)
{
    u16 shinyTag = GeneratedOverworldMonShinyRoll() ? OBJ_EVENT_MON_SHINY : 0;
    u16 species = SPECIES_NONE;

    switch (encounterType)
    {
    case ENCOUNTER_LAND:
        species = GetLocalLandMon();
        break;

    case ENCOUNTER_SURF:
        species = GetLocalWaterMon();
        break;

    case ENCOUNTER_ROCK_SMASH:
        species = GetLocalRockSmashMon();
        break;

    case ENCOUNTER_OLD_ROD:
        species = GetLocalFishingMon(OLD_ROD);
        break;

    case ENCOUNTER_GOOD_ROD:
        species = GetLocalFishingMon(GOOD_ROD);
        break;

    case ENCOUNTER_SUPER_ROD:
        species = GetLocalFishingMon(SUPER_ROD);
        break;
    }

    if (species != SPECIES_NONE)
        return species + OBJ_EVENT_GFX_SPECIES(NONE) + shinyTag;
    else
        return ReturnFixedSpeciesEncounter();
}

bool8 GeneratedOverworldMonShinyRoll(void)
{
    u8 shinyRolls = 1;

    if (CheckBagHasItem(ITEM_SHINY_CHARM, 1))
        shinyRolls += I_SHINY_CHARM_ADDITIONAL_ROLLS;
    if (LURE_STEP_COUNT != 0)
        shinyRolls += 1;
    
    while (shinyRolls > 0)
    {
        if (Random() < SHINY_ODDS)
        {
            return TRUE;
        }
        shinyRolls -= 1;
    }
    
    return FALSE;
}
