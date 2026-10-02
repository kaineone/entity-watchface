# Proposal

## Why

On the real Pebble Time 2 the staggered hour and minute read as two unrelated numbers, the
weather word crowded the date, and `01.10` is ambiguous for US readers. The owner asked for bold
numerals, a centred stack, temperature-only weather and a locale-aware date.

## What Changes

- Numerals change from Zen Dots to Orbitron Black (SIL OFL), digits only, per-platform sizes.
- Rectangular faces stack the hour over the minute on one centre axis, like the round faces.
- Weather shows only the temperature (`26°`); the condition word and the fit loop are removed.
- The date follows the watch locale: `thu oct 1` for en_US, `thu 1 oct` otherwise.

## Capabilities

### New Capabilities

### Modified Capabilities
- `time-display`: numeral face, centred stack, locale date.
- `weather`: temperature-only readout.

## Impact

package.json resources, `resources/fonts/Orbitron-Black.ttf` + OFL, `layout.c`, `main.c`,
`logic/fmt.{c,h}`, `logic/weather.{c,h}`, tests, README credits, store screenshots later.
