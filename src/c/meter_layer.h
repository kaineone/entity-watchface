#ifndef METER_LAYER_H
#define METER_LAYER_H

#include <pebble.h>
#include "logic/meter.h"
#include "logic/scanner.h"

Layer *meter_layer_create(GRect frame);
void meter_layer_destroy(void);
void meter_layer_set_mode(MeterMode mode);
void meter_layer_set_bursting(bool bursting);
void meter_layer_frame(int frames_left, int frames_total);
void meter_layer_set_shades(const ScannerShades *s);

#endif
