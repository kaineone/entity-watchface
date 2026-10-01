#include "status_layer.h"
#include "palette.h"

static Layer *s_link_layer = NULL;
static bool s_link_linked = true;

static Layer *s_quiet_layer = NULL;
static bool s_quiet_visible = false;

static void link_update_proc(Layer *layer, GContext *ctx) {
  (void)layer;
  const int heights[4] = {5, 8, 11, 14};
  const int layer_h = 14;

  graphics_context_set_antialiased(ctx, false);
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
