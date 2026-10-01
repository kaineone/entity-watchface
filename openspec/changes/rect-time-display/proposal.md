# Proposal

## Why

The face has no content yet. The time is the first thing a watchface must get right, and its
layout and quick-view reflow set the frame every later layer sits in.

## What Changes

- Pebble-free formatting module `src/c/logic/fmt.c`: hour, minute, am/pm and date strings.
- Per-platform layout table (`src/c/layout.h`) holding every frame for the normal and
  quick-view states; emery values now, gabbro added in the round change.
- Hour, minute, am/pm and top-left date text layers on emery, using the Zen Dots and
  JetBrains Mono resources and the face palette.
- Tick handling on `MINUTE_UNIT`; the date string is rebuilt only when the day changes.
- Quick view: digits switch to ZEN_48 and the layers move to their quick-view frames.

## Capabilities

### New Capabilities
- `time-display`: how hour, minute, am/pm and date are formatted, placed and refreshed.

### Modified Capabilities

## Impact

`src/c/main.c` grows window content; new `src/c/logic/fmt.{c,h}`, `src/c/layout.h`,
`src/c/palette.h`, host tests `tests/host/test_fmt.c`.
