#ifndef FMT_H
#define FMT_H

#include <stdbool.h>
#include <stddef.h>

#define FMT_HOUR_LEN   3
#define FMT_MINUTE_LEN 3
#define FMT_DATE_LEN   10
#define FMT_STEPS_LEN  16
#define FMT_BPM_LEN    10
#define FMT_DATE_LOCALE_LEN 12

void fmt_hour(char *buf, size_t n, int hour24, bool use24h);
void fmt_minute(char *buf, size_t n, int minute);
const char *fmt_ampm(int hour24);
void fmt_date(char *buf, size_t n, int wday, int mday, int mon0);
void fmt_steps(char *buf, size_t n, long steps);
void fmt_bpm(char *buf, size_t n, long bpm);
void fmt_date_locale(char *buf, size_t n, int wday, int mday, int mon0, bool month_first);
bool fmt_locale_month_first(const char *locale);

#endif
