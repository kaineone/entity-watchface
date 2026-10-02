# Spec Delta

## MODIFIED Requirements

### Requirement: Fetch on the phone
When weather is on, the phone SHALL get the location (timeout 15 s, accepting a cached fix up to
30 minutes old), fetch current temperature and weather code from Open-Meteo, and send a message
to the watch when the watch asks for it and then every 30 minutes. When weather is off it SHALL
not request the location or the network.

#### Scenario: Weather off
- **WHEN** the user has switched weather off
- **THEN** no location request and no network request are made

### Requirement: Fetch discipline
The phone SHALL make at most one weather request at a time, give up on a request after 10 s, and
retry once 3 minutes after a failure. When the face starts, the watch SHALL ask for weather only if
weather is on and its own reading is missing or at least 15 minutes old. Coordinates SHALL be
rounded to 0.1°.

#### Scenario: Re-opening the face
- **WHEN** the face is reopened 5 minutes after the watch received a reading
- **THEN** the watch does not ask for weather and no network request is made

#### Scenario: Second watch on the same phone
- **WHEN** a watch with no saved reading starts the face minutes after another watch fetched
- **THEN** it asks for weather and shows the temperature
