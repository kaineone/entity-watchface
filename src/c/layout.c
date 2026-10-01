#include "layout.h"

#if defined(PBL_ROUND)
static const FaceLayout s_layout = {
  .normal = {
    .date    = GRect(8, 8, 120, 16),
    .hour    = GRect(8, 30, 184, 64),
    .ampm    = GRect(140, 36, 52, 16),
    .minute  = GRect(8, 96, 184, 64),
    .meter   = GRect(6, 176, 189, 26),
    .link    = GRect(8, 208, 22, 14),
    .power   = GRect(120, 206, 72, 16),
    .quiet   = GRect(8, 104, 8, 8),
    .weather = GRect(72, 8, 120, 16),
    .digits  = DIGITS_LARGE
  },
  .peek = {
    .date    = GRect(8, 8, 120, 16),
    .hour    = GRect(8, 30, 184, 48),
    .ampm    = GRect(140, 36, 52, 16),
    .minute  = GRect(8, 80, 184, 48),
    .meter   = GRect(6, 176, 189, 26),
    .link    = GRect(8, 146, 22, 14),
    .power   = GRect(120, 144, 72, 16),
    .quiet   = GRect(8, 104, 8, 8),
    .weather = GRect(72, 8, 120, 16),
    .digits  = DIGITS_SMALL
  }
};
#else
static const FaceLayout s_layout = {
  .normal = {
    .date    = GRect(8, 8, 120, 16),
    .hour    = GRect(8, 30, 184, 64),
    .ampm    = GRect(140, 36, 52, 16),
    .minute  = GRect(8, 96, 184, 64),
    .meter   = GRect(6, 176, 189, 26),
    .link    = GRect(8, 208, 22, 14),
    .power   = GRect(120, 206, 72, 16),
    .quiet   = GRect(8, 104, 8, 8),
    .weather = GRect(72, 8, 120, 16),
    .digits  = DIGITS_LARGE
  },
  .peek = {
    .date    = GRect(8, 8, 120, 16),
    .hour    = GRect(8, 30, 184, 48),
    .ampm    = GRect(140, 36, 52, 16),
    .minute  = GRect(8, 80, 184, 48),
    .meter   = GRect(6, 176, 189, 26),
    .link    = GRect(8, 146, 22, 14),
    .power   = GRect(120, 144, 72, 16),
    .quiet   = GRect(8, 104, 8, 8),
    .weather = GRect(72, 8, 120, 16),
    .digits  = DIGITS_SMALL
  }
};
#endif

const FaceLayout *layout_get(void) {
  return &s_layout;
}
