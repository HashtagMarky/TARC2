#ifndef GUARD_DYNAMIC_MUSIC_H
#define GUARD_DYNAMIC_MUSIC_H

#include "gba/m4a_internal.h"
#include "constants/dynamic_music.h"

struct BackgroundMusicData
{
    bool8 volumeMax;
    u8 fadeStart;
    u16 trackBits:12;
    u16 fadeSpeed:4;
};

struct InstrumentMusicData
{
    u8 volumeMin:4;
    u8 fadeStart;
    u16 trackBits:12;
    u16 fadeSpeed:4;
};

struct DynamicMusicData
{
    struct BackgroundMusicData musicBackground;
    struct InstrumentMusicData musicInstrument[INSTRUMENT_COUNT];
};

void MPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void m4aMPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void UpdateMovementDynamicMusic(void);
void Task_UpdateMovementDynamicMusic(u8 taskId);

extern s16 gMapMusicVolume;

#endif // GUARD_DYNAMIC_MUSIC_H