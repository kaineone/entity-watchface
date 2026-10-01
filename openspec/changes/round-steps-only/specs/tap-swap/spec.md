# Spec Delta

## ADDED Requirements

### Requirement: Round shows steps only
On round platforms a tap SHALL replace only the top readout with today's steps; the bottom
readout SHALL keep showing the weather, and nothing reads `bpm`.

#### Scenario: Tap on the Round 2
- **WHEN** the user taps a Pebble Round 2
- **THEN** the top reads `6240 steps` and the bottom still reads the weather
