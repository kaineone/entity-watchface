#ifndef STATUS_LAYER_H
#define STATUS_LAYER_H

#include <pebble.h>

Layer *link_layer_create(GRect frame);
void link_layer_destroy(void);
void link_layer_set_linked(bool linked);

Layer *quiet_layer_create(GRect frame);
void quiet_layer_destroy(void);
void quiet_layer_set_visible(bool visible);

#endif
