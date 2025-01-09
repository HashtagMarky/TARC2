#include "global.h"
#include "gba/m4a_internal.h"
#include "sound.h"
#include "battle.h"
#include "m4a.h"
#include "main.h"
#include "pokemon.h"
#include "constants/cries.h"
#include "constants/songs.h"
#include "task.h"
#include <math.h>

struct Fanfare
{
    u16 songNum;
    u16 duration;
};

EWRAM_DATA struct MusicPlayerInfo* gMPlay_PokemonCry = NULL;
EWRAM_DATA u8 gPokemonCryBGMDuckingCounter = 0;

// BSBob Movement Dynamic Music
#include "movement_dynamic_music.h"
EWRAM_DATA s16 gMapMusicVolume = 0;

static u16 sCurrentMapMusic;
static u16 sNextMapMusic;
static u8 sMapMusicState;
static u8 sMapMusicFadeInSpeed;
static u16 sFanfareCounter;

COMMON_DATA bool8 gDisableMusic = 0;

extern struct ToneData gCryTable[];
extern struct ToneData gCryTable_Reverse[];

extern struct ToneData *const gVoiceLineTable[];

static void Task_Fanfare(u8 taskId);
static void CreateFanfareTask(void);
static void RestoreBGMVolumeAfterPokemonCry(void);
static u16 GetSpeciesCryMultiplier(u16 species);
static u16 GetSpeciesCryMultiplier_Type(u16 species, u16 minMultiplier, u16 maxMultiplier);
static u16 GetSpeciesCryMultiplier_Height(u16 species, u16 minMultiplier, u16 maxMultiplier);
static u16 GetSpeciesCryMultiplier_Weight(u16 species, u16 minMultiplier, u16 maxMultiplier);
static u16 GetSpeciesCryMultiplier_EggGroup(u16 species, u16 minMultiplier, u16 maxMultiplier);

