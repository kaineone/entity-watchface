# Proposal

## Why

The settings page uses Clay's stock grey and orange theme with all-caps headings, which looks
like a different product next to a black and gold face with a single red.

## What Changes

- A style block injected through a Clay `text` item restyles the page in the face's palette:
  black ground, gold labels and headings, FFAA55 values and switched-on toggles, 555555 rules,
  the meter-baseline red 550000 as the toggle track, square corners, no shadows, sentence-case
  headings, JetBrains Mono with a system monospace fallback.
- Behaviour and copy are unchanged.

## Capabilities

### New Capabilities

### Modified Capabilities
- `settings`: the page's look now follows the face palette.

## Impact

`src/pkjs/config.js` only.
