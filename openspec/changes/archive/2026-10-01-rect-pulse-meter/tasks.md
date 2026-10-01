# Tasks

## 1. Logic

- [x] 1.1 `src/c/logic/meter.{c,h}`: Meter struct, init, step, bar height/ink/dither query, mode functions, Bayer lookup
- [x] 1.2 `tests/host/test_meter.c`: bounce sequence, trail side both directions, heights and inks at d 0..6, rest re-randomised within 2..13, mode priority, timer gating, Bayer cells

## 2. Watch

- [x] 2.1 `src/c/meter_layer.{c,h}`: layer with update proc drawing bars, dither and baseline for all three modes
- [x] 2.2 `src/c/main.c`: services (connection, battery, focus), quiet-time poll in tick, timer start/stop, meter hidden in quick view

## 3. Verify

- [x] 3.1 Build clean, host tests green
- [x] 3.2 Emulator screenshots: animating, unlinked (`emu-bt-connection`), frozen (`emu-battery`), quick view

## 4. Hardware follow-up

- [ ] 4.1 On a real Pebble Time 2: turn phone Bluetooth off and confirm the meter goes grey and flat. The SDK 4.33.1 emery emulator does not deliver connection events from `emu-bt-connection` (neither handler fires), so the unlinked look was checked by forcing the state in a throwaway build.
- [ ] 4.2 On a real Pebble Time 2: check that the cursor (FF0000) reads apart from the d=1 trail bar (FF5500/FF5555). The emulator's display profile renders them within a few RGB values of each other.
