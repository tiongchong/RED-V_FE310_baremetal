#include "drivers/uart.h"

#include "fe310/fe310_bitfields.h"
#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

static uint32_t uart_divisor(uint32_t input_hz, uint32_t baudrate)
{
    if (baudrate == 0u) {
        return 0u;
    }

    uint32_t div = (input_hz + (baudrate / 2u)) / baudrate;
    return (div == 0u) ? 0u : (div - 1u);
}

void fe310_uart_set_baudrate(const fe310_uart_t *uart, uint32_t baudrate)
{
    fe310_write32(uart->base + FE310_UART_DIV, uart_divisor(uart->input_hz, baudrate));
}

void fe310_uart_init(const fe310_uart_t *uart, const fe310_uart_config_t *cfg)
{
    uint32_t txctrl = 0u;
    uint32_t rxctrl = 0u;
    uint32_t ie = 0u;

    fe310_uart_set_baudrate(uart, cfg->baudrate);

    if (cfg->tx_enable) {
        txctrl |= FE310_UART_TXCTRL_TXEN | FE310_UART_TXCTRL_TXCNT(1);
    }
    if (cfg->rx_enable) {
        rxctrl |= FE310_UART_RXCTRL_RXEN | FE310_UART_RXCTRL_RXCNT(1);
    }
    if (cfg->tx_irq_enable) {
        ie |= FE310_UART_IE_TXWM;
    }
    if (cfg->rx_irq_enable) {
        ie |= FE310_UART_IE_RXWM;
    }

    fe310_write32(uart->base + FE310_UART_TXCTRL, txctrl);
    fe310_write32(uart->base + FE310_UART_RXCTRL, rxctrl);
    fe310_write32(uart->base + FE310_UART_IE, ie);
}

bool fe310_uart_try_write_byte(const fe310_uart_t *uart, uint8_t byte)
{
    if ((fe310_read32(uart->base + FE310_UART_TXDATA) & FE310_UART_TXDATA_FULL) != 0u) {
        return false;
    }
    fe310_write32(uart->base + FE310_UART_TXDATA, byte);
    return true;
}

void fe310_uart_write_byte(const fe310_uart_t *uart, uint8_t byte)
{
    while (!fe310_uart_try_write_byte(uart, byte)) {
    }
}

bool fe310_uart_read_byte(const fe310_uart_t *uart, uint8_t *byte)
{
    uint32_t value = fe310_read32(uart->base + FE310_UART_RXDATA);
    if ((value & FE310_UART_RXDATA_EMPTY) != 0u) {
        return false;
    }
    *byte = (uint8_t)(value & 0xffu);
    return true;
}

void fe310_uart_write(const fe310_uart_t *uart, const void *buf, size_t len)
{
    const uint8_t *p = (const uint8_t *)buf;
    for (size_t i = 0; i < len; ++i) {
        fe310_uart_write_byte(uart, p[i]);
    }
}

void fe310_uart_write_string(const fe310_uart_t *uart, const char *s)
{
    while (*s != '\0') {
        fe310_uart_write_byte(uart, (uint8_t)*s);
        ++s;
    }
}

uint32_t fe310_uart_pending(const fe310_uart_t *uart)
{
    return fe310_read32(uart->base + FE310_UART_IP);
}
