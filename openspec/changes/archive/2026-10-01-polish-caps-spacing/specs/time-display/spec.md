# Spec Delta

## ADDED Requirements

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
