# Design

## Context
Greenfield. SDK 4.33.1 + pebble-tool 5.0.40 installed locally via uv; `pebble new-project` template
used as reference (`sdkVersion: "3"`, `enableMultiJS: true`, stock `wscript`). Research digests in
`docs/research/`.

## Goals / Non-Goals

**Goals:** reproducible build locally and in CI; a fast host test loop for pure logic; minimal
resource footprint.

**Non-Goals:** any watchface UI beyond a black window; pkjs content (later changes); emulator in CI.

## Decisions
- **targetPlatforms `["emery","gabbro"]` only.** Legacy platforms come in a later compat change.
- **Fonts as `font` resources with `characterRegex`** so only needed glyphs are rasterised (memory).
  Zen Dots digits only — the hour/minute layers never render anything else.
- **`src/c/logic/` is Pebble-free** (only `<stdint.h>`, `<stdbool.h>`, `<stddef.h>`, `<string.h>`),
  so it compiles both in the watch build (wscript globs `src/c/**/*.c`) and on the host.
- **Host harness = plain C + Makefile**, no framework dependency: `tests/host/test.h` with
  `CHECK(cond)` / `CHECK_EQ_INT(a,b)` counting failures; one `test_<module>.c` per module, each a
  separate binary; Makefile runs them all and fails on the first non-zero exit.
- **CI pins SDK 4.33.1** (not `latest`) for reproducibility; caches `~/.pebble-sdk` keyed on version.
  License prompt answered with `yes |` in CI. Node is preinstalled on ubuntu-latest.

## Risks / Trade-offs
- SDK install in CI may need extra apt packages → discover on first run, add them to the workflow.
- `pebble sdk install` downloads from Core Devices' servers; an outage blocks CI (cache mitigates).
