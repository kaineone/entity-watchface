# Design

## Decisions
- `bool status_health_ready(int fw_major, int fw_minor, bool movement_seen)`: true on 4.9 or later
  (or any major above 4), otherwise only once a movement update has been seen.
- main.c reads `watch_info_get_firmware_version()` once at window load. On older firmware it
  subscribes to health events with a handler that only sets `s_health_seen` on
  HealthEventMovementUpdate. It unsubscribes in window_unload, never inside the handler (the
  firmware touches its cache right after calling the handler).
- Double tap: when not swapped, call `swap_in()` only if health is ready. `swap_in()` reads steps
  first and returns without swapping if they are unavailable. On rectangular watches it replaces
  the temperature only when heart rate is available and above 0 (current firmware reports the
  metric as available with value 0 when there is no reading).
- Sources: google/pebble `src/fw/popups/health_tracking_ui.c` and
  `services/normal/activity/activity_metrics.c` (`activity_get_metric` shows the popup for the app
  task when tracking is off); coredevices/PebbleOS has no caller of the app popup.
