#ifndef RIM_H
#define RIM_H

#include "meter.h"

#define RIM_TICKS 60
#define RIM_MAX_LEN 18
#define RIM_REST_MIN 2
#define RIM_REST_MAX 7
#define RIM_FLAT_LEN 2
#define RIM_TRAIL_MAX 5

typedef struct {
  uint8_t rest[RIM_TICKS];
  uint32_t rng;
} RimMeter;

typedef struct {
  uint8_t len;
  MeterInk ink;
} TickStyle;

void rim_init(RimMeter *rm, uint32_t seed);
int rim_cursor(int minute, int second);
void rim_land(RimMeter *rm, int tick);
int rim_boost(int d);
TickStyle rim_tick(const RimMeter *rm, MeterMode mode, int minute, int second, int i);
TickStyle rim_tick_frac(const RimMeter *rm, MeterMode mode, bool bursting, int minute,
                        int second, int ms, int i);

#endif
