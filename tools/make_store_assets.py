#!/usr/bin/env python3
"""Generate store assets for the Entity Pebble watchface.

Run from the repo root:
    python3 tools/make_store_assets.py
"""

from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).parent.parent
FONT_DIR = ROOT / "resources" / "fonts"
STORE_DIR = ROOT / "store"

BG = (0, 0, 0)
GOLD = (170, 170, 85)
ACCENT = (255, 170, 85)
# The banner sits next to an emulator screenshot, which shows the Time 2's display profile;
# match its rendered gold and accent so the two read as one palette.
BANNER_GOLD = (174, 163, 130)
BANNER_ACCENT = (241, 173, 147)
RED = (255, 0, 0)
HEAT1 = (255, 85, 0)
HEAT3 = (255, 170, 0)
BASELINE = (85, 0, 0)

SCREENSHOT = ROOT / "store" / "screenshots" / "time2-1.png"

BANNER_SIZE = (720, 320)
WORDMARK_POS = (40, 104)
TAGLINE_X = 42


def save_image(img: Image.Image, path: Path) -> None:
    STORE_DIR.mkdir(parents=True, exist_ok=True)
    img.save(path)
    print(path.relative_to(ROOT))


def make_banner() -> Image.Image:
    img = Image.new("RGB", BANNER_SIZE, BG)
    draw = ImageDraw.Draw(img)
    draw.fontmode = "1"

    zen = ImageFont.truetype(str(FONT_DIR / "ZenDots-Regular.ttf"), 64)
    mono = ImageFont.truetype(str(FONT_DIR / "JetBrainsMono-Medium.ttf"), 20)

    # Wordmark: bounding box top-left at WORDMARK_POS.
    draw.text(WORDMARK_POS, "Entity", font=zen, fill=BANNER_GOLD, anchor="lt")

    # Tagline sits below the wordmark's ink.
    bbox = draw.textbbox(WORDMARK_POS, "Entity", font=zen, anchor="lt")
    tagline_y = int(bbox[3]) + 22
    draw.text((TAGLINE_X, tagline_y), "Entity is online.",
              font=mono, fill=BANNER_ACCENT, anchor="lt")

    screenshot = Image.open(SCREENSHOT).convert("RGB")
    img.paste(screenshot, (440, 46))

    return img


def make_icon(size: int) -> Image.Image:
    img = Image.new("RGB", (size, size), BG)
    draw = ImageDraw.Draw(img)

    if size == 144:
        start_x, pitch, bar_w = 16, 24, 16
        bottom_y, max_h = 112, 80
        base_y, base_h = 116, 4
    elif size == 48:
        start_x, pitch, bar_w = 5, 8, 6
        bottom_y, max_h = 38, 28
        base_y, base_h = 40, 2
    else:
        raise ValueError(f"Unsupported icon size: {size}")

    fractions = (0.30, 0.55, 0.80, 1.00, 0.30)
    colors = (GOLD, HEAT3, HEAT1, RED, GOLD)

    for i, (frac, color) in enumerate(zip(fractions, colors)):
        height = max(1, int(round(frac * max_h)))
        x = start_x + i * pitch
        y0 = bottom_y - height
        draw.rectangle((x, y0, x + bar_w - 1, bottom_y - 1), fill=color)
        draw.rectangle((x, base_y, x + bar_w - 1, base_y + base_h - 1),
                       fill=BASELINE)

    return img


def main() -> None:
    save_image(make_banner(), STORE_DIR / "banner-720x320.png")
    for size in (144, 48):
        save_image(make_icon(size), STORE_DIR / f"icon-{size}.png")


if __name__ == "__main__":
    main()
