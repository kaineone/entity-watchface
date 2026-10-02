#include "test.h"
#include "scanner.h"

static void test_argb(void) {
  CHECK_EQ_INT(scanner_argb(0x000000), 0xC0);
  CHECK_EQ_INT(scanner_argb(0xFFFFFF), 0xFF);
  CHECK_EQ_INT(scanner_argb(0xFF0000), 0xF0);
  CHECK_EQ_INT(scanner_argb(0x0055FF), 0xC7);
  CHECK_EQ_INT(scanner_argb(0x00AA55), 0xC9);
}

static const uint32_t s_cursor[9] = {
  0xFF0000, 0xFF0000, 0xFF0000,
  0xFF55AA, 0xAA00FF, 0x0055FF,
  0x00AAAA, 0x00AA55, 0xFF0000
};

static const uint32_t s_heat[9][SCANNER_HEAT_STEPS] = {
  {0xAA0000, 0xAA0000, 0x550000, 0x550000},
  {0xAA0000, 0xAA0000, 0x550000, 0x550000},
  {0xAA0000, 0xAA0000, 0x550000, 0x550000},
  {0xAA55AA, 0xAA5555, 0x550055, 0x550055},
  {0xAA00AA, 0x5500AA, 0x550055, 0x550055},
  {0x0055AA, 0x0055AA, 0x000055, 0x000055},
  {0x00AAAA, 0x005555, 0x005555, 0x005555},
  {0x00AA55, 0x005555, 0x005500, 0x005500},
  {0xAA0000, 0xAA0000, 0x550000, 0x550000}
};

static const uint32_t s_baseline[9] = {
  0x550000, 0x550000, 0x550000,
  0x550055, 0x550055, 0x000055,
  0x005555, 0x005500, 0x550000
};

static const bool s_outlined[9] = {
  true, false, false,
  true, true, true,
  true, true, false
};

static void test_rows(void) {
  for (int i = 0; i <= 8; i++) {
    ScannerShades sh = scanner_shades(i);
    CHECK_EQ_INT(sh.cursor, scanner_argb(s_cursor[i]));
    for (int j = 0; j < SCANNER_HEAT_STEPS; j++) {
      CHECK_EQ_INT(sh.heat[j], scanner_argb(s_heat[i][j]));
    }
    CHECK_EQ_INT(sh.baseline, scanner_argb(s_baseline[i]));
    CHECK_EQ_INT(scanner_hour_outlined(i), s_outlined[i]);
  }
}

static void test_out_of_range(void) {
  ScannerShades red = scanner_shades(0);

  ScannerShades m1 = scanner_shades(-1);
  CHECK_EQ_INT(m1.cursor, red.cursor);
  for (int j = 0; j < SCANNER_HEAT_STEPS; j++) {
    CHECK_EQ_INT(m1.heat[j], red.heat[j]);
  }
  CHECK_EQ_INT(m1.baseline, red.baseline);
  CHECK(!scanner_hour_outlined(-1));

  ScannerShades p9 = scanner_shades(9);
  CHECK_EQ_INT(p9.cursor, red.cursor);
  for (int j = 0; j < SCANNER_HEAT_STEPS; j++) {
    CHECK_EQ_INT(p9.heat[j], red.heat[j]);
  }
  CHECK_EQ_INT(p9.baseline, red.baseline);
  CHECK(!scanner_hour_outlined(9));
}

int main(void) {
  test_argb();
  test_rows();
  test_out_of_range();
  TEST_MAIN_END();
}
