#include <pebble.h>
#include <stdint.h>
#include <string.h>
#include "logic/fmt.h"
#include "logic/status.h"
#include "logic/weather.h"
#include "layout.h"
#include "palette.h"
#include "meter_layer.h"
#include "status_layer.h"
#include "settings_store.h"

#if defined(PBL_ROUND)
#include "rim_layer.h"
static Layer *meter_view_create(GRect frame) { return rim_layer_create(frame); }
static void meter_view_destroy(void) { rim_layer_destroy(); }
static void meter_view_set_mode(MeterMode mode) { rim_layer_set_mode(mode); }
#else
static Layer *meter_view_create(GRect frame) { return meter_layer_create(frame); }
static void meter_view_destroy(void) { meter_layer_destroy(); }
static void meter_view_set_mode(MeterMode mode) { meter_layer_set_mode(mode); }
#endif

#define WX_PERSIST_KEY 2

typedef struct __attribute__((__packed__)) {
  uint8_t version;
  int8_t cond;
  int16_t temp_c10;
  int32_t time;
} WxPersist;

static Window *s_window;
static TextLayer *s_date_layer;
static TextLayer *s_hour_layer;
static TextLayer *s_ampm_layer;
static TextLayer *s_minute_layer;
static TextLayer *s_power_layer;
static TextLayer *s_weather_layer;
static Layer *s_meter_layer;
static Layer *s_link_layer;
static Layer *s_quiet_layer;

static GFont s_font_large;
static GFont s_font_small;
static GFont s_font_label;

static bool s_font_large_custom = false;
static bool s_font_small_custom = false;
static bool s_font_label_custom = false;

static char s_date_buf[FMT_STEPS_LEN];
static char s_hour_buf[FMT_HOUR_LEN];
static char s_minute_buf[FMT_MINUTE_LEN];
static char s_ampm_buf[3];
static char s_power_buf[STATUS_POWER_LEN];
static char s_weather_buf[WEATHER_TEXT_LEN > FMT_BPM_LEN ? WEATHER_TEXT_LEN : FMT_BPM_LEN];

static GColor s_hour_color = PAL_HOUR_RED;
static int8_t s_hour12_pref = -1;

static int s_last_daykey = -1;
static bool s_peek = false;
static bool s_layout_applied = false;

static bool s_linked = false;
static bool s_quiet = false;
static bool s_focused = true;
static bool s_animate_pref = true;
static bool s_charging = false;
static bool s_vibe_pref = true;
static bool s_show_weather = true;
static bool s_fahrenheit = false;
static bool s_wx_color_set = false;
static bool s_wx_stale_drawn = false;

static int s_battery_pct = 100;
static int s_battery_threshold = 20;
static StatusInk s_power_ink;
static bool s_power_ink_set = false;
static GRect s_power_frame;
static GTextAlignment s_power_align;

static int s_wx_cond = 0;
static int s_wx_temp_c10 = 0;
static int32_t s_wx_time = 0;

static AppTimer *s_meter_timer = NULL;
static MeterMode s_meter_mode = MODE_FROZEN;

#if defined(PBL_ROUND)
static TimeUnits s_tick_units = MINUTE_UNIT;
#endif

static bool s_tap_pref = true;
static bool s_swapped = false;
static AppTimer *s_swap_timer = NULL;
static bool s_tap_subscribed = false;

static void meter_refresh(void);
static void update_tap_subscription(void);
static void swap_out(void);
static void handle_tap(AccelAxisType axis, int32_t direction);

static bool is_24h(void) {
  return s_hour12_pref == 0 || (s_hour12_pref == -1 && clock_is_24h_style());
}

