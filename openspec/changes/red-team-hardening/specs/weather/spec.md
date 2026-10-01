# Spec Delta

## ADDED Requirements

### Requirement: Weather input limits
The phone SHALL not send a reading with a temperature outside ±150 °C or from non-finite or
out-of-range coordinates. The watch SHALL ignore a reading whose condition is outside 0..4 or
whose temperature is outside ±99.9 °C, and SHALL ignore a persisted reading that fails the same
checks or is dated more than 5 minutes in the future.

#### Scenario: Absurd temperature
- **WHEN** a message carries a temperature of 2,000,000 tenths of a degree
- **THEN** the watch keeps its previous reading

### Requirement: Clock behind the reading
A reading dated more than 5 minutes after the watch's current time SHALL be treated as stale.

#### Scenario: Clock set back
- **WHEN** the watch clock moves back two hours after a reading
- **THEN** the readout shows the stale style

### Requirement: Fetch discipline
The phone SHALL make at most one weather request at a time, give up on a request after 10 s,
retry once 3 minutes after a failure, and skip a fetch on launch if the last successful fetch was
less than 15 minutes ago. Coordinates SHALL be rounded to 0.1°.

#### Scenario: Re-opening the face
- **WHEN** the face is reopened 5 minutes after a successful fetch
- **THEN** no network request is made
