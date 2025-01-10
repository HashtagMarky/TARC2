#include "global.h"
#include "dynamic_music.h"
#include "constants/dynamic_music.h"
#include "global.fieldmap.h"
#include "m4a.h"
#include "sound.h"
#include "constants/event_objects.h"
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

static u8 GetInstrumentFromMusician(void)
{
    u16 graphicsId = gObjectEvents[gSelectedObjectEvent].graphicsId;

    switch (graphicsId)
    {
    case OBJ_EVENT_GFX_ACCORDIONIST:
        return INSTRUMENT_ACCORDIAN;
        break;
    
    default:
        return INSTRUMENT_COUNT;
        break;
    }
}

void DynamicMusic_RestoreInstrument(void)
{
    u8 instrument = GetInstrumentFromMusician();
    u16 trackBits = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].trackBits;
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, 0x100);
}

void DynamicMusic_RemoveAllInstrument(void)
{
    u8 instrument = INSTRUMENT_ALL;
    u8 volumeMin = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].volumeMin;
    u16 trackBits = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].trackBits;
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, volumeMin);
}

void DynamicMusic_RestoreAllInstrument(void)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
}

void LittlerootOnlyMainMelody(void)
{
    u16 trackBits = (1 << 1) | (1 << 2) | (1 << 4);
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL - trackBits, 0);
}