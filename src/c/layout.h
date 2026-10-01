#ifndef LAYOUT_H
#define LAYOUT_H

#include <pebble.h>

typedef enum { DIGITS_LARGE, DIGITS_SMALL } DigitSize;

typedef struct {
  GRect date;
  GRect hour;
  GRect ampm;
  GRect minute;
  GRect meter;
  GRect link;
  GRect power;
  GRect quiet;
  GRect weather;
  GTextAlignment date_align;
  GTextAlignment weather_align;
  GTextAlignment hour_align;
  GTextAlignment ampm_align;
  GTextAlignment minute_align;
  GTextAlignment power_align;
  DigitSize digits;
} FaceFrames;

typedef struct {
  FaceFrames normal;
  FaceFrames peek;
  uint32_t font_large_res;
  uint32_t font_small_res;
  uint32_t label_font_res;
  bool hide_meter_in_peek;
  uint8_t meter_pitch;
  uint8_t meter_bar_w;
  uint8_t meter_max_h;
  uint8_t rim_outer_r;
  uint8_t rim_max_len;
} FaceLayout;

const FaceLayout *layout_get(void);

#endif
