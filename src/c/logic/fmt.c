#include "fmt.h"

#include <stdio.h>
#include <string.h>

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
  return (hour24 < 12) ? "AM" : "PM";
}

void fmt_date(char *buf, size_t n, int wday, int mday, int mon0) {
  static const char * const days[] = {
    "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"
  };
  const int day_idx = (wday >= 0 && wday <= 6) ? wday : 0;
  snprintf(buf, n, "%s %02d.%02d", days[day_idx], mday, mon0 + 1);
}

void fmt_steps(char *buf, size_t n, long steps) {
  if (steps < 0) {
    snprintf(buf, n, "-- STEPS");
  } else {
    snprintf(buf, n, "%ld STEPS", steps);
  }
}

void fmt_bpm(char *buf, size_t n, long bpm) {
  if (bpm <= 0) {
    snprintf(buf, n, "-- BPM");
  } else {
    snprintf(buf, n, "%ld BPM", bpm);
  }
}

void fmt_date_locale(char *buf, size_t n, int wday, int mday, int mon0, bool month_first) {
  static const char * const days[] = {
    "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"
  };
  static const char * const months[] = {
    "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
    "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
  };
  const int day_idx = (wday >= 0 && wday <= 6) ? wday : 0;
  const int mon_idx = (mon0 >= 0 && mon0 <= 11) ? mon0 : 0;

  if (month_first) {
    snprintf(buf, n, "%s %s %d", days[day_idx], months[mon_idx], mday);
  } else {
    snprintf(buf, n, "%s %d %s", days[day_idx], mday, months[mon_idx]);
  }
}

bool fmt_locale_month_first(const char *locale) {
  return locale != NULL && strncmp(locale, "en_US", 5) == 0;
}
