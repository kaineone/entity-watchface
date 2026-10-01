# Proposal

## Why

The face needs the small status signals from the design: whether the phone is reachable, the
battery level, and whether quiet time is on. A disconnect should also be felt, once, unless the
wearer asked for quiet.

## What Changes

- Link glyph (four rising bars) at the bottom left, gold when linked and grey when not.
- Battery percentage at the bottom right: gold normally, red at or below the low-battery
  threshold, `+` prefix in FFAA55 while charging.
- Quiet-time mark: a 2 px hollow square in AA5500 left of the minute, shown only during quiet time.
- One short vibration when the phone link drops, skipped during quiet time; nothing on reconnect.
- Quick view moves the glyph and percentage up to y 144.

## Capabilities

### New Capabilities
- `status-row`: link glyph, battery text, quiet mark and the disconnect vibration.

### Modified Capabilities

## Impact

New `src/c/logic/status.{c,h}`, `src/c/status_layer.{c,h}`, `tests/host/test_status.c`;
`layout.{c,h}` and `main.c` extended.
