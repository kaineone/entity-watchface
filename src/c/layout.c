#include "layout.h"

#if defined(PBL_ROUND) && PBL_DISPLAY_WIDTH >= 260
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
  .font_large_res = RESOURCE_ID_ORB_60,
  .font_small_res = RESOURCE_ID_ORB_48,
  .label_font_res = RESOURCE_ID_LABEL_16,
  .hide_meter_in_peek = false,
  .meter_pitch = 9,
  .meter_bar_w = 7,
  .meter_max_h = 22,
  .rim_outer_r = 127,
  .rim_max_len = 18
};
#elif defined(PBL_ROUND)
static const FaceLayout s_layout = {
  .normal = {
    .date    = GRect(0, 28, 180, 14),
    .date_align    = GTextAlignmentCenter,
    .hour    = GRect(0, 42, 180, 44),
    .hour_align    = GTextAlignmentCenter,
    .ampm    = GRect(128, 46, 30, 14),
    .ampm_align    = GTextAlignmentLeft,
    .minute  = GRect(0, 86, 180, 44),
    .minute_align  = GTextAlignmentCenter,
    .meter   = GRect(0, 0, 180, 180),
    .link    = GRect(24, 82, 22, 14),
    .power   = GRect(124, 82, 32, 14),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(86, 84, 8, 8),
    .weather = GRect(0, 136, 180, 14),
    .weather_align = GTextAlignmentCenter,
    .digits  = DIGITS_LARGE
  },
  .peek = {
    .date    = GRect(0, 28, 180, 14),
    .date_align    = GTextAlignmentCenter,
    .hour    = GRect(0, 42, 180, 44),
    .hour_align    = GTextAlignmentCenter,
    .ampm    = GRect(128, 46, 30, 14),
    .ampm_align    = GTextAlignmentLeft,
    .minute  = GRect(0, 86, 180, 44),
    .minute_align  = GTextAlignmentCenter,
    .meter   = GRect(0, 0, 180, 180),
    .link    = GRect(24, 82, 22, 14),
    .power   = GRect(124, 82, 32, 14),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(86, 84, 8, 8),
    .weather = GRect(0, 136, 180, 14),
    .weather_align = GTextAlignmentCenter,
    .digits  = DIGITS_SMALL
  },
  .font_large_res = RESOURCE_ID_ORB_42,
  .font_small_res = RESOURCE_ID_ORB_42,
  .label_font_res = RESOURCE_ID_LABEL_12,
  .hide_meter_in_peek = false,
  .meter_pitch = 6,
  .meter_bar_w = 5,
  .meter_max_h = 16,
  .rim_outer_r = 87,
  .rim_max_len = 12
};
#elif PBL_DISPLAY_WIDTH >= 200
static const FaceLayout s_layout = {
  .normal = {
    .date    = GRect(8, 8, 120, 16),
    .date_align    = GTextAlignmentLeft,
    .hour    = GRect(0, 28, 200, 64),
    .hour_align    = GTextAlignmentCenter,
    .ampm    = GRect(152, 32, 40, 16),
    .ampm_align    = GTextAlignmentRight,
    .minute  = GRect(0, 92, 200, 64),
    .minute_align  = GTextAlignmentCenter,
    .meter   = GRect(6, 176, 189, 26),
    .link    = GRect(8, 208, 22, 14),
    .power   = GRect(120, 206, 72, 16),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(8, 104, 8, 8),
    .weather = GRect(128, 8, 64, 16),
    .weather_align = GTextAlignmentRight,
    .digits  = DIGITS_LARGE
  },
  .peek = {
    .date    = GRect(8, 8, 120, 16),
    .date_align    = GTextAlignmentLeft,
    .hour    = GRect(0, 26, 200, 48),
    .hour_align    = GTextAlignmentCenter,
    .ampm    = GRect(152, 28, 40, 16),
    .ampm_align    = GTextAlignmentRight,
    .minute  = GRect(0, 74, 200, 48),
    .minute_align  = GTextAlignmentCenter,
    .meter   = GRect(6, 176, 189, 26),
    .link    = GRect(8, 146, 22, 14),
    .power   = GRect(120, 144, 72, 16),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(8, 104, 8, 8),
    .weather = GRect(128, 8, 64, 16),
    .weather_align = GTextAlignmentRight,
    .digits  = DIGITS_SMALL
  },
  .font_large_res = RESOURCE_ID_ORB_64,
  .font_small_res = RESOURCE_ID_ORB_48,
  .label_font_res = RESOURCE_ID_LABEL_16,
  .hide_meter_in_peek = true,
  .meter_pitch = 9,
  .meter_bar_w = 7,
  .meter_max_h = 22,
  .rim_outer_r = 127,
  .rim_max_len = 18
};
#else
static const FaceLayout s_layout = {
  .normal = {
    .date    = GRect(6, 4, 90, 14),
    .date_align    = GTextAlignmentLeft,
    .hour    = GRect(0, 18, 144, 48),
    .hour_align    = GTextAlignmentCenter,
    .ampm    = GRect(104, 22, 34, 14),
    .ampm_align    = GTextAlignmentRight,
    .minute  = GRect(0, 64, 144, 48),
    .minute_align  = GTextAlignmentCenter,
    .meter   = GRect(9, 126, 126, 20),
    .link    = GRect(6, 150, 22, 14),
    .power   = GRect(84, 148, 54, 14),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(6, 72, 8, 8),
    .weather = GRect(96, 4, 42, 14),
    .weather_align = GTextAlignmentRight,
    .digits  = DIGITS_LARGE
  },
  .peek = {
    .date    = GRect(6, 4, 90, 14),
    .date_align    = GTextAlignmentLeft,
    .hour    = GRect(0, 18, 144, 36),
    .hour_align    = GTextAlignmentCenter,
    .ampm    = GRect(104, 20, 34, 14),
    .ampm_align    = GTextAlignmentRight,
    .minute  = GRect(0, 52, 144, 36),
    .minute_align  = GTextAlignmentCenter,
    .meter   = GRect(9, 126, 126, 20),
    .link    = GRect(6, 98, 22, 14),
    .power   = GRect(84, 96, 54, 14),
    .power_align   = GTextAlignmentRight,
    .quiet   = GRect(6, 58, 8, 8),
    .weather = GRect(96, 4, 42, 14),
    .weather_align = GTextAlignmentRight,
    .digits  = DIGITS_SMALL
  },
  .font_large_res = RESOURCE_ID_ORB_48,
  .font_small_res = RESOURCE_ID_ORB_36,
  .label_font_res = RESOURCE_ID_LABEL_12,
  .hide_meter_in_peek = true,
  .meter_pitch = 6,
  .meter_bar_w = 5,
  .meter_max_h = 16,
  .rim_outer_r = 87,
  .rim_max_len = 12
};
#endif

const FaceLayout *layout_get(void) {
  return &s_layout;
}
