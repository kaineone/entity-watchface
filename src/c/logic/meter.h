#ifndef METER_H
#define METER_H

#include <stdint.h>
#include <stdbool.h>

#define METER_BARS 21
#define METER_MAX_H 22
#define METER_REST_MIN 2
#define METER_REST_MAX 13
#define METER_FLAT_H 2

typedef enum {
  INK_GOLD,
  INK_RED,
  INK_HEAT1,
  INK_HEAT2,
  INK_HEAT3,
  INK_HEAT4,
  INK_BASELINE,
  INK_DISABLED
} MeterInk;

typedef enum {
  MODE_UNLINKED,
  MODE_FROZEN,
  MODE_ANIMATING
} MeterMode;

typedef struct {
  uint8_t rest[METER_BARS];
  int8_t peak;
  int8_t dir;
  uint32_t rng;
} Meter;

typedef struct {
  uint8_t height;
  MeterInk ink;
  MeterInk ink_cool;
  bool dither;
} BarStyle;

void meter_init(Meter *m, uint32_t seed);
uint32_t meter_rand(Meter *m);
void meter_step(Meter *m);
int meter_boost(int d);
MeterInk meter_heat(int d);
BarStyle meter_bar(const Meter *m, MeterMode mode, int i);
MeterInk meter_baseline_ink(MeterMode mode);
int meter_bayer_value(int x, int y);
bool meter_bayer_cool(int x, int y);
int meter_ink_density(MeterInk ink);
MeterMode meter_mode(bool linked, bool animate_pref, int battery_pct, int threshold, bool charging, bool quiet, bool peek);
bool meter_timer_should_run(MeterMode mode, bool focused);

#endif
