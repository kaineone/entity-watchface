#include "weather.h"
#include <stdio.h>
#include <stdint.h>

const char *weather_word(int cond) {
  switch (cond) {
    case WX_CLEAR: return "clear";
    case WX_CLOUD: return "cloud";
    case WX_RAIN:  return "rain";
    case WX_STORM: return "storm";
    case WX_SNOW:  return "snow";
    default:       return "cloud";
  }
}

int weather_round_c10(int temp_c10, bool fahrenheit) {
  int64_t t = temp_c10;
  if (fahrenheit) {
    int64_t f100 = t * 18 + 3200;
    if (f100 >= 0) return (int)((f100 + 50) / 100);
    return (int)((f100 - 50) / 100);
  }
  if (t >= 0) return (int)((t + 5) / 10);
  return (int)((t - 5) / 10);
}

void weather_text_variant(char *buf, size_t n, int cond, int temp_c10, bool fahrenheit, bool stale, int level) {
  if (level < 0) level = 0;
  else if (level > 3) level = 3;

  int t_c10 = temp_c10;
  if (t_c10 < WEATHER_TEMP_C10_MIN) t_c10 = WEATHER_TEMP_C10_MIN;
  else if (t_c10 > WEATHER_TEMP_C10_MAX) t_c10 = WEATHER_TEMP_C10_MAX;

  int t = weather_round_c10(t_c10, fahrenheit);
  const char *prefix = (stale && level == 0) ? "~" : "";
  const char *sep = (level <= 1) ? " " : "";
  const char *deg = (level <= 2) ? "\xC2\xB0" : "";
  snprintf(buf, n, "%s%s%s%d%s", prefix, weather_word(cond), sep, t, deg);
}

void weather_text(char *buf, size_t n, int cond, int temp_c10, bool fahrenheit, bool stale) {
  weather_text_variant(buf, n, cond, temp_c10, fahrenheit, stale, 0);
}

bool weather_is_stale(int32_t now, int32_t last, int32_t max_age) {
  if (last <= 0) return true;
  int64_t now64 = now;
  if (last > now64 + WEATHER_FUTURE_SLACK) return true;
  int64_t age = now64 - last;
  return age > max_age;
}

bool weather_valid(int cond, int temp_c10) {
  return cond >= WX_CLEAR && cond <= WX_SNOW &&
         temp_c10 >= WEATHER_TEMP_C10_MIN && temp_c10 <= WEATHER_TEMP_C10_MAX;
}
