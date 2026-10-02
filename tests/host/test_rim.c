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
  CHECK_EQ_INT(rm.phase, 0);
  CHECK_EQ_INT(rm.amp, 0);
  CHECK_EQ_INT(rm.last_nearest, -1);

  for (int j = 0; j < 300; j++) {
    rim_land(&rm, j);
  }
  check_rest_range(&rm);

  /* rim_pos at key phases */
  rm.phase = 0;
  CHECK_EQ_INT(rim_pos(&rm), 0);
  rm.phase = 25;
  CHECK_EQ_INT(rim_pos(&rm), 30 * RIM_FP);
  rm.phase = 50;
  CHECK_EQ_INT(rim_pos(&rm), 60 * RIM_FP);
  rm.phase = 75;
  CHECK_EQ_INT(rim_pos(&rm), 30 * RIM_FP);

  /* monotonic lap and direction */
  {
    int prev = -1;
    for (int ph = 0; ph <= 50; ph++) {
      rm.phase = ph;
      int pos = rim_pos(&rm);
      CHECK(pos >= prev);
      prev = pos;
    }
    for (int ph = 50; ph <= 99; ph++) {
      rm.phase = ph;
      int pos = rim_pos(&rm);
      CHECK(pos <= prev);
      prev = pos;
    }
    rm.phase = 0;
    CHECK_EQ_INT(rim_dir(&rm), 1);
    rm.phase = 49;
    CHECK_EQ_INT(rim_dir(&rm), 1);
    rm.phase = 50;
    CHECK_EQ_INT(rim_dir(&rm), -1);
    rm.phase = 99;
    CHECK_EQ_INT(rim_dir(&rm), -1);
  }

  /* one-lap burst: 50 frames from 12 back to 12 clockwise */
  {
    int first_amp = -1;
    rim_start(&rm);
    for (int left = 50; left >= 1; left--) {
      rim_frame(&rm, left, 50);
      if (left == 50) first_amp = rm.amp;
    }
    CHECK_EQ_INT(rm.phase, 50);
    CHECK_EQ_INT(rim_pos(&rm), RIM_TICKS * RIM_FP);
    CHECK_EQ_INT(first_amp, RIM_FP / 5);
    CHECK_EQ_INT(rm.amp, RIM_FP / 10);
  }

  /* modes */
  {
    rim_init(&rm, 4242);
    for (int i = 0; i < RIM_TICKS; i++) {
      TickStyle st = rim_tick(&rm, MODE_UNLINKED, false, i);
      CHECK_EQ_INT(st.len, RIM_FLAT_LEN);
      CHECK_EQ_INT(st.ink, INK_DISABLED);

      st = rim_tick(&rm, MODE_FROZEN, false, i);
      CHECK_EQ_INT(st.len, RIM_FLAT_LEN);
      CHECK_EQ_INT(st.ink, INK_GOLD);

      st = rim_tick(&rm, MODE_ANIMATING, false, i);
      CHECK_EQ_INT(st.len, rm.rest[i]);
      CHECK_EQ_INT(st.ink, INK_GOLD);

      RimMeter r2 = rm;
      r2.amp = 0;
      st = rim_tick(&r2, MODE_ANIMATING, true, i);
      CHECK_EQ_INT(st.len, r2.rest[i]);
      CHECK_EQ_INT(st.ink, INK_GOLD);
    }
  }

  /* halfway: phase 25, full amplitude */
  {
    rim_init(&rm, 4242);
    rm.phase = 25;
    rm.amp = RIM_FP;

    TickStyle st = rim_tick(&rm, MODE_ANIMATING, true, 30);
    CHECK_EQ_INT(st.ink, INK_RED);
    CHECK_EQ_INT(st.len, RIM_MAX_LEN);

    st = rim_tick(&rm, MODE_ANIMATING, true, 29);
    CHECK_EQ_INT(st.ink, INK_HEAT1);
    CHECK_EQ_INT(st.len, rm.rest[29] + 10);

    st = rim_tick(&rm, MODE_ANIMATING, true, 28);
    CHECK_EQ_INT(st.ink, INK_HEAT2);
    CHECK_EQ_INT(st.len, rm.rest[28] + 6);

    st = rim_tick(&rm, MODE_ANIMATING, true, 27);
    CHECK_EQ_INT(st.ink, INK_HEAT3);
    CHECK_EQ_INT(st.len, rm.rest[27] + 3);

    st = rim_tick(&rm, MODE_ANIMATING, true, 26);
    CHECK_EQ_INT(st.ink, INK_HEAT4);
    CHECK_EQ_INT(st.len, rm.rest[26] + 1);

    st = rim_tick(&rm, MODE_ANIMATING, true, 25);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[25]);

    st = rim_tick(&rm, MODE_ANIMATING, true, 31);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[31]);
  }

  /* just after the turn, clockwise */
  {
    rim_init(&rm, 4242);
    rm.amp = RIM_FP;
    int ph_found = -1;
    for (int ph = 1; ph <= 20 && ph_found < 0; ph++) {
      rm.phase = ph;
      int nearest = (rim_pos(&rm) + RIM_FP / 2) / RIM_FP;
      if (nearest == 2) ph_found = ph;
    }
    CHECK(ph_found > 0);
    rm.phase = ph_found;
    TickStyle st = rim_tick(&rm, MODE_ANIMATING, true, 2);
    CHECK_EQ_INT(st.ink, INK_RED);
    CHECK_EQ_INT(st.len, RIM_MAX_LEN);

    st = rim_tick(&rm, MODE_ANIMATING, true, 1);
    CHECK(st.ink != INK_GOLD);
    CHECK(st.len > rm.rest[1]);

    st = rim_tick(&rm, MODE_ANIMATING, true, 0);
    CHECK(st.ink != INK_GOLD);
    CHECK(st.len > rm.rest[0]);

    st = rim_tick(&rm, MODE_ANIMATING, true, 59);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[59]);

    st = rim_tick(&rm, MODE_ANIMATING, true, 58);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[58]);
  }

  /* just after the turn, counter-clockwise */
  {
    rim_init(&rm, 4242);
    rm.amp = RIM_FP;
    int ph_found = -1;
    for (int ph = 51; ph <= 70 && ph_found < 0; ph++) {
      rm.phase = ph;
      int nearest = (rim_pos(&rm) + RIM_FP / 2) / RIM_FP;
      if (nearest == 58) ph_found = ph;
    }
    CHECK(ph_found > 0);
    rm.phase = ph_found;
    TickStyle st = rim_tick(&rm, MODE_ANIMATING, true, 58);
    CHECK_EQ_INT(st.ink, INK_RED);
    CHECK_EQ_INT(st.len, RIM_MAX_LEN);

    st = rim_tick(&rm, MODE_ANIMATING, true, 59);
    CHECK(st.ink != INK_GOLD);
    CHECK(st.len > rm.rest[59]);

    st = rim_tick(&rm, MODE_ANIMATING, true, 0);
    CHECK(st.ink != INK_GOLD);
    CHECK(st.len > rm.rest[0]);

    st = rim_tick(&rm, MODE_ANIMATING, true, 1);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[1]);

    st = rim_tick(&rm, MODE_ANIMATING, true, 2);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.len, rm.rest[2]);
  }

  /* ahead growth within one tick in front of the cursor */
  {
    rim_init(&rm, 4242);
    rm.amp = RIM_FP;
    for (int k = 1; k <= 3; k++) {
      rm.phase = 25 + k;
      int p = rim_pos(&rm);
      int n = (p + RIM_FP / 2) / RIM_FP;
      int a = n + 1;
      int diff = a * RIM_FP - p;
      if (diff < RIM_FP && diff >= 0) {
        int idx = a % RIM_TICKS;
        TickStyle st = rim_tick(&rm, MODE_ANIMATING, true, idx);
        CHECK_EQ_INT(st.ink, INK_HEAT1);
        CHECK(st.len > rm.rest[idx]);
      }
    }
  }

  /* no backward steps over the clockwise lap */
  {
    int prev = -1;
    for (int ph = 0; ph <= 50; ph++) {
      rm.phase = ph;
      int nearest = (rim_pos(&rm) + RIM_FP / 2) / RIM_FP;
      CHECK(nearest >= prev);
      prev = nearest;
    }
  }

  TEST_MAIN_END();
}
