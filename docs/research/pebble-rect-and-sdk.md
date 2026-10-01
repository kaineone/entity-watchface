# Pebble Time 2 (emery) native C watchface: SDK and platform reference

Compiled 2026-10-01 from official docs (developer.repebble.com = Core Devices fork of developer.rebble.io; source repo github.com/coredevices/sdk-docs) plus GitHub issues/PRs. Items marked NOT DOCUMENTED were searched and not found. Items marked CONFLICT have contradicting sources.
Note: WebFetch summaries were produced by a small model; re-verify exact strings before relying on them in code.

## 1. SDK install, auth, emulator, CI

### Install
- Install `uv`, then `uv tool install pebble-tool`; pebble-tool needs Python >= 3.10. https://developer.repebble.com/sdk/
- Then `pebble sdk install latest`, `pebble new-project myproject`, `pebble build`. https://developer.repebble.com/sdk/
- PyPI pebble-tool latest: 5.0.40 (2026-08-25). Repo: github.com/coredevices/pebble-tool (MIT, Python 3). https://pypi.org/project/pebble-tool/
- Toolchain (arm-none-eabi) and QEMU are NOT bundled; installed by `pebble sdk install`. https://pypi.org/project/pebble-tool/
- Third-party verified Linux setup (July 2026): pebble-tool 5.0.39, SDK 4.17, Python 3.13 via `uv tool install --force "pebble-tool==5.0.39" --python 3.13`, then `pebble sdk install latest` + `pebble sdk activate 4.17`. https://github.com/ArtRichards/pebble-time2-dev-setup
- Another image pins pebble-tool 5.0.37 + SDK 4.9.169, so the "latest" SDK number varies by source; run `pebble sdk list` to see. https://github.com/gregolsky/docker-pebble-sdk
- Linux emulator deps (Ubuntu 24.04): `nodejs npm libsdl2-2.0-0 libglib2.0-0t64 libpixman-1-0 zlib1g libsndio7.0 libpng16-16t64`; an X display is mandatory for the emulator window. https://github.com/ArtRichards/pebble-time2-dev-setup  (FAQ lists SDL2/glib/pixman/zlib: https://developer.repebble.com/faqs/)
- Gotchas (third-party, 2026): `pebble sdk install` exits 1 if the version already exists (treat as success in scripts); an interrupted install can leave `sdk-core` without `toolchain/` (fix: `pebble sdk uninstall <v>` then reinstall); "No SDK installed" despite listing means run `pebble sdk activate <v>`. https://github.com/ArtRichards/pebble-time2-dev-setup
- Gotcha: on hosts with IPv6 disabled, `pebble install --emulator emery` fails `[Errno 111] Connection refused` (pypkjs binds an IPv6 wildcard); the repo ships a patch script that must be re-applied after each tool reinstall. https://github.com/ArtRichards/pebble-time2-dev-setup
- Reset: `pebble wipe` (add `--everything` to also drop account). https://developer.repebble.com/faqs/
- SDK data dir: `~/.pebble-sdk` (Linux). https://github.com/coredevices/pebble-tool
- `/tmp/pb-emulator.json` records QEMU/pypkjs PIDs and ports; `pebble kill` stops emulator. https://github.com/ArtRichards/pebble-time2-dev-setup

### Is `pebble login` required?
- NOT required for build, emulator install, logs, screenshots. pebble-tool's own docstring: login is "Required for CloudPebble and publish flows"; auth is Firebase (browser OAuth). https://raw.githubusercontent.com/coredevices/pebble-tool/main/pebble_tool/commands/account.py
- Required for `pebble install --cloudpebble` to a physical watch: `pebble login` (GitHub), with the new Pebble mobile app and Developer Connect enabled via GitHub sign-in. https://developer.repebble.com/sdk/ and https://developer.repebble.com/faqs/
- Older docs also list timeline pin pushing (`insert-pin`) as needing login. https://developer.repebble.com/guides/tools-and-resources/pebble-tool/
- Developer Connection alternative (LAN): enable in app, pass Server IP via `--phone <ip>`. https://developer.repebble.com/guides/tools-and-resources/developer-connection/
- Core Devices app: BOTH "LAN developer mode" (Settings) AND per-device "Dev Connection" must be on, else connection refused. https://forum.rebble.io/t/pebble-tool-and-the-new-core-devices-app/256
- Publishing: developer dashboard https://developer.rePebble.com/dashboard (linked from https://developer.repebble.com). `publish.py` exists in pebble-tool commands. https://github.com/coredevices/pebble-tool/tree/main/pebble_tool/commands

