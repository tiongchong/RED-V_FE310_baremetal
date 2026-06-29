#!/usr/bin/env sh
set -eu

IMAGE="${1:-build/redv_redboard/fe310_cli.elf}"
JLINK_BIN="${JLINK:-JLinkExe}"
JLINK_DEVICE="${JLINK_DEVICE:-FE310}"
JLINK_IF="${JLINK_IF:-JTAG}"
JLINK_SPEED="${JLINK_SPEED:-4000}"
JLINK_FLASH_ADDR="${JLINK_FLASH_ADDR:-0x20010000}"

TMPDIR="${TMPDIR:-/tmp}"
CMD_FILE="${TMPDIR}/fe310_flash_$$.jlink"
trap 'rm -f "${CMD_FILE}"' EXIT INT TERM

{
  echo "ExitOnError 1"
  echo "r"
  echo "h"
  case "${IMAGE}" in
    *.bin)
      echo "loadfile ${IMAGE} ${JLINK_FLASH_ADDR}"
      ;;
    *)
      echo "loadfile ${IMAGE}"
      ;;
  esac
  echo "r"
  echo "g"
  echo "exit"
} > "${CMD_FILE}"

"${JLINK_BIN}" \
  -device "${JLINK_DEVICE}" \
  -if "${JLINK_IF}" \
  -speed "${JLINK_SPEED}" \
  -jtagconf -1,-1 \
  -autoconnect 1 \
  -nogui 1 \
  -CommandFile "${CMD_FILE}"
