# Proposal

## Why

The pulse meter is the face's signature: it shows at a glance whether the watch can reach the
phone. It is also the only continuous animation, so it decides most of the battery cost.

## What Changes

- Pebble-free `src/c/logic/meter.{c,h}`: bar rest heights, cursor stepping and bounce, trailing
  heat ramp, Bayer dither choice, and the mode decision (unlinked, frozen, animating).
- Meter layer on emery at 6,176,189,26 drawing 21 bars and the baseline.
- A 250 ms AppTimer that runs only while the meter is animating and the face has focus.
- Inputs: phone-app connection, battery level, quiet time, quick view, app focus. The battery
  threshold and the animate switch are static defaults (20 %, on) until the settings change.

## Capabilities

### New Capabilities
- `pulse-meter`: what the rectangular meter draws in each state and when it animates.

### Modified Capabilities

## Impact

New `src/c/logic/meter.{c,h}`, `src/c/meter_layer.{c,h}`, `tests/host/test_meter.c`;
`src/c/main.c` wires services. `layout.h` gains the meter frame.
