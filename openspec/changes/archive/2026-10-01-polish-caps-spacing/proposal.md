# Proposal

## Why

On the real Time 2 some numeral pairs read as fused: Orbitron's `1` flag and `7` top bar line up
about 3 px apart (`17`, `27`, `07`). The owner also asked for all-caps text for readability and
smooth rim ticks on round watches.

## What Changes

- Extra spacing between numerals via `trackingAdjust` on every Orbitron size.
- All text on the watch in capitals: `THU OCT 1`, `AM`/`PM`, `6240 STEPS`, `72 BPM`. The phone
  settings page keeps sentence case.
- Anti-aliased rim ticks on colour round watches.

## Capabilities

### New Capabilities

### Modified Capabilities
- `time-display`: numeral spacing and capitals.
- `round-face`: smooth rim ticks.

## Impact

package.json, `logic/fmt.c` + tests, `rim_layer.c`.
