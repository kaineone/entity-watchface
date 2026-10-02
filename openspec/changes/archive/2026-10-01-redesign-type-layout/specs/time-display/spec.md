# Spec Delta

## ADDED Requirements

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
