#ifndef GUARD_MOVEMENT_DYNAMIC_MUSIC_H
#define GUARD_MOVEMENT_DYNAMIC_MUSIC_H


// BSBob's Movement Dynamic Music.
// Minor edits made by HashtagMarky.

// Full credit must go to BSBob for this feature, allowing for music to change
// dynamically as the player character moves. Minor changes made include adding
// a start timer for music fading, control of whether the music starts at maximum
// or minimum volume, smoother tranitioning between maps of the same music.
// Again, the original work is NOT my own, but be found here:
// https://github.com/nielsmittertreiner/pokeemerald/commit/32285d055049019c4077cce7d0050270f32a546f#diff-16bfd88010208f0d23a0d16061b6cde26b2b2757ac649d62331b993e12827aecR17


#include "gba/m4a_internal.h"

void MPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void m4aMPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void UpdateMovementDynamicMusic(void);
void Task_UpdateMovementDynamicMusic(u8 taskId);

extern s16 gMapMusicVolume;

#endif // GUARD_MOVEMENT_DYNAMIC_MUSIC_H