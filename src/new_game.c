#include "global.h"
#include "new_game.h"
#include "random.h"
#include "pokemon.h"
#include "roamer.h"
#include "pokemon_size_record.h"
#include "script.h"
#include "lottery_corner.h"
#include "play_time.h"
#include "mauville_old_man.h"
#include "match_call.h"
#include "lilycove_lady.h"
#include "load_save.h"
#include "pokeblock.h"
#include "dewford_trend.h"
#include "berry.h"
#include "rtc.h"
#include "easy_chat.h"
#include "event_data.h"
#include "money.h"
#include "trainer_hill.h"
#include "tv.h"
#include "coins.h"
#include "text.h"
#include "overworld.h"
#include "mail.h"
#include "battle_records.h"
#include "item.h"
#include "pokedex.h"
#include "apprentice.h"
#include "frontier_util.h"
#include "pokedex.h"
#include "save.h"
#include "link_rfu.h"
#include "main.h"
#include "contest.h"
#include "item_menu.h"
#include "pokemon_storage_system.h"
#include "pokemon_jump.h"
#include "decoration_inventory.h"
#include "secret_base.h"
#include "player_pc.h"
#include "field_specials.h"
#include "berry_powder.h"
#include "mystery_gift.h"
#include "union_room_chat.h"
#include "constants/battle.h"
#include "constants/field_mugshots.h"
#include "constants/map_groups.h"
#include "constants/items.h"
#include "ikigai_characters.h"
#include "speedup.h"
#include "difficulty.h"
#include "follower_npc.h"
#include "fake_rtc.h"
#include "clock.h"

extern const u8 EventScript_ResetAllMapFlags[];
extern const u8 EventScript_ResetIkigaiMapFlags[];

static void ClearFrontierRecord(void);
static void UNUSED WarpToTruck(void);
static void ResetMiniGamesRecords(void);
static void ResetItemFlags(void);
static void ResetDexNav(void);

static void UNUSED WarpToShip(void);
static void WarpToEchofern(void);

EWRAM_DATA bool8 gDifferentSaveFile = FALSE;
EWRAM_DATA bool8 gEnableContestDebugging = FALSE;

static const struct ContestWinner sContestWinnerPicDummy =
{
    .monName = _(""),
    .trainerName = _("")
};

void SetTrainerId(u32 trainerId, u8 *dst)
{
    dst[0] = trainerId;
    dst[1] = trainerId >> 8;
    dst[2] = trainerId >> 16;
    dst[3] = trainerId >> 24;
}

u32 GetTrainerId(u8 *trainerId)
{
    return (trainerId[3] << 24) | (trainerId[2] << 16) | (trainerId[1] << 8) | (trainerId[0]);
}

void CopyTrainerId(u8 *dst, u8 *src)
{
    s32 i;
    for (i = 0; i < TRAINER_ID_LENGTH; i++)
        dst[i] = src[i];
}

static void InitPlayerTrainerId(void)
{
    u32 trainerId = (Random() << 16) | GetGeneratedTrainerIdLower();
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
}

