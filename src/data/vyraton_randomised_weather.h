#include "global.h"
#include "gba/gba.h"
#include "main.h"
#include "fake_rtc.h"
#include "constants/weather.h"

static const struct IkigaiRandomWeather sIkigaiRandomWeatherSpring[] =
{
    { WEATHER_NONE, 100 },
    { WEATHER_RAIN, 100 },
};

static const struct IkigaiRandomWeather sIkigaiRandomWeatherSummer[] =
{
    { WEATHER_NONE, 100 },
    { WEATHER_SUNNY, 100 },
};

static const struct IkigaiRandomWeather sIkigaiRandomWeatherAutumn[] =
{
    { WEATHER_NONE, 100 },
    { WEATHER_SUNNY_CLOUDS, 100 },
};

static const struct IkigaiRandomWeather sIkigaiRandomWeatherWinter[] =
{
    { WEATHER_NONE, 100 },
    { WEATHER_SNOW, 100 },
};

static const struct IkigaiRandomWeather *sIkigaiRandomWeather[SEASON_COUNT] =
{
    [SEASON_SPRING] = sIkigaiRandomWeatherSpring,
    [SEASON_SUMMER] = sIkigaiRandomWeatherSummer,
    [SEASON_AUTUMN] = sIkigaiRandomWeatherAutumn,
    [SEASON_WINTER] = sIkigaiRandomWeatherWinter,
};

static const size_t sIkigaiRandomWeatherCount[SEASON_COUNT] =
{
    [SEASON_SPRING] = sizeof(sIkigaiRandomWeatherSpring) / sizeof(struct IkigaiRandomWeather),
    [SEASON_SUMMER] = sizeof(sIkigaiRandomWeatherSummer) / sizeof(struct IkigaiRandomWeather),
    [SEASON_AUTUMN] = sizeof(sIkigaiRandomWeatherAutumn) / sizeof(struct IkigaiRandomWeather),
    [SEASON_WINTER] = sizeof(sIkigaiRandomWeatherWinter) / sizeof(struct IkigaiRandomWeather),
};