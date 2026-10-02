#include "test.h"
#include "glide.h"

static int pos_at_phase(int phase) {
  Glide g;
  glide_init(&g, 1);
  g.phase = (int16_t)phase;
  return glide_pos(&g);
}

int main(void) {
  Glide g;
  glide_init(&g, 12345);

  /* phase landmarks */
  CHECK_EQ_INT(glide_pos(&g), 0);
  g.phase = GLIDE_LEG_FRAMES;
  CHECK_EQ_INT(glide_pos(&g), (METER_BARS - 1) * GLIDE_FP);
  g.phase = GLIDE_PERIOD - 1;
  {
    int p_last = glide_pos(&g);
    CHECK(p_last >= 0 && p_last < GLIDE_FP);
  }

  /* monotonic leg 0 and leg 1 */
  for (int ph = 1; ph < GLIDE_LEG_FRAMES; ph++) {
    CHECK(pos_at_phase(ph) >= pos_at_phase(ph - 1));
  }
  for (int ph = GLIDE_LEG_FRAMES + 1; ph < GLIDE_PERIOD; ph++) {
    CHECK(pos_at_phase(ph) <= pos_at_phase(ph - 1));
  }

  /* easing: end steps smaller than middle step */
  {
    int step0_first = pos_at_phase(1) - pos_at_phase(0);
    int step0_mid   = pos_at_phase(GLIDE_LEG_FRAMES / 2) -
                      pos_at_phase(GLIDE_LEG_FRAMES / 2 - 1);
    int step0_last  = pos_at_phase(GLIDE_LEG_FRAMES - 1) -
                      pos_at_phase(GLIDE_LEG_FRAMES - 2);
    CHECK(step0_first < step0_mid);
    CHECK(step0_last  < step0_mid);
  }
  {
    int step1_first = pos_at_phase(GLIDE_LEG_FRAMES) -
                      pos_at_phase(GLIDE_LEG_FRAMES + 1);
    if (step1_first < 0) step1_first = -step1_first;
    int step1_mid   = pos_at_phase(GLIDE_LEG_FRAMES + GLIDE_LEG_FRAMES / 2 - 1) -
                      pos_at_phase(GLIDE_LEG_FRAMES + GLIDE_LEG_FRAMES / 2);
    if (step1_mid < 0) step1_mid = -step1_mid;
    int step1_last  = pos_at_phase(GLIDE_PERIOD - 2) -
                      pos_at_phase(GLIDE_PERIOD - 1);
    if (step1_last < 0) step1_last = -step1_last;
    CHECK(step1_first < step1_mid);
    CHECK(step1_last  < step1_mid);
  }

  /* direction per leg */
  for (int ph = 0; ph < GLIDE_LEG_FRAMES; ph++) {
    g.phase = (int16_t)ph;
    CHECK_EQ_INT(glide_dir(&g), 1);
  }
  for (int ph = GLIDE_LEG_FRAMES; ph < GLIDE_PERIOD; ph++) {
    g.phase = (int16_t)ph;
    CHECK_EQ_INT(glide_dir(&g), -1);
  }

  /* amplitude envelope */
  for (int left = 20; left >= 0; left--) {
    Glide ga;
    glide_init(&ga, 99);
    glide_frame(&ga, left, 20);
    int elapsed = 20 - left;
    int expected;
    if (elapsed < GLIDE_FADE_IN) {
      expected = GLIDE_FP * (elapsed + 1) / GLIDE_FADE_IN;
    } else if (left <= GLIDE_FADE_OUT) {
      expected = GLIDE_FP * left / GLIDE_FADE_OUT;
    } else {
      expected = GLIDE_FP;
    }
    CHECK_EQ_INT(ga.amp, expected);
  }

  /* rest range after many frames */
  {
    Glide gr;
    glide_init(&gr, 77);
    for (int f = 0; f < 1000; f++) {
      glide_frame(&gr, 1000, 2000);
    }
    for (int i = 0; i < METER_BARS; i++) {
      CHECK(gr.rest[i] >= METER_REST_MIN && gr.rest[i] <= METER_REST_MAX);
    }
  }

  /* idle style */
  g.phase = 15;
  g.amp = GLIDE_FP;
  for (int i = 0; i < METER_BARS; i++) {
    BarStyle st = glide_bar(&g, MODE_ANIMATING, false, i);
    CHECK_EQ_INT(st.height, g.rest[i]);
    CHECK_EQ_INT(st.ink, INK_GOLD);
    CHECK_EQ_INT(st.ink_cool, INK_GOLD);
    CHECK(!st.dither);
  }

  /* UNLINKED and FROZEN */
  {
    BarStyle su = glide_bar(&g, MODE_UNLINKED, true, 10);
    CHECK_EQ_INT(su.height, METER_FLAT_H);
    CHECK_EQ_INT(su.ink, INK_DISABLED);
    CHECK_EQ_INT(su.ink_cool, INK_DISABLED);
    CHECK(!su.dither);

    BarStyle sf = glide_bar(&g, MODE_FROZEN, true, 10);
    CHECK_EQ_INT(sf.height, METER_FLAT_H);
    CHECK_EQ_INT(sf.ink, INK_GOLD);
    CHECK_EQ_INT(sf.ink_cool, INK_GOLD);
    CHECK(!sf.dither);
  }

  /* full-amplitude burst: one RED, bounded, behind bars heated */
  {
    Glide gb;
    glide_init(&gb, 33);
    gb.amp = GLIDE_FP;
    gb.phase = 15;
    int red = 0;
    int p = glide_pos(&gb);
    int nearest = (p + GLIDE_FP / 2) / GLIDE_FP;
    int dir = glide_dir(&gb);
    for (int i = 0; i < METER_BARS; i++) {
      BarStyle st = glide_bar(&gb, MODE_ANIMATING, true, i);
      CHECK(st.height <= METER_MAX_H);
      if (st.ink == INK_RED) red++;

      int x = i * GLIDE_FP - p;
      if (x < 0) x = -x;
      bool behind = (i - nearest) * dir < 0;
      int d = (x + GLIDE_FP / 2) / GLIDE_FP;
      if (behind && d <= 4) {
        CHECK(st.ink != INK_GOLD);
      }
      if (!behind && i != nearest) {
        CHECK(st.ink == INK_GOLD);
      }
    }
    CHECK_EQ_INT(red, 1);
  }

  /* zero amplitude: no RED, all GOLD, heights at rest */
  {
    Glide gz;
    glide_init(&gz, 55);
    gz.amp = 0;
    gz.phase = 15;
    for (int i = 0; i < METER_BARS; i++) {
      BarStyle st = glide_bar(&gz, MODE_ANIMATING, true, i);
      CHECK(st.ink != INK_RED);
      CHECK(st.ink == INK_GOLD);
      CHECK_EQ_INT(st.height, gz.rest[i]);
      CHECK(!st.dither);
    }
  }

  TEST_MAIN_END();
}
