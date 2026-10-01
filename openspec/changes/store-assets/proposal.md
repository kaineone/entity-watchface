# Proposal

## Why

The face needs appstore material: screenshots for every platform, a banner, icons and listing text.

## What Changes

- Native-resolution emulator screenshots for all five platforms, plus a tap screenshot for the Time 2.
- `tools/make_store_assets.py` renders the banner (720×320) and icons (144, 48) without anti-aliasing.
- `store/listing.md`: a draft listing awaiting the owner's word-by-word approval.

## Capabilities

### New Capabilities
- `store-listing`: what the store material contains and the rule for publishing it.

### Modified Capabilities

## Impact

New `store/` files and one tool script. No watch code changes.
