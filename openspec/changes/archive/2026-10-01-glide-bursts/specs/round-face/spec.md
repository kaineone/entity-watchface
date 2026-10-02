# Spec Delta

## ADDED Requirements

### Requirement: Rim bursts with a sliding cursor
Round faces SHALL use the same bursts at 5 frames per second and SHALL NOT subscribe to second
ticks. During a burst the cursor follows the current second including milliseconds; its length is
split between the two ticks it lies between in proportion to its position. Outside bursts the
ticks rest in gold.

#### Scenario: Between seconds
- **WHEN** the time is 10:02:15.5 during a burst
- **THEN** ticks 15 and 16 both show part of the cursor length
