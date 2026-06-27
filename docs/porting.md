# Porting Notes

The default target is `redv_redboard`, using the FE310-G002 memory map and
UART0 as the CLI console.

## Clocks

`BOARD_CPU_HZ` defaults to `16000000`.  It is used for UART, SPI, and I2C
divisors.  If your loader configures PLL to another rate, build with:

```sh
make CPU_HZ=320000000
```

The timer tests use `BOARD_RTC_HZ=32768` for CLINT `mtime`, which is the usual
FE310 low-frequency time base.

## UART

The shell uses UART0:

```text
BOARD_CLI_UART_BASE = FE310_UART0_BASE
BOARD_CLI_UART_BAUD = 115200
```

Change these in `include/board/fe310_redboard.h` if your board routes the host
USB serial bridge to a different UART.

## Pinmux

Pinmux lives in `src/board/pinmux.c`.  Driver code does not assume pins; tests
call board helpers before using I2C0 or SPI1.

Review these macros for your exact schematic:

```text
BOARD_USER_LED_GPIO
BOARD_I2C0_SDA_GPIO
BOARD_I2C0_SCL_GPIO
BOARD_SPI1_SCK_GPIO
BOARD_SPI1_MISO_GPIO
BOARD_SPI1_MOSI_GPIO
BOARD_SPI1_CS0_GPIO
```

## Adding a Board

1. Add `include/board/<board>.h`.
2. Update `include/board/board.h` to select it.
3. Add or adjust `src/board/*.c` pinmux and clock helpers.
4. Build with `make BOARD=<board>`.
