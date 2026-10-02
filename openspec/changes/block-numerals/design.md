# Design

## Decisions
- **Geometry in Pebble-free logic** (`numeral_polys`): emits up to 16 polygons of up to 6 points per
  digit; the layer draws them with stack-allocated `GPath` structs (`gpath_draw_filled`), no heap.
- **Grid**: cell width = (row width − gap) / 2; text right-justified into the two cells.
- **Frames per platform** (x,y,w,h), normal / peek:
  emery: date 8,6,120,16 · weather 128,6,64,16 · hour 8,30,184,64 · minute 8,104,184,64 · gap 10 · stroke 17 ·
    meter 6,176,189,26 · link 8,208 · quiet 36,211 · ampm 70,206,60,16 · power 120,206,72,16;
    peek hour 8,26,184,52 · minute 8,84,184,52 · stroke 14 · link 8,146 · quiet 36,149 · ampm 70,144 · power 120,144.
  basalt/flint: date 6,3,90,16 · weather 96,3,42,16 · hour 6,20,132,46 · minute 6,72,132,46 · gap 8 · stroke 12 ·
    meter 9,126,126,20 · link 6,150 · quiet 32,153 · ampm 50,148,44,16 · power 84,148,54,16;
    peek hour 6,18,132,34 · minute 6,58,132,34 · stroke 9 · link 6,98 · quiet 32,101 · ampm 50,96 · power 84,96.
  gabbro: quiet 126,26 · date 0,40,260,16 · hour 40,64,180,58 · minute 40,130,180,58 · gap 10 · stroke 15 ·
    weather 0,192,260,16 · link 80,214 · ampm 105,212,50,16 · power 140,212,40,16.
  chalk: quiet 86,14 · date 0,26,180,16 · hour 26,44,128,40 · minute 26,90,128,40 · gap 8 · stroke 11 ·
    weather 0,134,180,16 · link 50,154 · ampm 72,152,36,16 · power 104,152,30,16.
- **Fonts**: `fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD)`; no custom font resources remain.
- **Sign-off**: emulator screenshots on all five platforms reviewed by the owner before merge.
