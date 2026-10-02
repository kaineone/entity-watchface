# Spec Delta

## ADDED Requirements

### Requirement: Eased continuous cursor
While a burst runs, the cursor position SHALL move continuously between bar 0 and bar 20 with
smoothstep easing, one leg (0 to 20 or back) taking about 3 s at 10 frames per second. The bar
nearest the cursor is red; bars behind it swell by the boost table interpolated at their
fractional distance and take the heat ramp by rounded distance; bars ahead rest in gold.
A bar's resting height is re-randomised when the cursor passes its centre.

#### Scenario: Mid-sweep
- **WHEN** the cursor sits halfway between bars 9 and 10 moving right
- **THEN** one of them is red and bars 5 to 8 carry the ramp

### Requirement: Bursts and idle
With the meter allowed to animate, a burst SHALL start for 25 s on a wrist flick, for about 4 s at
each minute change and when the face opens. Each burst fades in over about 0.5 s and settles over
about 1 s, the trail cooling to gold. Outside bursts the bars SHALL rest at their resting heights
in gold with no cursor and no timer running. Frozen and unlinked looks are unchanged.

#### Scenario: Idle face
- **WHEN** no burst is running and the meter may animate
- **THEN** the bars show their resting heights in AAAA55 and no frame timer exists

#### Scenario: Low battery during a burst
- **WHEN** the battery reaches the threshold during a burst
- **THEN** the burst stops and the frozen look is drawn
