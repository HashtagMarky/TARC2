#ifndef GUARD_DYNAMIC_MUSIC_H
#define GUARD_DYNAMIC_MUSIC_H

#include "gba/m4a_internal.h"

struct DynamicMusicData
{
    bool8 volumeMax;
    u8 fadeStart;
    u16 instrument:4;
    u16 trackBitsIntrument:12;
    u16 trackBitsBackground:12;
    u16 fadeSpeed:4;
};

void MPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void m4aMPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void UpdateMovementDynamicMusic(void);
void Task_UpdateMovementDynamicMusic(u8 taskId);

extern s16 gMapMusicVolume;

#endif // GUARD_DYNAMIC_MUSIC_H