static const struct Fanfare sFanfares[] = {
    [FANFARE_LEVEL_UP]                 = { MUS_LEVEL_UP                ,  80 },
    [FANFARE_HEAL]                     = { MUS_HEAL                    , 160 },
    [FANFARE_OBTAIN_BADGE]             = { MUS_OBTAIN_BADGE            , 340 },
    [FANFARE_OBTAIN_ITEM]              = { MUS_OBTAIN_ITEM             , 160 },
    [FANFARE_EVOLVED]                  = { MUS_EVOLVED                 , 220 },
    [FANFARE_OBTAIN_TMHM]              = { MUS_OBTAIN_TMHM             , 220 },
    [FANFARE_EVOLUTION_INTRO]          = { MUS_EVOLUTION_INTRO         ,  60 },
    [FANFARE_MOVE_DELETED]             = { MUS_MOVE_DELETED            , 180 },
    [FANFARE_OBTAIN_BERRY]             = { MUS_OBTAIN_BERRY            , 120 },
    [FANFARE_AWAKEN_LEGEND]            = { MUS_AWAKEN_LEGEND           , 710 },
    [FANFARE_SLOTS_JACKPOT]            = { MUS_SLOTS_JACKPOT           , 250 },
    [FANFARE_SLOTS_WIN]                = { MUS_SLOTS_WIN               , 150 },
    [FANFARE_TOO_BAD]                  = { MUS_TOO_BAD                 , 160 },
    [FANFARE_RG_JIGGLYPUFF]            = { MUS_RG_JIGGLYPUFF           , 400 },
    [FANFARE_RG_DEX_RATING]            = { MUS_RG_DEX_RATING           , 196 },
    [FANFARE_RG_OBTAIN_KEY_ITEM]       = { MUS_RG_OBTAIN_KEY_ITEM      , 170 },
    [FANFARE_RG_CAUGHT_INTRO]          = { MUS_RG_CAUGHT_INTRO         , 231 },
    [FANFARE_RG_PHOTO]                 = { MUS_RG_PHOTO                ,  90 },
    [FANFARE_RG_POKE_FLUTE]            = { MUS_RG_POKE_FLUTE           , 450 },
    [FANFARE_OBTAIN_B_POINTS]          = { MUS_OBTAIN_B_POINTS         , 313 },
    [FANFARE_REGISTER_MATCH_CALL]      = { MUS_REGISTER_MATCH_CALL     , 135 },
    [FANFARE_OBTAIN_SYMBOL]            = { MUS_OBTAIN_SYMBOL           , 318 },
    [FANFARE_DP_TV_END]                = { MUS_DP_TV_END               , 244 },
    [FANFARE_DP_OBTAIN_ITEM]           = { MUS_DP_OBTAIN_ITEM          , 160 },
    [FANFARE_DP_HEAL]                  = { MUS_DP_HEAL                 , 160 },
    [FANFARE_DP_OBTAIN_KEY_ITEM]       = { MUS_DP_OBTAIN_KEY_ITEM      , 170 },
    [FANFARE_DP_OBTAIN_TMHM]           = { MUS_DP_OBTAIN_TMHM          , 220 },
    [FANFARE_DP_OBTAIN_BADGE]          = { MUS_DP_OBTAIN_BADGE         , 340 },
    [FANFARE_DP_LEVEL_UP]              = { MUS_DP_LEVEL_UP             ,  80 },
    [FANFARE_DP_OBTAIN_BERRY]          = { MUS_DP_OBTAIN_BERRY         , 120 },
    [FANFARE_DP_PARTNER]               = { MUS_DP_LETS_GO_TOGETHER     , 180 },
    [FANFARE_DP_EVOLVED]               = { MUS_DP_EVOLVED              , 252 },
    [FANFARE_DP_POKETCH]               = { MUS_DP_POKETCH              , 120 },
    [FANFARE_DP_MOVE_DELETED]          = { MUS_DP_MOVE_DELETED         , 180 },
    [FANFARE_DP_ACCESSORY]             = { MUS_DP_OBTAIN_ACCESSORY     , 160 },
    [FANFARE_PL_TV_END]                = { MUS_PL_TV_END               , 230 },
    [FANFARE_PL_CLEAR_MINIGAME]        = { MUS_PL_WIN_MINIGAME         , 230 },
    [FANFARE_PL_OBTAIN_ARCADE_POINTS]  = { MUS_PL_OBTAIN_ARCADE_POINTS , 175 },
    [FANFARE_PL_OBTAIN_CASTLE_POINTS]  = { MUS_PL_OBTAIN_CASTLE_POINTS , 200 },
    [FANFARE_PL_OBTAIN_B_POINTS]       = { MUS_PL_OBTAIN_B_POINTS      , 264 },
    [FANFARE_HG_OBTAIN_KEY_ITEM]       = { MUS_HG_OBTAIN_KEY_ITEM      , 170 },
    [FANFARE_HG_LEVEL_UP]              = { MUS_HG_LEVEL_UP             ,  80 },
    [FANFARE_HG_HEAL]                  = { MUS_HG_HEAL                 , 160 },
    [FANFARE_HG_DEX_RATING_1]          = { MUS_HG_DEX_RATING_1         , 200 },
    [FANFARE_HG_DEX_RATING_2]          = { MUS_HG_DEX_RATING_2         , 180 },
    [FANFARE_HG_DEX_RATING_3]          = { MUS_HG_DEX_RATING_3         , 220 },
    [FANFARE_HG_DEX_RATING_4]          = { MUS_HG_DEX_RATING_4         , 210 },
    [FANFARE_HG_DEX_RATING_5]          = { MUS_HG_DEX_RATING_5         , 210 },
    [FANFARE_HG_DEX_RATING_6]          = { MUS_HG_DEX_RATING_6         , 370 },
    [FANFARE_HG_RECEIVE_EGG]           = { MUS_HG_OBTAIN_EGG           , 155 },
    [FANFARE_HG_OBTAIN_ITEM]           = { MUS_HG_OBTAIN_ITEM          , 160 },
    [FANFARE_HG_EVOLVED]               = { MUS_HG_EVOLVED              , 240 },
    [FANFARE_HG_OBTAIN_BADGE]          = { MUS_HG_OBTAIN_BADGE         , 340 },
    [FANFARE_HG_OBTAIN_TMHM]           = { MUS_HG_OBTAIN_TMHM          , 220 },
    [FANFARE_HG_VOLTORB_FLIP_1]        = { MUS_HG_CARD_FLIP            , 195 },
    [FANFARE_HG_VOLTORB_FLIP_2]        = { MUS_HG_CARD_FLIP_GAME_OVER  , 240 },
    [FANFARE_HG_ACCESSORY]             = { MUS_HG_OBTAIN_ACCESSORY     , 160 },
    [FANFARE_HG_REGISTER_POKEGEAR]     = { MUS_HG_POKEGEAR_REGISTERED  , 185 },
    [FANFARE_HG_OBTAIN_BERRY]          = { MUS_HG_OBTAIN_BERRY         , 120 },
    [FANFARE_HG_RECEIVE_POKEMON]       = { MUS_HG_RECEIVE_POKEMON      , 150 },
    [FANFARE_HG_MOVE_DELETED]          = { MUS_HG_MOVE_DELETED         , 180 },
    [FANFARE_HG_THIRD_PLACE]           = { MUS_HG_BUG_CONTEST_3RD_PLACE, 130 },
    [FANFARE_HG_SECOND_PLACE]          = { MUS_HG_BUG_CONTEST_2ND_PLACE, 225 },
    [FANFARE_HG_FIRST_PLACE]           = { MUS_HG_BUG_CONTEST_1ST_PLACE, 250 },
    [FANFARE_HG_POKEATHLON_NEW]        = { MUS_HG_POKEATHLON_READY     , 110 },
    [FANFARE_HG_WINNING_POKEATHLON]    = { MUS_HG_POKEATHLON_1ST_PLACE , 144 },
    [FANFARE_HG_OBTAIN_B_POINTS]       = { MUS_HG_OBTAIN_B_POINTS      , 264 },
    [FANFARE_HG_OBTAIN_ARCADE_POINTS]  = { MUS_HG_OBTAIN_ARCADE_POINTS , 175 },
    [FANFARE_HG_OBTAIN_CASTLE_POINTS]  = { MUS_HG_OBTAIN_CASTLE_POINTS , 200 },
    [FANFARE_HG_CLEAR_MINIGAME]        = { MUS_HG_WIN_MINIGAME         , 230 },
    [FANFARE_HG_PARTNER]               = { MUS_HG_LETS_GO_TOGETHER     , 180 },
};

