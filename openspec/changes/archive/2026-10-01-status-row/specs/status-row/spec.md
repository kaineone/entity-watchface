# Spec Delta

## Purpose
Defines the status elements on the Pebble Time 2 face: the phone-link glyph, the battery
percentage, the quiet-time mark, and the vibration on disconnect.

## ADDED Requirements

### Requirement: Link glyph
The glyph SHALL be four bars 4 px wide with 2 px gaps and heights 5, 8, 11 and 14 px,
bottom-aligned in the frame 8,208,22,14; AAAA55 when the phone app is connected, 555555 otherwise.

#### Scenario: Disconnected glyph
- **WHEN** the phone app connection is lost
- **THEN** all four bars turn 555555

### Requirement: Battery percentage
The percentage SHALL be drawn right-aligned in 120,206,72,16 with LABEL_16 as `<n>%`.
While charging or plugged in it SHALL read `+<n>%` in FFAA55. Otherwise it SHALL be FF0000 at or
below the low-battery threshold (default 20) and AAAA55 above it.

#### Scenario: Low and charging
- **WHEN** the battery is at 15 % and charging
- **THEN** the text reads `+15%` in FFAA55

#### Scenario: Low battery
- **WHEN** the battery is at 20 % and not charging
- **THEN** the text reads `20%` in FF0000

### Requirement: Quiet-time mark
While quiet time is active the face SHALL draw a hollow 8×8 square with a 2 px AA5500 border at
8,104; it SHALL be hidden otherwise. It is re-evaluated on every minute tick.

#### Scenario: Quiet time on
- **WHEN** quiet time is active at a minute tick
- **THEN** the mark is visible

### Requirement: Disconnect vibration
On losing the phone app connection the watch SHALL vibrate once with a short pulse, but only if
the vibrate setting is on (default on) and quiet time is not active. It SHALL NOT vibrate on
reconnect or when the face starts while already disconnected.

#### Scenario: Disconnect during quiet time
- **WHEN** the link drops while quiet time is active
- **THEN** there is no vibration

### Requirement: Quick view
In quick view the link glyph SHALL move to y 146 and the percentage to y 144; the quiet mark
stays put.

#### Scenario: Peek
- **WHEN** quick view appears
- **THEN** the glyph and the percentage sit above the peek with their bottoms at y 160
