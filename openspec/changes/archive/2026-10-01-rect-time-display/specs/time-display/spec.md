# Spec Delta

## Purpose
Defines how Entity shows the hour, minute, am/pm marker and date on the Pebble Time 2, how the
layout adapts to timeline quick view, and how often each element is refreshed.

## ADDED Requirements

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
