# Proposal

## Why

Every later feature needs a buildable Pebble project, a host-side test harness for pure logic,
and a CI gate so "PR is green" means something before adversarial review and merge.

## What Changes

- Pebble C project skeleton: `package.json` (watchface, uuid `310cef49-a859-4395-84f2-f2eb4b28841d`, displayName `Entity`,
  targets `emery` and `gabbro`), stock `wscript`, `src/c/main.c` showing a black window.
- Font resources declared per the handoff: `ZEN_64`, `ZEN_60`, `ZEN_48` (Zen Dots, digits only)
  and `LABEL_16` (JetBrains Mono Medium 16, `[a-z0-9 .°%+~]`).
- Host test harness `tests/host/` (plain gcc + a tiny assert macro, `make -C tests/host`) compiling
  Pebble-free modules from `src/c/logic/`.
- GitHub Actions workflow that installs pebble-tool + SDK 4.33.1, runs `pebble build` for all targets,
  runs host tests, and uploads the `.pbw`.
- Research digests in `docs/research/`.

## Capabilities

### New Capabilities
- `build-pipeline`: how the watchface is built, tested and gated in CI.

### Modified Capabilities

## Impact

New files only. Toolchain: pebble-tool (uv), SDK 4.33.1, gcc, GitHub Actions ubuntu-latest.
