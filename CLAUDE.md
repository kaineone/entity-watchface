# Entity: watchface for Pebble Time 2 and Pebble Round 2

Native C watchface (+ PebbleKit JS) for the **2026 Core Devices hardware**:
`emery` (Pebble Time 2, 200×228) first, then `gabbro` (Pebble Round 2, 260×260).
Older platforms (basalt/chalk/diorite/flint/aplite) are a later compat change; keep
layout in per-platform tables keyed on `PBL_*` feature macros so they can be added
without a fork.

## Source of truth
- Design spec: `design/handoff/README.md` (pixel-exact frames, palette, behaviour). LOCAL ONLY:
  `design/` is gitignored because the handoff carries client branding. Requirements that matter
  are restated in `openspec/specs/`. Never commit `design/` or name the client in shipped copy.
- Platform research: `docs/research/*.md` (official docs + SDK headers).
- Planning: OpenSpec (`openspec list`, `openspec show <change>`).

## Copy
All prose that ships (README, store text, PR bodies, config page) follows
`/home/elim/Documents/The Complete Field Guide to AI Writing Telltales.md`.

## Workflow (one OpenSpec change = one branch = one PR)
1. `openspec` proposal under `openspec/changes/<id>/` (proposal, design, tasks, spec deltas).
2. Branch `feat/<id>` from `main`.
3. Code is written by the Ollama worker (`tools/delegate.sh`, model kimi-k2.7-code:cloud),
   briefed with complete-file requests; output spliced with `tools/apply_files.py`.
   Briefs/outputs live in `.work/` (gitignored).
4. Push, open PR, wait for CI green, adversarial review, fix, merge, `openspec archive`.

## Build / test
- `pebble build` (SDK 4.33.1, pebble-tool via `uv tool install pebble-tool`).
- `make -C tests/host` runs host-side unit tests of pure logic (no Pebble headers).
- Emulator: ALWAYS pass `--vnc` (headless; no window on the desktop):
  `pebble install --emulator emery --vnc`, `pebble screenshot --emulator emery --vnc --no-open out.png`.

## Battery rules (non-negotiable)
- MINUTE_UNIT ticks by default; SECOND_UNIT only on round while the rim meter animates.
- Meter AppTimer runs only while animating; cancel it on freeze/unlink/quick view/unfocus.
- Mark only the changed layer dirty. Date text only changes on day change.
- Freeze (no animation) at battery ≤ threshold (config, default 20 %), quiet time,
  quick view, animate-off.
- No accel data service (tap service only), no font anti-aliasing, no alpha.
