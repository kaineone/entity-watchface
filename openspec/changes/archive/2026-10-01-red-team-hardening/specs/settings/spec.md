# Spec Delta

## ADDED Requirements

### Requirement: Strict message parsing
The watch SHALL only accept integer tuples or NUL-terminated string tuples for settings, and
string numbers SHALL be rejected when they do not parse completely or fall outside the int range.

#### Scenario: Wrapped number
- **WHEN** ClockFormat arrives as the string "4294967297"
- **THEN** the clock setting is unchanged
