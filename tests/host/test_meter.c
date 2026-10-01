#include "test.h"
#include "meter.h"

static void test_rest_bounds(void) {
  Meter m;
  meter_init(&m, 12345);
  for (int i = 0; i < METER_BARS; i++) {
    CHECK(m.rest[i] >= METER_REST_MIN && m.rest[i] <= METER_REST_MAX);
  }
  for (int step = 0; step < 200; step++) {
    meter_step(&m);
  }
  for (int i = 0; i < METER_BARS; i++) {
    CHECK(m.rest[i] >= METER_REST_MIN && m.rest[i] <= METER_REST_MAX);
  }
}

static void test_bounce(void) {
  Meter m;
  meter_init(&m, 12345);
  for (int i = 0; i < 20; i++) meter_step(&m);
  CHECK_EQ_INT(m.peak, 20);
  CHECK_EQ_INT(m.dir, 1);
  meter_step(&m);
  CHECK_EQ_INT(m.peak, 19);
  CHECK_EQ_INT(m.dir, -1);
  for (int i = 0; i < 19; i++) meter_step(&m);
  CHECK_EQ_INT(m.peak, 0);
  CHECK_EQ_INT(m.dir, -1);
  meter_step(&m);
  CHECK_EQ_INT(m.peak, 1);
  CHECK_EQ_INT(m.dir, 1);
}

static void test_right_ramp(void) {
  Meter m;
  meter_init(&m, 12345);
  m.peak = 10;
  m.dir = 1;
  for (int i = 6; i <= 9; i++) {
    BarStyle s = meter_bar(&m, MODE_ANIMATING, i);
    int d = i - 10;
    CHECK_EQ_INT(s.ink, meter_heat(-d));
    CHECK(s.dither);
  }
  BarStyle b9 = meter_bar(&m, MODE_ANIMATING, 9);
  BarStyle b6 = meter_bar(&m, MODE_ANIMATING, 6);
  CHECK_EQ_INT(b9.ink, INK_HEAT1);
  CHECK_EQ_INT(b6.ink, INK_HEAT4);
  BarStyle b5 = meter_bar(&m, MODE_ANIMATING, 5);
  CHECK_EQ_INT(b5.ink, INK_GOLD);
  CHECK(!b5.dither);
  for (int i = 11; i <= 20; i++) {
    BarStyle s = meter_bar(&m, MODE_ANIMATING, i);
    CHECK_EQ_INT(s.height, m.rest[i]);
    CHECK_EQ_INT(s.ink, INK_GOLD);
  }
}

static void test_left_ramp(void) {
  Meter m;
  meter_init(&m, 12345);
  m.peak = 10;
  m.dir = -1;
  for (int i = 11; i <= 14; i++) {
    BarStyle s = meter_bar(&m, MODE_ANIMATING, i);
    CHECK_EQ_INT(s.ink, meter_heat(i - 10));
    CHECK(s.dither);
  }
  for (int i = 0; i <= 9; i++) {
    BarStyle s = meter_bar(&m, MODE_ANIMATING, i);
    CHECK_EQ_INT(s.height, m.rest[i]);
    CHECK_EQ_INT(s.ink, INK_GOLD);
  }
}

static void test_heights_and_cursor(void) {
  Meter m;
  meter_init(&m, 12345);
  m.peak = 5;
  m.dir = 1;
  BarStyle cur = meter_bar(&m, MODE_ANIMATING, 5);
  CHECK_EQ_INT(cur.height, METER_MAX_H);
  CHECK_EQ_INT(cur.ink, INK_RED);
  m.rest[3] = 2;
  BarStyle b3 = meter_bar(&m, MODE_ANIMATING, 3);
  CHECK_EQ_INT(b3.height, 2 + meter_boost(2));
  m.rest[4] = METER_REST_MAX;
  BarStyle b4 = meter_bar(&m, MODE_ANIMATING, 4);
  CHECK_EQ_INT(b4.height, METER_MAX_H);
  for (int i = 0; i < METER_BARS; i++) {
    BarStyle s = meter_bar(&m, MODE_ANIMATING, i);
    CHECK(s.height <= METER_MAX_H);
  }
}

