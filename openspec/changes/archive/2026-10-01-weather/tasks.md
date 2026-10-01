# Tasks

## 1. Logic
- [x] 1.1 `src/c/logic/weather.{c,h}` + `tests/host/test_weather.c`
- [x] 1.2 `src/pkjs/weather.js` (code mapping, URL builder) + `tests/js/test_weather.js`, CI step

## 2. Wiring
- [x] 2.1 package.json: message keys WeatherCond, WeatherTempC10, WeatherRequest; `location` capability; font regex with `-`
- [x] 2.2 `src/pkjs/index.js`: ready + 30-minute interval, skip when off, answer WeatherRequest
- [x] 2.3 main.c / settings_store: receive weather, persist, show, age on the minute tick, hide when off, request on enable

## 3. Verify
- [x] 3.1 Build clean, host and node tests green
- [x] 3.2 Emulator: live fetch shows a reading; Fahrenheit toggle; stale look (forced in a throwaway build); weather off hides it
