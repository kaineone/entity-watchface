#include <pebble.h>
#include "settings_store.h"
#include "logic/settings.h"
#include "logic/weather.h"

#define SETTINGS_PERSIST_KEY 1

static Settings s_settings;
static SettingsChangedHandler s_on_change;
static WeatherReceivedHandler s_on_weather;
static JsReadyHandler s_on_js_ready;

static bool tuple_is_valid_int(const Tuple *t) {
  return t && (t->type == TUPLE_INT || t->type == TUPLE_UINT) &&
         (t->length == 1 || t->length == 2 || t->length == 4);
}

static bool tuple_is_valid_string(const Tuple *t) {
  if (!t || t->type != TUPLE_CSTRING || t->length == 0) return false;
  const char *str = t->value->cstring;  /* cstring is a zero-length array in the SDK */
  return str[t->length - 1] == '\0';
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
  if (tuple_is_valid_int(t)) {
    return settings_set_bool(field, tuple_int(t));
  }
  if (tuple_is_valid_string(t)) {
    const char *s = t->value->cstring;
    if (strcmp(s, "0") == 0) return settings_set_bool(field, 0);
    if (strcmp(s, "1") == 0) return settings_set_bool(field, 1);
  }
  return false;
}

void settings_store_request_weather(void) {
  DictionaryIterator *iter;
  if (app_message_outbox_begin(&iter) != APP_MSG_OK) return;
  (void)dict_write_uint8(iter, MESSAGE_KEY_WeatherRequest, 1);
  (void)app_message_outbox_send();
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  (void)context;
  bool changed = false;
  const Tuple *t;

  t = dict_find(iter, MESSAGE_KEY_ClockFormat);
  if (t) {
    if (tuple_is_valid_string(t)) {
      changed = settings_set_clock_str(&s_settings, t->value->cstring) || changed;
    } else if (tuple_is_valid_int(t)) {
      changed = settings_set_clock(&s_settings, tuple_int(t)) || changed;
    }
  }

  t = dict_find(iter, MESSAGE_KEY_HourColor);
  if (t) {
    if (tuple_is_valid_string(t)) {
      changed = settings_set_hour_color_str(&s_settings, t->value->cstring) || changed;
    } else if (tuple_is_valid_int(t)) {
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
    if (tuple_is_valid_string(t)) {
      changed = settings_set_low_battery_str(&s_settings, t->value->cstring) || changed;
    } else if (tuple_is_valid_int(t)) {
      changed = settings_set_low_battery(&s_settings, tuple_int(t)) || changed;
    }
  }

  if (changed) {
    persist_write_data(SETTINGS_PERSIST_KEY, &s_settings, sizeof(s_settings));
    if (s_on_change) s_on_change(&s_settings);
  }

  const Tuple *wc = dict_find(iter, MESSAGE_KEY_WeatherCond);
  const Tuple *wt = dict_find(iter, MESSAGE_KEY_WeatherTempC10);
  if (wc && wt && s_on_weather && tuple_is_valid_int(wc) && tuple_is_valid_int(wt)) {
    int cond = tuple_int(wc);
    int temp_c10 = tuple_int(wt);
    if (weather_valid(cond, temp_c10)) {
      s_on_weather(cond, temp_c10);
    }
  }

  const Tuple *jr = dict_find(iter, MESSAGE_KEY_JsReady);
  if (jr && s_on_js_ready) {
    s_on_js_ready();
  }
}

void settings_store_init(SettingsChangedHandler on_change, WeatherReceivedHandler on_weather,
                         JsReadyHandler on_js_ready) {
  Settings tmp;
  int len = persist_read_data(SETTINGS_PERSIST_KEY, &tmp, sizeof(tmp));
  if (len != sizeof(tmp) || !settings_valid(&tmp)) {
    settings_defaults(&tmp);
  }
  s_settings = tmp;

  s_on_change = on_change;
  s_on_weather = on_weather;
  s_on_js_ready = on_js_ready;
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(256, 64);
}

const Settings *settings_store_get(void) {
  return &s_settings;
}
