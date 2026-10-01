# Design

## Decisions
- Keyed on `PBL_ROUND`: the SDK has no heart-rate feature macro, and both round Pebbles (Time Round
  and Round 2) lack the sensor. Only `swap_in` changes; `swap_out` already rebuilds both readouts.
