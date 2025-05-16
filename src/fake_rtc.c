#include "global.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"
#include "datetime.h"
#include "rtc.h"
#include "fake_rtc.h"
#include "event_data.h"
#include "script.h"
#include "vyraton.h"

static void FakeRtc_CalcTimeDifference(struct Time *result, struct SiiRtcInfo *t1, struct Time *t2);

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

    Ikigai_TimeAdvanceFunctions();
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

void FakeRtc_ForwardTimeTo(u32 hour, u32 minute, u32 second)
{
    Script_PauseFakeRtc();
    struct Time diff, target;
    struct SiiRtcInfo *fakeRtc = FakeRtc_GetCurrentTime();

    target.hours = hour;
    target.minutes = minute;
    target.seconds = second;

    FakeRtc_CalcTimeDifference(&diff, fakeRtc, &target);
    FakeRtc_AdvanceTimeBy(0, diff.hours, diff.minutes, diff.seconds);
    Script_ResumeFakeRtc();
}

static void FakeRtc_CalcTimeDifference(struct Time *result, struct SiiRtcInfo *t1, struct Time *t2)
{
    result->seconds = t2->seconds - t1->second;
    result->minutes = t2->minutes - t1->minute;
    result->hours = t2->hours - t1->hour;
    result->days = t2->days - t1->day;

    if (result->seconds < 0)
    {
        result->seconds += SECONDS_PER_MINUTE;
        --result->minutes;
    }

    if (result->minutes < 0)
    {
        result->minutes += MINUTES_PER_HOUR;
        --result->hours;
    }

    if (result->hours < 0)
    {
        result->hours += HOURS_PER_DAY;
        --result->days;
    }
}

void FakeRtc_ManuallySetTime(u32 day, u32 hour, u32 minute, u32 second)
{
    FakeRtc_Reset();
    FakeRtc_AdvanceTimeBy(day, hour, minute, second);
}

u32 FakeRtc_GetSecondsRatio(void)
{
    return (OW_ALTERED_TIME_RATIO == GEN_8_PLA)   ? 60 :
           (OW_ALTERED_TIME_RATIO == GEN_9)       ? 20 :
           (OW_ALTERED_TIME_RATIO == TIME_DEBUG)  ?  1 :
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

u8 Ikigai_ReturnDaysInSeason(void)
{
    return IKIGAI_FULL_MONTH ? 28 : 14;
}

u8 Ikigai_GetYearFromDays(u32 days)
{
    return (days == 0) ? 0 : ((days - 1) / (SEASON_COUNT * Ikigai_ReturnDaysInSeason())) + 1;
}

u8 Ikigai_GetYear(void)
{
    RtcCalcLocalTime();
    return Ikigai_GetYearFromDays(gLocalTime.days);
}

enum Seasons Ikigai_GetSeasonFromDays(u32 days)
{
    if (days == 0)
        return SEASON_COUNT;

    days = (days - 1) % (SEASON_COUNT * Ikigai_ReturnDaysInSeason());

    return days / Ikigai_ReturnDaysInSeason();
}

enum Seasons Ikigai_GetSeason(void)
{
    RtcCalcLocalTime();
    return Ikigai_GetSeasonFromDays(gLocalTime.days);
}

enum Seasons Ikigai_FetchSeason(void)
{
    return Ikigai_GetSeasonFromDays(gLocalTime.days);
}

u8 Ikigai_GetDateFromDays(u32 days)
{
    return (days == 0) ? 0 : ((days - 1) % Ikigai_ReturnDaysInSeason()) + 1;
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

    daysDiff = (year * 112) + (newSeason * Ikigai_ReturnDaysInSeason());
    daysDiff -= days; 
    FakeRtc_AdvanceTimeBy(daysDiff, 0, 0, 0);
}

void Ikigai_SetToYearOneSeason(enum Seasons newSeason)
{
    u32 day;

    if (newSeason == SEASON_COUNT)
        day = 0;
    else
        day = 1 + (Ikigai_ReturnDaysInSeason() * newSeason);

    FakeRtc_ManuallySetTime(day, 0, 0, 0);
}

u8 Ikigai_GetSeasonalTimeHour(s32 days, enum TimeOfDay time, bool32 end)
{
    const u8 baseTime[TIMES_OF_DAY_COUNT][2] =
    {
        [TIME_MORNING] = {MORNING_HOUR_BEGIN, MORNING_HOUR_END},
        [TIME_DAY]     = {DAY_HOUR_BEGIN,     DAY_HOUR_END},
        [TIME_EVENING] = {EVENING_HOUR_BEGIN, EVENING_HOUR_END},
        [TIME_NIGHT]   = {NIGHT_HOUR_BEGIN,   NIGHT_HOUR_END},
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
    season = season >= SEASON_COUNT ? SEASON_SPRING : season;
    time = time >= TIMES_OF_DAY_COUNT ? OW_TIME_OF_DAY_FALLBACK : time;

    return baseTime[time][end] + seasonalAdjustments[season][time][end];
}
