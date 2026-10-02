# Proposal

## Why

The face needs appstore material and the README needs to show what the face looks like. The
earlier draft (PR #13) predates the block numerals, palette C, the temperature-only readout, the
double tap and the once-a-second round rim, so its screenshots and copy are out of date.

## What Changes

- Native-resolution emulator screenshots for all five platforms at one staged moment (Thu 1 Oct,
  21:47), plus a double-tap shot for the Time 2.
- `tools/make_store_assets.py` renders the banner (720×320, block-letter wordmark), icons (144, 48)
  and the README's hero and lineup images, with no anti-aliasing. The Silkscreen font it uses for
  the banner's small line ships with its OFL licence.
- Two animated GIFs recorded from the emulator, one per 2026 watch, with a slide between
  demonstrations, for the README.
- README: hero image, a "See it move" section with the GIFs, and the lineup of every watch.
- `store/listing.md`: a new draft listing that still needs the owner's word-by-word approval.

## Capabilities

### New Capabilities
- `store-listing`: what the store and README material contains and the rule for publishing it.

## Impact

New `store/` and `docs/images/` files, one tool script and a font. No watch code changes.
