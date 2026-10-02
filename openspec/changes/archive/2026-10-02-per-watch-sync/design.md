# Design

## Decisions
- **Startup message** (`startup.js`, pure, node-tested): reads the stored Clay settings JSON, keeps
  only the eight setting keys, turns booleans into 1/0, keeps numbers and strings, drops anything
  else, and adds `JsReady: 1`. Keys are message-key names, which PebbleKit JS maps itself.
- **Watch side**: `settings_store` calls a new `on_js_ready` handler after applying any settings in
  the same message. `main.c` requests weather when weather is shown and
  `weather_is_stale(now, s_wx_time, WEATHER_REFRESH_SECS)` (900 s); a missing reading has time 0,
  which is always stale.
- **Phone side**: `ready` sends the startup message (one retry after 3 s if it fails) and starts the
  30-minute forced refresh. The `entity-last-weather` throttle is removed.
