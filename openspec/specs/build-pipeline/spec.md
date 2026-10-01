# build-pipeline Specification

## Purpose
Defines how the entity watchface is built for its target platforms, how pure logic is unit-tested
off-device, and what a passing ("green") pull request means.

## Requirements

### Requirement: Watchface builds for all target platforms
The project SHALL build a single `.pbw` bundle containing binaries for `emery` and `gabbro` with
`pebble build` on SDK 4.33.1, and SHALL be flagged as a watchface.

#### Scenario: Clean build
- **WHEN** `pebble build` runs on a clean checkout
- **THEN** it exits 0 and produces `build/<name>.pbw` containing emery and gabbro binaries

#### Scenario: Watchface flag
- **WHEN** the bundle is installed on the emulator
- **THEN** it appears as a watchface (not an app) and renders a black background

### Requirement: Custom fonts are packaged with restricted glyph sets
The bundle SHALL include Zen Dots at 64, 60 and 48 px limited to digits, and JetBrains Mono Medium
at 16 px limited to `[a-z0-9 .°%+~]`, to bound resource memory.

#### Scenario: Font resources present
- **WHEN** the project builds
- **THEN** resources `ZEN_64`, `ZEN_60`, `ZEN_48` and `LABEL_16` are generated without warnings

### Requirement: Pure logic is unit-tested on the host
Logic that does not depend on Pebble APIs SHALL live in Pebble-free C modules and be covered by
host tests runnable with `make -C tests/host`, which exits non-zero on any failed assertion.

#### Scenario: Host tests run
- **WHEN** `make -C tests/host` runs
- **THEN** every test module compiles with `-Wall -Wextra -Werror` and all assertions pass

### Requirement: CI gates pull requests
Every push and pull request to `main` SHALL run the Pebble build and host tests in GitHub Actions,
and the PR SHALL be considered green only when both pass. The built `.pbw` SHALL be uploaded as an artifact.

#### Scenario: Failing test blocks
- **WHEN** a host test assertion fails on a PR
- **THEN** the CI check fails and the PR is not merged
