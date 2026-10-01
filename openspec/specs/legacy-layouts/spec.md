# legacy-layouts Specification

## Purpose
Defines how Entity lays out on the colour Pebbles that predate the 2026 models: the Time and Time
Steel (basalt, 144×168) and the Time Round (chalk, 180×180).

## Requirements

### Requirement: Basalt layout
On basalt the face SHALL use JetBrains Mono 12 for labels and Zen Dots 48 for digits, with frames
(x,y,w,h): date 6,4,80,14 left; weather 58,4,80,14 right; hour 6,18,132,48 left; am/pm 100,22,38,14
right; minute 6,64,132,48 right; quiet mark 6,72,8,8; meter 9,126,126,20; link glyph 6,150,22,14;
battery 84,148,54,14 right. The meter SHALL draw 21 bars 5 px wide at a 6 px pitch, heights
scaled from the 22 px design scale to 16 px (never below 2 px), baseline 2 px under a 2 px gap.

#### Scenario: Time Steel at 21:04
- **WHEN** the face runs on basalt
- **THEN** all elements are inside the 144×168 screen and no text is ellipsized

### Requirement: Basalt quick view
In quick view on basalt the digits SHALL switch to Zen Dots 36 with hour 6,18,132,36 and minute
6,50,132,36, am/pm at 100,20, the status row at y 96 (battery) and 98 (link), and the meter hidden.

#### Scenario: Peek on basalt
- **WHEN** a timeline peek appears on basalt
- **THEN** all visible elements sit above the peek

### Requirement: Chalk layout
On chalk the face SHALL use JetBrains Mono 12 and Zen Dots 42, with frames: date 0,28,180,14
centred; hour 0,42,180,44 centred; am/pm 128,46,30,14 left; minute 0,86,180,44 centred; link glyph
24,82,22,14; quiet mark 86,84,8,8; battery 124,82,32,14 right; weather 0,136,180,14 centred.
The rim SHALL have outer radius 87 around the screen centre with tick lengths scaled from the
18 px design scale to 12 px (never below 2 px).

#### Scenario: Time Round
- **WHEN** the face runs on chalk
- **THEN** the rim ticks sit inside the round display edge and the text is centred

### Requirement: No change on the 2026 models
Emery and gabbro output SHALL stay pixel-identical to before this change, and their bundles SHALL
not include the 12, 36 or 42 px fonts.

#### Scenario: Regression
- **WHEN** emery and gabbro screenshots are compared before and after this change at the same time
- **THEN** they match
