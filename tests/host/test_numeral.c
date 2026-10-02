#include "test.h"
#include "../../src/c/logic/numeral.h"
#include <stdbool.h>
#include <stddef.h>

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

static bool f(char d, int r, int c) { return PAT[d - '0'][r * 3 + c] == '1'; }

static int expected_total(char d) {
  if (d == '1') return 2;
  if (d < '0' || d > '9') return 0;
  int n = 0;
  for (int r = 0; r < 5; r++)
    for (int c = 0; c < 3; c++)
      if (f(d, r, c)) n++;
  return n;
}

static int expected_cuts(char d) {
  if (d == '1') return 1;
  if (d < '0' || d > '9') return 0;
  int n = 0;
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 3; c++) {
      if (!f(d, r, c)) continue;
      bool tl = ((r == 0) || !f(d, r - 1, c)) &&
                ((c == 0) || !f(d, r, c - 1));
      bool tr = (d == '7' && r == 0 && c == 2);
      if (tl || tr) n++;
    }
  }
  return n;
}

static bool overlap(const NumRect *a, const NumRect *b) {
  return a->x < b->x + b->w && a->x + a->w > b->x &&
         a->y < b->y + b->h && a->y + a->h > b->y;
}

int main(void) {
  NumRect out[32];

  for (int d = '0'; d <= '9'; d++) {
    CHECK_EQ_INT(numeral_rects((char)d, 8, 30, 88, 64, 17, out, 32),
                 expected_total((char)d));
  }
  CHECK_EQ_INT(numeral_rects('x', 8, 30, 88, 64, 17, out, 32), 0);
  CHECK_EQ_INT(numeral_rects(' ', 8, 30, 88, 64, 17, out, 32), 0);
  CHECK_EQ_INT(numeral_rects('8', 8, 30, 88, 64, 17, out, 0), 0);

  int xs[2] = {8, 6};
  int ys[2] = {30, 20};
  int ws[2] = {88, 62};
  int hs[2] = {64, 46};
  int ts[2] = {17, 12};

  for (int i = 0; i < 2; i++) {
    int x = xs[i], y = ys[i], w = ws[i], h = hs[i], t = ts[i];
    for (int d = '0'; d <= '9'; d++) {
      int n = numeral_rects((char)d, x, y, w, h, t, out, 32);
      CHECK(n >= 0);
      int area = 0;
      int cuts = 0;
      for (int p = 0; p < n; p++) {
        NumRect r = out[p];
        CHECK(r.x >= x && r.x + r.w <= x + w);
        CHECK(r.y >= y && r.y + r.h <= y + h);
        CHECK(r.w > 0 && r.h > 0);
        area += r.w * r.h;
        if (r.cut_tl || r.cut_tr) cuts++;
        for (int q = p + 1; q < n; q++) {
          CHECK(!overlap(&r, &out[q]));
        }
      }
      if (d == '8') CHECK_EQ_INT(area, w * h - (w - 2 * t) * (h - 3 * t));  /* minus the two holes */
      CHECK_EQ_INT(cuts, expected_cuts((char)d));

      if (d == '1') {
        CHECK_EQ_INT(out[0].x, x + w - t);
        CHECK_EQ_INT(out[0].w, t);
        CHECK_EQ_INT(out[0].h, h);
        CHECK(!out[0].cut_tl && !out[0].cut_tr);
        CHECK_EQ_INT(out[1].x, x + w - 2 * t);
        CHECK_EQ_INT(out[1].w, t);
        CHECK_EQ_INT(out[1].h, t);
        CHECK(out[1].cut_tl && !out[1].cut_tr);
      }
    }
  }

  CHECK_EQ_INT(numeral_rects('8', 8, 30, 88, 64, 17, out, 4), 4);

  int cx0, cx1, cw;
  numeral_cells(8, 184, 10, &cx0, &cx1, &cw);
  CHECK_EQ_INT(cw, 87);
  CHECK_EQ_INT(cx0, 8);
  CHECK_EQ_INT(cx1, 105);

  CHECK_EQ_INT(numeral_chamfer(17), 11);
  CHECK_EQ_INT(numeral_chamfer(3), 2);
  CHECK_EQ_INT(numeral_chamfer(1), 2);

  {
    NumRect g[NUMERAL_MAX_RECTS];
    CHECK_EQ_INT(numeral_rects('8', 0, 0, 40, 20, 8, g, NUMERAL_MAX_RECTS), 0);  /* h < 3t */
    CHECK_EQ_INT(numeral_rects('8', 0, 0, 10, 64, 8, g, NUMERAL_MAX_RECTS), 0);  /* w < 2t */
    CHECK_EQ_INT(numeral_rects('8', 0, 0, 40, 64, 0, g, NUMERAL_MAX_RECTS), 0);  /* t < 1 */
  }
  TEST_MAIN_END();
}