void InitMapMusic(void)
{
    gDisableMusic = FALSE;
    ResetMapMusic();
}

void MapMusicMain(void)
{
    switch (sMapMusicState)
    {
    case 0:
        break;
    case 1:
        sMapMusicState = 2;
        PlayBGM(sCurrentMapMusic);
        break;
    case 2:
    case 3:
    case 4:
        break;
    case 5:
        if (IsBGMStopped())
        {
            sNextMapMusic = 0;
            sMapMusicState = 0;
        }
        break;
    case 6:
        if (IsBGMStopped() && IsFanfareTaskInactive())
        {
            sCurrentMapMusic = sNextMapMusic;
            sNextMapMusic = 0;
            sMapMusicState = 2;
            PlayBGM(sCurrentMapMusic);
        }
        break;
    case 7:
        if (IsBGMStopped() && IsFanfareTaskInactive())
        {
            FadeInNewBGM(sNextMapMusic, sMapMusicFadeInSpeed);
            sCurrentMapMusic = sNextMapMusic;
            sNextMapMusic = 0;
            sMapMusicState = 2;
            sMapMusicFadeInSpeed = 0;
        }
        break;
    }
}

void ResetMapMusic(void)
{
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 0;
    sMapMusicFadeInSpeed = 0;
}

u16 GetCurrentMapMusic(void)
{
    return sCurrentMapMusic;
}

void PlayNewMapMusic(u16 songNum)
{
    sCurrentMapMusic = songNum;
    sNextMapMusic = 0;
    sMapMusicState = 1;
}

void StopMapMusic(void)
{
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 1;
}

