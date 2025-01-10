#ifndef GUARD_DYNAMIC_MUSIC_H
#define GUARD_DYNAMIC_MUSIC_H

#include "gba/m4a_internal.h"

struct DynamicMusicData
{
    u16 trackBits:12;
    u16 fadeSpeed:4;
    u8 fadeStart;
    bool8 volumeMax;
};

void MPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void m4aMPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void UpdateMovementDynamicMusic(void);
void Task_UpdateMovementDynamicMusic(u8 taskId);

extern s16 gMapMusicVolume;

#endif // GUARD_DYNAMIC_MUSIC_H