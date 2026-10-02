# Spec Delta

## ADDED Requirements

### Requirement: Temperature-only readout
The weather readout SHALL show only the rounded temperature with a degree sign (`26°`, `-3°`),
in °C or °F per the setting. A stale reading SHALL show a `~` prefix and, on colour watches, the
555555 colour. The condition is still received and stored but not shown.

#### Scenario: Stale on the Time 2
- **WHEN** the last reading is over an hour old
- **THEN** the readout reads `~26°` in 555555
