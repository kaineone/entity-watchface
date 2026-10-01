# Tasks

## 1. Project skeleton

- [x] 1.1 `package.json` with watchface flag, uuid, displayName `Entity`, targets emery+gabbro, font resources
- [x] 1.2 Stock `wscript`; `src/c/main.c` with a black watchface window (load/unload, no services yet)
- [x] 1.3 `pebble build` succeeds locally; screenshot on emery emulator (`--vnc`) shows black screen

## 2. Host test harness

- [x] 2.1 `src/c/logic/` directory with a first Pebble-free module (`clamp.c/.h`: `clamp_int`) used to prove the harness
- [x] 2.2 `tests/host/test.h`, `tests/host/test_clamp.c`, `tests/host/Makefile` (`-Wall -Wextra -Werror`, fails on any failed test)

## 3. CI

- [x] 3.1 `.github/workflows/build.yml`: checkout, uv, pebble-tool, SDK 4.33.1 (cached), `pebble build`, `make -C tests/host`, upload `.pbw`
- [ ] 3.2 CI green on the PR

## 4. Docs

- [x] 4.1 Commit `docs/research/` digests
