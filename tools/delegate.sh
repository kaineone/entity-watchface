#!/usr/bin/env bash
# Dispatch a coding brief to the headless Ollama worker and capture its answer.
#
#   tools/delegate.sh <brief.md> <out.md> [--think]
#
# --think keeps the reasoning in the output (use it to diagnose an empty answer:
# an empty response is almost always a contradiction in the brief).
# Control characters other than \n and \t are stripped from the output.
set -euo pipefail

MODEL="${WORKER_MODEL:-kimi-k2.7-code:cloud}"
brief="$1"
out="$2"
hide="--hidethinking"
[[ "${3:-}" == "--think" ]] && hide=""

raw="$(mktemp)"
trap 'rm -f "$raw"' EXIT

# shellcheck disable=SC2086
ollama run "$MODEL" $hide --nowordwrap < "$brief" > "$raw" 2> "${out}.stderr"

# Strip ANSI escapes, then any remaining control chars except tab/newline.
sed -E 's/\x1b\[[0-9;?]*[A-Za-z]//g' "$raw" | tr -d '\000-\010\013-\037\177' > "$out"

bytes=$(wc -c < "$out")
echo "worker=$MODEL bytes=$bytes out=$out"
if (( bytes < 16 )); then
  echo "WARNING: near-empty response; re-run with --think and read the reasoning" >&2
  exit 3
fi
