# Proposal

## Why

On the Round 2 the red tick only moved during short bursts, a few ticks at a time, and the
millisecond counter's offset from the second made it snap back once a second, so it read as a
pulse that inched forward. The owner wants the rim to work as designed: one tick per second, a
full lap each minute, turning at 12.

## What Changes

- While the rim may animate, round faces subscribe to second ticks and the red tick steps once a
  second: clockwise on even minutes, counter-clockwise on odd minutes, turning at 12.
- No bursts on round: no frame timer, no sub-second position, and a flick does not start one.
  Double tap still swaps the top readout.
- Freeze rules are unchanged; when the rim can't animate or the face loses focus, the face drops
  back to minute ticks.
- The fractional cursor (`rim_tick_frac`) is removed.

## Capabilities

### Modified Capabilities
- `round-face`: rim timing; bursts no longer apply to round.

## Impact

`src/c/logic/rim.{c,h}`, `tests/host/test_rim.c`, `src/c/rim_layer.{c,h}`, `src/c/main.c` (tick
subscription follows the meter mode on round; bursts are rectangular only), README. Applies to
gabbro and chalk. Battery: one rim redraw per second while animating, against about 20 frames per
minute change plus 125 per flick in bursts before; nothing at all while frozen or out of focus.
