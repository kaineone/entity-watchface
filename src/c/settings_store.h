#ifndef SETTINGS_STORE_H
#define SETTINGS_STORE_H

#include "logic/settings.h"

typedef void (*SettingsChangedHandler)(const Settings *s);

void settings_store_init(SettingsChangedHandler on_change);
const Settings *settings_store_get(void);

#endif
