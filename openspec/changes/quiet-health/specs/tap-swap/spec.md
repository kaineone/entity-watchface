# Spec Delta

## ADDED Requirements

### Requirement: Quiet when health is unavailable
The face SHALL NOT cause the watch to show a message asking the user to enable Pebble Health. On
firmware older than 4.9 it SHALL NOT read any health metric until the watch has delivered a step
update. A double tap SHALL do nothing while steps are unavailable. On rectangular watches, when
heart rate is unavailable, the swapped view SHALL keep the temperature instead of showing a
placeholder.

#### Scenario: Health off on a classic Pebble Time
- **WHEN** Pebble Health tracking is off and the user double taps
- **THEN** the top row stays as it is and no popup appears

#### Scenario: Steps without heart rate
- **WHEN** steps are available but heart rate is not, and the user double taps a Pebble Time
- **THEN** the top left shows the steps and the top right keeps the temperature
