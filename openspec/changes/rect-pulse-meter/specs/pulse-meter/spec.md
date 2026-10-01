# Spec Delta

## Purpose
Defines the 21-bar pulse meter on the Pebble Time 2: its geometry, the moving cursor with a
trailing heat ramp, the still states, and the conditions that start and stop its animation.

## ADDED Requirements

### Requirement: Geometry
The meter SHALL draw 21 bars, 7 px wide at x = 6 + 9·i, growing upward from y 198 with a maximum
height of 22 px, and a 2 px baseline strip under each bar at y 200 (7 px wide, same x).

#### Scenario: Frozen geometry
- **WHEN** the meter is frozen
- **THEN** every bar is 2 px tall (y 196 to 197), drawn in AAAA55, and every baseline strip is 550000

### Requirement: Cursor motion
While animating, the cursor SHALL move one bar per frame, bouncing between bar 0 and bar 20
without pausing or skipping an end bar. Each time it lands on a bar, that bar's resting height
SHALL be re-randomised within 2 to 13 px. Resting heights SHALL be randomised at start.

#### Scenario: Bounce
- **WHEN** the cursor is on bar 20 having moved right
- **THEN** the next frame puts it on bar 19 and its direction of travel is left

### Requirement: Trailing heat ramp
The cursor bar SHALL be 22 px tall and FF0000. Bars behind the cursor (on the side opposite its
direction of travel) at distance d SHALL be min(22, rest + boost(d)) tall, where boost is
16, 13, 7, 3, 1 for d = 0..4 and 0 beyond, and coloured heat(d): FF5500, FF5555, FFAA00, FFAA55
for d = 1..4 and AAAA55 from 5. Bars ahead of the cursor SHALL sit at their resting height in AAAA55.
Behind-bars at d 1 to 4 SHALL be dithered 50/50 between heat(d) and heat(d+1) using the 4×4 Bayer
matrix [0,8,2,10, 12,4,14,6, 3,11,1,9, 15,7,13,5] indexed by absolute screen (x mod 4, y mod 4):
cells below 8 take heat(d+1). The baseline stays 550000.

#### Scenario: Trail side
- **WHEN** the cursor is on bar 10 moving right
- **THEN** bars 6 to 9 carry the ramp and swell, and bars 11 to 20 are gold at resting height

### Requirement: States
The meter SHALL be in exactly one of three modes, checked in this order:
unlinked when the phone app is not connected (all bars 2 px, bars and baseline 555555, no cursor);
frozen when the battery is at or below the threshold (default 20 %), quiet time is active, the
animate setting is off, or quick view is showing; otherwise animating.

#### Scenario: Unlinked wins
- **WHEN** the phone disconnects while the battery is at 10 %
- **THEN** the meter shows the unlinked style

### Requirement: Animation cost
The frame timer (250 ms) SHALL run only in the animating mode while the face has focus, and
SHALL be cancelled in every other case. Each frame SHALL mark only the meter layer dirty. A
change into a still mode SHALL repaint the meter once. In quick view the meter layer SHALL be hidden.

#### Scenario: Notification over the face
- **WHEN** a notification takes focus while the meter is animating
- **THEN** the timer is cancelled and restarts when focus returns

#### Scenario: Quiet time starts
- **WHEN** quiet time becomes active
- **THEN** within one minute the timer is cancelled and the meter is painted frozen
