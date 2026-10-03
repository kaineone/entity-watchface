# Proposal

## Why

On a classic Pebble Time with Pebble Health off, opening the steps view made the watch show
"This app requires Pebble Health to work. Enable Health in the Pebble mobile app to continue."
The original Pebble firmware (before Core's 4.9 releases) shows that popup whenever an app or face
reads a health metric while Health tracking is off, and even the availability check reads one.
Core's firmware has removed it. The owner wants the face to quietly hide steps and heart rate
when they are not available instead.

## What Changes

- On firmware older than 4.9 the face never reads health data until the watch has delivered a
  step update (HealthEventMovementUpdate), which only happens while Health tracking runs.
  Subscribing to health events and receiving them never triggers the popup (verified in the
  original firmware source). Until then a double tap does nothing.
- On every firmware: if steps are not available, a double tap does nothing; if heart rate is not
  available, the temperature stays where the BPM would have gone (no "-- BPM").

## Capabilities

### Modified Capabilities
- `tap-swap`: quiet behaviour when health data is missing.

## Impact

`src/c/logic/status.{c,h}` + host test, `src/c/main.c`.