void FadeOutMapMusic(u8 speed)
{
    if (IsNotWaitingForBGMStop())
        FadeOutBGM(speed);
    sCurrentMapMusic = 0;
    sNextMapMusic = 0;
    sMapMusicState = 5;
}

void FadeOutAndPlayNewMapMusic(u16 songNum, u8 speed)
{
    FadeOutMapMusic(speed);
    sCurrentMapMusic = 0;
    sNextMapMusic = songNum;
    sMapMusicState = 6;
}

void FadeOutAndFadeInNewMapMusic(u16 songNum, u8 fadeOutSpeed, u8 fadeInSpeed)
{
    FadeOutMapMusic(fadeOutSpeed);
    sCurrentMapMusic = 0;
    sNextMapMusic = songNum;
    sMapMusicState = 7;
    sMapMusicFadeInSpeed = fadeInSpeed;
}

static void UNUSED FadeInNewMapMusic(u16 songNum, u8 speed)
{
    FadeInNewBGM(songNum, speed);
    sCurrentMapMusic = songNum;
    sNextMapMusic = 0;
    sMapMusicState = 2;
    sMapMusicFadeInSpeed = 0;
}

bool8 IsNotWaitingForBGMStop(void)
{
    if (sMapMusicState == 6)
        return FALSE;
    if (sMapMusicState == 5)
        return FALSE;
    if (sMapMusicState == 7)
        return FALSE;
    return TRUE;
}

void PlayFanfareByFanfareNum(u8 fanfareNum)
{
    u16 songNum;
    m4aMPlayStop(&gMPlayInfo_BGM);
    songNum = sFanfares[fanfareNum].songNum;
    sFanfareCounter = sFanfares[fanfareNum].duration;
    m4aSongNumStart(songNum);
}

bool8 WaitFanfare(bool8 stop)
{
    if (sFanfareCounter)
    {
        sFanfareCounter--;
        return FALSE;
    }
    else
    {
        if (!stop)
            m4aMPlayContinue(&gMPlayInfo_BGM);
        else
            m4aSongNumStart(MUS_DUMMY);

        return TRUE;
    }
}

// Unused
void StopFanfareByFanfareNum(u8 fanfareNum)
{
    m4aSongNumStop(sFanfares[fanfareNum].songNum);
}

void PlayFanfare(u16 songNum)
{
    s32 i;
    for (i = 0; (u32)i < ARRAY_COUNT(sFanfares); i++)
    {
        if (sFanfares[i].songNum == songNum)
        {
            PlayFanfareByFanfareNum(i);
            CreateFanfareTask();
            return;
        }
    }

    // songNum is not in sFanfares
    // Play first fanfare in table instead
    PlayFanfareByFanfareNum(0);
    CreateFanfareTask();
}

bool8 IsFanfareTaskInactive(void)
{
    if (FuncIsActiveTask(Task_Fanfare) == TRUE)
        return FALSE;
    return TRUE;
}

static void Task_Fanfare(u8 taskId)
{
    if (sFanfareCounter)
    {
        sFanfareCounter--;
    }
    else
    {
        m4aMPlayContinue(&gMPlayInfo_BGM);
        DestroyTask(taskId);
    }
}

static void CreateFanfareTask(void)
{
    if (FuncIsActiveTask(Task_Fanfare) != TRUE)
        CreateTask(Task_Fanfare, 80);
}

void FadeInNewBGM(u16 songNum, u8 speed)
{
    if (gDisableMusic)
        songNum = 0;
    if (songNum == MUS_NONE)
        songNum = 0;
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_BGM);
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0);
    m4aSongNumStop(songNum);
    m4aMPlayFadeIn(&gMPlayInfo_BGM, speed);
}

void FadeOutBGMTemporarily(u8 speed)
{
    m4aMPlayFadeOutTemporarily(&gMPlayInfo_BGM, speed);
}

