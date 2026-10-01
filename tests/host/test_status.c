#include "test.h"
#include "status.h"
#include <stdbool.h>
#include <string.h>

#define CHECK_STR(a, b) \
  do { \
    test_total++; \
    if (strcmp((a), (b)) != 0) { \
      fprintf(stderr, "FAIL %s:%d: \"%s\" != \"%s\"\n", __FILE__, __LINE__, (a), (b)); \
      test_failures++; \
    } \
  } while (0)

int main(void) {
  char buf[STATUS_POWER_LEN];

  status_power_text(buf, sizeof(buf), 0, false);
  CHECK_STR(buf, "0%");
  status_power_text(buf, sizeof(buf), 0, true);
  CHECK_STR(buf, "+0%");

  status_power_text(buf, sizeof(buf), 9, false);
  CHECK_STR(buf, "9%");
  status_power_text(buf, sizeof(buf), 9, true);
  CHECK_STR(buf, "+9%");

  status_power_text(buf, sizeof(buf), 84, false);
  CHECK_STR(buf, "84%");
  status_power_text(buf, sizeof(buf), 84, true);
  CHECK_STR(buf, "+84%");

  status_power_text(buf, sizeof(buf), 100, false);
  CHECK_STR(buf, "100%");
  status_power_text(buf, sizeof(buf), 100, true);
  CHECK_STR(buf, "+100%");

  status_power_text(buf, sizeof(buf), 150, false);
  CHECK_STR(buf, "100%");
  status_power_text(buf, sizeof(buf), 150, true);
  CHECK_STR(buf, "+100%");

  CHECK_EQ_INT(status_power_ink(21, 20, false), STATUS_GOLD);
  CHECK_EQ_INT(status_power_ink(20, 20, false), STATUS_RED);
  CHECK_EQ_INT(status_power_ink(15, 20, false), STATUS_RED);
  CHECK_EQ_INT(status_power_ink(15, 20, true), STATUS_ACCENT);
  CHECK_EQ_INT(status_power_ink(90, 20, true), STATUS_ACCENT);

  for (int was = 0; was <= 1; was++) {
    for (int now = 0; now <= 1; now++) {
      for (int vibe = 0; vibe <= 1; vibe++) {
        for (int quiet = 0; quiet <= 1; quiet++) {
          bool expected = (was == 1 && now == 0 && vibe == 1 && quiet == 0);
          CHECK_EQ_INT(status_should_vibrate(was, now, vibe, quiet), expected);
        }
      }
    }
  }

  TEST_MAIN_END();
}
