# Pebble Round 2 (gabbro) + Pebble Time 2 (emery): condensed reference
Compiled 2026-10-01. Scope: emery + gabbro only. chalk (2015 Pebble Time Round) cited only where its guides are the sole source of round API guidance.
Legend: [LOCAL] = read directly from installed SDK 4.33.1 (pebble-tool 5.0.40) at
`/home/elim/.local/share/pebble-sdk/SDKs/4.33.1/sdk-core/pebble/` (authoritative for this build; call it SDKROOT).
Online docs are partly stale: developer.repebble.com guides still pre-date gabbro in several places (flagged below).

## 1. Platform facts
- Platform name `gabbro` = Pebble Round 2; 260x260, round, 64 colours. Blog: "The Round 2 display is 260x260 pixels, up from 180x180 on Pebble Time Round." https://repebble.com/blog/cloudpebble-returns-plus-pure-javascript-and-round-2-sdk
- Blog says Round 2 build/emulator support shipped with the SDK around PebbleOS/SDK 4.9.127+ ("SDK 4.9.127 or later"; blog does not name an exact first gabbro SDK version). Same URL. First-gabbro-SDK-version: not documented beyond that.
- Available today: yes. Installed SDK 4.33.1 has `pebble/gabbro/` (headers, lib, qemu images) and `pebble install --emulator gabbro` is a valid choice. [LOCAL] `pebble install --help`; SDKROOT/gabbro/
- Emulator name: `gabbro`. Blog form `pebble install emulator --gabbro`; actual CLI in 5.0.40 is `pebble install --emulator gabbro` (also accepted by `pebble emu-set-timeline-quick-view --emulator gabbro`). [LOCAL] CLI help; example repo https://github.com/fsargent/pebble-slow-24h
- gabbro definition [LOCAL SDKROOT/common/tools/pebble_sdk_platform.py]:
  - DEFINES: PBL_PLATFORM_GABBRO, PBL_COLOR, PBL_ROUND, PBL_MICROPHONE, PBL_HEALTH, PBL_COMPASS, PBL_TOUCH, PBL_DISPLAY_WIDTH=260, PBL_DISPLAY_HEIGHT=260
  - TAGS (resource suffixes): gabbro, color, round, mic, health, compass, touch, 260w, 260h
  - Limits: app binary 128K, app RAM 128K, worker RAM 10K, resources 1024K (256K appstore limit), max font glyph 512, HAS_MODDABLE_XS true
  - No PBL_SPEAKER, PBL_RGB_BACKLIGHT, smartstrap on gabbro (those are in emery's list).
- emery definition [LOCAL same file]: 200x228, PBL_COLOR, PBL_RECT, PBL_TOUCH, PBL_SPEAKER, PBL_RGB_BACKLIGHT, PBL_MICROPHONE, PBL_HEALTH, PBL_COMPASS, smartstrap; TAGS emery,color,rect,mic,strap,health,strappower,compass,touch,speaker (+ w/h tags); 128K binary/RAM, 512 glyph.
- Same file: chalk is the old 180x180 PBL_ROUND platform; still in the SDK, not our target.
- Compat: "Old PTR apps/faces automatically scale and work great on PR2 without any changes" (i.e. chalk binaries are scaled; native gabbro build is better). Blog URL above.
- Online hardware table is unreliable/garbled and lists gabbro without specs: https://developer.repebble.com/guides/tools-and-resources/hardware-information/ ("not documented" there; trust [LOCAL]). Third-party spec: 1.3in, 64-colour e-paper, 260x260, 200 ppi (gadgetsandwearables.com, secondary source).
- PBL_PLATFORM_GABBRO is listed among platform defines: https://developer.repebble.com/guides/best-practices/building-for-every-pebble/
- New API on gabbro/emery: Touch service (`touch_service_subscribe(TouchServiceHandler, ctx)`, TouchEvent types Touchdown/Liftoff/PositionUpdate, int16 x,y, `non_navigational` flag). Touch sensor enabled only while subscribed. [LOCAL SDKROOT/gabbro/include/pebble.h ~L930-962]. Guard with `#ifdef PBL_TOUCH`. Not covered in online guides I fetched.

## 2. Round-display API and design guidance
(Round guide is chalk-era but the APIs are in the gabbro headers. https://developer.repebble.com/guides/user-interfaces/round-app-ui/)
- Macros: `PBL_RECT` / `PBL_ROUND`; `PBL_IF_RECT_ELSE(rect, round)` and `PBL_IF_ROUND_ELSE(round, rect)`. Note argument order differs; confirm with header: in gabbro pebble.h `PBL_IF_ROUND_ELSE(if_true, if_false)` returns if_true. [LOCAL SDKROOT/gabbro/include/pebble.h L3556]. Guide URL above.
- `PBL_DISPLAY_WIDTH` / `PBL_DISPLAY_HEIGHT`: defined per platform (260/260 gabbro, 200/228 emery). [LOCAL pebble_sdk_platform.py]. Online preprocessor doc page 404'd for me (https://developer.repebble.com/docs/c/preprocessor/): not verified there.
- Avoid hardcoded coords: use `layer_get_bounds(parent)` (or unobstructed bounds) rather than fixed sizes. https://developer.repebble.com/guides/user-interfaces/round-app-ui/ and https://developer.repebble.com/guides/best-practices/building-for-every-pebble/
- Circular drawing: `graphics_draw_arc()` (clockwise between angles in a GRect), `graphics_fill_radial()` (inner radius adjustable), `gpoint_from_polar()`, `DEG_TO_TRIGANGLE()`. Round guide URL above. Header: `TRIG_MAX_ANGLE 0x10000`, `DEG_TO_TRIGANGLE(a) = a*TRIG_MAX_ANGLE/360`, `grect_inset(GRect, GEdgeInsets)`, `gpoint_from_polar(GRect, GOvalScaleMode, int32_t)`. [LOCAL gabbro/include/pebble.h L314, L324, L4112, L4391-4402]
- Text flow / paging: TextLayer `text_layer_enable_screen_text_flow_and_paging(layer, inset)` (call AFTER adding layer to hierarchy); ScrollLayer `scroll_layer_set_paging(sl,true)`; manual: `graphics_text_attributes_enable_screen_text_flow(attrs, inset)` + `graphics_text_attributes_enable_paging(attrs, origin, bounds)`. Example inset value 5 px. Round guide URL above; function present in header [LOCAL L4963].
- Recommended margins: guide gives only the text-flow inset (example 5 px). No general safe-area/margin number documented. Community evidence: fit content to the inscribed square or inset ~40 px on 260px gabbro so corners are not clipped (PRs, not official): https://github.com/meded90/pebble-time-2-pixel-faces/pull/1 (inset ~40) and https://github.com/dmnd/illudere/pull/4 (digits fitted to inscribed square). Inscribed square of 260 circle is ~184 px (derived by me: 260/sqrt2).
- Framebuffer: use `gbitmap_get_data_row_info()` (returns row ptr + `min_x`/`max_x` visible bounds) instead of linear indexing on round. Round guide URL above. Round rows have varying valid x range; code that writes the framebuffer directly must clamp to min_x/max_x.
- Content indicators for scroll content: `scroll_layer_get_content_indicator`, `content_indicator_create`. Same URL.
- ActionBar width and similar system constants have per-platform values, gabbro 40 [LOCAL pebble.h L8036]. Irrelevant for a watchface.

## 3. Unobstructed area / Timeline Quick View on round
- UnobstructedArea API exists (SDK 4.0+): `layer_get_unobstructed_bounds()`, `unobstructed_area_service_subscribe()` with `.will_change/.change/.did_change`. https://developer.rebble.io/guides/user-interfaces/unobstructed-area/ and https://developer.repebble.com/guides/user-interfaces/unobstructed-area/
- Round: guide says "Timeline Quick View is not planned for the Chalk platform." Same URLs. Header comment: "Timeline Peek is also limited to rectangular platforms, thus using Unobstructed Area on Chalk will also result in no events." [LOCAL gabbro/include/pebble.h ~L6834] (text names Chalk only; gabbro-specific statement is "not documented" officially).
- Practical reading: gabbro is PBL_ROUND like chalk; community reports quick view is not enabled on round watches (forum thread https://forum.rebble.io/t/is-quick-view-supported-in-pebble-round/409, secondary). Unverified on hardware or emulator by me; test with `pebble emu-set-timeline-quick-view on --emulator gabbro`.
- Recommended handling (docs): fill the whole window regardless of obstruction (avoids artifacts); compute from `layer_get_unobstructed_bounds()` at runtime (peek is "51px in total" on rect, but compute, do not hardcode); use proportional layout. Debug: `pebble emu-set-timeline-quick-view on|off`. Guide URLs above.
- Design implication: emery needs the unobstructed path; gabbro will most likely behave as full-screen but using unobstructed bounds on both costs nothing and is future-proof.

## 4. Per-platform resources and one-codebase practice
- Filename tags with `~`: e.g. `image~color~round.png`; "All tags must match for the file to be used"; "the one with the most tags wins". https://developer.repebble.com/guides/app-resources/platform-specific/
- Online tag list on that page omits gabbro (and 260w/260h), and lists chalk/emery/flint etc. [guide not updated]. The SDK's own TAGS for gabbro are `gabbro, color, round, mic, health, compass, touch, 260w, 260h` [LOCAL], so `foo~gabbro.png`, `foo~round.png`, `foo~260w.png`, `foo~touch.png` should all resolve. Verify with an actual build before relying on exotic tags.
- Official advice: avoid platform-name tags (basalt, etc.) because new platforms need new files; prefer `color`, `round`, `rect`. Same guide URL. Counterpoint: real gabbro port used `background~gabbro.png` (260x260) because art was round-specific: https://github.com/meded90/pebble-time-2-pixel-faces/pull/1
- `targetPlatforms` in package.json (array under `pebble`) limits which platforms the app builds for; guide shows `"targetPlatforms": ["basalt"]` as a resource-level example. Same guide URL. Port example added `"gabbro"` next to `"emery"` in targetPlatforms. PR #1 URL above. For us: `["emery","gabbro"]`.
- Conditional compilation: guide strongly prefers feature defines over `PBL_PLATFORM_*`: "conditionally compile code using applicable feature defines instead of PBL_PLATFORM defines to be as specific as possible." https://developer.repebble.com/guides/best-practices/building-for-every-pebble/
  - Use `PBL_ROUND`/`PBL_RECT`, `PBL_COLOR`, `PBL_TOUCH`, `PBL_DISPLAY_WIDTH/HEIGHT` and `PBL_IF_ROUND_ELSE`. Reserve `PBL_PLATFORM_EMERY`/`PBL_PLATFORM_GABBRO` for genuinely platform-only tweaks.
  - Derive geometry from `layer_get_unobstructed_bounds(root)` at runtime; never use literals like 200/228/260 (same URL).
  - Both targets are PBL_COLOR, so one colour palette code path serves both; the only structural split is shape (round vs rect) and unobstructed area.
- Memory: both emery and gabbro cap app RAM at 128K and binary at 128K [LOCAL], so the budget is identical; gabbro 260x260 framebuffer (67.6K px at 8bpp, derived) vs emery 200x228 (45.6K px). Gabbro framebuffer is ~48% larger; watch full-screen bitmap/offscreen buffers.

## 5. Battery
- Official: "Many watchfaces unnecessarily tick once a second by using SECOND_UNIT ... when they only update the display once a minute." Use MINUTE_UNIT so wakeups per minute are reduced; HOUR_UNIT for minimal faces. https://developer.repebble.com/guides/best-practices/conserving-battery-life/
- Animations: a half-second animation every second drains faster than once per minute; prefer animating on wrist raise or tap/shake. Same URL.
- Backlight: keeping it on constantly kills battery in hours. Same URL.
- Tick timer service guide shows MINUTE_UNIT example; it does not quantify SECOND_UNIT cost: not documented. https://developer.repebble.com/guides/events-and-services/events/
- Round-specific power notes: not documented. No official gabbro battery guidance found. Third-party: Round 2 claimed 10-14 day battery life (reviews, e.g. https://gadgetsandwearables.com/technical-specs/pebble-round-2/), so second-hand ticking directly erodes that headline.
- Suggested approach (my inference, not a doc statement): subscribe to SECOND_UNIT only while the face is visible and a seconds element is shown (and optionally only after wrist-raise via accel tap/ `app_focus`), else MINUTE_UNIT.

## Gaps / not documented
- Exact SDK version that first added gabbro; official gabbro margin numbers; official statement that gabbro lacks quick view; round-specific battery notes; online docs for `PBL_DISPLAY_WIDTH` (404) and gabbro resource tag. Resolve by testing on the `gabbro` emulator.
