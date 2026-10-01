# Spec Delta

## ADDED Requirements

### Requirement: Last-resort fit
If a reading still does not fit beside the date after dropping the `~` and the space, the
degree sign SHALL be dropped as well (`clear-12`). On black-and-white platforms the `~` of a
stale reading is never dropped, so the steps there are: drop the space, then drop the degree sign.

#### Scenario: Stale long reading on flint
- **WHEN** a stale `-12°` clear reading must fit beside `thu 01.10` on flint
- **THEN** the readout shows `~clear-12`
