# Proposal

## Why

With the backlight off, the red hour turns nearly unreadable on the reflective display while the
cream minutes stay clear. The owner also wants more hour colours, and the scanner to take the hour
colour instead of always being red.

## What Changes

- A 2 px cream outline (the minute colour) around the hour digits on colour watches, for hours in
  red, pink, purple, blue, teal or green. Cream, gold and white hours have no outline. The outlined
  hour fills exactly the frame the plain hour did: the digit is inset by the outline width and its
  stroke is 1 px thinner, so layouts, the grid and the round rim clearance are unchanged.
- Hour colours: red, pink, purple, blue, teal, green, cream, gold, white (white is new as well).
- The scanner (cursor, trail and baseline, rectangular and round) takes the hour colour, with the
  trail in darker shades of it. Cream, gold and white hours keep a red scanner. Red's trail also
  moves to darker reds, replacing the orange-to-gold heat ramp.
- Black-and-white watches are unchanged.

## Capabilities

### Modified Capabilities
- `settings`: hour colour choices.
- `pulse-meter`, `round-face`: cursor, trail and baseline colours.
- `time-display`: hour outline.

## Impact

New `src/c/logic/scanner.{c,h}` + host test; `logic/settings.{c,h}` + test; `numeral_layer.{c,h}`;
`meter_layer.{c,h}`; `rim_layer.{c,h}`; `palette.h`; `main.c`; `src/pkjs/config.js`.