// L=A isnt set here for some reason.
static void SetDefaultOptions(void)
{
    gSaveBlock2Ptr->playerEmote = MUGSHOT_EMOTE_HAPPY;
    gSaveBlock2Ptr->ikigaiGymType = TYPE_NONE;
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_FASTER;
    gSaveBlock2Ptr->optionsTitleScreenRandomise = TRUE;
    gSaveBlock2Ptr->optionsSound = OPTIONS_SOUND_MONO;
    gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SHIFT;
    gSaveBlock2Ptr->optionsBattleScene = OPTIONS_BATTLE_SCENE_FULL_ANIMATION;
    gSaveBlock2Ptr->optionsUnitSystem = UNITS_METRIC;  //tx_optionsPlus
    gSaveBlock2Ptr->optionsDisableMatchCall = FALSE;    //tx_optionsPlus
    gSaveBlock2Ptr->optionsCurrentFont = 0;         //tx_optionsPlus
    gSaveBlock2Ptr->optionsSuppressNPCMugshots = FALSE;
    gSaveBlock2Ptr->optionsFollowerMugshots = MUGSHOT_FOLLOWER_ON;
    gSaveBlock2Ptr->optionsOverworldCatchSuccessMultiplyer = FALSE;
    gSaveBlock2Ptr->optionsBikeMusic = FALSE;
    gSaveBlock2Ptr->optionsSurfMusic = FALSE;
    gSaveBlock2Ptr->optionsOverworldSpeed = OPTIONS_SPEEDUP_NORMAL;
    gSaveBlock2Ptr->optionsNPCName = TRUE;
    gSaveBlock2Ptr->optionsDisableAutoSave = DEV_BUILD ? TRUE : FALSE;
    gSaveBlock2Ptr->optionsWildBattleSpeed = OPTIONS_SPEEDUP_NORMAL;
    gSaveBlock2Ptr->optionsTrainerBattleSpeed = OPTIONS_SPEEDUP_NORMAL;
    gSaveBlock2Ptr->optionsClockMode = TRUE;
    gSaveBlock2Ptr->optionsDamageNumbers = FALSE;
    gSaveBlock2Ptr->regionMapZoom = FALSE;
    gSaveBlock3Ptr->autoRun = FALSE;
    gSaveBlock3Ptr->autoBike = FALSE;
    gSaveBlock3Ptr->fastBike = FALSE;
    gSaveBlock3Ptr->fastSurf = FALSE;
}

void ClearPokedexFlags(void)
{
    gUnusedPokedexU8 = 0;
    memset(&gSaveBlock1Ptr->dexCaught, 0, sizeof(gSaveBlock1Ptr->dexCaught));
    memset(&gSaveBlock1Ptr->dexSeen, 0, sizeof(gSaveBlock1Ptr->dexSeen));
}

void ClearAllContestWinnerPics(void)
{
    s32 i;

    ClearContestWinnerPicsInContestHall();

    // Clear Museum paintings
    for (i = MUSEUM_CONTEST_WINNERS_START; i < NUM_CONTEST_WINNERS; i++)
        gSaveBlock1Ptr->contestWinners[i] = sContestWinnerPicDummy;
}

static void ClearFrontierRecord(void)
{
    CpuFill32(0, &gSaveBlock2Ptr->frontier, sizeof(gSaveBlock2Ptr->frontier));

    gSaveBlock2Ptr->frontier.opponentNames[0][0] = EOS;
    gSaveBlock2Ptr->frontier.opponentNames[1][0] = EOS;
}

static void UNUSED WarpToTruck(void)
{
    SetWarpDestination(MAP_GROUP(MAP_INSIDE_OF_TRUCK), MAP_NUM(MAP_INSIDE_OF_TRUCK), WARP_ID_NONE, -1, -1);
    WarpIntoMap();
}

static void UNUSED WarpToShip(void)
{
    FakeRtc_ManuallySetTime(0, Ikigai_GetSeasonalTimeHour(0, TIME_EVENING, TRUE), 0, 0);
    RtcInitLocalTimeOffset(Ikigai_GetSeasonalTimeHour(0, TIME_EVENING, TRUE), 0);
    InitTimeBasedEvents();
    FlagSet(FLAG_PAUSE_TIME);
    FlagSet(FLAG_HIDE_MAP_NAME_POPUP);
    FlagSet(FLAG_SUPPRESS_MUGSHOT);
    FlagSet(FLAG_SUPPRESS_FOLLOWER);
    SetWarpDestination(MAP_GROUP(MAP_VYRATON_OUTDOORS_OCEAN), MAP_NUM(MAP_VYRATON_OUTDOORS_OCEAN), WARP_ID_NONE, 4, 2);
    WarpIntoMap();
}

static void WarpToEchofern(void)
{
    FakeRtc_ManuallySetTime(0, 18, 0, 0);
    RtcInitLocalTimeOffset(18, 0);
    InitTimeBasedEvents();
    FlagSet(FLAG_PAUSE_TIME);
    FlagSet(FLAG_HIDE_MAP_NAME_POPUP);
    // FlagSet(FLAG_SUPPRESS_MUGSHOT);
    FlagSet(FLAG_SUPPRESS_FOLLOWER);
    FlagSet(FLAG_DONT_TRANSITION_MUSIC);
    // SetWarpDestination(MAP_GROUP(MAP_PLAYER_HOUSE), MAP_NUM(MAP_PLAYER_HOUSE), WARP_ID_NONE, 4, 3);
    SetWarpDestination(MAP_GROUP(MAP_MINDSCAPE), MAP_NUM(MAP_MINDSCAPE), WARP_ID_NONE, 25, 25);
    WarpIntoMap();
}

