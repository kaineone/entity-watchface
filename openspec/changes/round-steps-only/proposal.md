# Proposal

## Why

Neither round Pebble has a heart-rate sensor, so on round the tap readout always showed `-- bpm`.

## What Changes

- On round platforms a tap swaps only the top readout to steps; the bottom keeps the weather.

## Capabilities

### New Capabilities

### Modified Capabilities
- `tap-swap`: round shows steps only.

## Impact

`src/c/main.c` (`swap_in` under `PBL_ROUND`).
