# Design

## Decisions
- **Glide state in `RimMeter`**: `phase` 0..RIM_PERIOD-1 (RIM_PERIOD = 2 legs), `amp` 0..256,
  `last_nearest`. Position `rim_pos` is in 1/256-tick fixed point over the unwrapped lap 0..60·256;
  leg 0 runs 0→60 (clockwise), leg 1 runs 60→0, each through smoothstep 3t²−2t³ in integer maths.
  Position 60 is tick 0 at 12 o'clock.
- **Lap length** RIM_LEG_FRAMES = 50 frames (5 s at 10 fps). Peak speed is 1.5× average, about
  1.8 ticks per frame, which the 5-tick trail covers.
- **Fresh start** `rim_start` sets phase 0 and amp 0 so a burst from idle always begins at 12
  clockwise. Extending a running burst keeps the phase.
- **Amplitude** ramps up over 5 frames and down over the last 10, as on the rectangular meter.
  Boosts scale by amp; cursor ink is gold below 64, HEAT1 below 128, red otherwise; trail heat
  distance shifts toward gold by (256 − amp)·5/256.
- **Tick lengths** use the unwrapped coordinate j of each tick (tick 0 is j = 0 or j = 60,
  whichever is nearer the cursor), so the trail never wraps through 12.
  - nearest tick (round(pos)) → red, rest + boost(|x|).
  - behind (opposite to travel, |x| < 5 ticks) → rest + boost(|x|), heat by rounded distance.
  - ahead within one tick → rest + 16·(1 − |x|), HEAT1.
  - boost interpolates table {16, 10, 6, 3, 1, 0} at fractional distance; lengths clamp to 18.
- **Rest re-randomise** when the nearest tick changes.
- **main.c**: BURST_FRAME_MS 100 on all platforms; BURST_SHORT is one lap on round (50 frames) and
  40 frames on rect; a `meter_view_start` hook calls `rim_layer_start` on round when a burst begins
  from idle and does nothing on rect.