static void update_weather(void) {
  if (!s_weather_layer) return;
  if (s_swapped) return;

  layer_set_hidden(text_layer_get_layer(s_weather_layer), !s_show_weather);
  if (!s_show_weather) return;

  bool stale = false;
  char text[WEATHER_TEXT_LEN];
  if (s_wx_time == 0) {
    text[0] = '\0';
  } else {
    stale = weather_is_stale((int32_t)time(NULL), s_wx_time, WEATHER_STALE_SECS);

    int date_w = s_date_layer ? text_layer_get_content_size(s_date_layer).w : 0;
    GRect date_frame = s_date_layer ? layer_get_frame(text_layer_get_layer(s_date_layer)) : GRectZero;
    GRect weather_frame = layer_get_frame(text_layer_get_layer(s_weather_layer));
    int max_w = weather_frame.origin.x + weather_frame.size.w - (date_frame.origin.x + date_w + 4);

    if (date_frame.origin.y == weather_frame.origin.y) {
#if defined(PBL_BW)
      const int levels[3] = {0, 2, 3};
      for (int idx = 0; idx < 3; idx++) {
        int level = levels[idx];
        weather_text_variant(text, sizeof(text), s_wx_cond, s_wx_temp_c10, s_fahrenheit, stale, level);
        if ((level == 2 || level == 3) && stale) {
          memmove(text + 1, text, strlen(text) + 1);
          text[0] = '~';
        }
        GSize size = graphics_text_layout_get_content_size(text, s_font_label, GRect(0, 0, 200, weather_frame.size.h),
                                                           GTextOverflowModeTrailingEllipsis,
                                                           GTextAlignmentRight);
        if (size.w <= max_w) break;
      }
#else
      for (int level = 0; level <= 3; level++) {
        weather_text_variant(text, sizeof(text), s_wx_cond, s_wx_temp_c10, s_fahrenheit, stale, level);
        GSize size = graphics_text_layout_get_content_size(text, s_font_label, GRect(0, 0, 200, weather_frame.size.h),
                                                           GTextOverflowModeTrailingEllipsis,
                                                           GTextAlignmentRight);
        if (size.w <= max_w) break;
      }
#endif
    } else {
      weather_text_variant(text, sizeof(text), s_wx_cond, s_wx_temp_c10, s_fahrenheit, stale, 0);
    }
  }

  if (strcmp(s_weather_buf, text) != 0) {
    strcpy(s_weather_buf, text);
    text_layer_set_text(s_weather_layer, s_weather_buf);
  }

  bool draw_stale = stale && s_wx_time != 0;
  if (!s_wx_color_set || draw_stale != s_wx_stale_drawn) {
    s_wx_stale_drawn = draw_stale;
    s_wx_color_set = true;
    text_layer_set_text_color(s_weather_layer, draw_stale ? PAL_DISABLED : PAL_ACCENT);
  }
}

static void on_weather_received(int cond, int temp_c10) {
  if (!weather_valid(cond, temp_c10)) return;

  s_wx_cond = cond;
  s_wx_temp_c10 = temp_c10;
  s_wx_time = (int32_t)time(NULL);

  WxPersist p = {
    .version = 1,
    .cond = (int8_t)cond,
    .temp_c10 = (int16_t)temp_c10,
    .time = s_wx_time
  };
  (void)persist_write_data(WX_PERSIST_KEY, &p, sizeof(p));

  update_weather();
}

static void update_power(void) {
  char tmp[STATUS_POWER_LEN];
  status_power_text(tmp, sizeof(tmp), s_battery_pct, s_charging);

  bool text_changed = strcmp(s_power_buf, tmp) != 0;
  if (text_changed) {
    strcpy(s_power_buf, tmp);
    if (s_power_layer) text_layer_set_text(s_power_layer, s_power_buf);
  }

  StatusInk ink = status_power_ink(s_battery_pct, s_battery_threshold, s_charging);
  bool ink_changed = !s_power_ink_set || ink != s_power_ink;
  if (ink_changed) {
    s_power_ink = ink;
    s_power_ink_set = true;
  }

  if (s_power_layer) {
#if defined(PBL_BW)
    if (ink == STATUS_RED) {
      layer_set_frame(text_layer_get_layer(s_power_layer), s_power_frame);
      GSize size = text_layer_get_content_size(s_power_layer);
      int w = size.w;
      int right = s_power_frame.origin.x + s_power_frame.size.w;
      GRect r = GRect(right - (w + 4), s_power_frame.origin.y, w + 4, s_power_frame.size.h);
      layer_set_frame(text_layer_get_layer(s_power_layer), r);
      text_layer_set_text_alignment(s_power_layer, GTextAlignmentCenter);
      text_layer_set_background_color(s_power_layer, GColorWhite);
      text_layer_set_text_color(s_power_layer, GColorBlack);
    } else {
      layer_set_frame(text_layer_get_layer(s_power_layer), s_power_frame);
      text_layer_set_text_alignment(s_power_layer, s_power_align);
      text_layer_set_background_color(s_power_layer, GColorClear);
      text_layer_set_text_color(s_power_layer, GColorWhite);
    }
#else
    if (ink_changed) {
      GColor c = PAL_GOLD;
      if (ink == STATUS_RED) c = PAL_RED;
      else if (ink == STATUS_ACCENT) c = PAL_ACCENT;
      text_layer_set_text_color(s_power_layer, c);
    }
#endif
  }
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

  bool date_changed = false;
  if (!s_swapped) {
    int daykey = tick_time->tm_year * 400 + tick_time->tm_yday;
    if (daykey != s_last_daykey) {
      char dt[FMT_DATE_LEN];
      fmt_date(dt, sizeof(dt), tick_time->tm_wday, tick_time->tm_mday, tick_time->tm_mon);
      if (strcmp(s_date_buf, dt) != 0) {
        strcpy(s_date_buf, dt);
        if (s_date_layer) text_layer_set_text(s_date_layer, s_date_buf);
        date_changed = true;
      }
      s_last_daykey = daykey;
    }
  }
  if (date_changed) update_weather();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & MINUTE_UNIT) {
    update_time(tick_time);
    meter_refresh();
    update_weather();
  }
