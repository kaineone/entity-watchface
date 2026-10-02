#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>
#include <stdbool.h>

#define SETTINGS_VERSION 1

typedef enum {
  HOUR_RED = 0,
  HOUR_CREAM = 1,
  HOUR_GOLD = 2,
  HOUR_PINK = 3,
  HOUR_PURPLE = 4,
  HOUR_BLUE = 5,
  HOUR_TEAL = 6,
  HOUR_GREEN = 7,
  HOUR_WHITE = 8
} HourColor;

typedef struct {
  uint8_t version;
  int8_t clock;
  uint8_t hour_color;
  bool show_weather;
  bool fahrenheit;
  bool animate;
  bool vibe_disconnect;
  bool tap_swap;
  uint8_t low_battery;
} Settings;

void settings_defaults(Settings *s);
bool settings_valid(const Settings *s);
bool settings_set_clock(Settings *s, int v);
bool settings_set_clock_str(Settings *s, const char *v);
bool settings_set_hour_color(Settings *s, int v);
bool settings_set_hour_color_str(Settings *s, const char *v);
bool settings_set_low_battery(Settings *s, int v);
bool settings_set_low_battery_str(Settings *s, const char *v);
bool settings_set_bool(bool *field, int v);

#endif
