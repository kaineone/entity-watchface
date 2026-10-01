#include "layout.h"

#if defined(PBL_ROUND)
/* placeholder until the round change */
static const FaceLayout s_layout = {
  .normal = {
    .date   = GRect(8, 8, 120, 16),
    .hour   = GRect(8, 30, 184, 64),
    .ampm   = GRect(140, 36, 52, 16),
    .minute = GRect(8, 96, 184, 64),
    .meter  = GRect(6, 176, 189, 26),
    .digits = DIGITS_LARGE
  },
  .peek = {
    .date   = GRect(8, 8, 120, 16),
    .hour   = GRect(8, 30, 184, 48),
    .ampm   = GRect(140, 36, 52, 16),
    .minute = GRect(8, 80, 184, 48),
    .meter  = GRect(6, 176, 189, 26),
    .digits = DIGITS_SMALL
  }
};
#else
static const FaceLayout s_layout = {
  .normal = {
    .date   = GRect(8, 8, 120, 16),
    .hour   = GRect(8, 30, 184, 64),
    .ampm   = GRect(140, 36, 52, 16),
    .minute = GRect(8, 96, 184, 64),
    .meter  = GRect(6, 176, 189, 26),
    .digits = DIGITS_LARGE
  },
  .peek = {
    .date   = GRect(8, 8, 120, 16),
    .hour   = GRect(8, 30, 184, 48),
    .ampm   = GRect(140, 36, 52, 16),
    .minute = GRect(8, 80, 184, 48),
    .meter  = GRect(6, 176, 189, 26),
    .digits = DIGITS_SMALL
  }
};
#endif

const FaceLayout *layout_get(void) {
  return &s_layout;
}
