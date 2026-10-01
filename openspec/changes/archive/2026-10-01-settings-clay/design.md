# Design

## Context
main.c holds settings as statics with defaults (s_hour12_pref, s_hour_color, s_animate_pref,
s_vibe_pref, s_battery_threshold). No AppMessage or pkjs yet.

## Goals / Non-Goals
**Goals:** standard Clay flow, validated settings, one persisted blob, immediate apply.
**Non-Goals:** weather and tap behaviour (later changes consume the stored values); custom Clay CSS.

## Decisions
- **Clay with auto-handling** (`new Clay(config)`): Clay opens the page, stores values phone-side
  and sends them over AppMessage using the message keys. No custom pkjs logic beyond construction.
- **Message keys**: ClockFormat, HourColor, ShowWeather, Fahrenheit, Animate, VibeOnDisconnect,
  TapSwap, LowBattery. Selects send strings ("-1","0","1"; "red","cream","gold"; "10".."50");
  toggles send integers. The watch reads either form: string tuples via `atoi` or name lookup.
- **Pebble-free `settings.c`**: `Settings` struct, `settings_defaults`, and one setter per key that
  validates (`settings_set_clock`, `_hour_color`, `_low_battery`, `_bool`), returning whether the
  value changed. Host-tested.
- **Persistence**: `persist_write_data(PERSIST_SETTINGS=1, &settings, sizeof)` with a leading
  `version` byte; a version mismatch or short read falls back to defaults. Written once per
  received message, only if something changed.
- **AppMessage** opened at init with inbox 256 bytes, outbox 64 bytes (weather requests later).
- **Charging**: `meter_mode` gains a `charging` argument; low battery freezes only when not charging.

## Risks / Trade-offs
- Clay select values arrive as strings; handling both string and int tuples guards against Clay
  version differences.
