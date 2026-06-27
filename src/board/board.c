#include "board/board.h"

#include "drivers/clint.h"
#include "drivers/gpio.h"
#include "drivers/uart.h"
#include "fe310/mmio.h"

const fe310_uart_t board_cli_uart = {
    .base = BOARD_CLI_UART_BASE,
    .input_hz = BOARD_PERIPH_HZ,
};

const fe310_gpio_t board_gpio0 = {
    .base = BOARD_GPIO_BASE,
};

void board_early_init(void)
{
    fe310_disable_global_interrupts();
}

void board_init(void)
{
    fe310_uart_config_t uart_cfg = {
        .baudrate = BOARD_CLI_UART_BAUD,
        .tx_enable = true,
        .rx_enable = true,
        .tx_irq_enable = false,
        .rx_irq_enable = false,
    };

    fe310_gpio_set_mode(&board_gpio0, BOARD_USER_LED_GPIO, FE310_GPIO_MODE_OUTPUT);
    fe310_uart_init(&board_cli_uart, &uart_cfg);
}

void board_putc(char c)
{
    if (c == '\n') {
        fe310_uart_write_byte(&board_cli_uart, '\r');
    }
    fe310_uart_write_byte(&board_cli_uart, (uint8_t)c);
}

void board_write(const char *s)
{
    fe310_uart_write_string(&board_cli_uart, s);
}

uint32_t board_core_hz(void)
{
    return BOARD_CORE_HZ;
}

uint32_t board_periph_hz(void)
{
    return BOARD_PERIPH_HZ;
}

uint64_t board_mtime_ticks(void)
{
    return fe310_clint_get_mtime();
}
