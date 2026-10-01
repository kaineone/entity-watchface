# Proposal

## Why

Several behaviours are hard-coded defaults today: 12/24-hour, hour colour, the animation switch,
the disconnect buzz and the low-battery threshold. People need to change them from the Pebble app,
and the choices must survive a watch restart.

## What Changes

- A Clay settings page (`@rebble/clay` 1.1.0) opened from the Pebble app.
- Settings sent to the watch over AppMessage and persisted with `persist_write_data`.
- The face applies settings immediately: clock format, hour colour, animation, buzz on disconnect
  and the low-battery threshold. Weather, °F and tap-to-swap are stored now and used by later changes.
- The meter keeps animating on low battery while the watch is charging.

## Capabilities

### New Capabilities
- `settings`: which settings exist, their defaults, how they reach the watch and persist.

### Modified Capabilities
- `pulse-meter`: low battery no longer freezes the meter while charging.

## Impact

package.json (Clay dependency, `configurable` capability, message keys), new `src/pkjs/index.js`,
`src/pkjs/config.js`, `src/c/logic/settings.{c,h}`, `src/c/settings_store.{c,h}`, main.c wiring,
`meter.c` mode signature, host tests.
