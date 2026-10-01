# Design

## Context
Clay 1.1.0 has no theme API. Its `text` component renders `defaultValue` as raw HTML, so a
`<style>` element placed first applies to the whole page. Selectors come from Clay's sources:
`body`, `.section`, `.section > .component:after` (dividers), `.component-heading:first-child`,
`h1..h6` (uppercase), `.label`, `.component-select .value` and `.value:after` (triangle drawn
with `border-top-color`), `.component-toggle .slide` / `.marker` / `input:checked + .graphic`,
`button[type=submit]`.

## Goals / Non-Goals
**Goals:** the page reads as part of the same object as the face. **Non-Goals:** layout changes.

## Decisions
- Palette tokens only from the face; bright red reserved for the watch cursor.
- Square corners and 1 px rules echo the face's no-anti-aliasing rule.
- JetBrains Mono first in the font stack (the face's label font), system monospace fallback;
  no web font download, so the page still works offline.
- `!important` is used only where Clay's own rules are more specific.

## Risks / Trade-offs
- A Clay upgrade could rename classes; the page would fall back to Clay's stock look, not break.
