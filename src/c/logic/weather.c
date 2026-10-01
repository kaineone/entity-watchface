#include "weather.h"
#include <stdio.h>
#include <stdint.h>

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

void weather_temp_text(char *buf, size_t n, int temp_c10, bool fahrenheit, bool stale) {
  int t_c10 = temp_c10;
  if (t_c10 < WEATHER_TEMP_C10_MIN) t_c10 = WEATHER_TEMP_C10_MIN;
  else if (t_c10 > WEATHER_TEMP_C10_MAX) t_c10 = WEATHER_TEMP_C10_MAX;

  int t = weather_round_c10(t_c10, fahrenheit);
  snprintf(buf, n, "%s%d\xC2\xB0", stale ? "~" : "", t);
}
