#ifndef SCANNER_H
#define SCANNER_H

#include <stdint.h>
#include <stdbool.h>

#define SCANNER_HEAT_STEPS 4

typedef struct {
  uint8_t cursor;
  uint8_t heat[SCANNER_HEAT_STEPS];
  uint8_t baseline;
} ScannerShades;

ScannerShades scanner_shades(int hour_color);
bool scanner_hour_outlined(int hour_color);
uint8_t scanner_argb(uint32_t hex_rgb);

#endif
