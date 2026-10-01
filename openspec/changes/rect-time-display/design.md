# Design

## Context
Bootstrap gives a black window and font resources. Emery frames come from the local design
handoff; quick-view height on emery is about 51 px, so the unobstructed height is about 177
(computed at runtime, never hardcoded).

## Goals / Non-Goals

**Goals:** pixel-exact emery layout, cheap minute refresh, quick-view reflow, string logic
covered by host tests.

**Non-Goals:** meter, status row, weather, settings UI (later changes). Gabbro frames.

## Decisions
- **TextLayers, not a custom draw proc.** The SDK skips work for unchanged layers; we compare the
  new string with the cached buffer and only call `text_layer_set_text` + mark dirty on change.
- **`fmt` is Pebble-free** and takes plain ints (`hour 0-23`, `minute`, `wday 0-6`, `mday`,
  `mon 0-11`) and caller-provided buffers with sizes, returning nothing; the caller passes values
  from `struct tm`. This keeps it host-testable.
- **Layout table** `layout.h`: a `FaceLayout` struct of GRects plus font ids for normal and peek
  states, one `static const` instance per platform selected with `PBL_IF_ROUND_ELSE`-style
  preprocessor branches (`#if defined(PBL_ROUND)`). Emery only for now; gabbro gets a placeholder
  equal to emery values until the round change.
- **Palette** `palette.h`: named `GColor` macros using the SDK names (GColorBrass AAAA55,
  GColorSunsetOrange FF5555, GColorRajah FFAA55, GColorPastelYellow FFFFAA, GColorRed FF0000,
  GColorOrange FF5500, GColorChromeYellow FFAA00, GColorBulgarianRose 550000, GColorDarkGray
  555555, GColorWindsorTan AA5500).
- **Quick view** via `unobstructed_area_service_subscribe` using the `did_change` handler only
  (one relayout per transition) plus a layout pass at window load that reads
  `layer_get_unobstructed_bounds(window_root)`. Fonts for all three Zen sizes and LABEL_16 are
  loaded once at window load and unloaded at unload.
- **Hour colour and 12/24 override** are plain static variables now (defaults FF5555, follow
  system); the settings change will write them.

- **Digit frames are 184 px wide, not the handoff's 120.** Zen Dots at 64 px sets `04` wider
  than 120 px; the HTML mock let it overflow its box, but a Pebble TextLayer ellipsizes. Hour stays
  left-aligned at x 8 and minute right-aligned to x 192, so glyphs land where the design draws them.

## Risks / Trade-offs
- ZEN_64 is above the docs' recommended 48 px; digits-only keeps the resource small (build shows
  17 KB total resources). Verify glyph widths fit 120 px frames in the emulator screenshot.
