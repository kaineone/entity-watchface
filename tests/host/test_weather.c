#include "test.h"
#include "weather.h"
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define CHECK_STR(a, b) \
  do { \
    test_total++; \
    if (strcmp((a), (b)) != 0) { \
      fprintf(stderr, "FAIL %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
      test_failures++; \
    } \
  } while (0)

int main(void) {
  CHECK_EQ_INT(weather_round_c10(183, false), 18);
  CHECK_EQ_INT(weather_round_c10(184, false), 18);
  CHECK_EQ_INT(weather_round_c10(185, false), 19);
  CHECK_EQ_INT(weather_round_c10(186, false), 19);
  CHECK_EQ_INT(weather_round_c10(-34, false), -3);
  CHECK_EQ_INT(weather_round_c10(-35, false), -4);
  CHECK_EQ_INT(weather_round_c10(-36, false), -4);
  CHECK_EQ_INT(weather_round_c10(-5, false), -1);
  CHECK_EQ_INT(weather_round_c10(4, false), 0);
  CHECK_EQ_INT(weather_round_c10(0, false), 0);

  CHECK_EQ_INT(weather_round_c10(183, true), 65);
  CHECK_EQ_INT(weather_round_c10(0, true), 32);
  CHECK_EQ_INT(weather_round_c10(-400, true), -40);
  CHECK_EQ_INT(weather_round_c10(370, true), 99);

  int32_t now = 10000;
  CHECK(!weather_is_stale(now, now - 3600, 3600));
  CHECK(weather_is_stale(now, now - 3601, 3600));
  CHECK(weather_is_stale(now, 0, 3600));
  CHECK(weather_is_stale(now, -1, 3600));

  CHECK(!weather_valid(-1, 0));
  CHECK(weather_valid(0, 0));
  CHECK(weather_valid(4, 0));
  CHECK(!weather_valid(5, 0));
  CHECK(!weather_valid(0, -1000));
  CHECK(weather_valid(0, -999));
  CHECK(weather_valid(0, 999));
  CHECK(!weather_valid(0, 1000));

  CHECK(weather_round_c10(INT32_MAX, false) > 0);
  CHECK(weather_round_c10(INT32_MIN, false) < 0);
  CHECK(weather_round_c10(INT32_MAX, true) > 0);
  CHECK(weather_round_c10(INT32_MIN, true) < 0);

  CHECK(weather_is_stale(100, 5000, 3600));
  CHECK(!weather_is_stale(5000, 5200, 3600));
  CHECK(weather_is_stale(INT32_MIN, 1, 3600));
  CHECK(weather_is_stale(INT32_MAX, 1, 3600));

  char temp_buf[WEATHER_TEMP_TEXT_LEN];
  weather_temp_text(temp_buf, sizeof(temp_buf), 183, false, false);
  CHECK_STR(temp_buf, "18\xC2\xB0");
  weather_temp_text(temp_buf, sizeof(temp_buf), 183, true, false);
  CHECK_STR(temp_buf, "65\xC2\xB0");
  weather_temp_text(temp_buf, sizeof(temp_buf), -34, false, false);
  CHECK_STR(temp_buf, "-3\xC2\xB0");
  weather_temp_text(temp_buf, sizeof(temp_buf), 183, false, true);
  CHECK_STR(temp_buf, "~18\xC2\xB0");
  weather_temp_text(temp_buf, sizeof(temp_buf), 2000000, false, false);
  CHECK_STR(temp_buf, "100\xC2\xB0");
  weather_temp_text(temp_buf, sizeof(temp_buf), -2000000, true, false);
  CHECK_STR(temp_buf, "-148\xC2\xB0");

  weather_temp_text(temp_buf, sizeof(temp_buf), -2000000, true, true);
  CHECK(strlen(temp_buf) < WEATHER_TEMP_TEXT_LEN);
  weather_temp_text(temp_buf, sizeof(temp_buf), 2000000, false, true);
  CHECK(strlen(temp_buf) < WEATHER_TEMP_TEXT_LEN);
  weather_temp_text(temp_buf, sizeof(temp_buf), -999, true, true);
  CHECK(strlen(temp_buf) < WEATHER_TEMP_TEXT_LEN);
  weather_temp_text(temp_buf, sizeof(temp_buf), 999, false, true);
  CHECK(strlen(temp_buf) < WEATHER_TEMP_TEXT_LEN);

  TEST_MAIN_END();
}
