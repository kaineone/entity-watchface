# Spec Delta

## MODIFIED Requirements

### Requirement: Rim trail
The cursor tick SHALL be 18 px in the scanner colour. The trail covers the min(5, s) ticks the
cursor has passed since the turn at 12, on the side it came from; a trail tick at distance d SHALL
be min(18, rest + boost(d)) long with boost 8, 4, 2, 0 for d = 1..4, coloured with the scanner's
trail shade for d. All other ticks SHALL sit at their resting length in AAAA55. No dither. The
scanner colours follow the hour colour exactly as on the rectangular meter.

#### Scenario: Just after the turn
- **WHEN** the time is 10:02:02
- **THEN** ticks 1 and 0 carry the ramp (d 1 and 2) and tick 59 is gold at rest
