# Spec Delta

## ADDED Requirements

### Requirement: Saved settings follow every watch
When the face starts, the phone SHALL send the settings last saved on the settings page, if any, to
the watch, which SHALL apply them like a save. Only the eight setting keys SHALL be sent.

#### Scenario: Second watch
- **WHEN** Fahrenheit was saved while another watch was connected and the face then starts on this
  watch
- **THEN** this watch shows the temperature in Fahrenheit
