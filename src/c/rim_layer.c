#include "rim_layer.h"
#include "palette.h"

static Layer *s_layer = NULL;
static RimMeter s_rim;
static MeterMode s_mode = MODE_FROZEN;
static int s_minute = 0;
static int s_second = 0;
static int s_last_land = -1;
static int32_t s_sin[RIM_TICKS];
static int32_t s_cos[RIM_TICKS];

static GColor ink_color(MeterInk ink) {
  switch (ink) {
    case INK_GOLD:     return PAL_GOLD;
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
  int32_t orad = 127;

  graphics_context_set_antialiased(ctx, false);
  graphics_context_set_stroke_width(ctx, 1);

  for (int i = 0; i < RIM_TICKS; i++) {
    TickStyle st = rim_tick(&s_rim, s_mode, s_minute, s_second, i);
    if (st.len == 0) continue;

    int32_t irad = orad - st.len;
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
  s_minute = 0;
  s_second = 0;
  s_last_land = -1;

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

void rim_layer_set_time(int minute, int second) {
  bool changed = (minute != s_minute) || (second != s_second);
  s_minute = minute;
  s_second = second;

  if (s_mode == MODE_ANIMATING) {
    int c = rim_cursor(minute, second);
    if (c != s_last_land) {
      rim_land(&s_rim, c);
      s_last_land = c;
    }
  }

  if (changed && s_layer) layer_mark_dirty(s_layer);
}