bool8 IsBGMPausedOrStopped(void)
{
    if (gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE)
        return TRUE;
    if (!(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return TRUE;
    return FALSE;
}

void FadeInBGM(u8 speed)
{
    m4aMPlayFadeIn(&gMPlayInfo_BGM, speed);
}

void FadeOutBGM(u8 speed)
{
    // BSBob Movement Dynamic Music
    // m4aMPlayFadeOut(&gMPlayInfo_BGM, speed);
    if (FindTaskIdByFunc(Task_UpdateMovementDynamicMusic) != TASK_NONE)
        m4aMPlayFadeOutFromVol(&gMPlayInfo_BGM, speed, gMapMusicVolume);
    else
        m4aMPlayFadeOut(&gMPlayInfo_BGM, speed);
}

bool8 IsBGMStopped(void)
{
    if (!(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return TRUE;
    return FALSE;
}

void PlayCry_Normal(u16 species, s8 pan)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 85);
    PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, CRY_MODE_NORMAL);
    gPokemonCryBGMDuckingCounter = 2;
    RestoreBGMVolumeAfterPokemonCry();
}

void PlayCry_NormalNoDucking(u16 species, s8 pan, s8 volume, u8 priority)
{
    PlayCryInternal(species, pan, volume, priority, CRY_MODE_NORMAL);
}

// Assuming it's not CRY_MODE_DOUBLES, this is equivalent to PlayCry_Normal except it allows other modes.
void PlayCry_ByMode(u16 species, s8 pan, u8 mode)
{
    if (mode == CRY_MODE_DOUBLES)
    {
        PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
    }
    else
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 85);
        PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
        gPokemonCryBGMDuckingCounter = 2;
        RestoreBGMVolumeAfterPokemonCry();
    }
}

// Used when releasing multiple Pokémon at once in battle.
void PlayCry_ReleaseDouble(u16 species, s8 pan, u8 mode)
{
    if (mode == CRY_MODE_DOUBLES)
    {
        PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
    }
    else
    {
        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 85);
        PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
    }
}

// Duck the BGM but don't restore it. Not present in R/S
void PlayCry_DuckNoRestore(u16 species, s8 pan, u8 mode)
{
    if (mode == CRY_MODE_DOUBLES)
    {
        PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
    }
    else
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 85);
        PlayCryInternal(species, pan, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
        gPokemonCryBGMDuckingCounter = 2;
    }
}

void PlayCry_Script(u16 species, u8 mode)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 85);
    PlayCryInternal(species, 0, CRY_VOLUME, CRY_PRIORITY_NORMAL, mode);
    gPokemonCryBGMDuckingCounter = 2;
    RestoreBGMVolumeAfterPokemonCry();
}

