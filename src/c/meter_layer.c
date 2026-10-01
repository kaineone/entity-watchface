#include <pebble.h>
#include "layout.h"
#include "meter_layer.h"
#include "logic/meter.h"
#include "palette.h"

static Layer *s_layer;
static Meter s_meter;
static MeterMode s_mode;

static uint8_t s_pitch = 9;
static uint8_t s_bar_w = 7;
static uint8_t s_max_h = 22;

#if defined(PBL_COLOR)
static GColor color_for_ink(MeterInk ink) {
  switch (ink) {
    case INK_GOLD:     return PAL_GOLD;
    case INK_RED:      return PAL_RED;
    case INK_HEAT1:    return PAL_HEAT1;
    case INK_HEAT2:    return PAL_HEAT2;
    case INK_HEAT3:    return PAL_HEAT3;
    case INK_HEAT4:    return PAL_HEAT4;
    case INK_BASELINE: return PAL_BASELINE;
    case INK_DISABLED: return PAL_DISABLED;
    default:           return PAL_DISABLED;
  }
}
#endif

static void update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_antialiased(ctx, false);

  const GRect f = layer_get_frame(layer);

  for (int i = 0; i < METER_BARS; i++) {
    const BarStyle s = meter_bar(&s_meter, s_mode, i);

    int h = s.height;
    if (s_max_h != METER_MAX_H) {
      h = (h * s_max_h + METER_MAX_H / 2) / METER_MAX_H;
    }
    if (h < METER_FLAT_H) {
      h = METER_FLAT_H;
    }

    const int x = i * s_pitch;
    const GRect bar = GRect(x, s_max_h - h, s_bar_w, h);

#if defined(PBL_BW)
    const int density = meter_ink_density(s.ink);
    if (density >= 16) {
      graphics_context_set_fill_color(ctx, GColorWhite);
      graphics_fill_rect(ctx, bar, 0, GCornersAll);
    } else {
      graphics_context_set_stroke_color(ctx, GColorWhite);
      for (int dx = 0; dx < s_bar_w; dx++) {
        const int abs_x = f.origin.x + x + dx;
        for (int y = bar.origin.y; y < bar.origin.y + bar.size.h; y++) {
          const int abs_y = f.origin.y + y;
          if (meter_bayer_value(abs_x, abs_y) < density) {
            graphics_draw_pixel(ctx, GPoint(x + dx, y));
          }
        }
      }
    }
#else
    graphics_context_set_fill_color(ctx, color_for_ink(s.ink));
    graphics_fill_rect(ctx, bar, 0, GCornersAll);

    if (s.dither) {
      graphics_context_set_stroke_color(ctx, color_for_ink(s.ink_cool));
      for (int dx = 0; dx < s_bar_w; dx++) {
        const int abs_x = f.origin.x + x + dx;
        for (int y = bar.origin.y; y < bar.origin.y + bar.size.h; y++) {
          const int abs_y = f.origin.y + y;
          if (meter_bayer_cool(abs_x, abs_y)) {
            graphics_draw_pixel(ctx, GPoint(x + dx, y));
          }
        }
      }
    }
#endif

    const GRect base = GRect(x, s_max_h + 2, s_bar_w, 2);

#if defined(PBL_BW)
    const int base_density = meter_ink_density(meter_baseline_ink(s_mode));
    if (base_density >= 16) {
      graphics_context_set_fill_color(ctx, GColorWhite);
      graphics_fill_rect(ctx, base, 0, GCornersAll);
    } else {
      graphics_context_set_stroke_color(ctx, GColorWhite);
      for (int dx = 0; dx < base.size.w; dx++) {
        const int abs_x = f.origin.x + base.origin.x + dx;
        for (int y = base.origin.y; y < base.origin.y + base.size.h; y++) {
          const int abs_y = f.origin.y + y;
          if (meter_bayer_value(abs_x, abs_y) < base_density) {
            graphics_draw_pixel(ctx, GPoint(base.origin.x + dx, y));
          }
        }
      }
    }
#else
    graphics_context_set_fill_color(ctx, color_for_ink(meter_baseline_ink(s_mode)));
    graphics_fill_rect(ctx, base, 0, GCornersAll);
#endif
  }
}

Layer *meter_layer_create(GRect frame) {
  const FaceLayout *layout = layout_get();
  s_pitch = layout->meter_pitch;
  s_bar_w = layout->meter_bar_w;
  s_max_h = layout->meter_max_h;

  s_layer = layer_create(frame);
  if (!s_layer) return NULL;

  layer_set_update_proc(s_layer, update_proc);
  meter_init(&s_meter, (uint32_t)time(NULL));
  s_mode = MODE_FROZEN;

  return s_layer;
}

void meter_layer_destroy(void) {
  if (s_layer) {
    layer_destroy(s_layer);
    s_layer = NULL;
  }
}

void meter_layer_set_mode(MeterMode mode) {
  if (mode != s_mode) {
    s_mode = mode;
    if (s_layer) layer_mark_dirty(s_layer);
  }
}

void meter_layer_step(void) {
  meter_step(&s_meter);
  if (s_layer) layer_mark_dirty(s_layer);
}
