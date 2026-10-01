#include "status.h"

#include <stdio.h>

void status_power_text(char *buf, size_t n, int pct, bool charging) {
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  if (charging) {
    snprintf(buf, n, "+%d%%", pct);
  } else {
    snprintf(buf, n, "%d%%", pct);
  }
}

StatusInk status_power_ink(int pct, int threshold, bool charging) {
  if (charging) return STATUS_ACCENT;
  if (pct <= threshold) return STATUS_RED;
  return STATUS_GOLD;
}

bool status_should_vibrate(bool was_linked, bool now_linked, bool vibe_pref, bool quiet) {
  return was_linked && !now_linked && vibe_pref && !quiet;
}