void PlayCryInternal(u16 species, s8 pan, s8 volume, u8 priority, u8 mode)
{
    bool32 reverse;
    u32 release;
    u32 length;
    u32 pitch;
    u32 chorus;

    // Set default values
    // May be overridden depending on mode.
    length = 210;
    reverse = FALSE;
    release = 0;
    pitch = 15360;
    chorus = 0;

    switch (mode)
    {
    case CRY_MODE_NORMAL:
        break;
    case CRY_MODE_DOUBLES:
        length = 20;
        release = 225;
        break;
    case CRY_MODE_ENCOUNTER:
        release = 225;
        pitch = 15600;
        chorus = 20;
        volume = 90;
        break;
    case CRY_MODE_HIGH_PITCH:
        length = 50;
        release = 200;
        pitch = 15800;
        chorus = 20;
        volume = 90;
        break;
    case CRY_MODE_ECHO_START:
        length = 25;
        reverse = TRUE;
        release = 100;
        pitch = 15600;
        chorus = 192;
        volume = 90;
        break;
    case CRY_MODE_FAINT:
        release = 200;
        pitch = 14440;
        break;
    case CRY_MODE_ECHO_END:
        release = 220;
        pitch = 15555;
        chorus = 192;
        volume = 70;
        break;
    case CRY_MODE_ROAR_1:
        length = 10;
        release = 100;
        pitch = 14848;
        break;
    case CRY_MODE_ROAR_2:
        length = 60;
        release = 225;
        pitch = 15616;
        break;
    case CRY_MODE_GROWL_1:
        length = 15;
        reverse = TRUE;
        release = 125;
        pitch = 15200;
        break;
    case CRY_MODE_GROWL_2:
        length = 100;
        release = 225;
        pitch = 15200;
        break;
    case CRY_MODE_WEAK_DOUBLES:
        length = 20;
        release = 225;
        // fallthrough
    case CRY_MODE_WEAK:
        pitch = 15000;
        break;
    case CRY_MODE_RHH_INTRO:
        break;
    }

    SetPokemonCryVolume(volume);
    SetPokemonCryPanpot(pan);
    SetPokemonCryPitch(pitch);
    SetPokemonCryLength(length);
    SetPokemonCryProgress(0);
    SetPokemonCryRelease(release);
    SetPokemonCryChorus(chorus);
    SetPokemonCryPriority(priority);

    #if (P_CRIES_GENERIC && P_CRIES_ENABLED == FALSE)
    if (mode == CRY_MODE_RHH_INTRO)
    {
        gMPlay_PokemonCry = SetPokemonCryTone(reverse ? &gCryTable_Reverse[CRY_RHH_INTRO - 1] : &gCryTable[CRY_RHH_INTRO - 1]);
        return;
    }
    
    SetPokemonCryVolume((volume * GetSpeciesCryMultiplier(species)) / 1000);
    SetPokemonCryPanpot((pan * GetSpeciesCryMultiplier(species)) / 1000);
    SetPokemonCryPitch((pitch * GetSpeciesCryMultiplier(species)) / 1000);
    SetPokemonCryLength((length * GetSpeciesCryMultiplier(species)) / 1000);
    SetPokemonCryRelease((release * GetSpeciesCryMultiplier(species)) / 1000);
    SetPokemonCryChorus((chorus * GetSpeciesCryMultiplier(species)) / 1000);
    SetPokemonCryPriority((priority * GetSpeciesCryMultiplier(species)) / 1000);
    #endif //(P_CRIES_GENERIC && P_CRIES_ENABLED == FALSE)

    species = GetCryIdBySpecies(species);
    if (species != CRY_NONE)
    {
        species--;
        gMPlay_PokemonCry = SetPokemonCryTone(reverse ? &gCryTable_Reverse[species] : &gCryTable[species]);
    }
}

bool8 IsCryFinished(void)
{
    if (FuncIsActiveTask(Task_DuckBGMForPokemonCry) == TRUE)
    {
        return FALSE;
    }
    else
    {
        ClearPokemonCrySongs();
        return TRUE;
    }
}

void StopCryAndClearCrySongs(void)
{
    m4aMPlayStop(gMPlay_PokemonCry);
    ClearPokemonCrySongs();
}

void StopCry(void)
{
    m4aMPlayStop(gMPlay_PokemonCry);
}

bool8 IsCryPlayingOrClearCrySongs(void)
{
    if (IsPokemonCryPlaying(gMPlay_PokemonCry))
    {
        return TRUE;
    }
    else
    {
        ClearPokemonCrySongs();
        return FALSE;
    }
}

bool8 IsCryPlaying(void)
{
    if (IsPokemonCryPlaying(gMPlay_PokemonCry))
        return TRUE;
    else
        return FALSE;
}

void Task_DuckBGMForPokemonCry(u8 taskId)
{
    if (gPokemonCryBGMDuckingCounter)
    {
        gPokemonCryBGMDuckingCounter--;
        return;
    }

    if (!IsPokemonCryPlaying(gMPlay_PokemonCry))
    {
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 256);
        DestroyTask(taskId);
    }
}

static void RestoreBGMVolumeAfterPokemonCry(void)
{
    if (FuncIsActiveTask(Task_DuckBGMForPokemonCry) != TRUE)
        CreateTask(Task_DuckBGMForPokemonCry, 80);
}

void PlayBGM(u16 songNum)
{
    if (gDisableMusic)
        songNum = 0;
    if (songNum == MUS_NONE)
        songNum = 0;
    m4aSongNumStart(songNum);
}

void PlaySE(u16 songNum)
{
    m4aSongNumStart(songNum);
}

void PlaySE12WithPanning(u16 songNum, s8 pan)
{
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_SE1);
    m4aMPlayImmInit(&gMPlayInfo_SE2);
    m4aMPlayPanpotControl(&gMPlayInfo_SE1, TRACKS_ALL, pan);
    m4aMPlayPanpotControl(&gMPlayInfo_SE2, TRACKS_ALL, pan);
}

