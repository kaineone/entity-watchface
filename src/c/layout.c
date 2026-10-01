#include "layout.h"

#if defined(PBL_ROUND)
static const FaceLayout s_layout = {
  .normal = {
    .date    = GRect(0, 42, 260, 16),
    .date_align    = GTextAlignmentCenter,
    .hour    = GRect(0, 70, 260, 60),
    .hour_align    = GTextAlignmentCenter,
    .ampm    = GRect(184, 74, 40, 16),
    .ampm_align    = GTextAlignmentLeft,
    .minute  = GRect(0, 128, 260, 60),
    .minute_align  = GTextAlignmentCenter,
    .meter   = GRect(0, 0, 260, 260),
    .link    = GRect(40, 122, 22, 14),
    .power   = GRect(180, 122, 40, 16),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(126, 122, 8, 8),
    .weather = GRect(0, 200, 260, 16),
    .weather_align = GTextAlignmentCenter,
    .digits  = DIGITS_LARGE
  },
  .peek = {
    .date    = GRect(0, 30, 260, 16),
    .date_align    = GTextAlignmentCenter,
    .hour    = GRect(0, 54, 260, 48),
    .hour_align    = GTextAlignmentCenter,
    .ampm    = GRect(184, 58, 40, 16),
    .ampm_align    = GTextAlignmentLeft,
    .minute  = GRect(0, 108, 260, 48),
    .minute_align  = GTextAlignmentCenter,
    .meter   = GRect(0, 0, 260, 260),
    .link    = GRect(40, 104, 22, 14),
    .power   = GRect(180, 104, 40, 16),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(126, 104, 8, 8),
    .weather = GRect(0, 200, 260, 16),
    .weather_align = GTextAlignmentCenter,
    .digits  = DIGITS_SMALL
  },
  .font_large_res = RESOURCE_ID_ZEN_60,
  .font_small_res = RESOURCE_ID_ZEN_48,
  .hide_meter_in_peek = false
};
#else
static const FaceLayout s_layout = {
  .normal = {
    .date    = GRect(8, 8, 120, 16),
    .date_align    = GTextAlignmentLeft,
    .hour    = GRect(8, 30, 184, 64),
    .hour_align    = GTextAlignmentLeft,
    .ampm    = GRect(140, 36, 52, 16),
    .ampm_align    = GTextAlignmentRight,
    .minute  = GRect(8, 96, 184, 64),
    .minute_align  = GTextAlignmentRight,
    .meter   = GRect(6, 176, 189, 26),
    .link    = GRect(8, 208, 22, 14),
    .power   = GRect(120, 206, 72, 16),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(8, 104, 8, 8),
    .weather = GRect(72, 8, 120, 16),
    .weather_align = GTextAlignmentRight,
    .digits  = DIGITS_LARGE
  },
  .peek = {
    .date    = GRect(8, 8, 120, 16),
    .date_align    = GTextAlignmentLeft,
    .hour    = GRect(8, 30, 184, 48),
    .hour_align    = GTextAlignmentLeft,
    .ampm    = GRect(140, 36, 52, 16),
    .ampm_align    = GTextAlignmentRight,
    .minute  = GRect(8, 80, 184, 48),
    .minute_align  = GTextAlignmentRight,
    .meter   = GRect(6, 176, 189, 26),
    .link    = GRect(8, 146, 22, 14),
    .power   = GRect(120, 144, 72, 16),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(8, 104, 8, 8),
    .weather = GRect(72, 8, 120, 16),
    .weather_align = GTextAlignmentRight,
    .digits  = DIGITS_SMALL
  },
  .font_large_res = RESOURCE_ID_ZEN_64,
  .font_small_res = RESOURCE_ID_ZEN_48,
  .hide_meter_in_peek = true
};
#endif

const FaceLayout *layout_get(void) {
  return &s_layout;
}
