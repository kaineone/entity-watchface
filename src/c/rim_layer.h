#ifndef RIM_LAYER_H
#define RIM_LAYER_H

#include <pebble.h>
#include "logic/rim.h"

Layer *rim_layer_create(GRect frame);
void rim_layer_destroy(void);
void rim_layer_set_mode(MeterMode mode);
void rim_layer_set_time(int minute, int second);

#endif
