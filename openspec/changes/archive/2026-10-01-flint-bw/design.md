# Design

## Decisions
- **Density table in the logic** (`meter_ink_density(MeterInk)`) so it is host-tested; the layers
  use `meter_bayer_value(x, y)` (the raw 0..15 matrix value) and light a pixel when value < density.
  Solid inks (16) fall back to `graphics_fill_rect`.
- **palette.h**: under `PBL_BW` every named colour maps to GColorWhite except the background.
  Colour code paths that rely on colour difference get explicit `PBL_BW` branches: battery inverse,
  link density, weather `~` rule.
- **Layout**: the existing `#else` (small rect) branch serves both basalt and flint; flint is added
  to the ZEN_48, ZEN_36 and LABEL_12 resource targets.
- **Clay**: `capabilities: ["COLOR"]` on the hour-colour select hides it on black-and-white watches.
- Dither cost: up to 21 bars × 5×16 px per frame of `graphics_draw_pixel` on flint, only while
  animating. Acceptable; can move to row spans later if profiling says so.
