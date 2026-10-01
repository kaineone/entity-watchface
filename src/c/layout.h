#ifndef LAYOUT_H
#define LAYOUT_H

#include <pebble.h>

typedef enum { DIGITS_LARGE, DIGITS_SMALL } DigitSize;

typedef struct {
  GRect date;
  GRect hour;
  GRect ampm;
  GRect minute;
  DigitSize digits;
} FaceFrames;

typedef struct {
  FaceFrames normal;
  FaceFrames peek;
} FaceLayout;

const FaceLayout *layout_get(void);

#endif
