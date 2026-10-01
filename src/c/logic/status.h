#ifndef STATUS_H
#define STATUS_H

#include <stddef.h>
#include <stdbool.h>

typedef enum { STATUS_GOLD, STATUS_RED, STATUS_ACCENT } StatusInk;

#define STATUS_POWER_LEN 6

void status_power_text(char *buf, size_t n, int pct, bool charging);
StatusInk status_power_ink(int pct, int threshold, bool charging);
bool status_should_vibrate(bool was_linked, bool now_linked, bool vibe_pref, bool quiet);

#endif
