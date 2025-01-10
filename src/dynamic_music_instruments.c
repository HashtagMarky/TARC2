#include "global.h"
#include "dynamic_music.h"
#include "constants/dynamic_music.h"
#include "m4a.h"
#include "sound.h"
#include "constants/songs.h"

static const struct DynamicMusicData sInstrumentDynamicMusicData[] =
{
    [MUS_LITTLEROOT] =
    {
        .musicInstrument =
        {
            [INSTRUMENT_ACCORDIAN] =
            {
                .trackBits = 0b000011010110,
            },

            [INSTRUMENT_ALL] =
            {
                .trackBits = 0b000011010110,
            }
        }
    },
};

void LittlerootRemoveMainMelody(void)
{
    u8 instrument = INSTRUMENT_ALL;
    u16 trackBits = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].trackBits;
    
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