# settings Specification

## Purpose
Defines the user settings for Entity, their defaults and allowed values, and how they travel from
the phone to the watch and persist there.

## Requirements

### Requirement: Settings and defaults
The settings page SHALL offer, with these defaults:
clock format (follow watch, 24 hour, 12 hour; default follow watch); hour colour (red FF5555,
cream FFFFAA, gold FFAA55; default red); show weather (on); temperature in Fahrenheit (off);
animate the meter (on); buzz when the phone disconnects (on); tap to show steps and heart rate (on);
low-battery threshold (10, 20, 30 or 50 %; default 20).

#### Scenario: Fresh install
- **WHEN** the face runs for the first time with nothing saved
- **THEN** it follows the watch's 12/24-hour setting, the hour is red, the meter animates, and the threshold is 20 %

### Requirement: Delivery and persistence
Saving the page SHALL send the values to the watch, which SHALL apply them without a restart and
persist them so they survive the face being closed or the watch rebooting. Values outside the
allowed set SHALL be ignored, keeping the previous value.

#### Scenario: Change hour colour
- **WHEN** the user picks gold and saves
- **THEN** the hour turns FFAA55 at once and is still gold after switching faces and back

#### Scenario: Bad threshold
- **WHEN** a message carries a threshold of 37
- **THEN** the threshold stays at its previous value

### Requirement: Page follows the face palette
The settings page SHALL use only colours from the face palette: background 000000, labels and
headings AAAA55, current values and switched-on toggle markers FFAA55, rules and switched-off
toggles 555555, switched-on toggle tracks 550000, and a Save button in AAAA55 with 000000 text.
Headings SHALL be in sentence case, corners square, and sections drawn with 1 px rules instead of
filled panels. Bright red (FF0000) SHALL NOT appear on the page.

#### Scenario: Opening settings
- **WHEN** the user opens the settings page
- **THEN** it shows a black page with gold sentence-case headings and no orange or grey panels
