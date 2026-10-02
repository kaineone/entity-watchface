#include <pebble.h>
#include "rim_layer.h"
#include "layout.h"
#include "palette.h"

static Layer *s_layer = NULL;
static RimMeter s_rim;
static MeterMode s_mode = MODE_FROZEN;
static bool s_bursting = false;
static int32_t s_sin[RIM_TICKS];
static int32_t s_cos[RIM_TICKS];

static uint8_t s_outer_r = 127;
static uint8_t s_max_len = RIM_MAX_LEN;

static GColor ink_color(MeterInk ink) {
  switch (ink) {
    case INK_GOLD:     return PAL_REST;
    case INK_RED:      return PAL_RED;
    case INK_HEAT1:    return PAL_HEAT1;
    case INK_HEAT2:    return PAL_HEAT2;
    case INK_HEAT3:    return PAL_HEAT3;
    case INK_HEAT4:    return PAL_HEAT4;
    case INK_BASELINE: return PAL_BASELINE;
    case INK_DISABLED: return PAL_DISABLED;
    default:           return PAL_BG;
  }
}

static void update_proc(Layer *layer, GContext *ctx) {
  (void)layer;
  GRect b = layer_get_bounds(s_layer);
  int32_t cx = b.size.w / 2;
  int32_t cy = b.size.h / 2;
  int32_t orad = s_outer_r;

#if defined(PBL_COLOR)
  graphics_context_set_antialiased(ctx, true);
#else
  graphics_context_set_antialiased(ctx, false);
#endif
  graphics_context_set_stroke_width(ctx, 1);

  for (int i = 0; i < RIM_TICKS; i++) {
    TickStyle st = rim_tick(&s_rim, s_mode, s_bursting, i);
    if (st.len == 0) continue;

    int len = st.len;
    if (s_max_len != RIM_MAX_LEN) {
      len = (len * s_max_len + RIM_MAX_LEN / 2) / RIM_MAX_LEN;
    }
    if (len < RIM_FLAT_LEN) {
      len = RIM_FLAT_LEN;
    }

    int32_t irad = orad - len;
    int32_t sx = s_sin[i];
    int32_t c = s_cos[i];

    GPoint outer = GPoint((int16_t)(cx + (orad * sx / TRIG_MAX_RATIO)),
                          (int16_t)(cy - (orad * c / TRIG_MAX_RATIO)));
    GPoint inner = GPoint((int16_t)(cx + (irad * sx / TRIG_MAX_RATIO)),
                          (int16_t)(cy - (irad * c / TRIG_MAX_RATIO)));

    graphics_context_set_stroke_color(ctx, ink_color(st.ink));
    graphics_draw_line(ctx, outer, inner);
  }
}

Layer *rim_layer_create(GRect frame) {
  rim_init(&s_rim, (uint32_t)time(NULL));
  s_mode = MODE_FROZEN;
  s_bursting = false;

  const FaceLayout *layout = layout_get();
  s_outer_r = layout->rim_outer_r;
  s_max_len = layout->rim_max_len;

  for (int i = 0; i < RIM_TICKS; i++) {
    int32_t angle = TRIG_MAX_ANGLE * i / RIM_TICKS;
    s_sin[i] = sin_lookup(angle);
    s_cos[i] = cos_lookup(angle);
  }

  s_layer = layer_create(frame);
  if (s_layer) {
    layer_set_update_proc(s_layer, update_proc);
  }
  return s_layer;
}

void rim_layer_destroy(void) {
  if (s_layer) {
    layer_destroy(s_layer);
    s_layer = NULL;
  }
}

void rim_layer_set_mode(MeterMode mode) {
  if (mode != s_mode) {
    s_mode = mode;
    if (s_layer) layer_mark_dirty(s_layer);
  }
}

void rim_layer_set_bursting(bool bursting) {
  if (bursting != s_bursting) {
    s_bursting = bursting;
    if (s_layer) layer_mark_dirty(s_layer);
  }
}

void rim_layer_start(void) {
  rim_start(&s_rim);
}

void rim_layer_frame(int frames_left, int frames_total) {
  rim_frame(&s_rim, frames_left, frames_total);
  if (s_layer) layer_mark_dirty(s_layer);
}