#if defined(PBL_ROUND)
  if (s_tick_units == SECOND_UNIT) {
    rim_layer_set_time(tick_time->tm_min, tick_time->tm_sec);
  }
#endif
}

static void apply_layout(bool peek) {
  if (s_layout_applied && peek == s_peek) return;
  s_peek = peek;
  s_layout_applied = true;

  const FaceFrames *frames = peek ? &layout_get()->peek : &layout_get()->normal;

  if (s_date_layer) {
    layer_set_frame(text_layer_get_layer(s_date_layer), frames->date);
    text_layer_set_text_alignment(s_date_layer, frames->date_align);
  }
  if (s_hour_layer) {
    layer_set_frame(text_layer_get_layer(s_hour_layer), frames->hour);
    text_layer_set_text_alignment(s_hour_layer, frames->hour_align);
  }
  if (s_ampm_layer) {
    layer_set_frame(text_layer_get_layer(s_ampm_layer), frames->ampm);
    text_layer_set_text_alignment(s_ampm_layer, frames->ampm_align);
  }
  if (s_minute_layer) {
    layer_set_frame(text_layer_get_layer(s_minute_layer), frames->minute);
    text_layer_set_text_alignment(s_minute_layer, frames->minute_align);
  }
  if (s_power_layer) {
    layer_set_frame(text_layer_get_layer(s_power_layer), frames->power);
    text_layer_set_text_alignment(s_power_layer, frames->power_align);
    s_power_frame = frames->power;
    s_power_align = frames->power_align;
  }
  if (s_weather_layer) {
    layer_set_frame(text_layer_get_layer(s_weather_layer), frames->weather);
    text_layer_set_text_alignment(s_weather_layer, frames->weather_align);
  }
  if (s_link_layer) layer_set_frame(s_link_layer, frames->link);
  if (s_quiet_layer) layer_set_frame(s_quiet_layer, frames->quiet);

  if (s_hour_layer) {
    text_layer_set_font(s_hour_layer, frames->digits == DIGITS_LARGE ? s_font_large : s_font_small);
  }
  if (s_minute_layer) {
    text_layer_set_font(s_minute_layer, frames->digits == DIGITS_LARGE ? s_font_large : s_font_small);
  }

  if (s_meter_layer) {
    layer_set_frame(s_meter_layer, frames->meter);
    if (layout_get()->hide_meter_in_peek) {
      layer_set_hidden(s_meter_layer, peek);
    }
  }
}

static void unobstructed_did_change(void *context) {
  (void)context;
  Layer *root = window_get_root_layer(s_window);
  apply_layout(layer_get_unobstructed_bounds(root).size.h < layer_get_bounds(root).size.h);
  update_power();  /* re-fit the black-and-white low-battery box to the new frame */
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
  bool was = s_linked;
  s_linked = connected;
  if (status_should_vibrate(was, connected, s_vibe_pref, quiet_time_is_active())) {
    vibes_short_pulse();
  }
  link_layer_set_linked(connected);
  meter_refresh();
}

