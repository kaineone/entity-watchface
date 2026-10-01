#include "settings.h"

#include <stdlib.h>
#include <string.h>

void settings_defaults(Settings *s) {
  s->version = SETTINGS_VERSION;
  s->clock = -1;
  s->hour_color = HOUR_RED;
  s->show_weather = true;
  s->fahrenheit = false;
  s->animate = true;
  s->vibe_disconnect = true;
  s->tap_swap = true;
  s->low_battery = 20;
}

bool settings_valid(const Settings *s) {
  if (s->version != SETTINGS_VERSION) return false;
  if (s->clock < -1 || s->clock > 1) return false;
  if (s->hour_color > HOUR_GOLD) return false;
  if (s->low_battery != 10 && s->low_battery != 20 &&
      s->low_battery != 30 && s->low_battery != 50) return false;
  return true;
}

bool settings_set_clock(Settings *s, int v) {
  if (v < -1 || v > 1) return false;
  if (s->clock == (int8_t)v) return false;
  s->clock = (int8_t)v;
  return true;
}

bool settings_set_clock_str(Settings *s, const char *v) {
  if (v == NULL || *v == '\0') return false;
  char *end = NULL;
  long r = strtol(v, &end, 10);
  if (*end != '\0') return false;
  return settings_set_clock(s, (int)r);
}

bool settings_set_hour_color(Settings *s, int v) {
  if (v < HOUR_RED || v > HOUR_GOLD) return false;
  if (s->hour_color == (uint8_t)v) return false;
  s->hour_color = (uint8_t)v;
  return true;
}

bool settings_set_hour_color_str(Settings *s, const char *v) {
  if (v == NULL) return false;
  if (strcmp(v, "red") == 0) return settings_set_hour_color(s, HOUR_RED);
  if (strcmp(v, "cream") == 0) return settings_set_hour_color(s, HOUR_CREAM);
  if (strcmp(v, "gold") == 0) return settings_set_hour_color(s, HOUR_GOLD);
  if (*v == '\0') return false;
  char *end = NULL;
  long r = strtol(v, &end, 10);
  if (*end != '\0') return false;
  return settings_set_hour_color(s, (int)r);
}

bool settings_set_low_battery(Settings *s, int v) {
  if (v != 10 && v != 20 && v != 30 && v != 50) return false;
  if (s->low_battery == (uint8_t)v) return false;
  s->low_battery = (uint8_t)v;
  return true;
}

bool settings_set_low_battery_str(Settings *s, const char *v) {
  if (v == NULL || *v == '\0') return false;
  char *end = NULL;
  long r = strtol(v, &end, 10);
  if (*end != '\0') return false;
  return settings_set_low_battery(s, (int)r);
}

bool settings_set_bool(bool *field, int v) {
  bool nv = v != 0;
  if (*field == nv) return false;
  *field = nv;
  return true;
}
