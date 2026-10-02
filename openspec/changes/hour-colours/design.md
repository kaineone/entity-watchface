# Design

## Colours (64-colour palette hex)
| Hour | Hour fill | Cursor | Trail d1..d4 | Baseline | Outline |
|---|---|---|---|---|---|
| red (0) | FF0000 | FF0000 | AA0000 AA0000 550000 550000 | 550000 | yes |
| cream (1) | FFFFAA | FF0000 | AA0000 AA0000 550000 550000 | 550000 | no |
| gold (2) | FFAA00 | FF0000 | AA0000 AA0000 550000 550000 | 550000 | no |
| pink (3) | FF55AA | FF55AA | AA55AA AA5555 550055 550055 | 550055 | yes |
| purple (4) | AA00FF | AA00FF | AA00AA 5500AA 550055 550055 | 550055 | yes |
| blue (5) | 0055FF | 0055FF | 0055AA 0055AA 000055 000055 | 000055 | yes |
| teal (6) | 00AAAA | 00AAAA | 00AAAA 005555 005555 005555 | 005555 | yes |
| green (7) | 00AA55 | 00AA55 | 00AA55 005555 005500 005500 | 005500 | yes |
| white (8) | FFFFFF | FF0000 | AA0000 AA0000 550000 550000 | 550000 | no |

Trail shades are the hour colour mixed toward black at 20/40/55/70 % and snapped to the palette
(baseline 67 %), as in the approved mockup. Repeats are expected; the rectangular meter's existing
50/50 dither between heat(d) and heat(d+1) still applies. Values outside 0..8 fall back to red.
The outline colour is GColorYellow (on glass the same cream as the minutes). Cream and gold hours
keep today's fills (PAL_CREAM and PAL_ACCENT).

## Decisions
- **`logic/scanner.{c,h}`** (Pebble-free): `ScannerShades { uint8_t cursor, heat[4], baseline; }`
  as GColor8 ARGB bytes (0xC0 | RR<<4 | GG<<2 | BB); `scanner_shades(int hour_color)` and
  `scanner_hour_outlined(int hour_color)`; host-tested against the table above.
- **Meter and rim layers** take `*_set_shades(const ScannerShades *)` and map INK_RED, INK_HEAT1..4
  and INK_BASELINE through it on colour watches; black-and-white keeps the palette.h whites.
- **Numeral layer** gains `numeral_layer_set_outline(Layer *, bool on, GColor color)`. With the
  outline on, digits are laid out in the bounds inset by NUMERAL_OUTLINE (2) on every side, stroke
  minus 1; each rect (and each chamfer row, drawn as a shortened row instead of carved black) is
  first filled in the outline colour grown by 2 px on all sides, then the digit is filled. With it
  off, drawing is exactly as today.
