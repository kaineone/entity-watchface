# Design

## Context
Rect face complete. `layout.c` has a placeholder round table equal to emery. Research: gabbro has
no documented quick view (the SDK header limits Peek to rectangular platforms); code keeps reading
unobstructed bounds anyway.

## Goals / Non-Goals
**Goals:** spec round face, rim logic host-tested, no emery regressions.
**Non-Goals:** quick-view layout for round (no peek on round); chalk.

## Decisions
- **Boost table for the rim** from round(10·e^(−d²/5)): {10, 8, 4, 2, 0}; d 0 is the cursor (18).
- **Trail as a range, not a direction test**: trail ticks are cursor−1..cursor−k on even minutes
  and cursor+1..cursor+k on odd minutes (mod 60), k = min(5, s). This gives "never wraps through 12"
  for free and needs no stored direction.
- **`rim.c`** (Pebble-free): `RimMeter {rest[60], rng}`, `rim_init`, `rim_cursor(min, sec)`,
  `rim_land(rm, tick)`, `rim_tick(rm, mode, min, sec, i)` → `{len, ink}`. Inks reuse `MeterInk`.
- **`rim_layer.c`**: full-screen layer; endpoints from `sin_lookup`/`cos_lookup` with
  `TRIG_MAX_ANGLE * i / 60`; outer point at r 127, inner at r 127 − len; `graphics_draw_line`,
  antialiasing off, stroke width 1.
- **Ticks**: one handler for both units. `meter_refresh()` decides the unit on round: SECOND_UNIT
  while `meter_timer_should_run` holds, else MINUTE_UNIT; re-subscribe only on change. The minute
  work runs when `units_changed & MINUTE_UNIT`. On rect the AppTimer path is unchanged.
- **Layout table** gains `GTextAlignment` per text field and a `digits_large`/`digits_small`
  resource choice; fonts load from the table (ZEN_60/ZEN_48 on round, ZEN_64/ZEN_48 on emery).
- **Weather fit loop** only runs when the date and weather frames share a y.
