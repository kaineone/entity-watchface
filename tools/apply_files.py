#!/usr/bin/env python3
"""Splice worker output into the working tree.

The worker answers with complete file bodies in this envelope:

    === FILE: relative/path ===
    <full file contents>
    === END FILE ===

Usage: tools/apply_files.py <worker-output.md> [--root DIR] [--dry-run] [--allow PREFIX ...]
Paths must be relative, stay inside the root, avoid .git and symlinks, and fall under the
allowlist (src/, tests/, openspec/, resources/, store/, docs/, package.json, README.md);
.github/ and tools/ need an explicit --allow.
"""
import argparse
import pathlib
import re
import sys

OPEN = re.compile(r"^=== FILE: (?P<path>[^=]+?) ===\s*$")
CLOSE = re.compile(r"^=== END FILE ===\s*$")

DEFAULT_ALLOW = [
    "src/",
    "tests/",
    "openspec/",
    "resources/",
    "store/",
    "docs/",
    "package.json",
    "README.md",
]


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


def allowed_path(rel, allowed):
    # Entries ending in "/" are directory prefixes; anything else must match exactly.
    return any(rel.startswith(a) if a.endswith("/") else rel == a for a in allowed)


def validate(rel, root, allowed):
    if pathlib.Path(rel).is_absolute():
        return f"absolute path: {rel}"
    parts = pathlib.Path(rel).parts
    if ".." in parts:
        return f"contains ..: {rel}"
    if ".git" in parts:
        return f"contains .git: {rel}"
    if not allowed_path(rel, allowed):
        return f"not in allowlist: {rel}"
    dest = root / rel
    try:
        if dest.is_symlink():
            return f"destination is a symlink: {rel}"
    except OSError as exc:
        return f"cannot stat destination: {rel} ({exc})"
    resolved = dest.resolve()
    if root not in resolved.parents:
        return f"path outside root: {rel}"
    # A symlinked directory inside the root can redirect a harmless-looking path; re-check
    # the path the write will actually land on.
    real_rel = resolved.relative_to(root).as_posix()
    if ".git" in pathlib.PurePosixPath(real_rel).parts or not allowed_path(real_rel, allowed):
        return f"resolves to a disallowed location: {rel} -> {real_rel}"
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("output")
    ap.add_argument("--root", default=".")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument(
        "--allow",
        action="append",
        default=None,
        help="extra allowed path prefix (repeatable)",
    )
    args = ap.parse_args()

    root = pathlib.Path(args.root).resolve()
    allowed = DEFAULT_ALLOW + (args.allow or [])

    files = parse(pathlib.Path(args.output).read_text())
    if not files:
        sys.exit("no FILE blocks found")

    validated = []
    for rel, body in files:
        reason = validate(rel, root, allowed)
        if reason:
            print(f"refusing {reason}", file=sys.stderr)
            sys.exit(1)
        validated.append((rel, body))

    for rel, body in validated:
        dest = (root / rel).resolve()
        body = strip_fence(body)
        if not body.endswith("\n"):
            body += "\n"
        print(f"{'would write' if args.dry_run else 'wrote'} {rel} ({len(body)} bytes)")
        if not args.dry_run:
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_text(body)


if __name__ == "__main__":
    main()