static void test_mode_styles(void) {
  Meter m;
  meter_init(&m, 12345);
  for (int i = 0; i < METER_BARS; i++) {
    BarStyle u = meter_bar(&m, MODE_UNLINKED, i);
    CHECK_EQ_INT(u.height, METER_FLAT_H);
    CHECK_EQ_INT(u.ink, INK_DISABLED);
    CHECK_EQ_INT(u.ink_cool, INK_DISABLED);
    CHECK(!u.dither);
    BarStyle f = meter_bar(&m, MODE_FROZEN, i);
    CHECK_EQ_INT(f.height, METER_FLAT_H);
    CHECK_EQ_INT(f.ink, INK_GOLD);
    CHECK_EQ_INT(f.ink_cool, INK_GOLD);
    CHECK(!f.dither);
  }
  CHECK_EQ_INT(meter_baseline_ink(MODE_UNLINKED), INK_DISABLED);
  CHECK_EQ_INT(meter_baseline_ink(MODE_FROZEN), INK_BASELINE);
  CHECK_EQ_INT(meter_baseline_ink(MODE_ANIMATING), INK_BASELINE);
}

static void test_meter_mode(void) {
  CHECK_EQ_INT(meter_mode(false, true, 0, 0, false, false, false), MODE_UNLINKED);
  CHECK_EQ_INT(meter_mode(false, true, 5, 10, false, false, false), MODE_UNLINKED);
  CHECK_EQ_INT(meter_mode(true, true, 20, 20, false, false, false), MODE_FROZEN);
  CHECK_EQ_INT(meter_mode(true, true, 21, 20, false, false, false), MODE_ANIMATING);
  CHECK_EQ_INT(meter_mode(true, true, 100, 0, false, true, false), MODE_FROZEN);
  CHECK_EQ_INT(meter_mode(true, true, 100, 0, false, false, true), MODE_FROZEN);
  CHECK_EQ_INT(meter_mode(true, false, 100, 0, false, false, false), MODE_FROZEN);
  CHECK_EQ_INT(meter_mode(true, true, 15, 20, true, false, false), MODE_ANIMATING);
  CHECK_EQ_INT(meter_mode(true, true, 15, 20, true, true, false), MODE_FROZEN);
  CHECK_EQ_INT(meter_mode(false, true, 15, 20, true, false, false), MODE_UNLINKED);
}

static void test_timer_should_run(void) {
  CHECK(meter_timer_should_run(MODE_ANIMATING, true));
  CHECK(!meter_timer_should_run(MODE_ANIMATING, false));
  CHECK(!meter_timer_should_run(MODE_FROZEN, true));
  CHECK(!meter_timer_should_run(MODE_FROZEN, false));
  CHECK(!meter_timer_should_run(MODE_UNLINKED, true));
  CHECK(!meter_timer_should_run(MODE_UNLINKED, false));
}

static void test_bayer(void) {
  CHECK(meter_bayer_cool(0, 0));
  CHECK(!meter_bayer_cool(1, 0));
  CHECK(!meter_bayer_cool(0, 1));
  CHECK(meter_bayer_cool(1, 1));
  CHECK_EQ_INT(meter_bayer_cool(4, 4), meter_bayer_cool(0, 0));
  int count = 0;
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (meter_bayer_cool(x, y)) count++;
    }
  }
  CHECK_EQ_INT(count, 8);
}

static void test_bayer_value(void) {
  CHECK_EQ_INT(meter_bayer_value(0, 0), 0);
  CHECK_EQ_INT(meter_bayer_value(1, 0), 8);
  CHECK_EQ_INT(meter_bayer_value(3, 3), 5);
  CHECK_EQ_INT(meter_bayer_value(4, 5), 12);
  CHECK_EQ_INT(meter_bayer_value(-1, 0), 10);
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      CHECK_EQ_INT(meter_bayer_cool(x, y), meter_bayer_value(x, y) < 8);
    }
  }
}

static void test_ink_density(void) {
  CHECK_EQ_INT(meter_ink_density(INK_RED), 16);
  CHECK_EQ_INT(meter_ink_density(INK_HEAT1), 14);
  CHECK_EQ_INT(meter_ink_density(INK_HEAT2), 12);
  CHECK_EQ_INT(meter_ink_density(INK_HEAT3), 10);
  CHECK_EQ_INT(meter_ink_density(INK_HEAT4), 9);
  CHECK_EQ_INT(meter_ink_density(INK_GOLD), 8);
  CHECK_EQ_INT(meter_ink_density(INK_BASELINE), 4);
  CHECK_EQ_INT(meter_ink_density(INK_DISABLED), 2);
}

int main(void) {
  test_rest_bounds();
  test_bounce();
  test_right_ramp();
  test_left_ramp();
  test_heights_and_cursor();
  test_mode_styles();
  test_meter_mode();
  test_timer_should_run();
  test_bayer();
  test_bayer_value();
  test_ink_density();
  TEST_MAIN_END();
}
