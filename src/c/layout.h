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
  bool hide_meter_in_peek;
} FaceLayout;

const FaceLayout *layout_get(void);

#endif
