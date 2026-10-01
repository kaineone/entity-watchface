#include <pebble.h>
#include "logic/fmt.h"
#include "layout.h"
#include "palette.h"
#include "meter_layer.h"

#include <stdint.h>

static Window *s_window;
static TextLayer *s_date_layer;
static TextLayer *s_hour_layer;
static TextLayer *s_ampm_layer;
static TextLayer *s_minute_layer;
static Layer *s_meter_layer;

static GFont s_font_large;
static GFont s_font_small;
static GFont s_font_label;

static char s_date_buf[FMT_DATE_LEN];
static char s_hour_buf[FMT_HOUR_LEN];
static char s_minute_buf[FMT_MINUTE_LEN];
static char s_ampm_buf[3];

static GColor s_hour_color = PAL_HOUR_RED;
static int8_t s_hour12_pref = -1;

static int s_last_yday = -1;
static bool s_peek = false;
static bool s_layout_applied = false;

static bool s_linked = false;
static bool s_quiet = false;
static bool s_focused = true;
static bool s_animate_pref = true;
static int s_battery_pct = 100;
static int s_battery_threshold = 20;

static AppTimer *s_meter_timer = NULL;
static MeterMode s_meter_mode = MODE_FROZEN;

static void meter_refresh(void);

static bool is_24h(void) {
  return s_hour12_pref == 0 || (s_hour12_pref == -1 && clock_is_24h_style());
}

static void update_time(struct tm *tick_time) {
  char temp[3];

  const bool use24 = is_24h();
  const int hour24 = tick_time->tm_hour;

  fmt_hour(temp, sizeof(temp), hour24, use24);
  if (strcmp(s_hour_buf, temp) != 0) {
    strcpy(s_hour_buf, temp);
    if (s_hour_layer) text_layer_set_text(s_hour_layer, s_hour_buf);
  }

  fmt_minute(temp, sizeof(temp), tick_time->tm_min);
  if (strcmp(s_minute_buf, temp) != 0) {
    strcpy(s_minute_buf, temp);
    if (s_minute_layer) text_layer_set_text(s_minute_layer, s_minute_buf);
  }

  if (s_ampm_layer) {
    layer_set_hidden(text_layer_get_layer(s_ampm_layer), use24);
    if (!use24) {
      snprintf(temp, sizeof(temp), "%s", fmt_ampm(hour24));
      if (strcmp(s_ampm_buf, temp) != 0) {
        strcpy(s_ampm_buf, temp);
        text_layer_set_text(s_ampm_layer, s_ampm_buf);
      }
    }
  }

  if (tick_time->tm_yday != s_last_yday) {
    char dt[FMT_DATE_LEN];
    fmt_date(dt, sizeof(dt), tick_time->tm_wday, tick_time->tm_mday, tick_time->tm_mon);
    if (strcmp(s_date_buf, dt) != 0) {
      strcpy(s_date_buf, dt);
      if (s_date_layer) text_layer_set_text(s_date_layer, s_date_buf);
    }
    s_last_yday = tick_time->tm_yday;
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  (void)units_changed;
  update_time(tick_time);
  s_quiet = quiet_time_is_active();
  meter_refresh();
}

static void apply_layout(bool peek) {
  if (s_layout_applied && peek == s_peek) return;
  s_peek = peek;
  s_layout_applied = true;

  const FaceFrames *frames = peek ? &layout_get()->peek : &layout_get()->normal;

  if (s_date_layer) layer_set_frame(text_layer_get_layer(s_date_layer), frames->date);
  if (s_hour_layer) layer_set_frame(text_layer_get_layer(s_hour_layer), frames->hour);
  if (s_ampm_layer) layer_set_frame(text_layer_get_layer(s_ampm_layer), frames->ampm);
  if (s_minute_layer) layer_set_frame(text_layer_get_layer(s_minute_layer), frames->minute);

  if (s_hour_layer) {
    text_layer_set_font(s_hour_layer, frames->digits == DIGITS_LARGE ? s_font_large : s_font_small);
  }
  if (s_minute_layer) {
    text_layer_set_font(s_minute_layer, frames->digits == DIGITS_LARGE ? s_font_large : s_font_small);
  }

  if (s_meter_layer) {
    layer_set_frame(s_meter_layer, frames->meter);
    layer_set_hidden(s_meter_layer, peek);
  }
}

static void unobstructed_did_change(void *context) {
  (void)context;
  Layer *root = window_get_root_layer(s_window);
  apply_layout(layer_get_unobstructed_bounds(root).size.h < layer_get_bounds(root).size.h);
  meter_refresh();
}

static TextLayer *make_layer(GRect frame, GTextAlignment align, GFont font, GColor text_color) {
  TextLayer *layer = text_layer_create(frame);
  if (!layer) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "text_layer_create failed");
    return NULL;
  }
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, text_color);
  text_layer_set_text_alignment(layer, align);
  text_layer_set_font(layer, font);
  text_layer_set_overflow_mode(layer, GTextOverflowModeTrailingEllipsis);
  return layer;
}

