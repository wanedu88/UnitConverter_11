#!/usr/bin/env bash
# Generate tests/golden_master_expected.txt from UnitConverter CLI stdout.
# Usage: ./tests/scripts/generate_golden_master.sh [build_dir] [exe_name]

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
BUILD_DIR="${1:-build}"
EXE_NAME="${2:-UnitConverter}"
OUT_FILE="${ROOT}/tests/golden_master_expected.txt"
PROMPT='Insert value for converting (ex: meter:2.5): '

if [[ -x "${BUILD_DIR}/${EXE_NAME}" ]]; then
  EXE="${BUILD_DIR}/${EXE_NAME}"
elif [[ -x "${ROOT}/${BUILD_DIR}/${EXE_NAME}" ]]; then
  EXE="${ROOT}/${BUILD_DIR}/${EXE_NAME}"
else
  echo "Executable not found under ${BUILD_DIR}/${EXE_NAME}" >&2
  exit 1
fi

SCENARIOS=("meter:2.5" "feet:1.0" "yard:1.0" "meter:0.0")

strip_identity() {
  awk -v prompt="$PROMPT" '
    {
      line = $0
      sub("^" prompt, "", line)
      if (line !~ / = /) next
      n = split(line, left, " = ")
      if (n < 2) next
      split(left[1], lv, " ")
      split(left[2], rv, " ")
      if (lv[length(lv)] == rv[length(rv)]) next
      print line
    }
  '
}

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

{
  for i in "${!SCENARIOS[@]}"; do
    scenario="${SCENARIOS[$i]}"
    printf '[%s]\n' "$scenario"
    printf '%s\n' "$scenario" > "${TMP_DIR}/input.txt"
    "${EXE}" < "${TMP_DIR}/input.txt" > "${TMP_DIR}/actual.txt" 2>/dev/null || true
    strip_identity < "${TMP_DIR}/actual.txt"
    if [[ $i -lt $((${#SCENARIOS[@]} - 1)) ]]; then
      printf '%s\n' '---'
    fi
  done
} > "${OUT_FILE}"

echo "Wrote ${OUT_FILE}"
echo "Next: git add tests/golden_master_expected.txt"
