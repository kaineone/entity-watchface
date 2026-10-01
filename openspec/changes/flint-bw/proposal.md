# Proposal

## Why

The Pebble 2 Duo (flint) is a current, black-and-white 144×168 watch. Entity's colour palette
carries meaning (the red cursor, the heat trail, grey for disconnected or stale), so it needs a
black-and-white translation, not just a recolour.

## What Changes

- New target `flint` using the basalt frames and fonts.
- Colour is translated to white and to ordered-dither density: rest bars 50 %, cursor solid,
  trail 7/8, 3/4, 5/8, 9/16, baseline 25 %, disconnected bars and glyph 12.5 % / 25 %.
- Low battery shows the percentage inverted (black on white).
- Stale weather always keeps its `~` because grey cannot mark it.
- The hour-colour setting is hidden on black-and-white watches.

## Capabilities

### New Capabilities

### Modified Capabilities
- `legacy-layouts`: adds the black-and-white flint face.

## Impact

package.json, `palette.h`, `logic/meter.{c,h}` (+ tests), `meter_layer.c`, `status_layer.c`,
`main.c`, `src/pkjs/config.js`. Colour platforms unchanged.
