#include "glide.h"
#include <stdint.h>
#include <stdbool.h>

static uint32_t xorshift(uint32_t x) {
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return x;
}

static uint32_t rand_next(Glide *g) {
  g->rng = xorshift(g->rng);
  return g->rng;
}

void glide_init(Glide *g, uint32_t seed) {
  g->rng = seed ? seed : 1;
  for (int i = 0; i < METER_BARS; i++) {
    g->rest[i] = METER_REST_MIN + (rand_next(g) % (METER_REST_MAX - METER_REST_MIN + 1));
  }
  g->phase = 0;
  g->amp = 0;
  g->last_nearest = -1;
}

int glide_pos(const Glide *g) {
  int leg = g->phase < GLIDE_LEG_FRAMES ? 0 : 1;
  int t = g->phase - (leg * GLIDE_LEG_FRAMES);
  int u = t * GLIDE_FP / GLIDE_LEG_FRAMES;
  int s = (3 * u * u * GLIDE_FP - 2 * u * u * u) / (GLIDE_FP * GLIDE_FP);
  int max_pos = (METER_BARS - 1) * GLIDE_FP;
  int pos = (s * max_pos) / GLIDE_FP;
  if (leg) pos = max_pos - pos;
  return pos;
}

int glide_dir(const Glide *g) {
  return g->phase < GLIDE_LEG_FRAMES ? 1 : -1;
}

void glide_frame(Glide *g, int frames_left, int frames_total) {
  g->phase++;
  if (g->phase >= GLIDE_PERIOD) g->phase = 0;

  int elapsed = frames_total - frames_left;
  int amp;
  if (elapsed < GLIDE_FADE_IN) {
    amp = GLIDE_FP * (elapsed + 1) / GLIDE_FADE_IN;
  } else if (frames_left <= GLIDE_FADE_OUT) {
    amp = GLIDE_FP * frames_left / GLIDE_FADE_OUT;
  } else {
    amp = GLIDE_FP;
  }
  g->amp = (int16_t)amp;

  int nearest = (glide_pos(g) + GLIDE_FP / 2) / GLIDE_FP;
  if (nearest != g->last_nearest) {
    g->rest[nearest] = METER_REST_MIN + (rand_next(g) % (METER_REST_MAX - METER_REST_MIN + 1));
    g->last_nearest = (int8_t)nearest;
  }
}

BarStyle glide_bar(const Glide *g, MeterMode mode, bool bursting, int i) {
  BarStyle s;
  if (mode == MODE_UNLINKED) {
    s.height = METER_FLAT_H;
    s.ink = INK_DISABLED;
    s.ink_cool = INK_DISABLED;
    s.dither = false;
    return s;
  }
  if (mode == MODE_FROZEN) {
    s.height = METER_FLAT_H;
    s.ink = INK_GOLD;
    s.ink_cool = INK_GOLD;
    s.dither = false;
    return s;
  }
  if (!bursting || g->amp == 0) {
    s.height = g->rest[i];
    s.ink = INK_GOLD;
    s.ink_cool = INK_GOLD;
    s.dither = false;
    return s;
  }

  int p = glide_pos(g);
  int x = i * GLIDE_FP - p;
  if (x < 0) x = -x;
  int nearest = (p + GLIDE_FP / 2) / GLIDE_FP;
  int dir = glide_dir(g);
  bool behind = (i - nearest) * dir < 0;

  const int tab[6] = {16, 13, 7, 3, 1, 0};
  int d0 = x / GLIDE_FP;
  int frac = x % GLIDE_FP;
  int boost = 0;
  if (d0 < 5) {
    boost = tab[d0] + (tab[d0 + 1] - tab[d0]) * frac / GLIDE_FP;
  }
  boost = boost * g->amp / GLIDE_FP;

  if (i == nearest) {
    int h = g->rest[i] + boost;
    s.height = h < METER_MAX_H ? (uint8_t)h : METER_MAX_H;
    if (g->amp < 64) {
      s.ink = INK_GOLD;
    } else if (g->amp < 128) {
      s.ink = INK_HEAT1;
    } else {
      s.ink = INK_RED;
    }
    s.ink_cool = s.ink;
    s.dither = false;
    return s;
  }

  if (behind) {
    int d = (x + GLIDE_FP / 2) / GLIDE_FP;
    int shift = (GLIDE_FP - g->amp) * 5 / GLIDE_FP;
    int deff = d + shift;
    int h = g->rest[i] + boost;
    s.height = h < METER_MAX_H ? (uint8_t)h : METER_MAX_H;
    s.ink = meter_heat(deff);
    s.ink_cool = meter_heat(deff + 1);
    s.dither = (deff >= 1 && deff <= 4);
    return s;
  }

  s.height = g->rest[i];
  s.ink = INK_GOLD;
  s.ink_cool = INK_GOLD;
  s.dither = false;
  return s;
}
