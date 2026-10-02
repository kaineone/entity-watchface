#include "layout.h"

#if defined(PBL_ROUND) && PBL_DISPLAY_WIDTH >= 260
static const FaceLayout s_layout = {
  .normal = {
    .date = GRect(0, 38, 260, 16),
    .date_align = GTextAlignmentCenter,
    .weather = GRect(0, 186, 260, 16),
    .weather_align = GTextAlignmentCenter,
    .hour = GRect(48, 62, 164, 58),
    .minute = GRect(48, 128, 164, 58),
    .ampm = GRect(105, 204, 50, 16),
    .ampm_align = GTextAlignmentCenter,
    .meter = GRect(0, 0, 260, 260),
    .link = GRect(76, 206, 22, 14),
    .power = GRect(132, 204, 50, 16),
    .power_align = GTextAlignmentRight,
    .quiet = GRect(126, 24, 8, 8),
    .num_gap = 10,
    .num_stroke = 15
  },
  .peek = {
    .date = GRect(0, 38, 260, 16),
    .date_align = GTextAlignmentCenter,
    .weather = GRect(0, 186, 260, 16),
    .weather_align = GTextAlignmentCenter,
    .hour = GRect(48, 62, 164, 58),
    .minute = GRect(48, 128, 164, 58),
    .ampm = GRect(105, 204, 50, 16),
    .ampm_align = GTextAlignmentCenter,
    .meter = GRect(0, 0, 260, 260),
    .link = GRect(76, 206, 22, 14),
    .power = GRect(132, 204, 50, 16),
    .power_align = GTextAlignmentRight,
    .quiet = GRect(126, 24, 8, 8),
    .num_gap = 10,
    .num_stroke = 15
  },
  .hide_meter_in_peek = false,
  .show_link = true,
  .meter_pitch = 9,
  .meter_bar_w = 7,
  .meter_max_h = 22,
  .rim_outer_r = 127,
  .rim_max_len = 18
};
#elif defined(PBL_ROUND)
static const FaceLayout s_layout = {
  .normal = {
    .date = GRect(0, 28, 180, 16),
    .date_align = GTextAlignmentCenter,
    .weather = GRect(24, 132, 56, 16),
    .weather_align = GTextAlignmentRight,
    .hour = GRect(32, 46, 116, 38),
    .minute = GRect(32, 90, 116, 38),
    .ampm = GRect(80, 132, 20, 16),
    .ampm_align = GTextAlignmentCenter,
    .meter = GRect(0, 0, 180, 180),
    .link = GRect(0, 0, 0, 0),
    .power = GRect(100, 132, 56, 16),
    .power_align = GTextAlignmentLeft,
    .quiet = GRect(52, 32, 8, 8),
    .num_gap = 8,
    .num_stroke = 10
  },
  .peek = {
    .date = GRect(0, 28, 180, 16),
    .date_align = GTextAlignmentCenter,
    .weather = GRect(24, 132, 56, 16),
    .weather_align = GTextAlignmentRight,
    .hour = GRect(32, 46, 116, 38),
    .minute = GRect(32, 90, 116, 38),
    .ampm = GRect(80, 132, 20, 16),
    .ampm_align = GTextAlignmentCenter,
    .meter = GRect(0, 0, 180, 180),
    .link = GRect(0, 0, 0, 0),
    .power = GRect(100, 132, 56, 16),
    .power_align = GTextAlignmentLeft,
    .quiet = GRect(52, 32, 8, 8),
    .num_gap = 8,
    .num_stroke = 10
  },
  .hide_meter_in_peek = false,
  .show_link = false,
  .meter_pitch = 6,
  .meter_bar_w = 5,
  .meter_max_h = 16,
  .rim_outer_r = 87,
  .rim_max_len = 12
};
#elif PBL_DISPLAY_WIDTH >= 200
static const FaceLayout s_layout = {
  .normal = {
    .date = GRect(8, 6, 120, 16),
    .date_align = GTextAlignmentLeft,
    .weather = GRect(128, 6, 64, 16),
    .weather_align = GTextAlignmentRight,
    .hour = GRect(8, 30, 184, 64),
    .minute = GRect(8, 104, 184, 64),
    .ampm = GRect(70, 206, 60, 16),
    .ampm_align = GTextAlignmentCenter,
    .meter = GRect(6, 176, 189, 26),
    .link = GRect(8, 208, 22, 14),
    .power = GRect(120, 206, 72, 16),
    .power_align = GTextAlignmentRight,
    .quiet = GRect(36, 211, 8, 8),
    .num_gap = 10,
    .num_stroke = 17
  },
  .peek = {
    .date = GRect(8, 6, 120, 16),
    .date_align = GTextAlignmentLeft,
    .weather = GRect(128, 6, 64, 16),
    .weather_align = GTextAlignmentRight,
    .hour = GRect(8, 30, 184, 46),
    .minute = GRect(8, 84, 184, 46),
    .ampm = GRect(70, 144, 60, 16),
    .ampm_align = GTextAlignmentCenter,
    .meter = GRect(6, 176, 189, 26),
    .link = GRect(8, 146, 22, 14),
    .power = GRect(120, 144, 72, 16),
    .power_align = GTextAlignmentRight,
    .quiet = GRect(36, 149, 8, 8),
    .num_gap = 10,
    .num_stroke = 12
  },
  .hide_meter_in_peek = true,
  .show_link = true,
  .meter_pitch = 9,
  .meter_bar_w = 7,
  .meter_max_h = 22,
  .rim_outer_r = 127,
  .rim_max_len = 18
};
#else
static const FaceLayout s_layout = {
  .normal = {
    .date = GRect(6, 3, 90, 16),
    .date_align = GTextAlignmentLeft,
    .weather = GRect(96, 3, 42, 16),
    .weather_align = GTextAlignmentRight,
    .hour = GRect(6, 20, 132, 46),
    .minute = GRect(6, 72, 132, 46),
    .ampm = GRect(50, 148, 44, 16),
    .ampm_align = GTextAlignmentCenter,
    .meter = GRect(9, 126, 126, 20),
    .link = GRect(6, 150, 22, 14),
    .power = GRect(84, 148, 54, 16),
    .power_align = GTextAlignmentRight,
    .quiet = GRect(32, 153, 8, 8),
    .num_gap = 8,
    .num_stroke = 12
  },
  .peek = {
    .date = GRect(6, 3, 90, 16),
    .date_align = GTextAlignmentLeft,
    .weather = GRect(96, 3, 42, 16),
    .weather_align = GTextAlignmentRight,
    .hour = GRect(6, 25, 132, 30),
    .minute = GRect(6, 60, 132, 30),
    .ampm = GRect(50, 96, 44, 16),
    .ampm_align = GTextAlignmentCenter,
    .meter = GRect(9, 126, 126, 20),
    .link = GRect(6, 98, 22, 14),
    .power = GRect(84, 96, 54, 16),
    .power_align = GTextAlignmentRight,
    .quiet = GRect(32, 101, 8, 8),
    .num_gap = 8,
    .num_stroke = 8
  },
  .hide_meter_in_peek = true,
  .show_link = true,
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
