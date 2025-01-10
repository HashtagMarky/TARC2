#ifndef GUARD_MOVEMENT_DYNAMIC_MUSIC_H
#define GUARD_MOVEMENT_DYNAMIC_MUSIC_H


// Kurausukun's Movement Dynamic Music.
// Minor edits made by HashtagMarky.

// Full credit must go to Kurausukun for this feature, allowing for music to change
// dynamically as the player character moves. Minor changes made include adding
// a start timer for music fading, control of whether the music starts at maximum
// or minimum volume, smoother tranitioning between maps of the same music.
// Again, the original work is NOT my own, but be found here:
// https://github.com/Kurausukun/pokeemerald/tree/dyn_mus


#include "gba/m4a_internal.h"

void MPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void m4aMPlayFadeOutFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 volume);
void UpdateMovementDynamicMusic(void);
void Task_UpdateMovementDynamicMusic(u8 taskId);

extern s16 gMapMusicVolume;

#endif // GUARD_MOVEMENT_DYNAMIC_MUSIC_H