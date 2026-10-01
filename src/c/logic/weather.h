#ifndef WEATHER_H
#define WEATHER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define WEATHER_TEXT_LEN 16
#define WEATHER_STALE_SECS 3600
#define WEATHER_TEMP_C10_MAX 999
#define WEATHER_TEMP_C10_MIN -999
#define WEATHER_FUTURE_SLACK 300

typedef enum { WX_CLEAR = 0, WX_CLOUD = 1, WX_RAIN = 2, WX_STORM = 3, WX_SNOW = 4 } WeatherCond;

const char *weather_word(int cond);
int weather_round_c10(int temp_c10, bool fahrenheit);
void weather_text(char *buf, size_t n, int cond, int temp_c10, bool fahrenheit, bool stale);
void weather_text_variant(char *buf, size_t n, int cond, int temp_c10, bool fahrenheit, bool stale, int level);
bool weather_is_stale(int32_t now, int32_t last, int32_t max_age);
bool weather_valid(int cond, int temp_c10);

#endif
