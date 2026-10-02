# Spec Delta

## MODIFIED Requirements

### Requirement: Trailing heat ramp
The cursor bar SHALL be 22 px tall in the scanner colour. Bars behind the cursor (on the side
opposite its direction of travel) at distance d SHALL be min(22, rest + boost(d)) tall, where boost
is 16, 13, 7, 3, 1 for d = 0..4 and 0 beyond, and coloured heat(d), the scanner's trail shade for
d = 1..4, and AAAA55 from 5. Bars ahead of the cursor SHALL sit at their resting height in AAAA55.
Behind-bars at d 1 to 4 SHALL be dithered 50/50 between heat(d) and heat(d+1) using the 4×4 Bayer
matrix [0,8,2,10, 12,4,14,6, 3,11,1,9, 15,7,13,5] indexed by absolute screen (x mod 4, y mod 4):
cells below 8 take heat(d+1). The baseline is the scanner's baseline shade. The scanner colour,
trail shades and baseline follow the hour colour as listed in the design; cream, gold and white
hours use the red scanner. Black-and-white watches draw it all in white.

#### Scenario: Trail side
- **WHEN** the cursor is on bar 10 moving right
- **THEN** bars 6 to 9 carry the ramp and swell, and bars 11 to 20 are gold at resting height

#### Scenario: Blue hour
- **WHEN** the hour colour is blue
- **THEN** the cursor bar is 0055FF, the trail uses 0055AA then 000055, and the baseline is 000055
