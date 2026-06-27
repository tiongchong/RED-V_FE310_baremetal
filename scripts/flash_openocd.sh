#!/usr/bin/env sh
set -eu

ELF="${1:-build/redv_redboard/fe310_cli.elf}"

openocd \
  -f board/sifive-hifive1-revb.cfg \
  -c "program ${ELF} verify reset exit"
