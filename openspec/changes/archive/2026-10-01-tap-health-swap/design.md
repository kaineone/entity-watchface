# Design

## Context
Top-left is the date TextLayer, top-right the weather TextLayer with a fit loop. Health is listed
as a capability in the research; heart rate on emery is contradictory in the docs, so access is checked.

## Goals / Non-Goals
**Goals:** spec behaviour, near-zero idle cost. **Non-Goals:** live-updating values while swapped.

## Decisions
- **Reuse the two text layers.** A `s_swapped` flag makes the date path and `update_weather` leave
  the layers alone while swapped; on revert, force both to rebuild (clear their cached buffers).
- **Formatting** in `fmt.c`: `fmt_steps(buf, n, steps)` and `fmt_bpm(buf, n, bpm)`, negative or
  zero input meaning unavailable (`--`). Host-tested.
- **Access check** with `health_service_metric_accessible(metric, start, end)` over today for steps
  and `(now, now)` for heart rate before peeking it, as the official heart-rate guide does. A
  30-minute window reported heart rate as unavailable in the emulator even with a live reading.
- **Timer**: `app_timer_register(10000, ...)`; cancelled on early revert, on window unload, and
  when the setting turns off while swapped.
- **Colours**: the weather layer's colour is forced to accent while swapped and recomputed on revert.

## Risks / Trade-offs
- The emulator's health data may be empty; the `--` path covers it and is host-tested.
