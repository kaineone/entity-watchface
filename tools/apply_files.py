#!/usr/bin/env python3
"""Splice worker output into the working tree.

The worker answers with complete file bodies in this envelope:

    === FILE: relative/path ===
    <full file contents>
    === END FILE ===

Usage: tools/apply_files.py <worker-output.md> [--root DIR] [--dry-run]
Paths must be relative and stay inside the root.
"""
import argparse
import pathlib
import re
import sys

OPEN = re.compile(r"^=== FILE: (?P<path>[^=]+?) ===\s*$")
CLOSE = re.compile(r"^=== END FILE ===\s*$")


def parse(text):
    files, path, buf = [], None, []
    for line in text.splitlines(keepends=True):
        if path is None:
            m = OPEN.match(line)
            if m:
                path, buf = m.group("path").strip(), []
        elif CLOSE.match(line):
            files.append((path, "".join(buf)))
            path = None
        else:
            buf.append(line)
    if path is not None:
        sys.exit(f"unterminated FILE block for {path}")
    return files


def strip_fence(body):
    # Workers sometimes wrap the body in a markdown fence; drop exactly one.
    lines = body.splitlines(keepends=True)
    if len(lines) >= 2 and lines[0].startswith("```") and lines[-1].strip() == "```":
        return "".join(lines[1:-1])
    return body


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("output")
    ap.add_argument("--root", default=".")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    root = pathlib.Path(args.root).resolve()
    files = parse(pathlib.Path(args.output).read_text())
    if not files:
        sys.exit("no FILE blocks found")
    for rel, body in files:
        dest = (root / rel).resolve()
        if root not in dest.parents:
            sys.exit(f"refusing path outside root: {rel}")
        body = strip_fence(body)
        if not body.endswith("\n"):
            body += "\n"
        print(f"{'would write' if args.dry_run else 'wrote'} {rel} ({len(body)} bytes)")
        if not args.dry_run:
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_text(body)


if __name__ == "__main__":
    main()
