#include "status_layer.h"
#include "palette.h"
#include "logic/meter.h"

static Layer *s_link_layer = NULL;
static bool s_link_linked = true;

static Layer *s_quiet_layer = NULL;
static bool s_quiet_visible = false;

static void link_update_proc(Layer *layer, GContext *ctx) {
  const int heights[4] = {5, 8, 11, 14};
  const int layer_h = 14;

  graphics_context_set_antialiased(ctx, false);

#if defined(PBL_BW)
  if (!s_link_linked) {
    const GRect f = layer_get_frame(layer);
    graphics_context_set_stroke_color(ctx, GColorWhite);
    for (int k = 0; k < 4; k++) {
      const GRect bar = GRect(k * 6, layer_h - heights[k], 4, heights[k]);
      for (int dx = 0; dx < bar.size.w; dx++) {
        const int abs_x = f.origin.x + bar.origin.x + dx;
        for (int y = bar.origin.y; y < bar.origin.y + bar.size.h; y++) {
          const int abs_y = f.origin.y + y;
          if (meter_bayer_value(abs_x, abs_y) < 4) {
            graphics_draw_pixel(ctx, GPoint(bar.origin.x + dx, y));
          }
        }
      }
    }
    return;
  }
#endif

  graphics_context_set_fill_color(ctx, s_link_linked ? PAL_GOLD : PAL_DISABLED);

  for (int k = 0; k < 4; k++) {
    int h = heights[k];
    graphics_fill_rect(ctx, GRect(k * 6, layer_h - h, 4, h), 0, GCornerNone);
  }
}

Layer *link_layer_create(GRect frame) {
  s_link_layer = layer_create(frame);
  if (s_link_layer) {
    s_link_linked = true;
    layer_set_update_proc(s_link_layer, link_update_proc);
    layer_mark_dirty(s_link_layer);
  }
  return s_link_layer;
}

void link_layer_destroy(void) {
  if (s_link_layer) {
    layer_destroy(s_link_layer);
    s_link_layer = NULL;
  }
}

void link_layer_set_linked(bool linked) {
  if (!s_link_layer || linked == s_link_linked) return;
  s_link_linked = linked;
  layer_mark_dirty(s_link_layer);
}

static void quiet_update_proc(Layer *layer, GContext *ctx) {
  (void)layer;
  graphics_context_set_fill_color(ctx, PAL_QUIET);
  graphics_fill_rect(ctx, GRect(0, 0, 8, 2), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(0, 6, 8, 2), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(0, 2, 2, 4), 0, GCornerNone);
  graphics_fill_rect(ctx, GRect(6, 2, 2, 4), 0, GCornerNone);
}

Layer *quiet_layer_create(GRect frame) {
  s_quiet_layer = layer_create(frame);
  if (s_quiet_layer) {
    s_quiet_visible = false;
    layer_set_update_proc(s_quiet_layer, quiet_update_proc);
    layer_set_hidden(s_quiet_layer, true);
  }
  return s_quiet_layer;
}

void quiet_layer_destroy(void) {
  if (s_quiet_layer) {
    layer_destroy(s_quiet_layer);
    s_quiet_layer = NULL;
  }
}

void quiet_layer_set_visible(bool visible) {
  if (!s_quiet_layer || visible == s_quiet_visible) return;
  s_quiet_visible = visible;
  layer_set_hidden(s_quiet_layer, !visible);
}
