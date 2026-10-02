# Proposal

## Why

The meter stepped one bar every 250 ms all day: jerky to look at and the face's main battery cost.
The owner asked for smoother motion without spending more battery.

## What Changes

- The cursor moves continuously with eased ends (about 6 s per round trip) instead of jumping a bar.
- Animation runs in bursts: 25 s after a wrist flick, about 4 s at each minute change and when
  the face opens, then settles. Between bursts the bars rest in gold with no cursor.
- Round watches use the same bursts; the red tick slides between ticks instead of jumping.
- A double tap (two taps within 0.7 s) swaps the top row to steps; a single flick starts a burst.
- Roughly 80 % fewer frames per day than the continuous 4 fps animation.

## Capabilities

### New Capabilities

### Modified Capabilities
- `pulse-meter`: continuous eased motion, bursts, idle look.
- `round-face`: rim bursts with sliding cursor; no second ticks.
- `tap-swap`: double tap.

## Impact

New `src/c/logic/glide.{c,h}` + tests, `logic/rim.{c,h}` fractional cursor + tests,
`meter_layer.c`, `rim_layer.c`, `main.c`, `src/pkjs/config.js` label, README.
