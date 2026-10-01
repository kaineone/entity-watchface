#ifndef SETTINGS_STORE_H
#define SETTINGS_STORE_H

#include "logic/settings.h"

typedef void (*SettingsChangedHandler)(const Settings *s);
typedef void (*WeatherReceivedHandler)(int cond, int temp_c10);

void settings_store_init(SettingsChangedHandler on_change, WeatherReceivedHandler on_weather);
const Settings *settings_store_get(void);
void settings_store_request_weather(void);

#endif