void Sav2_ClearSetDefault(void)
{
    ClearSav2();
    SetDefaultOptions();
}

void ResetMenuAndMonGlobals(void)
{
    gDifferentSaveFile = FALSE;
    ResetPokedexScrollPositions();
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetBagScrollPositions();
    ResetPokeblockScrollPositions();
}

void NewGameInitData(void)
{
    if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_CORRUPT)
        RtcReset();

    gDifferentSaveFile = TRUE;
    gSaveBlock2Ptr->encryptionKey = 0;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetPokedex();
    ClearFrontierRecord();
    ClearSav1();
    ClearSav3();
    SetDefaultOptions();
    ClearAllMail();
    gSaveBlock2Ptr->specialSaveWarpFlags = 0;
    gSaveBlock2Ptr->gcnLinkFlags = 0;
    InitPlayerTrainerId();
    PlayTimeCounter_Reset();
    ClearPokedexFlags();
    InitEventData();
    ClearTVShowData();
    ResetGabbyAndTy();
    ClearSecretBases();
    ClearBerryTrees();
    SetMoney(&gSaveBlock1Ptr->money, 3000);
    SetCoins(0);
    ResetLinkContestBoolean();
    ResetGameStats();
    ClearAllContestWinnerPics();
    ClearPlayerLinkBattleRecords();
    InitSeedotSizeRecord();
    InitLotadSizeRecord();
    gPlayerPartyCount = 0;
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    DeactivateAllRoamers();
    gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    ClearBag();
    NewGameInitPCItems();
    ClearPokeblocks();
    ClearDecorationInventories();
    InitEasyChatPhrases();
    SetMauvilleOldMan();
    InitDewfordTrend();
    ResetFanClub();
    ResetLotteryCorner();
    // WarpToTruck();
    // WarpToShip();
    WarpToEchofern();
    RunScriptImmediately(EventScript_ResetAllMapFlags);
    RunScriptImmediately(EventScript_ResetIkigaiMapFlags);
    ResetMiniGamesRecords();
    InitUnionRoomChatRegisteredTexts();
    InitLilycoveLady();
    ResetAllApprenticeData();
    ClearRankingHallRecords();
    InitMatchCallCounters();
    ClearMysteryGift();
    WipeTrainerNameRecords();
    ResetTrainerHillResults();
    ResetContestLinkResults();
    SetCurrentDifficultyLevel(DIFFICULTY_NORMAL);
    ResetItemFlags();
    IkigaiCharacter_SetAllCharacterDefaultOpinion();
    SetDefaultPlayerNickname();
    ResetDexNav();
    ClearFollowerNPCData();
}

static void ResetMiniGamesRecords(void)
{
    CpuFill16(0, &gSaveBlock2Ptr->berryCrush, sizeof(struct BerryCrush));
    SetBerryPowder(&gSaveBlock2Ptr->berryCrush.berryPowderAmount, 0);
    ResetPokemonJumpRecords();
    CpuFill16(0, &gSaveBlock2Ptr->berryPick, sizeof(struct BerryPickingResults));
}

static void ResetItemFlags(void)
{
#if OW_SHOW_ITEM_DESCRIPTIONS == OW_ITEM_DESCRIPTIONS_FIRST_TIME
    memset(&gSaveBlock3Ptr->itemFlags, 0, sizeof(gSaveBlock3Ptr->itemFlags));
#endif
}

static void ResetDexNav(void)
{
#if USE_DEXNAV_SEARCH_LEVELS == TRUE
    memset(gSaveBlock3Ptr->dexNavSearchLevels, 0, sizeof(gSaveBlock3Ptr->dexNavSearchLevels));
#endif
    gSaveBlock3Ptr->dexNavChain = 0;
}
