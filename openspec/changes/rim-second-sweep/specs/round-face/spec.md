# Spec Delta

## MODIFIED Requirements

### Requirement: Rim states and cost
Unlinked SHALL draw all ticks 2 px in 555555; frozen all ticks 2 px in AAAA55 (same freeze rules
as the rectangular meter). Round faces SHALL subscribe to SECOND_UNIT ticks only while the rim
animates and the face has focus, and MINUTE_UNIT otherwise. Each second SHALL mark only the rim
layer dirty. Round faces SHALL NOT run bursts or any frame timer; a wrist flick does not change the
rim.

#### Scenario: Low battery on round
- **WHEN** the battery drops to the threshold while not charging
- **THEN** the face switches to minute ticks and draws the frozen rim once

#### Scenario: Flick on round
- **WHEN** the user flicks their wrist on a Pebble Round 2
- **THEN** the rim keeps stepping once a second and no frame timer starts

## REMOVED Requirements

### Requirement: Rim bursts with a sliding cursor
**Reason**: The rim steps once a second from second ticks, a full lap each minute; bursts and the
sub-second cursor made it creep and pulse.
**Migration**: Covered by "Rim cursor" and "Rim states and cost".
