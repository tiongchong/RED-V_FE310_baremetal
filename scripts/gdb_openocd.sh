#!/usr/bin/env sh
set -eu

ELF="${1:-build/redv_redboard/fe310_cli.elf}"
GDB="${GDB:-riscv64-unknown-elf-gdb}"
SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "${SCRIPT_DIR}/.." && pwd)"
OPENOCD_BIN="${OPENOCD:-openocd}"
OPENOCD_CFG="${OPENOCD_CFG:-${REPO_ROOT}/openocd/redv_redboard.cfg}"

"${OPENOCD_BIN}" -f "${OPENOCD_CFG}" &
OPENOCD_PID=$!
trap 'kill ${OPENOCD_PID} 2>/dev/null || true' EXIT
sleep 1

"${GDB}" "${ELF}" \
  -ex "target extended-remote localhost:3333" \
  -ex "monitor reset halt"
