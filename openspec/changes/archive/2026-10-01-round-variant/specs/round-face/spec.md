# Spec Delta

## Purpose
Defines the Entity layout on the Pebble Round 2 (gabbro, 260×260) and its rim meter.

## ADDED Requirements

### Requirement: Round layout
On gabbro the face SHALL use these frames (x,y,w,h): date 0,42,260,16 centred AAAA55; hour
0,70,260,60 ZEN_60 centred in the hour colour; am/pm 184,74,40,16 left-aligned AAAA55; minute
0,128,260,60 ZEN_60 centred AAAA55; link glyph 40,122,22,14; quiet mark 126,122,8,8; battery
180,122,40,16 right-aligned; weather 0,200,260,16 centred FFAA55. Colour rules match the
rectangular face.

#### Scenario: Default round face
- **WHEN** the face runs on gabbro at 21:04
- **THEN** `21` and `04` are centred one above the other with the date above and the weather below

### Requirement: Rim geometry
The rim SHALL have 60 ticks centred on 130,130. Tick i points at −90° + 6°·i (tick 0 at 12
o'clock, increasing clockwise) and is a 1 px line from radius 127 inward by its length.

#### Scenario: Frozen rim
- **WHEN** the meter is frozen
- **THEN** all 60 ticks are 2 px long and AAAA55

### Requirement: Rim cursor
While animating, the cursor tick SHALL be the current second s on even minutes and (60 − s) mod 60
on odd minutes, so it runs clockwise and then counter-clockwise, turning at 12 each minute. When
the cursor lands on a tick, that tick's resting length SHALL be re-randomised within 2 to 7 px.

#### Scenario: Odd minute
- **WHEN** the time is 10:01:15
- **THEN** the cursor is tick 45

### Requirement: Rim trail
The cursor tick SHALL be 18 px and FF0000. The trail covers the min(5, s) ticks the cursor has
passed since the turn at 12, on the side it came from; a trail tick at distance d SHALL be
min(18, rest + boost(d)) long with boost 8, 4, 2, 0 for d = 1..4, coloured FF5500, FF5555,
FFAA00, FFAA55 for d = 1..4. All other ticks SHALL sit at their resting length in AAAA55. No dither.

#### Scenario: Just after the turn
- **WHEN** the time is 10:02:02
- **THEN** ticks 1 and 0 carry the ramp (d 1 and 2) and tick 59 is gold at rest

### Requirement: Rim states and cost
Unlinked SHALL draw all ticks 2 px in 555555; frozen all ticks 2 px in AAAA55 (same freeze rules
as the rectangular meter). The face SHALL subscribe to SECOND_UNIT ticks only on gabbro and only
while the rim animates and the face has focus; otherwise MINUTE_UNIT. Each second SHALL mark only
the rim layer dirty.

#### Scenario: Low battery on round
- **WHEN** the battery drops to the threshold while not charging
- **THEN** the face switches to minute ticks and draws the frozen rim once
