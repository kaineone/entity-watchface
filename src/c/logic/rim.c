#include <stdbool.h>
#include <stdint.h>
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
  rm->phase = 0;
  rm->amp = 0;
  rm->last_nearest = -1;
}

void rim_start(RimMeter *rm) {
  rm->phase = 0;
  rm->amp = 0;
  rm->last_nearest = -1;
}

void rim_land(RimMeter *rm, int tick) {
  int t = mod_tick(tick);
  rm->rest[t] = RIM_REST_MIN + (rand_next(rm) % (RIM_REST_MAX - RIM_REST_MIN + 1));
}

int rim_pos(const RimMeter *rm) {
  int leg = rm->phase < RIM_LEG_FRAMES ? 0 : 1;
  int t = rm->phase - (leg * RIM_LEG_FRAMES);
  int u = t * RIM_FP / RIM_LEG_FRAMES;
  int s = (3 * u * u * RIM_FP - 2 * u * u * u) / (RIM_FP * RIM_FP);
  int max = RIM_TICKS * RIM_FP;
  int pos = (s * max) / RIM_FP;
  if (leg) pos = max - pos;
  return pos;
}

int rim_dir(const RimMeter *rm) {
  return rm->phase < RIM_LEG_FRAMES ? 1 : -1;
}

void rim_frame(RimMeter *rm, int frames_left, int frames_total) {
  rm->phase++;
  if (rm->phase >= RIM_PERIOD) rm->phase = 0;

  int elapsed = frames_total - frames_left;
  int amp;
  if (elapsed < RIM_FADE_IN) {
    amp = RIM_FP * (elapsed + 1) / RIM_FADE_IN;
  } else if (frames_left <= RIM_FADE_OUT) {
    amp = RIM_FP * frames_left / RIM_FADE_OUT;
  } else {
    amp = RIM_FP;
  }
  rm->amp = (int16_t)amp;

  int nearest = ((rim_pos(rm) + RIM_FP / 2) / RIM_FP) % RIM_TICKS;
  if (nearest != rm->last_nearest) {
    rim_land(rm, nearest);
    rm->last_nearest = (int8_t)nearest;
  }
}

int rim_boost_fp(int x) {
  static const int tab[6] = {16, 10, 6, 3, 1, 0};
  int d0 = x / RIM_FP;
  if (d0 >= 5) return 0;
  int frac = x % RIM_FP;
  return tab[d0] + (tab[d0 + 1] - tab[d0]) * frac / RIM_FP;
}

TickStyle rim_tick(const RimMeter *rm, MeterMode mode, bool bursting, int i) {
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
  if (!bursting || rm->amp == 0) {
    s.len = rm->rest[i];
    s.ink = INK_GOLD;
    return s;
  }

  int32_t p = rim_pos(rm);
  int dir = rim_dir(rm);
  int32_t nearest_j = (p + RIM_FP / 2) / RIM_FP;

  int32_t j = i;
  if (i == 0) {
    int32_t d60 = RIM_TICKS * RIM_FP - p;
    if (d60 < p) j = RIM_TICKS;
  }

  int32_t x = j * RIM_FP - p;
  int32_t ax = x < 0 ? -x : x;
  bool ahead = (x * dir > 0);

  int boost = 0;
  if (ahead) {
    if (ax < RIM_FP) {
      boost = 16 * (RIM_FP - ax) / RIM_FP;
    }
  } else {
    boost = rim_boost_fp((int)ax);
  }
  boost = boost * rm->amp / RIM_FP;

  int len = (int)rm->rest[i] + boost;
  if (len > RIM_MAX_LEN) len = RIM_MAX_LEN;
  s.len = (uint8_t)len;

  if (j == nearest_j) {
    if (rm->amp < 64) {
      s.ink = INK_GOLD;
    } else if (rm->amp < 128) {
      s.ink = INK_HEAT1;
    } else {
      s.ink = INK_RED;
    }
  } else if (boost == 0) {
    s.ink = INK_GOLD;
  } else if (ahead) {
    int shift = (RIM_FP - rm->amp) * 5 / RIM_FP;
    s.ink = meter_heat(1 + shift);
  } else {
    int d = (int)((ax + RIM_FP / 2) / RIM_FP);
    int shift = (RIM_FP - rm->amp) * 5 / RIM_FP;
    s.ink = meter_heat(d + shift);
  }
  return s;
}
