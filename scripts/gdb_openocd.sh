#!/usr/bin/env sh
set -eu

ELF="${1:-build/redv_redboard/fe310_cli.elf}"
GDB="${GDB:-riscv64-unknown-elf-gdb}"

openocd -f board/sifive-hifive1-revb.cfg &
OPENOCD_PID=$!
trap 'kill ${OPENOCD_PID} 2>/dev/null || true' EXIT
sleep 1

"${GDB}" "${ELF}" \
  -ex "target extended-remote localhost:3333" \
  -ex "monitor reset halt"
