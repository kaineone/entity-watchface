# Design

## Decisions
- **Clock-driven cursor**: `rim_cursor(minute, second)` and `rim_tick(rm, mode, minute, second, i)`
  (already on main and host-tested) drive the rim. The trail covers min(5, s) ticks since the turn.
- **Tick unit follows the mode**: `update_tick_subscription()` in main.c picks SECOND_UNIT on round
  when the meter mode is ANIMATING and the face has focus, else MINUTE_UNIT, and resubscribes only
  when that changes. `meter_refresh()` calls it, so every freeze trigger (battery, quiet time,
  quick view, unlink, animate-off, focus) switches the unit. On switching to seconds the rim is
  set to the current time at once.
- **Tick handler** sets the rim time on every tick (round), then runs the minute work when
  MINUTE_UNIT changed.
- **No bursts on round**: `start_burst` returns at once on round; the round meter-view burst hooks
  are no-ops. BURST_FRAME_MS is 100 (rectangular only).
- **Rest re-randomise** when the cursor lands on a new tick, in `rim_layer_set_time`.
