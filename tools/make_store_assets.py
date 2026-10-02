#!/usr/bin/env python3
"""Generate store assets and README preview images for the Entity watchface.

Creates:
- store/banner-720x320.png
- store/icon-144.png
- store/icon-48.png
- docs/images/hero.png
- docs/images/lineup.png

Run from the repository root:
    python3 tools/make_store_assets.py
"""

from pathlib import Path
from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parent.parent
SCREEN_DIR = ROOT / "store" / "screenshots"
FONT_PATH = ROOT / "tools" / "fonts" / "Silkscreen-Regular.ttf"

STORE_DIR = ROOT / "store"
DOCS_IMG_DIR = ROOT / "docs" / "images"

# Watchface display palette.
CREAM = (255, 238, 171)
ROSE = (227, 84, 98)
AMBER = (241, 170, 134)
BG = (0, 0, 0)

# Icon / true palette.
RED = (255, 0, 0)
ORANGE = (255, 85, 0)
CHROME = (255, 170, 0)
REST = (170, 170, 0)
BASE = (85, 0, 0)

README_BG = (18, 18, 18)

BLOCK_PATTERNS = {
    'E': ['111', '100', '111', '100', '111'],
    'N': ['111', '101', '101', '101', '101'],
    'T': ['111', '010', '010', '010', '010'],
    'I': ['111', '010', '010', '010', '111'],
    'Y': ['101', '101', '111', '010', '010'],
}


def load_image(path):
    """Load an input image without resizing or anti-aliasing."""
    return Image.open(path).convert('RGBA')


def scale2(img):
    """Exact 2x integer nearest-neighbour scale."""
    return img.resize((img.width * 2, img.height * 2), Image.NEAREST)


def write_image(img, path):
    """Save an image, creating parent directories if needed, and print the path."""
    path.parent.mkdir(parents=True, exist_ok=True)
    img.save(path)
    print(path)


def draw_block_word(img, word, x, y, t, h, color, sp):
    """Draw a word using the 3x5 block-letter grid with top-left corner carving."""
    draw = ImageDraw.Draw(img)

    gap = h - 3 * t
    a = gap // 2
    b = gap - a
    row_heights = [t, a, t, b, t]

    row_tops = [0]
    for rh in row_heights[:-1]:
        row_tops.append(row_tops[-1] + rh)

    letter_width = 3 * t
    k = max(2, t * 2 // 3)

    for li, ch in enumerate(word.upper()):
        pattern = BLOCK_PATTERNS[ch]
        base_x = x + li * (letter_width + sp)
        cells = []

        for r in range(5):
            for c in range(3):
                if pattern[r][c] == '1':
                    cx = base_x + c * t
                    cy = y + row_tops[r]
                    cw = t
                    chh = row_heights[r]
                    draw.rectangle(
                        [cx, cy, cx + cw - 1, cy + chh - 1],
                        fill=color,
                    )
                    cells.append((r, c, cx, cy, cw, chh))

        # Carve the top-left corner of qualifying cells.
        for r, c, cx, cy, cw, chh in cells:
            up_empty = (r == 0) or (pattern[r - 1][c] == '0')
            left_empty = (c == 0) or (pattern[r][c - 1] == '0')
            if up_empty and left_empty:
                for j in range(k):
                    if j >= chh:
                        break
                    length = k - j
                    if length > cw:
                        length = cw
                    draw.rectangle(
                        [cx, cy + j, cx + length - 1, cy + j],
                        fill=BG,
                    )


def make_banner():
    """Generate store/banner-720x320.png."""
    img = Image.new('RGB', (720, 320), BG)

    draw_block_word(img, "ENTITY", 26, 120, t=7, h=40, color=CREAM, sp=6)

    draw = ImageDraw.Draw(img)
    draw.fontmode = "1"
    font = ImageFont.truetype(str(FONT_PATH), 8)
    draw.text((26, 176), "ENTITY IS ONLINE.", font=font, fill=AMBER)

    time_img = load_image(SCREEN_DIR / "time2-1.png")
    img.paste(time_img, (226, 46))

    round_img = load_image(SCREEN_DIR / "round2-1.png")
    mask = Image.new('L', round_img.size, 0)
    mask_draw = ImageDraw.Draw(mask)
    mask_draw.ellipse(
        (0, 0, round_img.width - 1, round_img.height - 1),
        fill=255,
    )
    img.paste(round_img, (444, 30), mask)

    write_image(img, STORE_DIR / "banner-720x320.png")


def make_icon(size, bar_w, pitch, start_x, bottom_y, max_h, base_y, base_h):
    """Generate one meter-bar icon image."""
    img = Image.new('RGB', (size, size), BG)
    draw = ImageDraw.Draw(img)

    fractions = [0.30, 0.55, 0.80, 1.00, 0.30]
    colors = [REST, CHROME, ORANGE, RED, REST]

    for i, (frac, color) in enumerate(zip(fractions, colors)):
        h = max(1, round(frac * max_h))
        x = start_x + i * pitch
        y = bottom_y - h
        draw.rectangle([x, y, x + bar_w - 1, bottom_y - 1], fill=color)
        draw.rectangle(
            [x, base_y, x + bar_w - 1, base_y + base_h - 1],
            fill=BASE,
        )

    return img


def make_icons():
    """Generate store/icon-144.png and store/icon-48.png."""
    icon_144 = make_icon(
        144, bar_w=16, pitch=24, start_x=16,
        bottom_y=112, max_h=80, base_y=116, base_h=4,
    )
    write_image(icon_144, STORE_DIR / "icon-144.png")

    icon_48 = make_icon(
        48, bar_w=6, pitch=8, start_x=5,
        bottom_y=38, max_h=28, base_y=40, base_h=2,
    )
    write_image(icon_48, STORE_DIR / "icon-48.png")


def make_hero():
    """Generate docs/images/hero.png."""
    time_img = scale2(load_image(SCREEN_DIR / "time2-1.png"))
    round_img = scale2(load_image(SCREEN_DIR / "round2-1.png"))

    margin = 48
    gap = 48
    width = margin + time_img.width + gap + round_img.width + margin
    height = max(time_img.height, round_img.height) + 2 * margin

    img = Image.new('RGB', (width, height), README_BG)
    img.paste(time_img, (margin, (height - time_img.height) // 2))
    img.paste(
        round_img,
        (margin + time_img.width + gap, (height - round_img.height) // 2),
    )

    write_image(img, DOCS_IMG_DIR / "hero.png")


def make_lineup():
    """Generate docs/images/lineup.png."""
    names = ["time2-1", "round2-1", "time-1", "timeround-1", "pebble2duo-1"]
    images = [scale2(load_image(SCREEN_DIR / f"{name}.png")) for name in names]

    margin = 40
    gap = 32
    width = margin + sum(i.width for i in images) + gap * (len(images) - 1) + margin
    height = max(i.height for i in images) + 2 * margin

    img = Image.new('RGB', (width, height), README_BG)
    baseline_y = height - margin
    x = margin
    for shot in images:
        y = baseline_y - shot.height
        img.paste(shot, (x, y))
        x += shot.width + gap

    write_image(img, DOCS_IMG_DIR / "lineup.png")


def main():
    make_banner()
    make_icons()
    make_hero()
    make_lineup()


if __name__ == "__main__":
    main()
