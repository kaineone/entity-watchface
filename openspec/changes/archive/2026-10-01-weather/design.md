# Design

## Context
Settings already hold `show_weather` and `fahrenheit`. Clay keeps the page values in phone
localStorage under `clay-settings`, which pkjs can read to skip work when weather is off.

## Goals / Non-Goals
**Goals:** cheap weather (one fetch per 30 min, nothing when off), clear staleness, testable mapping.
**Non-Goals:** forecasts, location names, retries beyond the next 30-minute tick.

## Decisions
- **Open-Meteo** `https://api.open-meteo.com/v1/forecast?latitude=..&longitude=..&current=temperature_2m,weather_code`.
  No key, so nothing secret ships in the bundle.
- **Phone sends a condition index (0 clear, 1 cloud, 2 rain, 3 storm, 4 snow) and tenths of °C.**
  The watch converts to °F and rounds, so one message works for both settings.
- **Geolocation** `{ timeout: 15000, maximumAge: 1800000, enableHighAccuracy: false }`: coarse and
  cached, the cheapest fix that's still right for weather.
- **Mapping in `src/pkjs/weather.js`** as a plain function exported for a node test
  (`tests/js/test_weather.js`, run in CI).
- **Watch logic in `src/c/logic/weather.c`**: text formatting (word, rounding, °F, `~` when stale)
  and `weather_is_stale(now, last, 3600)`. Rounding is half away from zero.
- **Persistence** key 2: `{int8 cond; int16 temp_c10; int32 time}` with a version byte.
- **Ageing** is checked on the minute tick; the text layer only changes when the string or colour changes.
- **Settings change** to weather on: the watch asks for an update by sending `WeatherRequest`;
  pkjs answers with a fetch. Weather off: hide the layer.
- **Font**: LABEL_16 regex becomes `[a-z0-9 .°%+~-]`.

## Risks / Trade-offs
- The pkjs process only runs while the face is open on a connected phone; a 30-minute interval is
  the practical floor anyway.
