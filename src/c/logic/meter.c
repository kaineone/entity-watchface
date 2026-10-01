#include "meter.h"

static int abs_i(int x) {
  return x < 0 ? -x : x;
}

uint32_t meter_rand(Meter *m) {
  uint32_t x = m->rng;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  m->rng = x;
  return x;
}

void meter_init(Meter *m, uint32_t seed) {
  const int range = METER_REST_MAX - METER_REST_MIN + 1;
  m->rng = seed ? seed : 1;
  m->peak = 0;
  m->dir = 1;
  for (int i = 0; i < METER_BARS; i++) {
    m->rest[i] = (uint8_t)(METER_REST_MIN + (meter_rand(m) % (uint32_t)range));
  }
}

void meter_step(Meter *m) {
  const int range = METER_REST_MAX - METER_REST_MIN + 1;
  int next = m->peak + m->dir;
  if (next < 0 || next > 20) {
    m->dir = (int8_t)(-m->dir);
  }
  m->peak = (int8_t)(m->peak + m->dir);
  m->rest[m->peak] = (uint8_t)(METER_REST_MIN + (meter_rand(m) % (uint32_t)range));
}

int meter_boost(int d) {
  d = abs_i(d);
  switch (d) {
    case 0: return 16;
    case 1: return 13;
    case 2: return 7;
    case 3: return 3;
    case 4: return 1;
    default: return 0;
  }
}

MeterInk meter_heat(int d) {
  d = abs_i(d);
  switch (d) {
    case 0: return INK_RED;
    case 1: return INK_HEAT1;
    case 2: return INK_HEAT2;
    case 3: return INK_HEAT3;
    case 4: return INK_HEAT4;
    default: return INK_GOLD;
  }
}

BarStyle meter_bar(const Meter *m, MeterMode mode, int i) {
  BarStyle s;
  if (mode == MODE_UNLINKED) {
    s.height = METER_FLAT_H;
    s.ink = INK_DISABLED;
    s.ink_cool = INK_DISABLED;
    s.dither = false;
  } else if (mode == MODE_FROZEN) {
    s.height = METER_FLAT_H;
    s.ink = INK_GOLD;
    s.ink_cool = INK_GOLD;
    s.dither = false;
  } else {
    int d = abs_i(i - m->peak);
    if (i == m->peak) {
      s.height = METER_MAX_H;
      s.ink = INK_RED;
      s.ink_cool = INK_RED;
      s.dither = false;
    } else if ((i - m->peak) * m->dir < 0) {
      int h = m->rest[i] + meter_boost(d);
      if (h > METER_MAX_H) h = METER_MAX_H;
      s.height = (uint8_t)h;
      s.ink = meter_heat(d);
      s.ink_cool = meter_heat(d + 1);
      s.dither = (d >= 1 && d <= 4);
    } else {
      s.height = m->rest[i];
      s.ink = INK_GOLD;
      s.ink_cool = INK_GOLD;
      s.dither = false;
    }
  }
  return s;
}

MeterInk meter_baseline_ink(MeterMode mode) {
  return (mode == MODE_UNLINKED) ? INK_DISABLED : INK_BASELINE;
}

bool meter_bayer_cool(int x, int y) {
  static const uint8_t matrix[16] = {
    0, 8, 2, 10,
    12, 4, 14, 6,
    3, 11, 1, 9,
    15, 7, 13, 5
  };
  int xi = ((x % 4) + 4) % 4;
  int yi = ((y % 4) + 4) % 4;
  return matrix[yi * 4 + xi] < 8;
}

MeterMode meter_mode(bool linked, bool animate_pref, int battery_pct, int threshold, bool charging, bool quiet, bool peek) {
  if (!linked) return MODE_UNLINKED;
  if ((battery_pct <= threshold && !charging) || quiet || !animate_pref || peek) return MODE_FROZEN;
  return MODE_ANIMATING;
}

bool meter_timer_should_run(MeterMode mode, bool focused) {
  return (mode == MODE_ANIMATING) && focused;
}
