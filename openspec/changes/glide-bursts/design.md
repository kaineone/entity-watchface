# Design

## Decisions
- **Position** in 1/256-bar fixed point. Phase runs 0..PERIOD-1 per round trip (PERIOD 60 frames);
  each leg maps t in [0,1] through smoothstep 3t²−2t³ in integer maths. Direction is the leg.
- **Amplitude** 0..256 ramps up over 5 frames and down over the last 10 frames of a burst; boosts
  scale by amplitude and the heat distance shifts toward gold as amplitude falls, so bursts settle.
- **Boost interpolation** between table points {16,13,7,3,1,0} at fractional distance.
- **`glide.c`** is Pebble-free and host-tested; `meter.c` keeps mode, heat, Bayer and density.
- **Burst controller** in main.c: one AppTimer (100 ms rect, 200 ms round) only while a burst runs
  and the mode is ANIMATING and focused; frames left counter; mode changes cancel it.
- **Round** uses `rim_tick_frac` with `time_ms` sub-second position; the tick unit is MINUTE_UNIT always.
- **Double tap** detected from tap timestamps (`time_ms`), 700 ms window.
- **Budget**: bursts ≈ 40 frames/minute plus ≈ 250 per flick, versus 240 frames/minute today.
