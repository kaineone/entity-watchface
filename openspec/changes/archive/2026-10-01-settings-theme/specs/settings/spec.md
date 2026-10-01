# Spec Delta

## ADDED Requirements

### Requirement: Page follows the face palette
The settings page SHALL use only colours from the face palette: background 000000, labels and
headings AAAA55, current values and switched-on toggle markers FFAA55, rules and switched-off
toggles 555555, switched-on toggle tracks 550000, and a Save button in AAAA55 with 000000 text.
Headings SHALL be in sentence case, corners square, and sections drawn with 1 px rules instead of
filled panels. Bright red (FF0000) SHALL NOT appear on the page.

#### Scenario: Opening settings
- **WHEN** the user opens the settings page
- **THEN** it shows a black page with gold sentence-case headings and no orange or grey panels
