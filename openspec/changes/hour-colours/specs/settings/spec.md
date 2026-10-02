# Spec Delta

## MODIFIED Requirements

### Requirement: Settings and defaults
The settings page SHALL offer, with these defaults:
clock format (follow watch, 24 hour, 12 hour; default follow watch); hour colour (red, pink,
purple, blue, teal, green, cream, gold, white; default red); show weather (on); temperature in
Fahrenheit (off); animate the meter (on); buzz when the phone disconnects (on); tap to show steps
and heart rate (on); low-battery threshold (10, 20, 30 or 50 %; default 20).

#### Scenario: Fresh install
- **WHEN** the face runs for the first time with nothing saved
- **THEN** it follows the watch's 12/24-hour setting, the hour is red, the meter animates, and the threshold is 20 %
