# Design

## Context
Time display exists (TextLayers, layout table, minute ticks). No services beyond tick and
unobstructed area yet. Research: no app-facing system low-power API exists; quiet time has no
change event, only `quiet_time_is_active()`.

## Goals / Non-Goals

**Goals:** pixel-exact meter, cheapest correct animation loop, all decision logic host-tested.

**Non-Goals:** link glyph, battery text, vibe on disconnect (status-row change); settings UI;
round rim meter.

## Decisions
- **Integer boost table** `{16,13,7,3,1}` replaces `round(16·e^(−d²/5))`: no libm on the watch.
- **Direction flips on the step after an end bar**: step() checks whether peak+dir leaves 0..20
  and flips first. So at bar 20 the trail is still on the left, and the next frame is bar 19 with
  the trail on the right. The ramp never blinks out at the bounce.
- **Colours as an enum** (`MeterInk`: GOLD, RED, HEAT1..HEAT4, BASELINE, DISABLED) in the logic
  module; `meter_layer.c` maps them to `GColor`. Keeps logic Pebble-free.
- **RNG**: xorshift32 inside the `Meter` struct, seeded from `time(NULL)` on the watch and a fixed
  seed in tests.
- **Mode function** `meter_mode(linked, animate_pref, battery_pct, threshold, quiet, peek)` and a
  separate `meter_timer_should_run(mode, focused)`. Focus only pauses the timer (no repaint), so a
  notification doesn't flip the meter to the frozen look.
- **Dithered bars**: fill the bar with heat(d), then draw the cooler pixels with
  `graphics_draw_pixel` where Bayer < 8 (2 of every 4 pixels per row). At most 4 bars × 77 pixels.
- **Inputs**: `connection_service_subscribe` (pebble_app_connection_handler),
  `battery_state_service_subscribe`, `app_focus_service_subscribe_handlers` (did_focus),
  quiet time polled in the minute tick, peek from the existing unobstructed handler.
  A single `meter_refresh_mode()` recomputes the mode and starts/stops the timer.

## Risks / Trade-offs
- Pebble re-renders the whole window when any layer is dirty, so the text layers redraw at 4 fps
  too. The OS only pushes changed rows to the display. Acceptable per design; measure later on
  hardware.
