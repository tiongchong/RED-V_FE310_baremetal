#!/usr/bin/env sh
set -eu

IMAGE="${1:-build/redv_redboard/fe310_cli.elf}"
SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(CDPATH= cd -- "${SCRIPT_DIR}/.." && pwd)"
OPENOCD_BIN="${OPENOCD:-openocd}"
OPENOCD_CFG="${OPENOCD_CFG:-${REPO_ROOT}/openocd/redv_redboard.cfg}"

case "${IMAGE}" in
  *.bin)
    "${OPENOCD_BIN}" \
      -f "${OPENOCD_CFG}" \
      -c "flash write_image erase ${IMAGE} 0x20010000 bin; verify_image ${IMAGE} 0x20010000 bin; reset; exit"
    ;;
  *)
    "${OPENOCD_BIN}" \
      -f "${OPENOCD_CFG}" \
      -c "program ${IMAGE} verify reset exit"
    ;;
esac
