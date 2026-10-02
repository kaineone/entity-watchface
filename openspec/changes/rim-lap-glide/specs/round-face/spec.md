# Spec Delta

## ADDED Requirements

### Requirement: Rim lap cursor
While a burst runs, the cursor SHALL glide a full lap clockwise from tick 0 at 12 o'clock back to
12, slow to a stop there, then glide a full lap counter-clockwise back to 12, repeating. Each lap
SHALL take about 5 s with smoothstep easing so the cursor slows into and out of each turn, and its
position SHALL never step backwards within a lap. The cursor SHALL NOT depend on the time of day.
When the nearest tick changes, that tick's resting length SHALL be re-randomised within 2 to 7 px.

#### Scenario: Halfway round
- **WHEN** a burst from idle has run for half of its first lap
- **THEN** the cursor is at tick 30 moving clockwise

#### Scenario: Turning at 12
- **WHEN** the cursor finishes a clockwise lap
- **THEN** it slows to a stop at tick 0 and the next lap runs counter-clockwise

### Requirement: Rim lap trail
The tick nearest the cursor SHALL be FF0000 at full burst strength. Ticks behind it, up to 5 ticks
back and never past the turn at 12, SHALL swell by a boost interpolated at their fractional
distance from table {16, 10, 6, 3, 1, 0} and take the heat ramp FF5500, FF5555, FFAA00, FFAA55 by
rounded distance 1..4. The tick just ahead SHALL grow as the cursor approaches it. Lengths clamp
at 18 px. All other ticks SHALL sit at their resting length in AAAA55. No dither.

#### Scenario: Just after the turn
- **WHEN** the cursor has travelled two ticks clockwise from 12
- **THEN** ticks 1 and 0 carry the ramp and tick 59 is gold at rest

### Requirement: Rim lap bursts
Round faces SHALL use bursts at 10 frames per second and SHALL NOT subscribe to second ticks. A
burst from idle SHALL start at 12 going clockwise. The burst at each minute change and when the
face opens SHALL last exactly one lap so it ends at 12; a wrist flick SHALL run 25 s. Each burst
fades in over about 0.5 s and settles over about 1 s. Outside bursts the ticks rest in gold with no
timer running.

#### Scenario: Minute change
- **WHEN** the minute changes with the rim allowed to animate
- **THEN** the cursor makes one clockwise lap from 12 to 12 and the rim settles to gold

## MODIFIED Requirements

### Requirement: Rim states and cost
Unlinked SHALL draw all ticks 2 px in 555555; frozen all ticks 2 px in AAAA55 (same freeze rules
as the rectangular meter). The face SHALL use MINUTE_UNIT ticks on every platform and SHALL run a
frame timer only while a burst animates and the face has focus. Each frame SHALL mark only the rim
layer dirty.

#### Scenario: Low battery on round
- **WHEN** the battery drops to the threshold while not charging
- **THEN** the face switches to minute ticks and draws the frozen rim once

## REMOVED Requirements

### Requirement: Rim cursor
**Reason**: The cursor no longer follows the second hand; it glides full laps during bursts.
**Migration**: Replaced by "Rim lap cursor".

### Requirement: Rim trail
**Reason**: Trail extent is now measured from the turn at 12 in lap position, not in seconds.
**Migration**: Replaced by "Rim lap trail".

### Requirement: Rim bursts with a sliding cursor
**Reason**: Bursts no longer read the clock and now run at 10 frames per second.
**Migration**: Replaced by "Rim lap bursts".
