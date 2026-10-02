# time-display Specification

## Purpose
Defines how Entity shows the hour, minute, am/pm marker and date on the Pebble Time 2, how the
layout adapts to timeline quick view, and how often each element is refreshed.

## Requirements

### Requirement: Hour and minute formatting
The hour SHALL be shown with a leading zero in 24-hour mode (`09`, `21`) and without one in
12-hour mode (`9`, `12`); 12-hour mode maps 0 to 12. The minute SHALL always be two digits.
12-hour vs 24-hour SHALL follow `clock_is_24h_style()` until the user overrides it in settings.

#### Scenario: 24-hour morning
- **WHEN** the time is 09:04 in 24-hour mode
- **THEN** the hour reads `09` and the minute reads `04`, and no am/pm marker is shown

#### Scenario: 12-hour midnight and noon
- **WHEN** the time is 00:30 or 12:30 in 12-hour mode
- **THEN** the hour reads `12` and the am/pm marker reads `am` or `pm` respectively

### Requirement: Date readout
The top-left readout SHALL show `<dow> <dd>.<mm>` with a lower-case three-letter English day
name, e.g. `tue 08.09` for Tuesday 8 September.

#### Scenario: Date format
- **WHEN** the date is Tuesday 8 September
- **THEN** the readout reads `tue 08.09`

### Requirement: Emery layout and colours
On emery (200×228) the face SHALL use a black background and these frames (x,y,w,h), single
line, trailing ellipsis on overflow:
top-left readout 8,8,120,16 LABEL_16 left-aligned AAAA55; hour 8,30,184,64 ZEN_64 left-aligned
FF5555 by default; am/pm 140,36,52,16 LABEL_16 right-aligned AAAA55 (12-hour only);
minute 8,96,184,64 ZEN_64 right-aligned AAAA55. Two-digit values such as `00` and `08`
SHALL render in full with no ellipsis.

#### Scenario: Default appearance
- **WHEN** the face is shown on emery at 21:04 in 24-hour mode
- **THEN** `21` is drawn in FF5555 at the hour frame and `04` in AAAA55 at the minute frame

### Requirement: Quick view reflow
When the unobstructed height is less than the full screen height, the digits SHALL switch to
ZEN_48 with the hour frame at 8,30,184,48 and the minute frame at 8,80,184,48; the top row is
unchanged. When the obstruction clears, the normal layout SHALL be restored.

#### Scenario: Peek appears and clears
- **WHEN** timeline quick view is turned on and then off
- **THEN** the digits shrink to ZEN_48 at the quick-view frames, then return to ZEN_64 at the normal frames

### Requirement: Refresh cost
The face SHALL subscribe to `MINUTE_UNIT` ticks only for time display, SHALL update the date
text only when the day changes, and SHALL not redraw text layers whose string did not change.

#### Scenario: Minute tick on the same day
- **WHEN** a minute tick arrives and the day has not changed
- **THEN** the date layer's text is not reset

### Requirement: Bold centred numerals
Hour and minute SHALL be drawn in Orbitron Black, each centred horizontally on the screen, the
hour above the minute, with the hour in the hour colour and the minute in AAAA55 (white on
black-and-white watches). In quick view both shrink to the platform's small numeral size.

#### Scenario: Time 2 at 21:04
- **WHEN** the face shows 21:04 on emery
- **THEN** `21` and `04` share one vertical centre line and neither touches the meter or the top row

### Requirement: Locale date
The date SHALL read `<dow> <mon> <d>` (e.g. `thu oct 1`) when the watch locale is en_US and
`<dow> <d> <mon>` (e.g. `thu 1 oct`) otherwise, with lower-case English three-letter names and
no leading zero on the day.

#### Scenario: US locale
- **WHEN** the watch locale is en_US on 1 October, a Thursday
- **THEN** the date reads `thu oct 1`

### Requirement: Separated numerals
Every two-digit hour and minute, on every platform, SHALL show visible space between its two
digits at the top of the glyphs (no `1` flag or `7` bar reading as joined). This SHALL be verified
by a sweep of all 60 minute strings, all 24 hour strings and 12-hour hours 1 to 12 on each platform.

#### Scenario: Seventeen
- **WHEN** the time is 17:17
- **THEN** the `1` and `7` read as two separate digits in both rows

### Requirement: Capitals on the watch
All text on the watch face SHALL be upper case (date, am/pm, steps, bpm). Numbers, `%`, `°`, `~`,
`+` and `-` are unchanged.

#### Scenario: Date
- **WHEN** it is Thursday 1 October in en_US
- **THEN** the date reads `THU OCT 1`

### Requirement: Hour outline
On colour watches, when the hour colour is red, pink, purple, blue, teal or green, the hour digits
SHALL carry a 2 px outline in the minute colour, following every edge including the chamfers. The
outlined hour SHALL occupy the same frame as an un-outlined one: the digits are inset by 2 px on
each side and drawn with a stroke 1 px thinner. Cream, gold and white hours, and black-and-white
watches, SHALL have no outline.

#### Scenario: Backlight off
- **WHEN** the hour is red and the backlight is off
- **THEN** the hour digits are ringed in cream and line up with the minute digits' edges

#### Scenario: Cream hour
- **WHEN** the hour colour is cream
- **THEN** the hour is drawn without an outline, exactly as before