static void handle_battery(BatteryChargeState charge) {
  s_battery_pct = charge.charge_percent;
  s_charging = charge.is_charging || charge.is_plugged;
  update_power();
  meter_refresh();
}

static void handle_focus(bool in_focus) {
  s_focused = in_focus;
  meter_refresh();
}

#if !defined(PBL_ROUND)
static void meter_timer_cb(void *data) {
  (void)data;
  s_meter_timer = NULL;
  meter_layer_step();
  if (meter_timer_should_run(s_meter_mode, s_focused)) {
    s_meter_timer = app_timer_register(250, meter_timer_cb, NULL);
  }
}
#endif

static void meter_refresh(void) {
  s_quiet = quiet_time_is_active();
  quiet_layer_set_visible(s_quiet);

  MeterMode mode = meter_mode(s_linked, s_animate_pref, s_battery_pct,
                              s_battery_threshold, s_charging, s_quiet, s_peek);
  s_meter_mode = mode;
  meter_view_set_mode(mode);

#if defined(PBL_ROUND)
  TimeUnits want = meter_timer_should_run(mode, s_focused) ? SECOND_UNIT : MINUTE_UNIT;
  if (want != s_tick_units) {
    s_tick_units = want;
    tick_timer_service_subscribe(want, tick_handler);
    if (want == SECOND_UNIT) {
      time_t now = time(NULL);
      struct tm *t = localtime(&now);
      rim_layer_set_time(t->tm_min, t->tm_sec);
    }
  }
#else
  bool run = meter_timer_should_run(mode, s_focused);
  if (run && !s_meter_timer) {
    s_meter_timer = app_timer_register(250, meter_timer_cb, NULL);
  } else if (!run && s_meter_timer) {
    app_timer_cancel(s_meter_timer);
    s_meter_timer = NULL;
  }
#endif
}

static long read_steps(void) {
  time_t start = time_start_of_today();
  time_t end = time(NULL);
  if (health_service_metric_accessible(HealthMetricStepCount, start, end) &
      HealthServiceAccessibilityMaskAvailable) {
    return (long)health_service_sum_today(HealthMetricStepCount);
  }
  return -1;
}

#if !defined(PBL_ROUND)
static long read_bpm(void) {
  time_t end = time(NULL);
  time_t start = end;  /* the HR guide checks (now, now) before peeking the current value */
  if (health_service_metric_accessible(HealthMetricHeartRateBPM, start, end) &
      HealthServiceAccessibilityMaskAvailable) {
    return (long)health_service_peek_current_value(HealthMetricHeartRateBPM);
  }
  return -1;
}
#endif

static void swap_timer_cb(void *data) {
  (void)data;
  s_swap_timer = NULL;
  swap_out();
}

static void swap_in(void) {
  s_swapped = true;

  char tmp[FMT_STEPS_LEN];
  long steps = read_steps();
  fmt_steps(tmp, sizeof(tmp), steps);
  if (strcmp(s_date_buf, tmp) != 0) {
    strcpy(s_date_buf, tmp);
    if (s_date_layer) text_layer_set_text(s_date_layer, s_date_buf);
  }

#if !defined(PBL_ROUND)
  /* Round Pebbles have no heart-rate sensor: keep the weather on the bottom row. */
  char wx_tmp[FMT_BPM_LEN];
  long bpm = read_bpm();
  fmt_bpm(wx_tmp, sizeof(wx_tmp), bpm);
  if (strcmp(s_weather_buf, wx_tmp) != 0) {
    strcpy(s_weather_buf, wx_tmp);
    if (s_weather_layer) text_layer_set_text(s_weather_layer, s_weather_buf);
  }
  if (s_weather_layer) {
    layer_set_hidden(text_layer_get_layer(s_weather_layer), false);
    text_layer_set_text_color(s_weather_layer, PAL_ACCENT);
  }
  s_wx_color_set = false;
#endif

  if (s_swap_timer) app_timer_cancel(s_swap_timer);
  s_swap_timer = app_timer_register(10000, swap_timer_cb, NULL);
}

static void swap_out(void) {
  if (s_swap_timer) {
    app_timer_cancel(s_swap_timer);
    s_swap_timer = NULL;
  }
  s_swapped = false;
  s_date_buf[0] = '\0';
  s_last_daykey = -1;
  s_weather_buf[0] = '\0';
  s_wx_color_set = false;
  time_t now = time(NULL);
  update_time(localtime(&now));
  update_weather();
}

