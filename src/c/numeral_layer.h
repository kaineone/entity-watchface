#ifndef NUMERAL_LAYER_H
#define NUMERAL_LAYER_H

#include <pebble.h>

Layer *numeral_layer_create(GRect frame, GColor color, int gap, int stroke);
void numeral_layer_destroy(Layer *l);
void numeral_layer_set_text(Layer *l, const char *text);
void numeral_layer_set_color(Layer *l, GColor c);
void numeral_layer_set_metrics(Layer *l, int gap, int stroke);

#endif
