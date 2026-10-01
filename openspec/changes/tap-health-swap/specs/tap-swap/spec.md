# Spec Delta

## Purpose
Defines how a tap on the Pebble Time 2 swaps the top row of Entity to steps and heart rate and
how it returns.

## ADDED Requirements

### Requirement: Tap swaps the top row
When the tap setting is on, an accelerometer tap SHALL replace the top-left text with today's step
count as `<n> steps` (AAAA55) and the top-right text with the latest heart rate as `<n> bpm`
(FFAA55), shown even when weather is off. A value the watch cannot provide SHALL read `--`
(`-- steps`, `-- bpm`). After 10 seconds, or on the next tap, the row SHALL return to date and weather.

#### Scenario: Tap and wait
- **WHEN** the user taps the watch and waits
- **THEN** the top row shows `6240 steps` and `72 bpm`, and 10 seconds later shows the date and weather again

#### Scenario: No heart-rate reading
- **WHEN** heart rate is not accessible
- **THEN** the right side reads `-- bpm`

### Requirement: Cost
The tap service SHALL be subscribed only while the tap setting is on, health values SHALL be read
only when a tap swaps the row in, and the 10-second timer SHALL exist only while swapped.

#### Scenario: Setting off
- **WHEN** the tap setting is turned off
- **THEN** the face unsubscribes from the tap service and a tap does nothing
