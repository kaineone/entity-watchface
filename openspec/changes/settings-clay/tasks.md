# Tasks

## 1. Logic
- [x] 1.1 `src/c/logic/settings.{c,h}` + `tests/host/test_settings.c`
- [x] 1.2 `meter_mode` charging argument + tests updated

## 2. Phone and watch
- [x] 2.1 package.json: Clay dependency, `capabilities: ["configurable"]`, message keys
- [x] 2.2 `src/pkjs/index.js`, `src/pkjs/config.js`
- [x] 2.3 `src/c/settings_store.{c,h}` (persist + AppMessage inbox) and main.c apply path

## 3. Verify
- [x] 3.1 Build clean, host tests green
- [x] 3.2 Emulator: open config (`pebble emu-app-config`), change colour and clock format, screenshot; restart face and confirm persistence

## 4. Follow-ups
- [ ] 4.1 Style the Clay page in the face palette (black, gold, one red) instead of Clay's grey and orange; Clay has no theme API, so this needs an injected style block (separate change)
