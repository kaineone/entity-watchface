# Tasks

## 1. Logic

- [x] 1.1 `src/c/logic/fmt.{c,h}`: `fmt_hour`, `fmt_minute`, `fmt_ampm`, `fmt_date`
- [x] 1.2 `tests/host/test_fmt.c` covering 0/9/12/13/23 h in both modes, minute padding, all seven day names, date padding

## 2. Watch

- [x] 2.1 `src/c/palette.h` and `src/c/layout.h` (emery frames, peek frames)
- [x] 2.2 `src/c/main.c`: load fonts, create text layers, MINUTE_UNIT tick, change-only text updates, date on day change
- [x] 2.3 Quick-view reflow through `unobstructed_area_service` did_change + initial pass

## 3. Verify

- [x] 3.1 `pebble build` clean, `make -C tests/host` green
- [x] 3.2 Emulator screenshots on emery: normal, quick view on, 12-hour mode
