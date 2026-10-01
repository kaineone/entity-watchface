# Proposal

## Why

People with the original Pebble Time, Time Steel and Time Round should be able to use Entity too.
These are colour watches with smaller screens than the 2026 models.

## What Changes

- New targets `basalt` (144×168 rect) and `chalk` (180×180 round), colour palette unchanged.
- Smaller fonts declared only where they are used: Zen Dots 36 and 42, JetBrains Mono 12.
  Existing big fonts are also restricted to the platforms that draw them.
- The layout table carries the label font and meter/rim geometry per platform; the meter keeps
  all 21 bars at a 6 px pitch with heights scaled to 16 px; the chalk rim has radius 87 and ticks
  scaled to 12 px.

## Capabilities

### New Capabilities
- `legacy-layouts`: Entity on basalt and chalk.

### Modified Capabilities

## Impact

package.json, `layout.{c,h}`, `meter_layer.c`, `rim_layer.c`, `main.c`. Emery and gabbro output
must stay pixel-identical.
