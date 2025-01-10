// Kurausukun's Movement Dynamic Music.
// Minor edits made by HashtagMarky.

// Full credit must go to Kurausukun for this feature, allowing for music to change
// dynamically as the player character moves. Minor changes made include adding
// a start timer for music fading, control of whether the music starts at maximum
// or minimum volume, smoother tranitioning between maps of the same music.
// Again, the original work is NOT my own, but be found here:
// https://github.com/Kurausukun/pokeemerald/tree/dyn_mus


#include "global.h"
#include "dynamic_music.h"
#include "constants/dynamic_music.h"
#include "field_effect.h"
#include "global.fieldmap.h"
#include "m4a.h"
#include "overworld.h"
#include "sound.h"
#include "task.h"
#include "constants/field_effects.h"
#include "constants/songs.h"

static const struct DynamicMusicData sMovementDynamicMusicData[] =
{
    [MUS_LITTLEROOT] =
    {
        .musicBackground =
        {
            .volumeMax = FALSE,
            .fadeStart = 0,
            .trackBits = 0b000110111001,
            .fadeSpeed = 7,
        },
    },
};

static void Task_UpdateMovementDynamicMusicWait(u8 taskId);

void MPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume)
{
    if (mplayInfo->ident == ID_NUMBER)
    {
        mplayInfo->ident++;
        mplayInfo->fadeOC = speed;
        mplayInfo->fadeOI = speed;
        mplayInfo->fadeOV = ((volume / 4) << FADE_VOL_SHIFT);
        mplayInfo->ident = ID_NUMBER;
    }
}

void m4aMPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume)
{
    MPlayFadeOutFromVol(mplayInfo, speed, volume);
}

#define tOrigMapId data[0]
#define tWaitForFly data[1]
#define tStartMaxVolume data[2]
#define tFadeStartTimer data[3]
void UpdateMovementDynamicMusic(void)
{
    u8 taskId;
    if (FindTaskIdByFunc(Task_UpdateMovementDynamicMusicWait) != TASK_NONE || FindTaskIdByFunc(Task_UpdateMovementDynamicMusic) != TASK_NONE)
        return;
    taskId = CreateTask(Task_UpdateMovementDynamicMusicWait, 64);
    
    if (FieldEffectActiveListContains(FLDEFF_FLY_IN) || FieldEffectActiveListContains(FLDEFF_USE_FLY))
        gTasks[taskId].tWaitForFly = TRUE;
    else
        gTasks[taskId].tWaitForFly = FALSE;
        
    
    gMapMusicVolume = 0;
    gTasks[taskId].tOrigMapId = (gSaveBlock1Ptr->location.mapGroup << 8) | (gSaveBlock1Ptr->location.mapNum);
    gTasks[taskId].tStartMaxVolume = FALSE;
    gTasks[taskId].tFadeStartTimer = 0;
}

void Task_UpdateMovementDynamicMusic(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u16 currentMapId = (gSaveBlock1Ptr->location.mapGroup << 8) | (gSaveBlock1Ptr->location.mapNum);
    u16 currentMusic = GetCurrentMapMusic();
    u16 trackBits = sMovementDynamicMusicData[GetCurrentMapMusic()].musicBackground.trackBits;
    u16 fadeSpeed = sMovementDynamicMusicData[GetCurrentMapMusic()].musicBackground.fadeSpeed + 1;
    u16 fadeStart = sMovementDynamicMusicData[GetCurrentMapMusic()].musicBackground.fadeStart;
    bool8 volumeMax = sMovementDynamicMusicData[GetCurrentMapMusic()].musicBackground.volumeMax;

    if (task->tFadeStartTimer < fadeStart)
    {
        task->tFadeStartTimer++;
        return;
    }

    if (volumeMax == TRUE && task->tStartMaxVolume == FALSE)
    {
        gMapMusicVolume = 256;
        task->tStartMaxVolume = TRUE;
    }
    
    if (currentMapId != task->tOrigMapId
        && GetCurrLocationDefaultMusic() != currentMusic)
    {
        m4aMPlayFadeOutFromVol(&gMPlayInfo_BGM, 8, gMapMusicVolume);
        DestroyTask(taskId);
        return;
    }
    if (gPlayerAvatar.runningState == NOT_MOVING)
    {
        gMapMusicVolume -= fadeSpeed;
        if (gMapMusicVolume <= 0)
            gMapMusicVolume = 0;
    }
    else if (gPlayerAvatar.runningState == MOVING)
    {
        gMapMusicVolume += fadeSpeed;
        if (gMapMusicVolume >= 256)
            gMapMusicVolume = 256;
    }
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, (u16)gMapMusicVolume);
}

static void Task_UpdateMovementDynamicMusicWait(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u16 trackBits = sMovementDynamicMusicData[GetCurrentMapMusic()].musicBackground.trackBits;
    switch (task->tWaitForFly)
    {
    case TRUE:
        if (!FieldEffectActiveListContains(FLDEFF_FLY_IN))
        {
            task->func = Task_UpdateMovementDynamicMusic;
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, 0);
        }
        break;
    case FALSE:
        if (BGMusicStopped())
            task->func = Task_UpdateMovementDynamicMusic;
        break;
    }
}
#undef tOrigMapId
#undef tWaitForFly
#undef tStartMaxVolume
#undef tFadeStartTimer
