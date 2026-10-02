# Spec Delta

## ADDED Requirements

### Requirement: Drawn block numerals
Hour and minute SHALL be drawn as filled polygons, not text. Each digit occupies a cell of a
two-cell row grid that spans the screen width minus a margin (8 px on the Time 2). Digit strokes
are about 17/64 of the row height. Top-left convex corners are cut at 45° by two thirds of the
stroke; the 7 also has its top-right corner cut; no other corners are cut. A 1 is a vertical
bar with a top-left flag, aligned to the right edge of its cell. A single-digit 12-hour hour sits
in the right cell.

#### Scenario: Nine o'clock in 12-hour mode
- **WHEN** the time is 9:47 in 12-hour mode
- **THEN** the hour's left cell is empty, `9` fills the right cell, and `PM`/`AM` shows in the status row

### Requirement: Labels and palette
All label text SHALL use the Pebble system font Gothic 14 Bold in upper case. Colours: hour FF0000
(or the chosen hour colour), minute and labels FFFF00, temperature FFAA00, meter resting bars
AAAA00 with the heat ramp FF5500, FFAA00, FFAA55, FFFF00 and the cursor FF0000. Black-and-white
watches use white.

#### Scenario: Default face
- **WHEN** the face shows with default settings on a colour watch
- **THEN** the hour is red, the minute and labels yellow, and the temperature amber
