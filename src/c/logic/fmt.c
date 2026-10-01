#include "fmt.h"

#include <stdio.h>

void fmt_hour(char *buf, size_t n, int hour24, bool use24h) {
  if (use24h) {
    snprintf(buf, n, "%02d", hour24);
  } else {
    int h = hour24 % 12;
    if (h == 0) {
      h = 12;
    }
    snprintf(buf, n, "%d", h);
  }
}

void fmt_minute(char *buf, size_t n, int minute) {
  snprintf(buf, n, "%02d", minute);
}

const char *fmt_ampm(int hour24) {
  return (hour24 < 12) ? "am" : "pm";
}

void fmt_date(char *buf, size_t n, int wday, int mday, int mon0) {
  static const char * const days[] = {
    "sun", "mon", "tue", "wed", "thu", "fri", "sat"
  };
  const int day_idx = (wday >= 0 && wday <= 6) ? wday : 0;
  snprintf(buf, n, "%s %02d.%02d", days[day_idx], mday, mon0 + 1);
}

void fmt_steps(char *buf, size_t n, long steps) {
  if (steps < 0) {
    snprintf(buf, n, "-- steps");
  } else {
    snprintf(buf, n, "%ld steps", steps);
  }
}

void fmt_bpm(char *buf, size_t n, long bpm) {
  if (bpm <= 0) {
    snprintf(buf, n, "-- bpm");
  } else {
    snprintf(buf, n, "%ld bpm", bpm);
  }
}
