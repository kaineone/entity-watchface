#include <pebble.h>
#include "settings_store.h"
#include "logic/settings.h"

#define SETTINGS_PERSIST_KEY 1

static Settings s_settings;
static SettingsChangedHandler s_on_change;

static int cstring_to_int(const char *s) {
  int sign = 1;
  int v = 0;
  if (!s) return 0;
  while (*s == ' ') s++;
  if (*s == '-') { sign = -1; s++; }
  while (*s >= '0' && *s <= '9') {
    v = v * 10 + (*s - '0');
    s++;
  }
  return v * sign;
}

static int tuple_int(const Tuple *t) {
  switch (t->length) {
    case 1:
      return (t->type == TUPLE_UINT) ? (int)t->value->uint8 : (int)t->value->int8;
    case 2:
      return (t->type == TUPLE_UINT) ? (int)t->value->uint16 : (int)t->value->int16;
    case 4:
      return (t->type == TUPLE_UINT) ? (int)t->value->uint32 : (int)t->value->int32;
  }
  return 0;
}

static bool set_bool_from_tuple(bool *field, const Tuple *t) {
  if (t->type == TUPLE_CSTRING) {
    return settings_set_bool(field, cstring_to_int(t->value->cstring));
  }
  return settings_set_bool(field, tuple_int(t));
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  (void)context;
  bool changed = false;
  const Tuple *t;

  t = dict_find(iter, MESSAGE_KEY_ClockFormat);
  if (t) {
    if (t->type == TUPLE_CSTRING) {
      changed = settings_set_clock_str(&s_settings, t->value->cstring) || changed;
    } else {
      changed = settings_set_clock(&s_settings, tuple_int(t)) || changed;
    }
  }

  t = dict_find(iter, MESSAGE_KEY_HourColor);
  if (t) {
    if (t->type == TUPLE_CSTRING) {
      changed = settings_set_hour_color_str(&s_settings, t->value->cstring) || changed;
    } else {
      changed = settings_set_hour_color(&s_settings, tuple_int(t)) || changed;
    }
  }

  t = dict_find(iter, MESSAGE_KEY_ShowWeather);
  if (t) changed = set_bool_from_tuple(&s_settings.show_weather, t) || changed;

  t = dict_find(iter, MESSAGE_KEY_Fahrenheit);
  if (t) changed = set_bool_from_tuple(&s_settings.fahrenheit, t) || changed;

  t = dict_find(iter, MESSAGE_KEY_Animate);
  if (t) changed = set_bool_from_tuple(&s_settings.animate, t) || changed;

  t = dict_find(iter, MESSAGE_KEY_VibeOnDisconnect);
  if (t) changed = set_bool_from_tuple(&s_settings.vibe_disconnect, t) || changed;

  t = dict_find(iter, MESSAGE_KEY_TapSwap);
  if (t) changed = set_bool_from_tuple(&s_settings.tap_swap, t) || changed;

  t = dict_find(iter, MESSAGE_KEY_LowBattery);
  if (t) {
    if (t->type == TUPLE_CSTRING) {
      changed = settings_set_low_battery_str(&s_settings, t->value->cstring) || changed;
    } else {
      changed = settings_set_low_battery(&s_settings, tuple_int(t)) || changed;
    }
  }

  if (changed) {
    persist_write_data(SETTINGS_PERSIST_KEY, &s_settings, sizeof(s_settings));
    if (s_on_change) s_on_change(&s_settings);
  }
}

void settings_store_init(SettingsChangedHandler on_change) {
  Settings tmp;
  int len = persist_read_data(SETTINGS_PERSIST_KEY, &tmp, sizeof(tmp));
  if (len != sizeof(tmp) || !settings_valid(&tmp)) {
    settings_defaults(&tmp);
  }
  s_settings = tmp;

  s_on_change = on_change;
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(256, 64);
}

const Settings *settings_store_get(void) {
  return &s_settings;
}
