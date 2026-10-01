# Proposal

## Why

A red-team pass (security, battery, adversarial inputs) found no critical issues but several
medium and low ones: unchecked weather values that can overflow integer maths, stale weather
shown as fresh after a clock change, quiet time noticed up to a minute late, repeated phone-side
fetches, loose AppMessage parsing, a crash path on font failure, and tooling and CI hardening.

## What Changes

- Weather: clamp on the phone (reject |t| > 150 °C, invalid coordinates) and on the watch
  (cond 0..4, temp_c10 within ±999), 64-bit-safe maths, stale when the clock is behind the
  reading, persisted readings validated on load.
- Phone: XHR timeout, single in-flight fetch, one retry after a failure, no refetch within 15 minutes
  of a successful one, coordinates rounded to 0.1°.
- AppMessage: only NUL-terminated strings and integer tuples are accepted; strtol range checks.
- Watch lifecycle: quiet time re-read whenever the meter mode is recomputed; taps ignored while
  unfocused; system-font fallback if a custom font fails; date cache keyed on year and day;
  one font handle when large and small digits share a resource.
- Tooling and CI: `tools/apply_files.py` path allowlist and symlink refusal; CI actions pinned to
  commit SHAs and pebble-tool pinned to 5.0.40.

## Capabilities

### New Capabilities

### Modified Capabilities
- `weather`: input limits and clock-change staleness.
- `settings`: stricter message parsing.
- `pulse-meter`: quiet time takes effect as soon as any event recomputes the mode.

## Impact

`src/c/logic/{weather,settings}.c`, `src/c/settings_store.c`, `src/c/main.c`, `src/pkjs/*.js`,
tests, `tools/apply_files.py`, `.github/workflows/build.yml`.
