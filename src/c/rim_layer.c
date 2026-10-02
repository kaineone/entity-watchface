#include <pebble.h>
#include "rim_layer.h"
#include "layout.h"
#include "palette.h"

static Layer *s_layer = NULL;
static RimMeter s_rim;
static MeterMode s_mode = MODE_FROZEN;
static int s_minute = 0;
static int s_second = 0;
static int s_last_land = -1;
static int32_t s_sin[RIM_TICKS];
static int32_t s_cos[RIM_TICKS];
static ScannerShades s_shades;

static uint8_t s_outer_r = 127;
static uint8_t s_max_len = RIM_MAX_LEN;

static GColor ink_color(MeterInk ink) {
#if defined(PBL_COLOR)
  switch (ink) {
    case INK_GOLD:     return PAL_REST;
    case INK_RED:      return (GColor){ .argb = s_shades.cursor };
    case INK_HEAT1:    return (GColor){ .argb = s_shades.heat[0] };
    case INK_HEAT2:    return (GColor){ .argb = s_shades.heat[1] };
    case INK_HEAT3:    return (GColor){ .argb = s_shades.heat[2] };
    case INK_HEAT4:    return (GColor){ .argb = s_shades.heat[3] };
    case INK_BASELINE: return (GColor){ .argb = s_shades.baseline };
    case INK_DISABLED: return PAL_DISABLED;
    default:           return PAL_BG;
  }
#else
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
#endif
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
    TickStyle st = rim_tick(&s_rim, s_mode, s_minute, s_second, i);
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

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  s_minute = t->tm_min;
  s_second = t->tm_sec;
  s_last_land = -1;

  const FaceLayout *layout = layout_get();
  s_outer_r = layout->rim_outer_r;
  s_max_len = layout->rim_max_len;
  s_shades = scanner_shades(0);

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
  s_minute = minute;
  s_second = second;
  if (s_mode == MODE_ANIMATING) {
    int c = rim_cursor(minute, second);
    if (c != s_last_land) {
      rim_land(&s_rim, c);
      s_last_land = c;
    }
  }
  if (s_layer) layer_mark_dirty(s_layer);
}

void rim_layer_set_shades(const ScannerShades *s) {
  if (!s) return;
  s_shades = *s;
  if (s_layer) layer_mark_dirty(s_layer);
}
