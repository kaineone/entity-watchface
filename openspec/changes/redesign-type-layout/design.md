# Design

## Decisions
- **Orbitron Black** pinned as a static weight-900 instance (fonttools instancer) because the
  Pebble font converter needs static TTFs. Digits only (`[0-9]`).
- **Resources** keep per-platform `targetPlatforms`: emery large/small, gabbro, basalt/flint, chalk
  sizes chosen from emulator screenshots (start: emery 64/48, gabbro 60/48, basalt+flint 48/36,
  chalk 42). Zen Dots resources are removed.
- **Layout**: the rect tables get centred hour and minute frames spanning the full width with
  `GTextAlignmentCenter`; am/pm stays top-right of the hour row.
- **Locale** from `i18n_get_system_locale()` read at each date rebuild (it can change at runtime);
  `strncmp(locale, "en_US", 5) == 0` selects month-first. Formatting in `fmt_date_locale` (pure).
- **Weather**: `weather_temp_text(buf, n, temp_c10, fahrenheit, stale)` → `26°` / `~26°`; the
  word, levels and fit loop are deleted. `weather_word` stays for possible future use only if
  referenced; otherwise removed.
