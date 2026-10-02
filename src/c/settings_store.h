#ifndef SETTINGS_STORE_H
#define SETTINGS_STORE_H

#include "logic/settings.h"

typedef void (*SettingsChangedHandler)(const Settings *s);
typedef void (*WeatherReceivedHandler)(int cond, int temp_c10);
typedef void (*JsReadyHandler)(void);

void settings_store_init(SettingsChangedHandler on_change, WeatherReceivedHandler on_weather,
                         JsReadyHandler on_js_ready);
const Settings *settings_store_get(void);
void settings_store_request_weather(void);

#endif
