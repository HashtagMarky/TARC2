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
    u16 trackBits:12;
    u16 volumeMinSixteenth:4;
    u16 flagInstrument;
};

struct DynamicMusicData
{
    struct BackgroundMusicData musicBackground;
    struct InstrumentMusicData musicInstrument[INSTRUMENT_COUNT];
};

void MPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void m4aMPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void MPlayFadeOutTrackBitsFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 trackBits, u16 volume);
void m4aMPlayFadeOutTrackBitsFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 trackBits, u16 volume);
void UpdateMovementDynamicMusic(void);
void Task_UpdateMovementDynamicMusic(u8 taskId);

extern s16 gMapMusicVolume;

#endif // GUARD_DYNAMIC_MUSIC_H