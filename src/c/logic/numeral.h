#ifndef NUMERAL_H
#define NUMERAL_H

#include <stdint.h>
#include <stdbool.h>

#define NUMERAL_MAX_RECTS 16

typedef struct {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  bool cut_tl;
  bool cut_tr;
} NumRect;

int numeral_chamfer(int t);
int numeral_rects(char digit, int x, int y, int w, int h, int t, NumRect *out, int max);
void numeral_cells(int x, int w, int gap, int *cell_x0, int *cell_x1, int *cell_w);

#endif
