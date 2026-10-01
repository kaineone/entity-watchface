# Proposal

## Why

The top-right readout is empty. The design shows the current conditions there, and the settings
page already has switches for weather and Fahrenheit.

## What Changes

- Phone side (PebbleKit JS): get the location, fetch current conditions from Open-Meteo (no API
  key), map the WMO weather code to one of five words, and send it to the watch on start and
  every 30 minutes. Skip all of it when weather is switched off.
- Watch side: show `<word> <temp>°` right-aligned at the top in FFAA55, converted to °F on the
  watch when asked. After 60 minutes without an update it turns 555555 with a `~` prefix.
- The last reading is persisted so the face shows it straight away after a restart.
- The label font gains the `-` glyph for temperatures below zero.

## Capabilities

### New Capabilities
- `weather`: fetching, sending, formatting and ageing of the weather readout.

### Modified Capabilities

## Impact

package.json (message keys, `location` capability, font glyphs), `src/pkjs/index.js`,
new `src/pkjs/weather.js`, `src/c/logic/weather.{c,h}`, main.c, host tests, a node test for the
code mapping, CI step for it.
