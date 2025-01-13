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
            [INSTRUMENT_ACCORDION] =
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

    [MUS_HG_AZALEA] =
    {
        .musicInstrument =
        {
            [INSTRUMENT_ACCORDION] =
            {
                .trackBits = 0b000000000000,
            },
            [INSTRUMENT_KEYTAR] =
            {
                .trackBits = 0b001000100000,
            },
            [INSTRUMENT_ERHU] =
            {
                .trackBits = 0b000000001100,
            },
            [INSTRUMENT_GUITAR] =
            {
                .trackBits = 0b000000000000,
            },
            [INSTRUMENT_WHISTLE] =
            {
                .trackBits = 0b000010000000,
            },
            [INSTRUMENT_FLUTE] =
            {
                .trackBits = 0b110000000001,
            },
            [INSTRUMENT_ALL] =
            {
                .trackBits = 0b111010101101,
            },
        }
    }
};

void MPlayFadeOutTrackBitsFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 trackBits, u16 volume)
{
    s32 i;
    u32 bit;
    struct MusicPlayerTrack *track;

    if (mplayInfo->ident == ID_NUMBER)
    {
        mplayInfo->ident++;
        mplayInfo->fadeOC = speed;
        mplayInfo->fadeOI = speed;

        i = mplayInfo->trackCount;
        track = mplayInfo->tracks;
        bit = 1;

        while (i > 0)
        {
            if (trackBits & bit)
            {
                if (track->flags & MPT_FLG_EXIST && track->volX > 0)
                {
                    track->volX = volume;
                    track->flags |= MPT_FLG_VOLCHG;
                }
            }

            i--;
            track++;
            bit <<= 1;
        }

        mplayInfo->fadeOV = ((volume / 4) << FADE_VOL_SHIFT);
        mplayInfo->ident = ID_NUMBER;
    }
}

void m4aMPlayFadeOutTrackBitsFromVol(struct MusicPlayerInfo *mplayInfo, u16 speed, u16 trackBits, u16 volume)
{
    MPlayFadeOutTrackBitsFromVol(mplayInfo, speed, trackBits, volume);
}

static u8 GetInstrumentFromMusician(void)
{
    u16 graphicsId = gObjectEvents[gSelectedObjectEvent].graphicsId;

    switch (graphicsId)
    {
    case OBJ_EVENT_GFX_ACCORDIONIST:
        return INSTRUMENT_ACCORDION;
        break;
    
    case OBJ_EVENT_GFX_KEYTARIST:
        return INSTRUMENT_KEYTAR;
        break;
    
    case OBJ_EVENT_GFX_ERHUIST:
        return INSTRUMENT_ERHU;
        break;
    
    case OBJ_EVENT_GFX_GUITARIST:
        return INSTRUMENT_GUITAR;
        break;
    
    case OBJ_EVENT_GFX_WHISTLER:
        return INSTRUMENT_WHISTLE;
        break;
    
    case OBJ_EVENT_GFX_FLUTIST:
        return INSTRUMENT_FLUTE;
        break;
    
    default:
        return INSTRUMENT_COUNT;
        break;
    }
}

// Remove Instruments
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

void ScrCmd_DynamicMusic_RemoveMusicianInstrument(void)
{
    DynamicMusic_RemoveInstrument(
        GetInstrumentFromMusician()
    );
}

void ScrCmd_DynamicMusic_RemoveAllInstrumentNotPlaying(void)
{
    DynamicMusic_RemoveAllInstrumentNotPlaying();
}

// Restore Instruments
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

void DynamicMusic_RestoreAllInstrumentPlaying(void)
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

void ScrCmd_DynamicMusic_RestoreMusicianInstrument(void)
{
    DynamicMusic_RestoreInstrument(
        GetInstrumentFromMusician()
    );
}

void ScrCmd_DynamicMusic_RestoreAllInstrumentPlaying(void)
{
    DynamicMusic_RestoreAllInstrumentPlaying();
}

// Play Only Instruments
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

void UNUSED DynamicMusic_PlayOnlyInstrumentNotPlaying(void)
{
    for (u8 instrument = 0; instrument < INSTRUMENT_ALL; instrument++)
    {
        u16 flag = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].flagInstrument;

        if (flag != 0 && !FlagGet(flag))
        {
            DynamicMusic_RestoreInstrument(instrument);
        }
    }
}

void DynamicMusic_PlayOnlyInstrumentPlaying(void)
{
    m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0);
    for (u8 instrument = 0; instrument < INSTRUMENT_ALL; instrument++)
    {
        u16 flag = sInstrumentDynamicMusicData[GetCurrentMapMusic()].musicInstrument[instrument].flagInstrument;

        if (flag != 0 && FlagGet(flag))
        {
            DynamicMusic_RestoreInstrument(instrument);
        }
    }
}

void ScrCmd_DynamicMusic_PlayOnlyMusicianInstrument(void)
{
    DynamicMusic_PlayOnlyInstrument(
        GetInstrumentFromMusician()
    );
}