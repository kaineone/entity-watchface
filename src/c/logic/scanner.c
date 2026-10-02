#include "scanner.h"

static uint8_t level(uint8_t channel) {
  return channel / 85; /* 0x00, 0x55, 0xAA, 0xFF -> 0, 1, 2, 3 */
}

uint8_t scanner_argb(uint32_t hex_rgb) {
  uint8_t r = level((uint8_t)(hex_rgb >> 16));
  uint8_t g = level((uint8_t)(hex_rgb >> 8));
  uint8_t b = level((uint8_t)hex_rgb);
  return (uint8_t)(0xC0 | (r << 4) | (g << 2) | b);
}

/* Source colours from the hour-colour table (24-bit RGB). */
static const uint32_t s_cursor[9] = {
  0xFF0000, /* 0 red    */
  0xFF0000, /* 1 cream  */
  0xFF0000, /* 2 gold   */
  0xFF55AA, /* 3 pink   */
  0xAA00FF, /* 4 purple */
  0x0055FF, /* 5 blue   */
  0x00AAAA, /* 6 teal   */
  0x00AA55, /* 7 green  */
  0xFF0000, /* 8 white  */
};

static const uint32_t s_heat[9][SCANNER_HEAT_STEPS] = {
  {0xAA0000, 0xAA0000, 0x550000, 0x550000}, /* red/cream/gold/white */
  {0xAA0000, 0xAA0000, 0x550000, 0x550000},
  {0xAA0000, 0xAA0000, 0x550000, 0x550000},
  {0xAA55AA, 0xAA5555, 0x550055, 0x550055}, /* pink */
  {0xAA00AA, 0x5500AA, 0x550055, 0x550055}, /* purple */
  {0x0055AA, 0x0055AA, 0x000055, 0x000055}, /* blue */
  {0x00AAAA, 0x005555, 0x005555, 0x005555}, /* teal */
  {0x00AA55, 0x005555, 0x005500, 0x005500}, /* green */
  {0xAA0000, 0xAA0000, 0x550000, 0x550000}, /* white */
};

static const uint32_t s_baseline[9] = {
  0x550000, /* red/cream/gold */
  0x550000,
  0x550000,
  0x550055, /* pink/purple */
  0x550055,
  0x000055, /* blue */
  0x005555, /* teal */
  0x005500, /* green */
  0x550000, /* white */
};

static const bool s_outlined[9] = {
  true,  /* red    */
  false, /* cream  */
  false, /* gold   */
  true,  /* pink   */
  true,  /* purple */
  true,  /* blue   */
  true,  /* teal   */
  true,  /* green  */
  false, /* white  */
};

ScannerShades scanner_shades(int hour_color) {
  if (hour_color < 0 || hour_color > 8) {
    hour_color = 0;
  }

  ScannerShades s;
  s.cursor = scanner_argb(s_cursor[hour_color]);
  for (int i = 0; i < SCANNER_HEAT_STEPS; i++) {
    s.heat[i] = scanner_argb(s_heat[hour_color][i]);
  }
  s.baseline = scanner_argb(s_baseline[hour_color]);
  return s;
}

bool scanner_hour_outlined(int hour_color) {
  if (hour_color < 0 || hour_color > 8) {
    return false;
  }
  return s_outlined[hour_color];
}
