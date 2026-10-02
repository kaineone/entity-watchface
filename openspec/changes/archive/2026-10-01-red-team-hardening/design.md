# Design

## Decisions
- Fixes follow the verified red-team list; items needing impossible OS inputs (tm_hour 24) or only internal callers (meter i out of range) are not changed.
- The weather fit loop is left as is here because the redesign removes it; the clamp alone removes the overflow input.
- Location rounding 0.1° (about 10 km) is plenty for current conditions.
