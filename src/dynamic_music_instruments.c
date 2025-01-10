#include "global.h"
#include "dynamic_music.h"
#include "constants/dynamic_music.h"
#include "m4a.h"
#include "sound.h"

void LittlerootRemoveMainMelody(void)
{
    u16 trackBits = (1 << 1) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 7);
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, 0);
}

void LittlerootRestoreMainMelody(void)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
}

void LittlerootOnlyMainMelody(void)
{
    u16 trackBits = (1 << 1) | (1 << 2) | (1 << 4);
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL - trackBits, 0);
}