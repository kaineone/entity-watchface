# Proposal

## Why

With two watches on one phone, the Pebble Time showed no temperature after the face started, and
it stayed in Celsius while the Time 2 showed Fahrenheit. Both come from state the phone keeps for
the app as a whole while the watch keeps its own:

- The phone skipped the weather fetch on launch when any watch had fetched in the last 15 minutes.
  The Time 2 had fetched 12 minutes earlier, and the Time had nothing saved, so it stayed blank
  until the 30-minute refresh.
- Settings reach a watch only when the settings page is saved, so a second watch never got them.

## What Changes

- When the face starts, the phone sends one message with the saved settings (if any) and a
  `JsReady` flag. The watch applies the settings and, if weather is on and its own reading is
  missing or at least 15 minutes old, asks the phone for weather.
- The phone no longer throttles the launch fetch itself; the watch decides from its own reading.
  The 30-minute refresh, one-request-at-a-time rule, 10 s timeout and single retry stay.

## Capabilities

### Modified Capabilities
- `weather`: who decides the launch fetch.
- `settings`: saved settings are sent at startup.

## Impact

`package.json` (new message key `JsReady`), `src/pkjs/index.js`, new `src/pkjs/startup.js` + test,
`src/c/settings_store.{c,h}`, `src/c/main.c`, `src/c/logic/weather.h` (refresh constant) + test,
CI runs the new JS test.
