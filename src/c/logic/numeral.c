#include <stdint.h>
#include <stdbool.h>
#include "numeral.h"

#define ROWS 5
#define COLS 3

static const char *PAT[10] = {
  "111101101101111",
  "000000000000000",
  "111001111100111",
  "111001111001111",
  "101101111001001",
  "111100111001111",
  "111100111101111",
  "111001001001001",
  "111101111101111",
  "111101111001111"
};

static bool filled(int d, int r, int c) { return PAT[d][r * COLS + c] == '1'; }

int numeral_chamfer(int t) {
  int k = (t * 2) / 3;
  if (k < 2) k = 2;
  return k;
}

static void push(NumRect *out, int *n, int max,
                 int x0, int y0, int w0, int h0,
                 bool tl, bool tr) {
  if (*n >= max) return;
  out[*n] = (NumRect){
    .x = (int16_t)x0, .y = (int16_t)y0,
    .w = (int16_t)w0, .h = (int16_t)h0,
    .cut_tl = tl, .cut_tr = tr
  };
  (*n)++;
}

int numeral_rects(char digit, int x, int y, int w, int h, int t, NumRect *out, int max) {
  if (max < 1) return 0;
  /* Degenerate boxes (too short for three strokes, too narrow for two) draw nothing. */
  if (t < 1 || h < 3 * t || w < 2 * t) return 0;

  if (digit == '1') {
    int n = 0;
    push(out, &n, max, x + w - t, y, t, h, false, false);
    push(out, &n, max, x + w - 2 * t, y, t, t, true, false);
    return n;
  }

  if (digit < '0' || digit > '9') return 0;

  int d = digit - '0';
  int a = (h - 3 * t) / 2;
  int b = h - 3 * t - a;

  int col_x[3] = {x, x + t, x + w - t};
  int col_w[3] = {t, w - 2 * t, t};
  int row_h[5] = {t, a, t, b, t};

  int row_y[5];
  int yy = y;
  for (int r = 0; r < ROWS; r++) {
    row_y[r] = yy;
    yy += row_h[r];
  }

  int n = 0;
  for (int r = 0; r < ROWS && n < max; r++) {
    for (int c = 0; c < COLS && n < max; c++) {
      if (!filled(d, r, c)) continue;

      bool above = (r == 0) || !filled(d, r - 1, c);
      bool left  = (c == 0) || !filled(d, r, c - 1);
      bool tl = above && left;
      bool tr = (digit == '7' && r == 0 && c == 2);

      push(out, &n, max, col_x[c], row_y[r], col_w[c], row_h[r], tl, tr);
    }
  }
  return n;
}

void numeral_cells(int x, int w, int gap, int *cell_x0, int *cell_x1, int *cell_w) {
  int cw = (w - gap) / 2;
  if (cell_w)  *cell_w = cw;
  if (cell_x0) *cell_x0 = x;
  if (cell_x1) *cell_x1 = x + cw + gap;
}
