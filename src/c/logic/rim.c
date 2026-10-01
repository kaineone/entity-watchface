#include "rim.h"

static uint32_t xorshift(uint32_t x) {
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return x;
}

static uint32_t rand_next(RimMeter *rm) {
  rm->rng = xorshift(rm->rng);
  return rm->rng;
}

static int mod_tick(int t) {
  t %= RIM_TICKS;
  if (t < 0) t += RIM_TICKS;
  return t;
}

void rim_init(RimMeter *rm, uint32_t seed) {
  rm->rng = seed ? seed : 1;
  for (int i = 0; i < RIM_TICKS; i++) {
    rim_land(rm, i);
  }
}

int rim_cursor(int minute, int second) {
  if (minute % 2 == 0) {
    return mod_tick(second);
  }
  return mod_tick(60 - second);
}

void rim_land(RimMeter *rm, int tick) {
  int t = mod_tick(tick);
  rm->rest[t] = RIM_REST_MIN + (rand_next(rm) % (RIM_REST_MAX - RIM_REST_MIN + 1));
}

int rim_boost(int d) {
  int a = d < 0 ? -d : d;
  if (a == 0) return 10;
  if (a == 1) return 8;
  if (a == 2) return 4;
  if (a == 3) return 2;
  if (a == 4) return 0;
  return 0;
}

TickStyle rim_tick(const RimMeter *rm, MeterMode mode, int minute, int second, int i) {
  TickStyle s;
  if (mode == MODE_UNLINKED) {
    s.len = RIM_FLAT_LEN;
    s.ink = INK_DISABLED;
    return s;
  }
  if (mode == MODE_FROZEN) {
    s.len = RIM_FLAT_LEN;
    s.ink = INK_GOLD;
    return s;
  }

  int c = rim_cursor(minute, second);
  if (i == c) {
    s.len = RIM_MAX_LEN;
    s.ink = INK_RED;
    return s;
  }

  int k = second < RIM_TRAIL_MAX ? second : RIM_TRAIL_MAX;
  for (int d = 1; d <= k; d++) {
    int t = (minute % 2 == 0) ? mod_tick(c - d) : mod_tick(c + d);
    if (t == i) {
      int len = rm->rest[i] + rim_boost(d);
      s.len = len < RIM_MAX_LEN ? (uint8_t)len : RIM_MAX_LEN;
      s.ink = meter_heat(d);
      return s;
    }
  }

  s.len = rm->rest[i];
  s.ink = INK_GOLD;
  return s;
}
