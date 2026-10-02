#ifndef RIM_H
#define RIM_H

#include "meter.h"

#define RIM_TICKS 60
#define RIM_MAX_LEN 18
#define RIM_REST_MIN 2
#define RIM_REST_MAX 7
#define RIM_FLAT_LEN 2

#define RIM_FP 256
#define RIM_LEG_FRAMES 50
#define RIM_PERIOD (2 * RIM_LEG_FRAMES)
#define RIM_FADE_IN 5
#define RIM_FADE_OUT 10

typedef struct {
  uint8_t rest[RIM_TICKS];
  uint32_t rng;
  int16_t phase;
  int16_t amp;
  int8_t last_nearest;
} RimMeter;

typedef struct {
  uint8_t len;
  MeterInk ink;
} TickStyle;

void rim_init(RimMeter *rm, uint32_t seed);
void rim_land(RimMeter *rm, int tick);
void rim_start(RimMeter *rm);
int rim_pos(const RimMeter *rm);
int rim_dir(const RimMeter *rm);
void rim_frame(RimMeter *rm, int frames_left, int frames_total);
int rim_boost_fp(int x);
TickStyle rim_tick(const RimMeter *rm, MeterMode mode, bool bursting, int i);

#endif
