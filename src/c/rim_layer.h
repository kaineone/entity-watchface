#ifndef RIM_LAYER_H
#define RIM_LAYER_H

#include <pebble.h>
#include "logic/rim.h"

Layer *rim_layer_create(GRect frame);
void rim_layer_destroy(void);
void rim_layer_set_mode(MeterMode mode);
void rim_layer_set_bursting(bool bursting);
void rim_layer_start(void);
void rim_layer_frame(int frames_left, int frames_total);

#endif
