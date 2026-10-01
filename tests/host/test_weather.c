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
  CHECK_STR(weather_word(-1), "cloud");
  CHECK_STR(weather_word(0), "clear");
  CHECK_STR(weather_word(1), "cloud");
  CHECK_STR(weather_word(2), "rain");
  CHECK_STR(weather_word(3), "storm");
  CHECK_STR(weather_word(4), "snow");
  CHECK_STR(weather_word(5), "cloud");

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

  char buf[32];
  weather_text(buf, sizeof(buf), 0, 183, false, false);
  CHECK_STR(buf, "clear 18\xC2\xB0");
  weather_text(buf, sizeof(buf), 0, 183, true, false);
  CHECK_STR(buf, "clear 65\xC2\xB0");
  weather_text(buf, sizeof(buf), 4, -34, false, false);
  CHECK_STR(buf, "snow -3\xC2\xB0");
  weather_text(buf, sizeof(buf), 1, 183, false, true);
  CHECK_STR(buf, "~cloud 18\xC2\xB0");

  int32_t now = 10000;
  CHECK(!weather_is_stale(now, now - 3600, 3600));
  CHECK(weather_is_stale(now, now - 3601, 3600));
  CHECK(weather_is_stale(now, 0, 3600));
  CHECK(weather_is_stale(now, -1, 3600));

  weather_text_variant(buf, sizeof(buf), 0, -120, false, true, 0);
  CHECK_STR(buf, "~clear -12\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 0, -120, false, true, 1);
  CHECK_STR(buf, "clear -12\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 0, -120, false, true, 2);
  CHECK_STR(buf, "clear-12\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 0, -120, false, true, 3);
  CHECK_STR(buf, "clear-12");

  weather_text_variant(buf, sizeof(buf), 2, 370, false, false, 0);
  CHECK_STR(buf, "rain 37\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 2, 370, false, false, 1);
  CHECK_STR(buf, "rain 37\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 2, 370, false, false, 2);
  CHECK_STR(buf, "rain37\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 2, 370, false, false, 3);
  CHECK_STR(buf, "rain37");

  weather_text_variant(buf, sizeof(buf), 3, -400, true, true, 0);
  CHECK_STR(buf, "~storm -40\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 3, -400, true, true, 1);
  CHECK_STR(buf, "storm -40\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 3, -400, true, true, 2);
  CHECK_STR(buf, "storm-40\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 3, -400, true, true, 3);
  CHECK_STR(buf, "storm-40");

  weather_text(buf, sizeof(buf), 1, 183, false, true);
  CHECK_STR(buf, "~cloud 18\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 1, 183, false, true, 0);
  CHECK_STR(buf, "~cloud 18\xC2\xB0");

  weather_text(buf, sizeof(buf), 4, -34, false, false);
  CHECK_STR(buf, "snow -3\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 4, -34, false, false, 0);
  CHECK_STR(buf, "snow -3\xC2\xB0");

  weather_text_variant(buf, sizeof(buf), 1, 183, false, false, 3);
  CHECK_STR(buf, "cloud18");

  weather_text_variant(buf, sizeof(buf), 0, -120, false, true, -3);
  CHECK_STR(buf, "~clear -12\xC2\xB0");
  weather_text_variant(buf, sizeof(buf), 0, -120, false, true, 5);
  CHECK_STR(buf, "clear-12");

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

  weather_text(buf, sizeof(buf), 0, 2000000, false, false);
  CHECK_STR(buf, "clear 100\xC2\xB0");

  const int extreme_temps[] = {-2000000, -999, 0, 999, 2000000};
  for (int ci = 0; ci <= 4; ci++) {
    for (int lvl = 0; lvl <= 3; lvl++) {
      for (int st = 0; st < 2; st++) {
        for (int f = 0; f < 2; f++) {
          for (size_t ti = 0; ti < sizeof(extreme_temps)/sizeof(extreme_temps[0]); ti++) {
            weather_text_variant(buf, sizeof(buf), ci, extreme_temps[ti], f, st, lvl);
            size_t len = strlen(buf);
            CHECK(len < 16);
          }
        }
      }
    }
  }

  TEST_MAIN_END();
}
