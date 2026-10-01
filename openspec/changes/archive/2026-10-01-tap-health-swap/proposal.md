# Proposal

## Why

The design lets a tap on the watch swap the top row from date and weather to today's steps and
heart rate. The setting exists; the behaviour does not.

## What Changes

- Accelerometer tap toggles the top row to `<n> steps` (left) and `<n> bpm` (right) for 10 seconds,
  then it returns to date and weather. A second tap returns early.
- Steps come from `health_service_sum_today(HealthMetricStepCount)` and heart rate from
  `health_service_peek_current_value(HealthMetricHeartRateBPM)`, read at the moment of the tap.
  Unavailable values show `--`.
- The tap service is subscribed only while the tap setting is on.

## Capabilities

### New Capabilities
- `tap-swap`: the tap gesture and the steps / heart-rate readout.

### Modified Capabilities

## Impact

package.json (`health` capability), `src/c/logic/fmt.{c,h}`, `tests/host/test_fmt.c`, `src/c/main.c`.
