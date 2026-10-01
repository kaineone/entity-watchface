# Design

## Context
Meter and time exist. main.c already tracks s_linked, s_battery_pct, s_quiet and the threshold.

## Goals / Non-Goals
**Goals:** spec-exact status row; no work unless its input changed.
**Non-Goals:** settings UI (the vibrate switch is a static default here); weather staleness.

## Decisions
- **Pure logic** `status.c`: `status_power_text(buf, n, pct, charging)`, `status_power_ink(pct,
  threshold, charging)` returning its own `StatusInk` enum (GOLD, RED, ACCENT) so the module stays
  independent of meter.h, and `status_should_vibrate(was_linked, now_linked, vibe_pref, quiet)`.
- **Charging = is_charging || is_plugged**, matching the handoff.
- **Two tiny custom layers** (link glyph, quiet mark) plus one TextLayer for the percentage. Each
  is marked dirty only when its own input changes.
- **Vibration edge detection** in main.c's connection handler using the previous s_linked; the
  start-up path initialises s_linked from peek without calling the vibrate logic.

## Risks / Trade-offs
- Disconnect events cannot be produced in the SDK 4.33.1 emery emulator (found in the meter change);
  the vibration rule is covered by host tests and needs a hardware check.
