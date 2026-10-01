#ifndef METER_LAYER_H
#define METER_LAYER_H

#include <pebble.h>
#include "logic/meter.h"

Layer *meter_layer_create(GRect frame);
void meter_layer_destroy(void);
void meter_layer_set_mode(MeterMode mode);
void meter_layer_step(void);

#endif
