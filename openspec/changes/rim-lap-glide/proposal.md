# Proposal

## Why

On the Round 2 the red tick follows the clock at one tick per second, so a 4 s or 25 s burst only
creeps a few ticks round the rim. The millisecond counter is also out of step with the second, so
the tick snaps back once a second and reads as a pulse. The owner wants the rim to scan a full
circle up to 12, then turn and come back smoothly, the way the square meter glides.

## What Changes

- The rim cursor glides a full lap clockwise from 12 back to 12, eases to a stop, then laps back
  counter-clockwise, with smoothstep easing at both ends (about 5 s per lap).
- The cursor is no longer tied to the clock; `time_ms` is not read for the rim.
- Each new burst starts at 12 going clockwise. The burst at each minute change and when the face
  opens is exactly one lap, so it ends at 12. A flick runs 25 s (five laps).
- Round bursts run at 10 frames per second like the rectangular meter (was 5).
- The tick just ahead of the cursor grows as the cursor approaches it, so length moves round the
  rim continuously instead of jumping tick to tick.

## Capabilities

### Modified Capabilities
- `round-face`: rim cursor, trail and burst rules.

## Impact

`src/c/logic/rim.{c,h}` (glide state replaces the clock-based cursor), `src/c/rim_layer.{c,h}`,
`src/c/main.c` burst constants and burst start hook, `tests/host/test_rim.c`. Applies to gabbro
and chalk (both use the rim). Battery: round frames during bursts go from about 20 to 50 per minute
change and from 125 to 250 per flick; idle cost is unchanged (no timer outside bursts).
