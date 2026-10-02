#include "test.h"
#include "rim.h"

static void check_rest_range(const RimMeter *rm) {
  for (int i = 0; i < RIM_TICKS; i++) {
    CHECK(rm->rest[i] >= RIM_REST_MIN && rm->rest[i] <= RIM_REST_MAX);
  }
}

int main(void) {
  RimMeter rm;
  rim_init(&rm, 4242);
  check_rest_range(&rm);

  for (int j = 0; j < 300; j++) {
    rim_land(&rm, j);
  }
  check_rest_range(&rm);

  /* rim_cursor */
  CHECK_EQ_INT(rim_cursor(0, 0), 0);
  CHECK_EQ_INT(rim_cursor(0, 15), 15);
  CHECK_EQ_INT(rim_cursor(0, 59), 59);
  CHECK_EQ_INT(rim_cursor(1, 0), 0);
  CHECK_EQ_INT(rim_cursor(1, 15), 45);
  CHECK_EQ_INT(rim_cursor(1, 59), 1);
  CHECK_EQ_INT(rim_cursor(2, 30), 30);
  CHECK_EQ_INT(rim_cursor(59, 1), 59);

  /* even minute 2, second 2 */
  {
    int m = 2, s = 2;
    TickStyle st;

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 2);
    CHECK_EQ_INT(st.len, RIM_MAX_LEN);
    CHECK_EQ_INT(st.ink, INK_RED);

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 1);
    CHECK_EQ_INT(st.ink, INK_HEAT1);
    CHECK_EQ_INT(st.len, rm.rest[1] + rim_boost(1));

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 0);
    CHECK_EQ_INT(st.ink, INK_HEAT2);
    CHECK_EQ_INT(st.len, rm.rest[0] + rim_boost(2));

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 59);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[59]);

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 3);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[3]);
  }

  /* odd minute 1, second 2 */
  {
    int m = 1, s = 2;
    TickStyle st;

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 58);
    CHECK_EQ_INT(st.len, RIM_MAX_LEN);
    CHECK_EQ_INT(st.ink, INK_RED);

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 59);
    CHECK_EQ_INT(st.ink, INK_HEAT1);
    CHECK_EQ_INT(st.len, rm.rest[59] + rim_boost(1));

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 0);
    CHECK_EQ_INT(st.ink, INK_HEAT2);
    CHECK_EQ_INT(st.len, rm.rest[0] + rim_boost(2));

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 57);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[57]);
  }

  /* second 0: cursor only, no trail */
  {
    int m = 0, s = 0;
    for (int i = 0; i < RIM_TICKS; i++) {
      TickStyle st = rim_tick(&rm, MODE_ANIMATING, m, s, i);
      if (i == 0) {
        CHECK_EQ_INT(st.ink, INK_RED);
        CHECK_EQ_INT(st.len, RIM_MAX_LEN);
      } else {
        CHECK_EQ_INT(st.ink, INK_GOLD);
        CHECK_EQ_INT(st.len, rm.rest[i]);
      }
    }
  }

  /* second 30, even minute: full trail then back to rest */
  {
    int m = 0, s = 30;
    for (int d = 1; d <= RIM_TRAIL_MAX; d++) {
      int t = (30 - d + RIM_TICKS) % RIM_TICKS;
      TickStyle st = rim_tick(&rm, MODE_ANIMATING, m, s, t);
      CHECK_EQ_INT(st.ink, meter_heat(d));
      CHECK_EQ_INT(st.len, rm.rest[t] + rim_boost(d));
    }
    TickStyle st24 = rim_tick(&rm, MODE_ANIMATING, m, s, 24);
    CHECK_EQ_INT(st24.ink, INK_GOLD);
    CHECK_EQ_INT(st24.len, rm.rest[24]);
  }

  /* trail length formula, under and at cap */
  {
    RimMeter rm2;
    rim_init(&rm2, 4242);
    for (int i = 0; i < RIM_TICKS; i++) rm2.rest[i] = 2;

    int m = 0, s = 30, c = 30;
    TickStyle sc = rim_tick(&rm2, MODE_ANIMATING, m, s, c);
    CHECK_EQ_INT(sc.len, RIM_MAX_LEN);

    for (int d = 1; d <= RIM_TRAIL_MAX; d++) {
      int t = (c - d + RIM_TICKS) % RIM_TICKS;
      TickStyle st = rim_tick(&rm2, MODE_ANIMATING, m, s, t);
      CHECK_EQ_INT(st.len, 2 + rim_boost(d));
      CHECK(st.len <= RIM_MAX_LEN);
    }

    for (int i = 0; i < RIM_TICKS; i++) rm2.rest[i] = 30;

    TickStyle sc2 = rim_tick(&rm2, MODE_ANIMATING, m, s, c);
    CHECK_EQ_INT(sc2.len, RIM_MAX_LEN);
    for (int d = 1; d <= RIM_TRAIL_MAX; d++) {
      int t = (c - d + RIM_TICKS) % RIM_TICKS;
      TickStyle st = rim_tick(&rm2, MODE_ANIMATING, m, s, t);
      CHECK_EQ_INT(st.len, RIM_MAX_LEN);
    }

    int non_trail = (c - RIM_TRAIL_MAX - 1 + RIM_TICKS) % RIM_TICKS;
    TickStyle stnt = rim_tick(&rm2, MODE_ANIMATING, m, s, non_trail);
    CHECK_EQ_INT(stnt.len, 30);
    CHECK_EQ_INT(stnt.ink, INK_GOLD);
  }

  /* UNLINKED and FROZEN */
  {
    for (int i = 0; i < RIM_TICKS; i++) {
      TickStyle stu = rim_tick(&rm, MODE_UNLINKED, 0, 30, i);
      CHECK_EQ_INT(stu.len, RIM_FLAT_LEN);
      CHECK_EQ_INT(stu.ink, INK_DISABLED);

      TickStyle stf = rim_tick(&rm, MODE_FROZEN, 0, 30, i);
      CHECK_EQ_INT(stf.len, RIM_FLAT_LEN);
      CHECK_EQ_INT(stf.ink, INK_GOLD);
    }
  }

  /* odd minute 3, second 0: cursor only, no trail right after the turn */
  {
    int m = 3, s = 0;
    TickStyle st;

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 0);
    CHECK_EQ_INT(st.len, RIM_MAX_LEN);
    CHECK_EQ_INT(st.ink, INK_RED);

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 1);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[1]);
  }

  /* odd minute 3, second 2: cursor and two trailing ticks after the turn */
  {
    int m = 3, s = 2;
    TickStyle st;

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 58);
    CHECK_EQ_INT(st.len, RIM_MAX_LEN);
    CHECK_EQ_INT(st.ink, INK_RED);

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 59);
    CHECK_EQ_INT(st.ink, INK_HEAT1);
    CHECK_EQ_INT(st.len, rm.rest[59] + rim_boost(1));

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 0);
    CHECK_EQ_INT(st.ink, INK_HEAT2);
    CHECK_EQ_INT(st.len, rm.rest[0] + rim_boost(2));

    st = rim_tick(&rm, MODE_ANIMATING, m, s, 57);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[57]);
  }

  TEST_MAIN_END();
}
