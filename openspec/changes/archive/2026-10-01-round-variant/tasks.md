# Tasks

## 1. Logic
- [x] 1.1 `src/c/logic/rim.{c,h}` + `tests/host/test_rim.c`

## 2. Watch
- [x] 2.1 layout table: alignments, digit fonts, gabbro frames
- [x] 2.2 `src/c/rim_layer.{c,h}`; main.c platform switch, unit switching, fit-loop row check

## 3. Verify
- [x] 3.1 Build clean for emery and gabbro, tests green
- [x] 3.2 Gabbro emulator screenshots: animating at even and odd minutes, frozen, unlinked (forced), weather, tap swap
- [x] 3.3 Emery regression screenshots unchanged

## 4. Follow-ups
- [ ] 4.1 Unlinked rim not screenshotted (emulator cannot drop the phone link; same code path as frozen with a different ink, covered by host tests)
- [ ] 4.2 Decide whether the Round 2 tap readout should omit heart rate: the gabbro SDK tags list no heart-rate sensor, so it always reads `-- bpm`
