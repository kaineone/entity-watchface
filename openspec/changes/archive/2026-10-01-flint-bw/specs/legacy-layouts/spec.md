# Spec Delta

## ADDED Requirements

### Requirement: Black-and-white palette
On black-and-white platforms all text and glyphs SHALL be white on black, and meter inks SHALL be
drawn as white pixels where the 4×4 Bayer value at the absolute screen position is below a
density out of 16: cursor 16, heat 1 14, heat 2 12, heat 3 10, heat 4 9, rest (gold) 8,
baseline 4, disconnected 2.

#### Scenario: Animating on flint
- **WHEN** the meter animates on flint
- **THEN** the cursor bar is solid white, its trail fades through denser-to-lighter dither, and resting bars are a 50 % checker

### Requirement: Black-and-white signals
On black-and-white platforms the battery text SHALL be drawn black on a white box at or below the
threshold (unless charging); the link glyph SHALL be drawn at density 4 when disconnected; stale
weather SHALL always keep its `~` prefix (only the space may be dropped to fit); and the settings
page SHALL not offer the hour colour.

#### Scenario: Low battery on flint
- **WHEN** the battery is at 15 % and not charging on flint
- **THEN** `15%` appears black on a white box

### Requirement: Flint layout
Flint SHALL use the basalt frames, fonts (Zen Dots 48 and 36, JetBrains Mono 12) and meter geometry.

#### Scenario: Pebble 2 Duo at 21:04
- **WHEN** the face runs on flint
- **THEN** it matches the basalt layout in white
