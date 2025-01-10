#include "global.h"
#include "dynamic_music.h"
#include "constants/dynamic_music.h"
#include "event_data.h"
#include "global.fieldmap.h"
#include "m4a.h"
#include "sound.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
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
                .flagInstrument = FLAG_TEMP_1,
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

void DynamicMusic_RemoveInstrument(u32 instrument)
{
    if (instrument == INSTRUMENT_COUNT)
    {
        return;
    }

    u8 volumeMin = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].volumeMinSixteenth * 16;
    u16 trackBits = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].trackBits;
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, volumeMin);
}

void DynamicMusic_RemoveAllInstrument(void)
{
    u8 instrument = INSTRUMENT_ALL;
    u8 volumeMin = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].volumeMinSixteenth * 16;
    u16 trackBits = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].trackBits;
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, volumeMin);
}

void DynamicMusic_RemoveAllInstrumentNotPlaying(void)
{
    for (u8 instrument = 0; instrument < INSTRUMENT_ALL; instrument++)
    {
        u16 flag = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].flagInstrument;

        if (flag != 0 && !FlagGet(flag))
        {
            DynamicMusic_RemoveInstrument(instrument);
        }
    }
}

void DynamicMusic_RestoreInstrument(u32 instrument)
{
    if (instrument == INSTRUMENT_COUNT)
    {
        return;
    }

    u16 trackBits = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].trackBits;
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, 0x100);
}

void DynamicMusic_RestoreAllInstrument(void)
{
    u8 instrument = INSTRUMENT_ALL;
    u16 trackBits = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].trackBits;
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, trackBits, 0x100);
}

void DynamicMusic_RestoreAllInstrumentNotPlaying(void)
{
    for (u8 instrument = 0; instrument < INSTRUMENT_ALL; instrument++)
    {
        u16 flag = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].flagInstrument;

        if (flag != 0 && FlagGet(flag))
        {
            DynamicMusic_RestoreInstrument(instrument);
        }
    }
}

void DynamicMusic_PlayOnlyInstrument(u32 instrument)
{
    if (instrument == INSTRUMENT_COUNT)
    {
        return;
    }

    u16 trackBits = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].trackBits;
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, ~trackBits, 0);
}

void DynamicMusic_PlayOnlyAllInstrument(void)
{
    u8 instrument = INSTRUMENT_ALL;
    u16 trackBits = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].trackBits;
    
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, ~trackBits, 0);
}

void DynamicMusic_PlayOnlyInstrumentNotPlaying(void)
{
    for (u8 instrument = 0; instrument < INSTRUMENT_ALL; instrument++)
    {
        u16 flag = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].flagInstrument;

        if (flag != 0 && FlagGet(flag))
        {
            DynamicMusic_RestoreInstrument(instrument);
        }
    }
}

void ScrCmd_DynamicMusic_RemoveInstrument(void)
{
    DynamicMusic_RemoveInstrument(
        GetInstrumentFromMusician()
    );
}

void ScrCmd_DynamicMusic_RestoreInstrument(void)
{
    DynamicMusic_RestoreInstrument(
        GetInstrumentFromMusician()
    );
}

void ScrCmd_DynamicMusic_PlayOnlyInstrument(void)
{
    DynamicMusic_PlayOnlyInstrument(
        GetInstrumentFromMusician()
    );
}