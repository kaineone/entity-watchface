# Proposal

## Why

On the real watch the owner found the numerals not angular enough and too small for their space,
the label font poor on this display, and the colours too muted. Research into the panels
(docs/research/displays.md) showed pixel-grid text and luminance contrast matter most here.

## What Changes

- Time numerals become custom-drawn angular block glyphs: a 3×5 stroke grid, heavy stroke,
  45° chamfers on top-left convex corners and on the top-right of the 7. Each digit sits
  right-aligned in a fixed two-cell grid spanning the screen width minus a small margin.
- Labels use the Pebble system font Gothic 14 Bold. Orbitron and JetBrains Mono are removed.
- Palette C: hour FF0000, minute and labels FFFF00, accent FFAA00, meter rest AAAA00, heat
  FF5500 / FFAA00 / FFAA55 / FFFF00. Black and white on the Pebble 2 Duo.
- The quiet mark and AM/PM move to the status row; round faces get a status row under the weather.

## Capabilities

### New Capabilities

### Modified Capabilities
- `time-display`: drawn numerals, label font, layout and palette.

## Impact

New `src/c/logic/numeral.{c,h}` + tests, new `src/c/numeral_layer.{c,h}`; `layout.{c,h}`,
`palette.h`, `main.c`, package.json resources; README credits.
