#ifndef GLIDE_H
#define GLIDE_H

#include "meter.h"
#define GLIDE_FP 256              /* fixed point: 256 = one bar */
#define GLIDE_LEG_FRAMES 30       /* frames for 0 -> 20 (or back) */
#define GLIDE_PERIOD (2 * GLIDE_LEG_FRAMES)
#define GLIDE_FADE_IN 5
#define GLIDE_FADE_OUT 10

typedef struct {
  uint8_t rest[METER_BARS];
  uint32_t rng;
  int16_t phase;        /* 0 .. GLIDE_PERIOD-1 */
  int16_t amp;          /* 0 .. 256 */
  int8_t last_nearest;  /* nearest bar at the previous frame, -1 none */
} Glide;

void glide_init(Glide *g, uint32_t seed);
int glide_pos(const Glide *g);              /* 0 .. 20*GLIDE_FP */
int glide_dir(const Glide *g);              /* +1 first leg, -1 second leg */
void glide_frame(Glide *g, int frames_left, int frames_total);
BarStyle glide_bar(const Glide *g, MeterMode mode, bool bursting, int i);

#endif
