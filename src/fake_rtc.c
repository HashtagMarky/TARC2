#include "global.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"
#include "datetime.h"
#include "rtc.h"
#include "fake_rtc.h"
#include "event_data.h"
#include "script.h"

#include "calendar.h"

void FakeRtc_Reset(void)
{
#if OW_USE_FAKE_RTC
    memset(&gSaveBlock3Ptr->fakeRTC, 0, sizeof(gSaveBlock3Ptr->fakeRTC));
    gSaveBlock3Ptr->fakeRTC.year = 0; // offset by gGen3Epoch.year
    gSaveBlock3Ptr->fakeRTC.month = gGen3Epoch.month;
    gSaveBlock3Ptr->fakeRTC.day = gGen3Epoch.day;
    gSaveBlock3Ptr->fakeRTC.dayOfWeek = gGen3Epoch.dayOfWeek;
#endif
}

struct SiiRtcInfo *FakeRtc_GetCurrentTime(void)
{
#if OW_USE_FAKE_RTC
    return &gSaveBlock3Ptr->fakeRTC;
#else
    return NULL;
#endif
}

void FakeRtc_GetRawInfo(struct SiiRtcInfo *rtc)
{
    struct SiiRtcInfo *fakeRtc = FakeRtc_GetCurrentTime();
    if (fakeRtc != NULL)
        memcpy(rtc, fakeRtc, sizeof(struct SiiRtcInfo));
}

void FakeRtc_TickTimeForward(void)
{
    if (!OW_USE_FAKE_RTC)
        return;

    if (FlagGet(OW_FLAG_PAUSE_TIME))
        return;

    FakeRtc_AdvanceTimeBy(0, 0, 0, FakeRtc_GetSecondsRatio());
}

void FakeRtc_AdvanceTimeBy(u32 days, u32 hours, u32 minutes, u32 seconds)
{
    struct DateTime dateTime;
    struct SiiRtcInfo *rtc = FakeRtc_GetCurrentTime();

    ConvertRtcToDateTime(&dateTime, rtc);
    DateTime_AddSeconds(&dateTime, seconds);
    DateTime_AddMinutes(&dateTime, minutes);
    DateTime_AddHours(&dateTime, hours);
    DateTime_AddDays(&dateTime, days);
    ConvertDateTimeToRtc(rtc, &dateTime);
}

void FakeRtc_ManuallySetTime(u32 day, u32 hour, u32 minute, u32 second)
{
    FakeRtc_Reset();
    FakeRtc_AdvanceTimeBy(day, hour, minute, second);
}

void AdvanceScript(void)
{
    FakeRtc_AdvanceTimeBy(300, 0, 0, 0);
}

u32 FakeRtc_GetSecondsRatio(void)
{
    return (OW_ALTERED_TIME_RATIO == GEN_8_PLA) ? 60 :
           (OW_ALTERED_TIME_RATIO == GEN_9)     ? 20 :
                                                  1;
}

STATIC_ASSERT((OW_FLAG_PAUSE_TIME == 0 || OW_USE_FAKE_RTC == TRUE), FakeRtcMustBeTrueToPauseTime);

void Script_PauseFakeRtc(void)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);

    FlagSet(OW_FLAG_PAUSE_TIME);
}

void Script_ResumeFakeRtc(void)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);

    FlagClear(OW_FLAG_PAUSE_TIME);
}

void Script_ToggleFakeRtc(void)
{
    Script_RequestEffects(SCREFF_V1 | SCREFF_SAVE);

    FlagToggle(OW_FLAG_PAUSE_TIME);
}

u8 Ikigai_GetYearFromDays(u32 days)
{
    return (days == 0) ? 0 : ((days - 1) / (SEASON_COUNT * DAYS_IN_SEASON)) + 1;
}

enum Seasons Ikigai_GetSeasonFromDays(u32 days)
{
    if (days == 0)
        return SEASON_COUNT;

    while (days > SEASON_COUNT * DAYS_IN_SEASON)
        days -= SEASON_COUNT * DAYS_IN_SEASON;

    return (days - 1) / DAYS_IN_SEASON;
}

u8 Ikigai_GetDateFromDays(u32 days)
{
    return (days == 0) ? 0 : ((days - 1) % DAYS_IN_SEASON) + 1;
}

void UNUSED Ikigai_SetToNextSeason(s16 days, enum Seasons newSeason)
{
    if (newSeason > SEASON_COUNT)
        return;
    
    u8 year = Ikigai_GetYearFromDays(days);
    u8 currentSeason = Ikigai_GetSeasonFromDays(days);
    s16 daysDiff;

    if (newSeason <= currentSeason || newSeason == SEASON_COUNT)
        year++;

    daysDiff = (year * 112) + (newSeason * 28);
    daysDiff -= days; 
    FakeRtc_AdvanceTimeBy(daysDiff * 24, 0, 0);
}

u8 Ikigai_GetSeasonalTimeHour(s32 days, enum TimeOfDay time, bool32 begin)
{
    const u8 baseTime[SEASON_COUNT][2] =
    {
        {MORNING_HOUR_BEGIN,    MORNING_HOUR_END},
        {DAY_HOUR_BEGIN,        DAY_HOUR_END},
        {EVENING_HOUR_BEGIN,    EVENING_HOUR_END},
        {NIGHT_HOUR_BEGIN,      NIGHT_HOUR_END},
    };

    const s8 seasonalAdjustments[SEASON_COUNT][TIMES_OF_DAY_COUNT][2] =
    {
        [SEASON_SPRING] = {
            [TIME_MORNING]  = {  0,  0  },
            [TIME_DAY]      = {  0,  0  },
            [TIME_EVENING]  = {  0,  0  },
            [TIME_NIGHT]    = {  0,  0  },
        },
        
        [SEASON_SUMMER] = {
            [TIME_MORNING]  = {  0, -2  },
            [TIME_DAY]      = { -2,  4  },
            [TIME_EVENING]  = {  4,  4  },
            [TIME_NIGHT]    = {  4,  0  },
        },
        
        [SEASON_AUTUMN] = {
            [TIME_MORNING]  = {  2,  0  },
            [TIME_DAY]      = {  0, -1  },
            [TIME_EVENING]  = { -1,  3  },
            [TIME_NIGHT]    = {  3,  2  },
        },
        
        [SEASON_WINTER] = {
            [TIME_MORNING]  = {  3,  0  },
            [TIME_DAY]      = {  0, -2  },
            [TIME_EVENING]  = { -2, -1  },
            [TIME_NIGHT]    = { -1,  3  },
        },
    };

    enum Seasons season = Ikigai_GetSeasonFromDays(days);
    time = time >= TIMES_OF_DAY_COUNT ? TIME_DAY : time;

    return baseTime[time][begin] + seasonalAdjustments[season][time][begin];
}
