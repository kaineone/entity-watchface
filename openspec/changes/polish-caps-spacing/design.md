# Design

## Decisions
- `trackingAdjust` per Orbitron resource, scaled with size (64/60: 6, 48: 5, 42/36: 4).
- Label glyph sets switch from a-z to A-Z; strings come from the logic tables, so only `fmt.c` changes.
- The design rule "nothing anti-aliased" is lifted for rim ticks at the owner's request; text stays 1-bit (the SDK font format is 1 bpp).
