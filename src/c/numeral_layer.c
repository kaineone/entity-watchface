#include <pebble.h>
#include "logic/numeral.h"
#include "numeral_layer.h"

#define NUMERAL_OUTLINE 1
#define SHAPE_BUF_RECTS (NUMERAL_MAX_RECTS + 2 * 12)

typedef struct {
  char text[3];
  GColor color;
  GColor outline_color;
  int16_t gap;
  int16_t stroke;
  bool outline;
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

static int build_shape_rects(const NumRect *in, int n, int stroke, NumRect *out, int max) {
  int k = numeral_chamfer(stroke);
  int m = 0;
  for (int i = 0; i < n && m < max; i++) {
    const NumRect *r = &in[i];
    if ((r->cut_tl || r->cut_tr) && k > 0) {
      int rows = (k < r->h) ? k : r->h;
      for (int j = 0; j < rows && m < max; j++) {
        int inset = k - j;
        int16_t x = r->x;
        int16_t w = r->w;
        if (r->cut_tl) { x += (int16_t)inset; w -= (int16_t)inset; }
        if (r->cut_tr) { w -= (int16_t)inset; }
        if (w > 0) {
          out[m++] = (NumRect){ x, (int16_t)(r->y + j), w, 1, false, false };
        }
      }
      if (r->h > k && m < max) {
        out[m++] = (NumRect){ r->x, (int16_t)(r->y + k), r->w, (int16_t)(r->h - k), false, false };
      }
    } else {
      out[m++] = *r;
    }
  }
  return m;
}

static void draw_digit_outlined(GContext *ctx, char ch, int x, int y, int w, int h, int t,
                                GColor color, GColor outline_color) {
  NumRect base[NUMERAL_MAX_RECTS];
  int n = numeral_rects(ch, x, y, w, h, t, base, NUMERAL_MAX_RECTS);

  NumRect shapes[SHAPE_BUF_RECTS];
  int m = build_shape_rects(base, n, t, shapes, SHAPE_BUF_RECTS);

  graphics_context_set_fill_color(ctx, outline_color);
  for (int i = 0; i < m; i++) {
    NumRect r = shapes[i];
    graphics_fill_rect(ctx, GRect(r.x - NUMERAL_OUTLINE, r.y - NUMERAL_OUTLINE, r.w + 2 * NUMERAL_OUTLINE, r.h + 2 * NUMERAL_OUTLINE), 0, GCornerNone);
  }

  graphics_context_set_fill_color(ctx, color);
  for (int i = 0; i < m; i++) {
    NumRect r = shapes[i];
    graphics_fill_rect(ctx, GRect(r.x, r.y, r.w, r.h), 0, GCornerNone);
  }
}

static void update_proc(Layer *layer, GContext *ctx) {
  NumeralData *d = (NumeralData *)layer_get_data(layer);
  GRect b = layer_get_bounds(layer);

  if (d->outline) {
    graphics_context_set_antialiased(ctx, false);

    int ox = NUMERAL_OUTLINE;
    int oy = NUMERAL_OUTLINE;
    int ow = b.size.w - 2 * NUMERAL_OUTLINE;
    int oh = b.size.h - 2 * NUMERAL_OUTLINE;
    int ot = d->stroke - 1;
    if (ot < 1) ot = 1;

    int x0, x1, cw;
    numeral_cells(ox, ow, d->gap, &x0, &x1, &cw);

    size_t len = strlen(d->text);
    if (len == 1) {
      draw_digit_outlined(ctx, d->text[0], x1, oy, cw, oh, ot, d->color, d->outline_color);
    } else if (len == 2) {
      draw_digit_outlined(ctx, d->text[0], x0, oy, cw, oh, ot, d->color, d->outline_color);
      draw_digit_outlined(ctx, d->text[1], x1, oy, cw, oh, ot, d->color, d->outline_color);
    }
  } else {
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
}

Layer *numeral_layer_create(GRect frame, GColor color, int gap, int stroke) {
  Layer *l = layer_create_with_data(frame, sizeof(NumeralData));
  if (!l) return NULL;

  NumeralData *d = (NumeralData *)layer_get_data(l);
  d->text[0] = '\0';
  d->color = color;
  d->outline_color = color;
  d->gap = (int16_t)gap;
  d->stroke = (int16_t)stroke;
  d->outline = false;

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

void numeral_layer_set_outline(Layer *l, bool on, GColor color) {
  if (!l) return;
  NumeralData *d = (NumeralData *)layer_get_data(l);
  if (d->outline != on || !gcolor_equal(d->outline_color, color)) {
    d->outline = on;
    d->outline_color = color;
    layer_mark_dirty(l);
  }
}
