# Spec Delta

## ADDED Requirements

### Requirement: Quiet time takes effect promptly
Whenever the meter mode is recomputed (minute tick, connection, battery, focus, quick view or a
settings change), quiet time SHALL be read afresh.

#### Scenario: Quiet time starts mid-minute and focus changes
- **WHEN** quiet time starts at 22:00:10 and a notification is dismissed at 22:00:20
- **THEN** the meter is frozen at 22:00:20, not at 22:01:00