static void handle_tap(AccelAxisType axis, int32_t direction) {
  (void)axis;
  (void)direction;
  if (!s_focused) return;
  if (!s_tap_pref) return;
  if (s_swapped) swap_out(); else swap_in();
}

static void update_tap_subscription(void) {
  if (s_tap_pref && !s_tap_subscribed) {
    accel_tap_service_subscribe(handle_tap);
    s_tap_subscribed = true;
  } else if (!s_tap_pref && s_tap_subscribed) {
    accel_tap_service_unsubscribe();
    s_tap_subscribed = false;
    if (s_swapped) swap_out();
  }
}

static void apply_settings(const Settings *s, bool redraw) {
  bool was_show = s_show_weather;

  s_hour12_pref = s->clock;

  switch (s->hour_color) {
    case HOUR_CREAM: s_hour_color = PAL_CREAM; break;
    case HOUR_GOLD: s_hour_color = PAL_ACCENT; break;
    default: s_hour_color = PAL_HOUR_RED; break;
  }

  s_animate_pref = s->animate;
  s_vibe_pref = s->vibe_disconnect;
  s_battery_threshold = s->low_battery;
  s_show_weather = s->show_weather;
  s_fahrenheit = s->fahrenheit;
  s_tap_pref = s->tap_swap;

  if (redraw) {
    if (s_hour_layer) text_layer_set_text_color(s_hour_layer, s_hour_color);
    s_hour_buf[0] = '\0';
    time_t now = time(NULL);
    update_time(localtime(&now));
    update_power();
    update_weather();
    meter_refresh();

    if (!was_show && s_show_weather) {
      settings_store_request_weather();
    }
  }

  if (s_date_layer) update_tap_subscription();
}

static void on_settings_changed(const Settings *s) {
  apply_settings(s, true);
}

static void window_load(Window *window) {
  (void)window;

  const FaceLayout *layout = layout_get();
  ResHandle large_handle = resource_get_handle(layout->font_large_res);
  ResHandle small_handle = resource_get_handle(layout->font_small_res);

  if (layout->font_large_res == layout->font_small_res) {
    s_font_large = fonts_load_custom_font(large_handle);
    s_font_small = s_font_large;
    s_font_large_custom = (s_font_large != NULL);
    s_font_small_custom = false;
  } else {
    s_font_large = fonts_load_custom_font(large_handle);
    s_font_small = fonts_load_custom_font(small_handle);
    s_font_large_custom = (s_font_large != NULL);
    s_font_small_custom = (s_font_small != NULL);
  }

  s_font_label = fonts_load_custom_font(resource_get_handle(layout->label_font_res));
  s_font_label_custom = (s_font_label != NULL);

  if (!s_font_large || !s_font_small || !s_font_label) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "font load failed");
  }
  if (!s_font_large) {
    s_font_large = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    s_font_large_custom = false;
  }
  if (!s_font_small) {
    s_font_small = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    s_font_small_custom = false;
  }
  if (!s_font_label) {
    s_font_label = fonts_get_system_font(FONT_KEY_GOTHIC_14);
    s_font_label_custom = false;
  }

  Layer *root = window_get_root_layer(s_window);
  const FaceFrames *frames = &layout_get()->normal;

  s_date_layer = make_layer(frames->date, GTextAlignmentLeft, s_font_label, PAL_GOLD);
  s_hour_layer = make_layer(frames->hour, GTextAlignmentLeft, s_font_large, s_hour_color);
  s_ampm_layer = make_layer(frames->ampm, GTextAlignmentRight, s_font_label, PAL_GOLD);
  s_minute_layer = make_layer(frames->minute, GTextAlignmentRight, s_font_large, PAL_GOLD);
  s_meter_layer = meter_view_create(frames->meter);
  s_power_layer = make_layer(frames->power, GTextAlignmentRight, s_font_label, PAL_GOLD);
  s_weather_layer = make_layer(frames->weather, GTextAlignmentRight, s_font_label, PAL_ACCENT);
  s_link_layer = link_layer_create(frames->link);
  s_quiet_layer = quiet_layer_create(frames->quiet);

  if (!s_date_layer || !s_hour_layer || !s_ampm_layer || !s_minute_layer ||
      !s_meter_layer || !s_power_layer || !s_weather_layer || !s_link_layer || !s_quiet_layer) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "layer create failed");
    return;
  }

  layer_add_child(root, text_layer_get_layer(s_date_layer));
  layer_add_child(root, text_layer_get_layer(s_hour_layer));
  layer_add_child(root, text_layer_get_layer(s_ampm_layer));
  layer_add_child(root, text_layer_get_layer(s_minute_layer));
  layer_add_child(root, s_meter_layer);
  layer_add_child(root, text_layer_get_layer(s_power_layer));
  layer_add_child(root, text_layer_get_layer(s_weather_layer));
  layer_add_child(root, s_link_layer);
  layer_add_child(root, s_quiet_layer);

  const bool peek = layer_get_unobstructed_bounds(root).size.h < layer_get_bounds(root).size.h;
  apply_layout(peek);

  time_t now = time(NULL);
  update_time(localtime(&now));

  s_linked = connection_service_peek_pebble_app_connection();
  BatteryChargeState charge = battery_state_service_peek();
  s_battery_pct = charge.charge_percent;
  s_charging = charge.is_charging || charge.is_plugged;
  s_quiet = quiet_time_is_active();

  update_power();
  link_layer_set_linked(s_linked);
  quiet_layer_set_visible(s_quiet);
  update_weather();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
