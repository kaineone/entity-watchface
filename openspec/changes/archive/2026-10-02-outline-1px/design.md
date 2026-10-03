# Design

`NUMERAL_OUTLINE` goes from 2 to 1, and the outline pass grows each shape rect by
`NUMERAL_OUTLINE` on every side: `GRect(r.x - NUMERAL_OUTLINE, r.y - NUMERAL_OUTLINE,
r.w + 2 * NUMERAL_OUTLINE, r.h + 2 * NUMERAL_OUTLINE)`. The inset already uses the constant.
