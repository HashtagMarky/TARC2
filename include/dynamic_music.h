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

// Movement
void MPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void m4aMPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void UpdateMovementDynamicMusic(void);
void Task_UpdateMovementDynamicMusic(u8 taskId);
void Task_UpdateMovementDynamicMusicWait(u8 taskId);

// Instruments
void MPlayFadeOutTrackBitsFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 trackBits, u16 volume);
void m4aMPlayFadeOutTrackBitsFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 trackBits, u16 volume);
void DynamicMusic_RemoveInstrument(u32 instrument);
void DynamicMusic_RemoveAllInstrument(void);
void DynamicMusic_RemoveAllInstrumentNotPlaying(void);
void DynamicMusic_RestoreInstrument(u32 instrument);
void DynamicMusic_RestoreAllInstrument(void);
void DynamicMusic_RestoreAllInstrumentPlaying(void);
void DynamicMusic_PlayOnlyInstrument(u32 instrument);
void DynamicMusic_PlayOnlyAllInstrument(void);
void UNUSED DynamicMusic_PlayOnlyInstrumentNotPlaying(void);
void DynamicMusic_PlayOnlyInstrumentPlaying(void);

extern s16 gMapMusicVolume;

#endif // GUARD_DYNAMIC_MUSIC_H