#include <pebble.h>
#include "logic/numeral.h"
#include "numeral_layer.h"

typedef struct {
  char text[3];
  GColor color;
  int16_t gap;
  int16_t stroke;
} NumeralData;

static void draw_digit(GContext *ctx, char ch, int x, int y, int w, int h, int t, GColor color) {
  NumRect rects[NUMERAL_MAX_RECTS];
  int n = numeral_rects(ch, x, y, w, h, t, rects, NUMERAL_MAX_RECTS);

  graphics_context_set_fill_color(ctx, color);
  for (int i = 0; i < n; i++) {
    NumRect r = rects[i];
    graphics_fill_rect(ctx, GRect(r.x, r.y, r.w, r.h), 0, GCornerNone);
  }

  int k = numeral_chamfer(t);
  graphics_context_set_fill_color(ctx, GColorBlack);
  for (int i = 0; i < n; i++) {
    NumRect r = rects[i];
    if (r.cut_tl) {
      for (int j = 0; j < k; j++) {
        graphics_fill_rect(ctx, GRect(r.x, r.y + j, k - j, 1), 0, GCornerNone);
      }
    }
    if (r.cut_tr) {
      for (int j = 0; j < k; j++) {
        graphics_fill_rect(ctx, GRect(r.x + r.w - (k - j), r.y + j, k - j, 1), 0, GCornerNone);
      }
    }
  }
}

static void update_proc(Layer *layer, GContext *ctx) {
  NumeralData *d = (NumeralData *)layer_get_data(layer);
  GRect b = layer_get_bounds(layer);

  graphics_context_set_antialiased(ctx, false);
  graphics_context_set_fill_color(ctx, d->color);

  int x0, x1, cw;
  numeral_cells(0, b.size.w, d->gap, &x0, &x1, &cw);

  size_t len = strlen(d->text);
  if (len == 1) {
    draw_digit(ctx, d->text[0], x1, 0, cw, b.size.h, d->stroke, d->color);
  } else if (len == 2) {
    draw_digit(ctx, d->text[0], x0, 0, cw, b.size.h, d->stroke, d->color);
    draw_digit(ctx, d->text[1], x1, 0, cw, b.size.h, d->stroke, d->color);
  }
}

Layer *numeral_layer_create(GRect frame, GColor color, int gap, int stroke) {
  Layer *l = layer_create_with_data(frame, sizeof(NumeralData));
  if (!l) return NULL;

  NumeralData *d = (NumeralData *)layer_get_data(l);
  d->text[0] = '\0';
  d->color = color;
  d->gap = (int16_t)gap;
  d->stroke = (int16_t)stroke;

  layer_set_update_proc(l, update_proc);
  return l;
}

void numeral_layer_destroy(Layer *l) {
  if (l) layer_destroy(l);
}

void numeral_layer_set_text(Layer *l, const char *text) {
  if (!l) return;
  NumeralData *d = (NumeralData *)layer_get_data(l);

  const char *src = text ? text : "";
  size_t len = strlen(src);
  if (len > 2) len = 2;

  char next[3];
  strncpy(next, src, len);
  next[len] = '\0';

  if (strcmp(d->text, next) != 0) {
    strcpy(d->text, next);
    layer_mark_dirty(l);
  }
}

void numeral_layer_set_color(Layer *l, GColor c) {
  if (!l) return;
  NumeralData *d = (NumeralData *)layer_get_data(l);
  if (!gcolor_equal(d->color, c)) {
    d->color = c;
    layer_mark_dirty(l);
  }
}

void numeral_layer_set_metrics(Layer *l, int gap, int stroke) {
  if (!l) return;
  NumeralData *d = (NumeralData *)layer_get_data(l);
  int16_t g = (int16_t)gap;
  int16_t s = (int16_t)stroke;
  if (d->gap != g || d->stroke != s) {
    d->gap = g;
    d->stroke = s;
    layer_mark_dirty(l);
  }
}
