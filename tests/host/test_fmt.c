#include "fmt.h"
#include "test.h"

#include <string.h>

#define CHECK_STR(a, b) \
  do { \
    if (strcmp((a), (b)) != 0) { \
      fprintf(stderr, "FAIL %s:%d: '%s' != '%s'\n", __FILE__, __LINE__, (a), (b)); \
    } \
    CHECK(strcmp((a), (b)) == 0); \
  } while (0)

int main(void) {
  char buf[FMT_DATE_LEN > FMT_HOUR_LEN ? FMT_DATE_LEN : FMT_HOUR_LEN];

  fmt_hour(buf, sizeof(buf), 0, true);
  CHECK_STR(buf, "00");
  fmt_hour(buf, sizeof(buf), 9, true);
  CHECK_STR(buf, "09");
  fmt_hour(buf, sizeof(buf), 12, true);
  CHECK_STR(buf, "12");
  fmt_hour(buf, sizeof(buf), 13, true);
  CHECK_STR(buf, "13");
  fmt_hour(buf, sizeof(buf), 23, true);
  CHECK_STR(buf, "23");

  fmt_hour(buf, sizeof(buf), 0, false);
  CHECK_STR(buf, "12");
  fmt_hour(buf, sizeof(buf), 9, false);
  CHECK_STR(buf, "9");
  fmt_hour(buf, sizeof(buf), 12, false);
  CHECK_STR(buf, "12");
  fmt_hour(buf, sizeof(buf), 13, false);
  CHECK_STR(buf, "1");
  fmt_hour(buf, sizeof(buf), 23, false);
  CHECK_STR(buf, "11");

  fmt_minute(buf, sizeof(buf), 0);
  CHECK_STR(buf, "00");
  fmt_minute(buf, sizeof(buf), 4);
  CHECK_STR(buf, "04");
  fmt_minute(buf, sizeof(buf), 59);
  CHECK_STR(buf, "59");

  CHECK_STR(fmt_ampm(0), "AM");
  CHECK_STR(fmt_ampm(11), "AM");
  CHECK_STR(fmt_ampm(12), "PM");
  CHECK_STR(fmt_ampm(23), "PM");

  fmt_date(buf, sizeof(buf), 0, 1, 0);
  CHECK_STR(buf, "SUN 01.01");
  fmt_date(buf, sizeof(buf), 1, 9, 8);
  CHECK_STR(buf, "MON 09.09");
  fmt_date(buf, sizeof(buf), 2, 10, 9);
  CHECK_STR(buf, "TUE 10.10");
  fmt_date(buf, sizeof(buf), 3, 31, 11);
  CHECK_STR(buf, "WED 31.12");
  fmt_date(buf, sizeof(buf), 4, 8, 8);
  CHECK_STR(buf, "THU 08.09");
  fmt_date(buf, sizeof(buf), 5, 25, 5);
  CHECK_STR(buf, "FRI 25.06");
  fmt_date(buf, sizeof(buf), 6, 2, 1);
  CHECK_STR(buf, "SAT 02.02");

  char steps_buf[FMT_STEPS_LEN];
  fmt_steps(steps_buf, sizeof(steps_buf), 6240);
  CHECK_STR(steps_buf, "6240 STEPS");
  fmt_steps(steps_buf, sizeof(steps_buf), 0);
  CHECK_STR(steps_buf, "0 STEPS");
  fmt_steps(steps_buf, sizeof(steps_buf), -1);
  CHECK_STR(steps_buf, "-- STEPS");
  fmt_steps(steps_buf, sizeof(steps_buf), 123456);
  CHECK_STR(steps_buf, "123456 STEPS");

  char bpm_buf[FMT_BPM_LEN];
  fmt_bpm(bpm_buf, sizeof(bpm_buf), 72);
  CHECK_STR(bpm_buf, "72 BPM");
  fmt_bpm(bpm_buf, sizeof(bpm_buf), 0);
  CHECK_STR(bpm_buf, "-- BPM");
  fmt_bpm(bpm_buf, sizeof(bpm_buf), -5);
  CHECK_STR(bpm_buf, "-- BPM");
  fmt_bpm(bpm_buf, sizeof(bpm_buf), 180);
  CHECK_STR(bpm_buf, "180 BPM");

  char locale_buf[FMT_DATE_LOCALE_LEN];
  fmt_date_locale(locale_buf, sizeof(locale_buf), 4, 1, 9, true);
  CHECK_STR(locale_buf, "THU OCT 1");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 4, 1, 9, false);
  CHECK_STR(locale_buf, "THU 1 OCT");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 3, 30, 8, true);
  CHECK_STR(locale_buf, "WED SEP 30");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 3, 30, 8, false);
  CHECK_STR(locale_buf, "WED 30 SEP");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 31, 11, true);
  CHECK_STR(locale_buf, "SUN DEC 31");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 31, 11, false);
  CHECK_STR(locale_buf, "SUN 31 DEC");

  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 0, true);
  CHECK_STR(locale_buf, "SUN JAN 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 1, true);
  CHECK_STR(locale_buf, "SUN FEB 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 2, true);
  CHECK_STR(locale_buf, "SUN MAR 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 3, true);
  CHECK_STR(locale_buf, "SUN APR 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 4, true);
  CHECK_STR(locale_buf, "SUN MAY 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 5, true);
  CHECK_STR(locale_buf, "SUN JUN 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 6, true);
  CHECK_STR(locale_buf, "SUN JUL 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 7, true);
  CHECK_STR(locale_buf, "SUN AUG 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 8, true);
  CHECK_STR(locale_buf, "SUN SEP 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 9, true);
  CHECK_STR(locale_buf, "SUN OCT 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 10, true);
  CHECK_STR(locale_buf, "SUN NOV 15");
  fmt_date_locale(locale_buf, sizeof(locale_buf), 0, 15, 11, true);
  CHECK_STR(locale_buf, "SUN DEC 15");

  fmt_date_locale(locale_buf, sizeof(locale_buf), 7, 1, 12, true);
  CHECK_STR(locale_buf, "SUN JAN 1");

  CHECK(fmt_locale_month_first("en_US"));
  CHECK(fmt_locale_month_first("en_US.UTF-8"));
  CHECK(!fmt_locale_month_first("en_GB"));
  CHECK(!fmt_locale_month_first("de_DE"));
  CHECK(!fmt_locale_month_first(""));
  CHECK(!fmt_locale_month_first(NULL));

  TEST_MAIN_END();
}
