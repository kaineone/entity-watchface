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

  CHECK_STR(fmt_ampm(0), "am");
  CHECK_STR(fmt_ampm(11), "am");
  CHECK_STR(fmt_ampm(12), "pm");
  CHECK_STR(fmt_ampm(23), "pm");

  fmt_date(buf, sizeof(buf), 0, 1, 0);
  CHECK_STR(buf, "sun 01.01");
  fmt_date(buf, sizeof(buf), 1, 9, 8);
  CHECK_STR(buf, "mon 09.09");
  fmt_date(buf, sizeof(buf), 2, 10, 9);
  CHECK_STR(buf, "tue 10.10");
  fmt_date(buf, sizeof(buf), 3, 31, 11);
  CHECK_STR(buf, "wed 31.12");
  fmt_date(buf, sizeof(buf), 4, 8, 8);
  CHECK_STR(buf, "thu 08.09");
  fmt_date(buf, sizeof(buf), 5, 25, 5);
  CHECK_STR(buf, "fri 25.06");
  fmt_date(buf, sizeof(buf), 6, 2, 1);
  CHECK_STR(buf, "sat 02.02");

  char steps_buf[FMT_STEPS_LEN];
  fmt_steps(steps_buf, sizeof(steps_buf), 6240);
  CHECK_STR(steps_buf, "6240 steps");
  fmt_steps(steps_buf, sizeof(steps_buf), 0);
  CHECK_STR(steps_buf, "0 steps");
  fmt_steps(steps_buf, sizeof(steps_buf), -1);
  CHECK_STR(steps_buf, "-- steps");
  fmt_steps(steps_buf, sizeof(steps_buf), 123456);
  CHECK_STR(steps_buf, "123456 steps");

  char bpm_buf[FMT_BPM_LEN];
  fmt_bpm(bpm_buf, sizeof(bpm_buf), 72);
  CHECK_STR(bpm_buf, "72 bpm");
  fmt_bpm(bpm_buf, sizeof(bpm_buf), 0);
  CHECK_STR(bpm_buf, "-- bpm");
  fmt_bpm(bpm_buf, sizeof(bpm_buf), -5);
  CHECK_STR(bpm_buf, "-- bpm");
  fmt_bpm(bpm_buf, sizeof(bpm_buf), 180);
  CHECK_STR(bpm_buf, "180 bpm");

  TEST_MAIN_END();
}
