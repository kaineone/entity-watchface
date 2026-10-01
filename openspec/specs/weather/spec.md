# weather Specification

## Purpose
Defines how Entity gets the current weather on the phone, sends it to the watch, and shows and
ages it in the top-right readout of the Pebble Time 2 face.

## Requirements

### Requirement: Fetch on the phone
When weather is on, the phone SHALL get the location (timeout 15 s, accepting a cached fix up to
30 minutes old), fetch current temperature and weather code from Open-Meteo, and send a message
to the watch when the face starts and then every 30 minutes. When weather is off it SHALL not
request the location or the network.

#### Scenario: Weather off
- **WHEN** the user has switched weather off
- **THEN** no location request and no network request are made

### Requirement: Condition words
WMO codes SHALL map to: 0 and 1 → `clear`; 2, 3, 45, 48 → `cloud`; 51–67 and 80–82 → `rain`;
71–77, 85, 86 → `snow`; 95–99 → `storm`; anything else → `cloud`.

#### Scenario: Thunderstorm
- **WHEN** Open-Meteo reports code 95
- **THEN** the watch shows `storm`

### Requirement: Readout
The top-right readout SHALL be drawn at 72,8,120,16, LABEL_16, right-aligned, as `<word> <t>°`
where t is the temperature rounded to a whole degree in °C, or in °F when that setting is on
(converted on the watch from tenths of a degree). Colour FFAA55. Hidden when weather is off.
Temperatures below zero SHALL show a minus sign.
The readout's text box SHALL stay at least 4 px from the end of the date's rendered text (about
6 px of visible space once glyph side bearings are counted). When the full reading would come closer, it SHALL first drop the `~` stale prefix (the grey colour still marks it
stale), then drop the space between word and temperature.

#### Scenario: Fahrenheit
- **WHEN** the phone reports 18.3 °C and Fahrenheit is on
- **THEN** the readout reads `clear 65°`

#### Scenario: Long stale reading
- **WHEN** the reading is stale and reads `~clear -12°`, which does not fit beside `thu 01.10`
- **THEN** the readout shows `clear-12°` in 555555

#### Scenario: Below zero
- **WHEN** the phone reports −3.4 °C
- **THEN** the readout reads `snow -3°`

### Requirement: Staleness and persistence
If no weather update has arrived for 60 minutes, the readout SHALL turn 555555 and gain a `~`
prefix (`~clear 18°`). The last reading and its time SHALL be persisted and shown after a
restart, with staleness judged from the stored time. Before any reading exists, the readout is empty.

#### Scenario: Phone away for an hour
- **WHEN** 61 minutes pass since the last update
- **THEN** the readout reads `~clear 18°` in 555555
