# Proposal

## Why

The face should also run on the Pebble Round 2. The design has a round layout with the meter
wrapped around the rim as 60 ticks.

## What Changes

- Round layout (260×260): centred date at the top, weather at the bottom, Zen Dots 60 px digits,
  status row across the middle.
- Rim meter: 60 one-pixel ticks around the edge. The red tick follows the seconds, clockwise on
  even minutes and counter-clockwise on odd ones, with a trailing heat ramp that only grows after
  the turn at 12.
- Second ticks only on round and only while the rim animates; otherwise minute ticks.
- The layout table gains per-platform text alignment and digit fonts; the weather fit check only
  applies when date and weather share a row.

## Capabilities

### New Capabilities
- `round-face`: the Pebble Round 2 layout and the rim meter.

### Modified Capabilities

## Impact

New `src/c/logic/rim.{c,h}`, `src/c/rim_layer.{c,h}`, `tests/host/test_rim.c`; `layout.{c,h}`,
`main.c`. Emery behaviour is unchanged.