### Emulator and commands (from pebble-tool source, emucontrol.py)
Source for all items below: https://raw.githubusercontent.com/coredevices/pebble-tool/main/pebble_tool/commands/emucontrol.py
- Run: `pebble build && pebble install --emulator emery --logs` (use `--logs` on install rather than a separate `pebble logs`; multiple pypkjs clients can wedge). https://github.com/lukemeyer/trickplayer-pebble
- `pebble screenshot [file]` options: `--no-correction`, `--no-open` (macOS), `--all-platforms` (builds and shoots every supported platform into `screenshots/`), `--gif-all-platforms` (needs ffmpeg, `--gif-fps` default 30, syncs to minute boundaries). https://raw.githubusercontent.com/coredevices/pebble-tool/main/pebble_tool/commands/screenshot.py
- `pebble screenshot --emulator emery shot.png` verified working. https://github.com/ArtRichards/pebble-time2-dev-setup
- `emu-tap --direction {x+,x-,y+,y-,z+,z-}` (accelerometer tap, default x+; this is NOT a touch tap).
- `emu-battery --percent 0-100 [--charging]`.
- `emu-bt-connection --connected yes|no`.
- `emu-set-timeline-quick-view on|off` (also documented https://developer.repebble.com/guides/user-interfaces/unobstructed-area/).
- `emu-time-format --format 12h|24h`; `emu-set-time HH:MM:SS|unix [--utc]`; `emu-set-content-size small|medium|large|x-large`.
- `emu-button click|push|release back|up|select|down [--duration --repeat --interval]`.
- `emu-accel`, `emu-compass`, `emu-app-config [--file]`, `emu-control`.
- Health injectors: `emu-steps`, `emu-distance`, `emu-calories`, `emu-active-time`, `emu-sleep`, `emu-heart-rate bpm [--quality]` (source labels heart-rate "emery only", see CONFLICT in section 2).
- NOT DOCUMENTED: a touch-event emulation command.

### Headless/CI
- Docker `ghcr.io/gregolsky/pebble-sdk:latest` (also Docker Hub `gregolsky/pebble-sdk`), rebuilt weekly, contains pebble-tool + SDK + Python 3.13 + Node LTS. `docker run --rm -v "$PWD":/work -w /work ghcr.io/gregolsky/pebble-sdk:latest pebble build`. Headless emulator: env `SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy`. https://github.com/gregolsky/docker-pebble-sdk
- Docker `ghcr.io/skylord123/docker-coredevices-pebble-tool:latest` (Core Devices pebble-tool + SDK preinstalled); example build/release GitHub Actions workflows upload `.pbw` artifacts. https://github.com/skylord123/docker-coredevices-pebble-tool
- Older images (andredumas/docker-pebble-dev etc.) predate the Python 3 tool; avoid. Search result list: https://github.com/andredumas/docker-pebble-dev
- No official Core Devices CI action found. NOT DOCUMENTED officially.

## 2. Pebble Time 2 / emery specifics

- Hardware table (official): emery = Pebble Time 2: 200x228, 64 colours, "Max 128k app size", 6-axis IMU + compass + barometer, microphone (+2nd mic), touch screen YES, RGB multicolour backlight, 4 buttons, rectangle, 1.5". https://developer.repebble.com/guides/tools-and-resources/hardware-information/
- CONFLICT, heart rate: hardware table says emery HRM = "No"; the HRM guide says "Pebble Time 2 and Pebble 2 (excluding SE)" have HRM; `emu-heart-rate` is labelled "emery only". Treat HR as unverified on retail PT2; always guard with `health_service_metric_accessible()` and handle 0/unavailable. https://developer.repebble.com/guides/events-and-services/hrm/ ; https://raw.githubusercontent.com/coredevices/pebble-tool/main/pebble_tool/commands/emucontrol.py
- Sibling platforms: flint (Pebble 2 SE/Duo, also 200x228 colour, no HR, touch listed Yes), gabbro (Round 2, 260x260 round). https://developer.repebble.com/guides/tools-and-resources/hardware-information/
- Emery platform introduced with SDK 4.2 (beta4, 2016-10-11); original resolution 200x228 at 202 PPI; legacy apps run in "Bezel Mode" at 144x168. https://developer.rebble.io/blog/2016/10/11/Emery-SDK-Beta/
- pebble-tool added emery support in v4.5-rc1 (2023). https://github.com/pebble/pebble-tool/releases
- Macros: `PBL_PLATFORM_EMERY`, `PBL_COLOR`, `PBL_DISPLAY_WIDTH/HEIGHT`, `PBL_TOUCH` ("running on hardware with a touch screen"); docs strongly recommend feature defines over `PBL_PLATFORM_*`. https://developer.repebble.com/guides/best-practices/building-for-every-pebble/
- Touch API (TouchService, gesture recognizers): "Touch input is currently NOT supported in watchfaces ... restricted to watchapps". Check `touch_service_is_enabled()`. Touch sensor draws power continuously while subscribed. Firmware 4.32+ maps touch to button navigation, default on in 4.33. https://developer.repebble.com/guides/events-and-services/touch/
- New 2026 APIs per July 2026 update: Touch Screen API, Speaker API, RGB Backlight API, launch-reason detection, Alloy (JS apps with FFI). PT2 battery about 21 days (their claim). https://repebble.com/blog/pebble-mega-update-july-2026
- RGB backlight C API: `light_set_color(GColor)`, `light_set_color_rgb888(uint32_t)`, `light_set_system_color()`, `light_enable_interaction()`, `light_enable()`, `light_is_on()`. SDK version not stated. https://developer.repebble.com/docs/c/User_Interface/Light/
- Content size: emery maps user Text Size Small/Med/Large to ContentSize Medium/Large/ExtraLarge; read once in init via `preferred_content_size()`. https://developer.repebble.com/guides/user-interfaces/content-size/
- System fonts: some LECO 60px variants are "Emery and newer only". https://developer.repebble.com/guides/app-resources/system-fonts/
- Alloy (JavaScript/Moddable) is the new first-class option, but this project is native C. https://developer.repebble.com/guides/alloy/

### package.json (app metadata)
- `targetPlatforms`: array; defaults to ALL if omitted. For a PT2-only face use `["emery"]` (consider adding `"flint"` since it is also 200x228 colour, unverified layout parity). https://developer.repebble.com/guides/tools-and-resources/app-metadata/
- `watchapp.watchface: true` marks a watchface (default false); also `hiddenApp`, `onlyShownOnCommunication`. Same source.
- `capabilities`: "location", "configurable", "health" are the supported ones. Same source. "health" is required for HealthService; "location" for `navigator.geolocation`; "configurable" shows the gear icon for Clay. https://developer.repebble.com/guides/events-and-services/health/ ; https://developer.repebble.com/guides/communication/using-pebblekit-js/ ; https://developer.repebble.com/guides/user-interfaces/app-configuration/
- `pebble.enableMultiJS` default true (required by Clay). `resources.media` max 256 entries. App-metadata page above.
- Example from tutorial: `"capabilities": ["location","configurable"], "messageKeys": ["TEMPERATURE","CONDITIONS","REQUEST_WEATHER","BackgroundColor","TextColor","TemperatureUnit","ShowDate"]`. https://developer.repebble.com/tutorials/watchface-tutorial/part6/
- Resource suffixes `~color`/`~bw` exist; an `~emery` suffix is NOT confirmed in the fetched text. https://developer.repebble.com/guides/best-practices/building-for-every-pebble/ and https://developer.repebble.com/guides/app-resources/platform-specific/

## 3. Battery and performance

- Tick once per minute: `tick_timer_service_subscribe(MINUTE_UNIT, handler)`; consider HOUR_UNIT for minimal faces; avoid SECOND_UNIT. https://developer.repebble.com/guides/best-practices/conserving-battery-life/
- Animations: long-running ones drain battery; run them on events (tap/shake, or at `tm_sec == 0`) and offer a toggle. Same source.
- Accelerometer data: lower rate (`accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ)`) and batch (`accel_data_service_subscribe(10, ...)`); rates 10/25 (default)/50/100 Hz; cannot `accel_service_peek()` while data-subscribed. Prefer not subscribing at all for a watchface. https://developer.repebble.com/docs/c/Foundation/Event_Service/AccelerometerService/
- `accel_tap_service_subscribe(handler)` fires per tap event; official power cost NOT DOCUMENTED. Same page. Tap on emery is available (emulate with `emu-tap`).
- Bluetooth: keep `app_comm_set_sniff_interval(SNIFF_INTERVAL_NORMAL)`; cache data with persist_* to reduce updates; return to low-power state right after transfers. Battery page above.
- Backlight: use Light API sparingly, return to automatic control. Vibes: minimize, allow disable. Battery page above.
- Dirty layers: official battery guide does not discuss `layer_mark_dirty` granularity (NOT DOCUMENTED there). Alloy docs recommend partial redraw `render.begin(x,y,w,h)` and precomputing fonts/positions (Alloy only). https://developer.repebble.com/guides/alloy/watchfaces/
- AppTimer cost per frame: NOT DOCUMENTED beyond "long-running animations drain battery".
- `quiet_time_is_active()` (SDK 4.3+): apps should avoid vibes when active; check inside tick. https://developer.repebble.com/sdk/changelogs/4.3/ ; user-facing doc https://developer.rebble.io/docs/c/User_Interface/Preferences/ (the repebble.com /Foundation/Preferences/ path 404s).
- `app_focus_service_subscribe(handler)` / `_subscribe_handlers({will_focus, did_focus})`: notifies when a notification/modal covers the app; docs suggest pausing games and syncing intro animations. Use it to pause animation timers when unfocused. https://developer.repebble.com/docs/c/Foundation/Event_Service/AppFocusService/
- `battery_state_service_subscribe/peek` gives `charge_percent`, charging flag; `connection_service_subscribe` for BT state (use handlers to skip sends while disconnected). https://developer.repebble.com/guides/events-and-services/events/
- `backlight_service_subscribe` (SDK 4.9+). https://developer.repebble.com/docs/c/Foundation/Event_Service/BacklightService/
- Unobstructed area (timeline quick view): `layer_get_unobstructed_bounds()`; `unobstructed_area_service_subscribe(UnobstructedAreaHandlers{will_change,change,did_change})`; quick-view height about 51px incl. 2px border, compute at runtime. Test with `pebble emu-set-timeline-quick-view on|off`. https://developer.repebble.com/guides/user-interfaces/unobstructed-area/
- Low power mode / battery saver: OS-level only. PebbleOS v4.9.175 (2026-05-08) enters a "low power watchface" at 4% battery (was 2%); v4.9.183 (2026-05-28) shows date on it; "Battery Saver" is a backlight setting in Settings > Display > Backlight. Search snippet from PebbleOS changelog https://ndocs.repebble.com/PebbleOS-Changelog-25efbb55ea84801da04bfcf73c9346e1 (page not directly fetchable; verify). An app-facing API to detect low power / battery saver: NOT DOCUMENTED. Use `battery_state_service_peek()` as a proxy.
- Health: need `"capabilities":["health"]`; not on aplite; check `health_service_metric_accessible()` first. https://developer.repebble.com/guides/events-and-services/health/
- `health_service_events_subscribe()` allocates up to 2048 bytes on the app heap and returns false if memory is short. https://developer.repebble.com/docs/c/Foundation/Event_Service/HealthService/
- Metrics: StepCount, ActiveSeconds, WalkedDistanceMeters, SleepSeconds, HeartRateBPM, ActiveKCalories, RestingKCalories. Events: SignificantUpdate, MovementUpdate, SleepUpdate, MetricAlert, HeartRateUpdate. Same page.
- Cheap pattern for steps: read `health_service_sum_today(HealthMetricStepCount)` on tick or on SignificantUpdate; per-call cost NOT DOCUMENTED. `health_service_peek_current_value` returns 0 for cumulative metrics. Same page.
- HR sampling: default period 10 min, settable 1..600 s via `health_service_set_heart_rate_sample_period`, persists after exit unless cancelled ("always cancel before exiting"). https://developer.repebble.com/guides/events-and-services/hrm/ ; HealthService page above.
- Touch: unsupported in watchfaces anyway, so no battery concern. https://developer.repebble.com/guides/events-and-services/touch/

## 4. Fonts

- Custom font resource: `{"type":"font","name":"EXAMPLE_FONT_20","file":"x.ttf","characterRegex":"[0-9:]","compatibility":"2.7"}`; name must end with the pixel size; becomes `RESOURCE_ID_<name>`; load with `fonts_load_custom_font()` and unload with `fonts_unload_custom_font()`. https://developer.repebble.com/guides/app-resources/fonts/
- `characterRegex` is a Python regex that restricts glyphs to shrink the resource (e.g. `[0-9:APM ]`). Same page.
- Recommended max size 48 (page text); a search snippet says build errors if generated font data is too large (about 60px cap for some fonts). Same page; snippet via https://github.com/reaperfied/91-dub-v5-plus
- `compatibility: "2.7"` selects the pre-2.8 renderer. Same page.
- `trackingAdjust`: appears in real projects as an integer pixel adjustment (search snippet); the official fonts page does NOT document it. Verify in a test build before relying on it.
- Custom font memory/heap cost per font: NOT DOCUMENTED.
- Anti-aliasing: `graphics_context_set_antialiased(ctx, bool)` applies to STROKE drawing only (default true); no doc on text AA or forcing 1-bit text. https://developer.repebble.com/docs/c/Graphics/Graphics_Context/
- Stroke width: only odd widths fully supported (even rounded down); 0 ignored. Same page.
- System fonts: Raster Gothic 14-28, Bitham 30-42, Roboto Condensed 21/Bold 49, Droid Serif 28 bold, LECO 20-60 (60px emery+ only); emoji only in Gothic 18/24 (+bold). https://developer.repebble.com/guides/app-resources/system-fonts/
- Coloured custom fonts example (third-party): https://duhrer.github.io/2025-02-24-pebble-watch-face-colour/

## 5. AppMessage, PebbleKit JS, persist, Clay, weather

- `app_message_open(inbox, outbox)`; size = sum of key/value sizes of the largest message; tutorials use 64/256, 128/128, and 256 for Clay payloads. Dropped-inbox callback fires on overflow; wait for the outbox-sent callback before sending another. https://developer.repebble.com/guides/communication/sending-and-receiving-data/ ; https://developer.repebble.com/tutorials/watchface-tutorial/part4/ ; https://developer.repebble.com/tutorials/watchface-tutorial/part6/
- Hard max sizes (`app_message_inbox_size_maximum()`): NOT DOCUMENTED in fetched pages. Dictionary size: `(int)received->end - (int)received->dictionary`. https://developer.repebble.com/faqs/
- Types: u/int 8/16/32, cstring, byte array. Same send/receive page.
- `messageKeys` in package.json (list, or mapping); become `MESSAGE_KEY_<name>` in C via pebble.h; array syntax `"LapTimes[10]"` creates indexed keys. https://developer.repebble.com/guides/communication/using-pebblekit-js/
- JS must wait for the `ready` event before sending. Same page.
- persist_*: 4 kB total per app, 256 bytes per value (`PERSIST_DATA_MAX_LENGTH`), uint32 keys; bool/int/string/data + `persist_exists/delete`. Write at launch/exit, version your storage scheme. https://developer.repebble.com/guides/events-and-services/persistent-storage/
- Clay: official guide calls it "the recommended approach"; install `pebble package install @rebble/clay` (SDK 3.13+, `enableMultiJS` true); old name `pebble-clay`. https://developer.repebble.com/guides/user-interfaces/app-configuration/ ; https://github.com/pebble-dev/clay
- `@rebble/clay` is a revival (announced 2025-12-21, v1.0.6 by 2026-01-02) adding flint/all-platform builds. https://forum.rebble.io/t/rebble-clay-new-way-to-configure-your-pebble-and-core-apps-and-faces/331
- Compatibility of Clay pages with the new Core Devices app: the official guide says it matches the app style; no explicit statement or test list found. The tutorial exists for C (watchface-tutorial part6) and Alloy, implying support. A forum note reports `emu-app-config` timeouts with Safari on macOS only. Treat as "likely works, verify on device".
- Weather: official tutorial uses Open-Meteo (keyless): phone-side PKJS gets `navigator.geolocation.getCurrentPosition(..., {timeout:15000, maximumAge:60000})`, XHRs Open-Meteo, maps WMO weather codes, sends TEMPERATURE/CONDITIONS over AppMessage; watch requests refresh with REQUEST_WEATHER when `tm_min % 30 == 0`. Needs `"location"` capability. https://developer.repebble.com/tutorials/watchface-tutorial/part4/
- Fetching on-watch is discouraged (memory); phone-side is the pattern. Third-party statement: https://github.com/SlyeghtlyRye/pebble-watchfaces (Alloy context)
- Handle location denial with fallback values. https://developer.repebble.com/guides/communication/using-pebblekit-js/
- Open-Meteo itself: not an official Pebble doc beyond the tutorial; its terms/rate limits NOT covered here.

## 6. Memory limits and gotchas (emery)

- Official: "Max 128k app size" for emery. https://developer.repebble.com/guides/tools-and-resources/hardware-information/
- BUT as of SDK 4.33.1 / fw 4.33 the real cap was about 64 KiB for static footprint (uint16 `virtual_size`), build error "Must be 65535 bytes or smaller" when adding 8 KB of statics; actual emery segment is 135,168 bytes. Issue closed by PR #2174. https://github.com/coredevices/PebbleOS/issues/1873
- PR #2174 (merged 2026-10-01, i.e. today) lifts the cap to 128 KiB via a new header struct version 0x10.0x01 and SDK 0x6b; older firmware rejects oversized apps with a clear error. Need a new SDK AND new firmware; not yet in a release as of today (unverified). https://github.com/coredevices/PebbleOS/pull/2174
  Practical rule: budget under 64 KiB static (code is separate; data/bss is what counts) until a release containing the fix is on your watch.
- Heap: aplite about 24 KB; newer platforms "substantially more" (no exact C figure documented). Use `heap_bytes_free()` / `heap_bytes_used()`; read the "Free RAM" line in build output. https://developer.repebble.com/faqs/
- Alloy/XS apps share about 122.5 KB heap on emery; that is Alloy-specific, shown as a reference for total RAM. https://github.com/coredevices/pebbleos/issues/1621 ; https://github.com/lukemeyer/trickplayer-pebble
- Memory exhaustion in a watch app can crash/reset the watch (third-party note); keep a known-good build. https://github.com/lukemeyer/trickplayer-pebble
- HealthService subscription costs up to 2 KB heap. https://developer.repebble.com/docs/c/Foundation/Event_Service/HealthService/
- Resources: max 256 media entries per app. https://developer.repebble.com/guides/tools-and-resources/app-metadata/
- Build for all content sizes; do not hardcode 200x228 or quick-view height; use `layer_get_unobstructed_bounds` and `PBL_DISPLAY_*`. https://developer.repebble.com/guides/user-interfaces/content-size/ ; https://developer.repebble.com/guides/user-interfaces/unobstructed-area/
- Color: use `GColor` 64-colour palette; preview with https://developer.repebble.com/guides/tools-and-resources/color-picker/
- Docs hub / TOC for further reading: https://developer.repebble.com/guides/toc/
