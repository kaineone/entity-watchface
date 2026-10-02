# Spec Delta

## ADDED Requirements

### Requirement: Hour outline
On colour watches, when the hour colour is red, pink, purple, blue, teal or green, the hour digits
SHALL carry a 2 px outline in the minute colour, following every edge including the chamfers. The
outlined hour SHALL occupy the same frame as an un-outlined one: the digits are inset by 2 px on
each side and drawn with a stroke 1 px thinner. Cream, gold and white hours, and black-and-white
watches, SHALL have no outline.

#### Scenario: Backlight off
- **WHEN** the hour is red and the backlight is off
- **THEN** the hour digits are ringed in cream and line up with the minute digits' edges

#### Scenario: Cream hour
- **WHEN** the hour colour is cream
- **THEN** the hour is drawn without an outline, exactly as before
