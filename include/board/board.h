#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include "board/fe310_redboard.h"
#include "drivers/gpio.h"
#include "drivers/uart.h"

extern const fe310_uart_t board_cli_uart;
extern const fe310_gpio_t board_gpio0;

void board_early_init(void);
void board_init(void);
void board_putc(char c);
void board_write(const char *s);
uint32_t board_core_hz(void);
uint32_t board_periph_hz(void);
uint64_t board_mtime_ticks(void);
void board_configure_i2c0_pins(void);
void board_configure_spi1_pins(void);

#endif