#if defined(PBL_ROUND)
  s_tick_units = MINUTE_UNIT;
#endif

  unobstructed_area_service_subscribe((UnobstructedAreaHandlers) {
    .did_change = unobstructed_did_change
  }, NULL);

  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = handle_connection
  });
  battery_state_service_subscribe(handle_battery);
  app_focus_service_subscribe_handlers((AppFocusHandlers) {
    .did_focus = handle_focus
  });

  update_tap_subscription();
  meter_refresh();
}

static void window_unload(Window *window) {
  (void)window;

  if (s_swap_timer) {
    app_timer_cancel(s_swap_timer);
    s_swap_timer = NULL;
  }
  if (s_tap_subscribed) {
    accel_tap_service_unsubscribe();
    s_tap_subscribed = false;
  }
  s_swapped = false;

  if (s_meter_timer) {
    app_timer_cancel(s_meter_timer);
    s_meter_timer = NULL;
  }

  connection_service_unsubscribe();
  battery_state_service_unsubscribe();
  app_focus_service_unsubscribe();

  meter_view_destroy();
  s_meter_layer = NULL;

  link_layer_destroy();
  s_link_layer = NULL;
  quiet_layer_destroy();
  s_quiet_layer = NULL;

  if (s_power_layer) {
    text_layer_destroy(s_power_layer);
    s_power_layer = NULL;
  }
  if (s_weather_layer) {
    text_layer_destroy(s_weather_layer);
    s_weather_layer = NULL;
  }
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

  if (s_font_large && s_font_large_custom) {
    fonts_unload_custom_font(s_font_large);
  }
  if (s_font_small && s_font_small_custom && s_font_small != s_font_large) {
    fonts_unload_custom_font(s_font_small);
  }
  if (s_font_label && s_font_label_custom) {
    fonts_unload_custom_font(s_font_label);
  }
  s_font_large = NULL;
  s_font_small = NULL;
  s_font_label = NULL;
  s_font_large_custom = false;
  s_font_small_custom = false;
  s_font_label_custom = false;
}

static void load_weather_persist(void) {
  WxPersist p;
  int len = persist_read_data(WX_PERSIST_KEY, &p, sizeof(p));
  int32_t now = (int32_t)time(NULL);
  if (len == sizeof(p) && p.version == 1 &&
      weather_valid(p.cond, p.temp_c10) &&
      p.time <= now + WEATHER_FUTURE_SLACK) {
    s_wx_cond = p.cond;
    s_wx_temp_c10 = p.temp_c10;
    s_wx_time = p.time;
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

  settings_store_init(on_settings_changed, on_weather_received);
  apply_settings(settings_store_get(), false);
  load_weather_persist();

  window_stack_push(s_window, true);
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  unobstructed_area_service_unsubscribe();
  app_message_deregister_callbacks();
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