static void handle_connection(bool connected) {
  s_linked = connected;
  meter_refresh();
}

static void handle_battery(BatteryChargeState charge) {
  s_battery_pct = charge.charge_percent;
  meter_refresh();
}

static void handle_focus(bool in_focus) {
  s_focused = in_focus;
  meter_refresh();
}

static void meter_timer_cb(void *data) {
  (void)data;
  s_meter_timer = NULL;
  meter_layer_step();
  if (meter_timer_should_run(s_meter_mode, s_focused)) {
    s_meter_timer = app_timer_register(250, meter_timer_cb, NULL);
  }
}

static void meter_refresh(void) {
  MeterMode mode = meter_mode(s_linked, s_animate_pref, s_battery_pct,
                              s_battery_threshold, s_quiet, s_peek);
  meter_layer_set_mode(mode);
  s_meter_mode = mode;

  bool run = meter_timer_should_run(mode, s_focused);
  if (run && !s_meter_timer) {
    s_meter_timer = app_timer_register(250, meter_timer_cb, NULL);
  } else if (!run && s_meter_timer) {
    app_timer_cancel(s_meter_timer);
    s_meter_timer = NULL;
  }
}

static void window_load(Window *window) {
  (void)window;

  s_font_large = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ZEN_64));
  s_font_small = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ZEN_48));
  s_font_label = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_LABEL_16));
  if (!s_font_large || !s_font_small || !s_font_label) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "font load failed");
  }

  Layer *root = window_get_root_layer(s_window);
  const FaceFrames *frames = &layout_get()->normal;

  s_date_layer = make_layer(frames->date, GTextAlignmentLeft, s_font_label, PAL_GOLD);
  s_hour_layer = make_layer(frames->hour, GTextAlignmentLeft, s_font_large, s_hour_color);
  s_ampm_layer = make_layer(frames->ampm, GTextAlignmentRight, s_font_label, PAL_GOLD);
  s_minute_layer = make_layer(frames->minute, GTextAlignmentRight, s_font_large, PAL_GOLD);
  s_meter_layer = meter_layer_create(frames->meter);

  if (!s_date_layer || !s_hour_layer || !s_ampm_layer || !s_minute_layer || !s_meter_layer) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "layer create failed");
    return;
  }

  layer_add_child(root, text_layer_get_layer(s_date_layer));
  layer_add_child(root, text_layer_get_layer(s_hour_layer));
  layer_add_child(root, text_layer_get_layer(s_ampm_layer));
  layer_add_child(root, text_layer_get_layer(s_minute_layer));
  layer_add_child(root, s_meter_layer);

  const bool peek = layer_get_unobstructed_bounds(root).size.h < layer_get_bounds(root).size.h;
  apply_layout(peek);

  time_t now = time(NULL);
  update_time(localtime(&now));

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  unobstructed_area_service_subscribe((UnobstructedAreaHandlers) {
    .did_change = unobstructed_did_change
  }, NULL);

  s_linked = connection_service_peek_pebble_app_connection();
  s_battery_pct = battery_state_service_peek().charge_percent;
  s_quiet = quiet_time_is_active();

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = handle_connection
  });
  battery_state_service_subscribe(handle_battery);
  app_focus_service_subscribe_handlers((AppFocusHandlers) {
    .did_focus = handle_focus
  });

  meter_refresh();
}

static void window_unload(Window *window) {
  (void)window;

  if (s_meter_timer) {
    app_timer_cancel(s_meter_timer);
    s_meter_timer = NULL;
  }

  connection_service_unsubscribe();
  battery_state_service_unsubscribe();
  app_focus_service_unsubscribe();

  meter_layer_destroy();
  s_meter_layer = NULL;

  if (s_date_layer) {
    text_layer_destroy(s_date_layer);
    s_date_layer = NULL;
  }
  if (s_hour_layer) {
    text_layer_destroy(s_hour_layer);
    s_hour_layer = NULL;
  }
  if (s_ampm_layer) {
    text_layer_destroy(s_ampm_layer);
    s_ampm_layer = NULL;
  }
  if (s_minute_layer) {
    text_layer_destroy(s_minute_layer);
    s_minute_layer = NULL;
  }

  if (s_font_large) {
    fonts_unload_custom_font(s_font_large);
    s_font_large = NULL;
  }
  if (s_font_small) {
    fonts_unload_custom_font(s_font_small);
    s_font_small = NULL;
  }
  if (s_font_label) {
    fonts_unload_custom_font(s_font_label);
    s_font_label = NULL;
  }
}

static void init(void) {
  s_window = window_create();
  if (!s_window) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "window_create failed");
    return;
  }

  window_set_background_color(s_window, GColorBlack);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(s_window, true);
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  unobstructed_area_service_unsubscribe();
  if (s_window) {
    window_destroy(s_window);
    s_window = NULL;
  }
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
