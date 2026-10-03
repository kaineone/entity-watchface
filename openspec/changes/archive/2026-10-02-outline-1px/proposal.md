# Proposal

## Why

On the real Time 2 the 2 px cream outline around the hour reads as too heavy and makes the digit
harder to read. The owner wants it at 1 px.

## What Changes

- The hour outline becomes 1 px. The digit is inset by 1 px on each side (still with the 1 px
  thinner stroke), so the outlined hour keeps filling the same frame.
- The grow amount in the outline pass uses NUMERAL_OUTLINE instead of a literal 2.

## Capabilities

### Modified Capabilities
- `time-display`: outline width.

## Impact

`src/c/numeral_layer.c` only.