void PlaySE1WithPanning(u16 songNum, s8 pan)
{
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_SE1);
    m4aMPlayPanpotControl(&gMPlayInfo_SE1, TRACKS_ALL, pan);
}

void PlaySE2WithPanning(u16 songNum, s8 pan)
{
    m4aSongNumStart(songNum);
    m4aMPlayImmInit(&gMPlayInfo_SE2);
    m4aMPlayPanpotControl(&gMPlayInfo_SE2, TRACKS_ALL, pan);
}

void SE12PanpotControl(s8 pan)
{
    m4aMPlayPanpotControl(&gMPlayInfo_SE1, TRACKS_ALL, pan);
    m4aMPlayPanpotControl(&gMPlayInfo_SE2, TRACKS_ALL, pan);
}

bool8 IsSEPlaying(void)
{
    if ((gMPlayInfo_SE1.status & MUSICPLAYER_STATUS_PAUSE) && (gMPlayInfo_SE2.status & MUSICPLAYER_STATUS_PAUSE))
        return FALSE;
    if (!(gMPlayInfo_SE1.status & MUSICPLAYER_STATUS_TRACK) && !(gMPlayInfo_SE2.status & MUSICPLAYER_STATUS_TRACK))
        return FALSE;
    return TRUE;
}

bool8 IsBGMPlaying(void)
{
    if (gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE)
        return FALSE;
    if (!(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK))
        return FALSE;
    return TRUE;
}

bool8 IsSpecialSEPlaying(void)
{
    if (gMPlayInfo_SE3.status & MUSICPLAYER_STATUS_PAUSE)
        return FALSE;
    if (!(gMPlayInfo_SE3.status & MUSICPLAYER_STATUS_TRACK))
        return FALSE;
    return TRUE;
}

void PlayVoiceLine(u16 voice, u8 voiceLine, u8 voiceVolume, u16 bgVolume)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, bgVolume);

    SetPokemonCryVolume(voiceVolume);
    SetPokemonCryPanpot(0);
    SetPokemonCryPitch(15360);
    SetPokemonCryLength(500);
    SetPokemonCryProgress(0);
    SetPokemonCryRelease(0);
    SetPokemonCryChorus(0);
    SetPokemonCryPriority(CRY_PRIORITY_NORMAL);

    gMPlay_PokemonCry = SetPokemonCryTone(&gVoiceLineTable[voice][voiceLine]);

    gPokemonCryBGMDuckingCounter = 2;
    RestoreBGMVolumeAfterPokemonCry();
}

static u16 GetSpeciesCryMultiplier(u16 species)
{
    // Min and Max sclaed multipliers, corresponding to a maximum of a 15% variation.
    u16 minMultiplier = 850;  // Corresponds to 0.85x or 85%
    u16 maxMultiplier = 1150; // Corresponds to 1.15x or 115%

    u16 effectType = GetSpeciesCryMultiplier_Type(species, minMultiplier, maxMultiplier);
    u16 effectHeight = GetSpeciesCryMultiplier_Height(species, minMultiplier, maxMultiplier);
    u16 effectWeight = 2000 - GetSpeciesCryMultiplier_Weight(species, minMultiplier, maxMultiplier);
    u16 effectEggGroup = GetSpeciesCryMultiplier_EggGroup(species, minMultiplier, maxMultiplier);

    // Average of effects (still in fixed-point)
    u16 multiplier = (effectHeight * 4 / 16) + (effectWeight * 8 / 16) + (effectType * 2 / 16) + (effectEggGroup * 2 / 16);

    // Capping of effects
    multiplier = (multiplier < minMultiplier) ? minMultiplier : ((multiplier > maxMultiplier) ? maxMultiplier : multiplier);

    // DebugPrintf("\n%S:\nType Multiplier: %d\nHeight Multiplier: %d\nWeight Multiplier: %d\nEgg Group Multiplier: %d\nTotal Multiplier: %d", gSpeciesInfo[species].speciesName, effectType, effectHeight, effectWeight, effectEggGroup, multiplier);
    return multiplier;
}

