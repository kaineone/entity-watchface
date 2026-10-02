#ifndef LAYOUT_H
#define LAYOUT_H

#include <pebble.h>

typedef struct {
  GRect date;
  GRect weather;
  GRect hour;
  GRect minute;
  GRect ampm;
  GRect meter;
  GRect link;
  GRect power;
  GRect quiet;
  GTextAlignment date_align;
  GTextAlignment weather_align;
  GTextAlignment ampm_align;
  GTextAlignment power_align;
  uint8_t num_gap;
  uint8_t num_stroke;
} FaceFrames;

typedef struct {
  FaceFrames normal;
  FaceFrames peek;
  bool hide_meter_in_peek;
  bool show_link;
  uint8_t meter_pitch;
  uint8_t meter_bar_w;
  uint8_t meter_max_h;
  uint8_t rim_outer_r;
  uint8_t rim_max_len;
} FaceLayout;

const FaceLayout *layout_get(void);

#endif
