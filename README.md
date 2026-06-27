# FE310 RedBoard Bare-Metal Driver Lab

This repository is a register-level bare-metal firmware scaffold for the
SparkFun RED-V RedBoard / FE310 RedBoard family.  It is organized so each
FE310 IP block has a small low-level driver and a matching CLI-driven hardware
test.

The target workflow is:

1. Build and flash the firmware.
2. Open the board UART from Windows, Linux, or macOS.
3. Run validation commands through the embedded CLI.
4. Modify individual drivers or tests without touching unrelated code.

The CLI is provided by `funbiscuit/embedded-cli` under
`third_party/embedded-cli`.

## Quick Start

Install a RISC-V bare-metal toolchain that provides either
`riscv64-unknown-elf-gcc` or `riscv32-unknown-elf-gcc`.
The default ISA flag is `-march=rv32imac_zicsr`, which is required by newer
RISC-V toolchains for CSR instructions used during trap and interrupt setup.

```sh
make
make flash
make cli PORT=/dev/ttyACM0
```

Windows example:

```powershell
python scripts/serial_cli.py --port COM5 --baud 115200
```

macOS example:

```sh
python3 scripts/serial_cli.py --port /dev/cu.usbmodemXXXX --baud 115200
```

Linux example:

```sh
python3 scripts/serial_cli.py --port /dev/ttyACM0 --baud 115200
```

At the prompt:

```text
FE310> help
FE310> info
FE310> test list
FE310> test run gpio.output pin=5 value=1
FE310> test run uart.poll message=hello
FE310> test run i2c.scan
```

## Repository Map

```text
include/fe310/       SoC memory map, bit fields, IRQ IDs, MMIO helpers
include/board/       FE310 RedBoard clocks, pins, and board API
include/drivers/     Low-level IP driver public APIs
include/cli/         CLI and test registry APIs
src/board/           Board init, clock, and pinmux helpers
src/drivers/         Register-level IP driver implementations
src/cli/             embedded-cli port and command bindings
startup/             Reset code, trap entry, and minimal syscalls
tests/               CLI-callable validation tests by IP block
scripts/             Flash/debug/serial helper scripts
docs/                Porting notes, command list, and test matrix
examples/            Small examples that reuse the same drivers
```

## Default Board Assumptions

The default board config is `redv_redboard`:

- FE310-G002 core, RV32IMAC.
- UART0 is the CLI UART at 115200 baud.
- Built-in LED is FE310 GPIO 5.
- I2C0 is intended for the RED-V Qwiic connector.
- OTP tests are read-only unless explicitly compiled and confirmed.

If your boot path configures a different core/peripheral clock, override it at
build time:

```sh
make BOARD=redv_redboard CPU_HZ=16000000
```

See [docs/porting.md](docs/porting.md) before changing clocks or pinmux.

## Safety Notes

The OTP programming test is intentionally disabled.  OTP is one-time
programmable; the default firmware only exposes read/dump commands.

Watchdog reset and interrupt tests intentionally disturb normal control flow.
Read [docs/test_matrix.md](docs/test_matrix.md) before running destructive or
reset-oriented tests.