static u16 GetSpeciesCryMultiplier_Type(u16 species, u16 minMultiplier, u16 maxMultiplier)
{
    /////////////////////////////  Formula  /////////////////////////////
    //                                                                 //
    //      (max - min)                         (max - min)            //
    //    --------------- * (type1 + type2) + --------------- + min    //
    //         2 * 21                              2 * 21              //
    //                                                                 //
    /////////////////////////////////////////////////////////////////////

    u16 multiplier = (((maxMultiplier - minMultiplier) / (2 * NUMBER_OF_MON_TYPES))
                * gSpeciesInfo[species].types[0] + gSpeciesInfo[species].types[1])
                + ((maxMultiplier - minMultiplier) / (2 * NUMBER_OF_MON_TYPES))
                + minMultiplier;
    
    return multiplier;
}

// Functions to approximate the square root of a number using the Newton-Raphson method
float ApproximateSquareRoot(float x)
{
    if (x <= 0) return 0; // Ensure no negative inputs or zero, returns 0 for edge cases

    float guess = x / 2.0; // Initial guess
    for (int i = 0; i < 5; i++) // Iterate a few times for better approximation
    {
        guess = (guess + x / guess) / 2.0;
    }
    return guess;
}

static u16 GetSpeciesCryMultiplier_Height(u16 species, u16 minMultiplier, u16 maxMultiplier)
{
    u16 minHeight = GetSpeciesHeight(SPECIES_JOLTIK);
    u16 maxHeight = GetSpeciesHeight(SPECIES_ETERNATUS);
    u16 speciesHeight = GetSpeciesHeight(species);

    // Normalise and apply square root approximation for scaling
    float normalisedHeight = (float)(speciesHeight - minHeight) / (maxHeight - minHeight);
    float adjustedHeight = ApproximateSquareRoot(normalisedHeight);

    // Clamp to ensure values between 0 and 1
    if (adjustedHeight < 0) adjustedHeight = 0;
    if (adjustedHeight > 1) adjustedHeight = 1;

    // Map to multiplier range
    u16 multiplier = minMultiplier + (u16)((maxMultiplier - minMultiplier) * adjustedHeight);

    return multiplier;
}

static u16 GetSpeciesCryMultiplier_Weight(u16 species, u16 minMultiplier, u16 maxMultiplier)
{
    u16 minWeight = GetSpeciesWeight(SPECIES_GASTLY);
    u16 maxWeight = GetSpeciesWeight(SPECIES_COSMOEM);
    u16 speciesWeight = GetSpeciesWeight(species);

    // Normalise and apply square root approximation for scaling
    float normalisedWeight = (float)(speciesWeight - minWeight) / (maxWeight - minWeight);
    float adjustedWeight = ApproximateSquareRoot(normalisedWeight);

    // Clamp to ensure values between 0 and 1
    if (adjustedWeight < 0) adjustedWeight = 0;
    if (adjustedWeight > 1) adjustedWeight = 1;

    // Map to multiplier range
    u16 multiplier = minMultiplier + (u16)((maxMultiplier - minMultiplier) * adjustedWeight);

    return multiplier;
}

static u16 GetSpeciesCryMultiplier_EggGroup(u16 species, u16 minMultiplier, u16 maxMultiplier)
{
    ////////////////////////////  Formula  ////////////////////////////
    //                                                               //
    //      (max - min)                       (max - min)            //
    //    --------------- * (egg1 + egg2) + --------------- + min    //
    //         2 * 16                            2 * 16              //
    //                                                               //
    ///////////////////////////////////////////////////////////////////

    u16 multiplier = (((maxMultiplier - minMultiplier) / (2 * (EGG_GROUP_NO_EGGS_DISCOVERED + 1)))
                * gSpeciesInfo[species].eggGroups[0] + gSpeciesInfo[species].eggGroups[1])
                + ((maxMultiplier - minMultiplier) / (2 * (EGG_GROUP_NO_EGGS_DISCOVERED + 1)))
                + minMultiplier;
    
    return multiplier;
}