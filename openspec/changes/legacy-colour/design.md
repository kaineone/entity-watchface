# Design

## Decisions
- **Per-resource `targetPlatforms`** (supported by the SDK resource generator): ZEN_64 emery;
  ZEN_60 gabbro; ZEN_48 all; ZEN_42 chalk; ZEN_36 basalt, flint; LABEL_16 emery, gabbro;
  LABEL_12 basalt, chalk, flint. RESOURCE_ID_* constants are only referenced inside the matching
  platform branch of `layout.c`.
- **Platform selection by display features** (`PBL_ROUND`, `PBL_DISPLAY_WIDTH`), per the docs'
  advice to prefer feature macros.
- **Geometry in the table**: `label_font_res`, `meter_pitch`, `meter_bar_w`, `meter_max_h`,
  `rim_outer_r`, `rim_max_len`. Emery values (9, 7, 22) and gabbro (127, 18) reproduce today's output.
- **Scaling** is done in the layers, not the logic: `h' = (h * max + half) / design_max`,
  clamped to at least 2. With max equal to the design max this is the identity, so emery/gabbro
  are unchanged. The meter's baseline sits at `max_h + 2`.
- **Rim centre** comes from the layer bounds, so radius alone changes for chalk.
- flint (black and white) is a separate change.

## Risks / Trade-offs
- basalt and chalk have less app RAM than the 2026 models; check the build's memory report.
