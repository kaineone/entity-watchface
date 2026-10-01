# Spec Delta

## MODIFIED Requirements

### Requirement: States
The meter SHALL be in exactly one of three modes, checked in this order:
unlinked when the phone app is not connected (all bars 2 px, bars and baseline 555555, no cursor);
frozen when the battery is at or below the threshold (default 20 %) and the watch is not charging
or plugged in, quiet time is active, the animate setting is off, or quick view is showing;
otherwise animating.

#### Scenario: Unlinked wins
- **WHEN** the phone disconnects while the battery is at 10 %
- **THEN** the meter shows the unlinked style

#### Scenario: Charging on low battery
- **WHEN** the battery is at 15 % and the watch is charging
- **THEN** the meter animates
