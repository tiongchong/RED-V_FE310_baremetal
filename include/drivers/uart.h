#ifndef FE310_DRIVER_UART_H
#define FE310_DRIVER_UART_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uintptr_t base;
    uint32_t input_hz;
} fe310_uart_t;

typedef struct {
    uint32_t baudrate;
    bool tx_enable;
    bool rx_enable;
    bool tx_irq_enable;
    bool rx_irq_enable;
} fe310_uart_config_t;

void fe310_uart_init(const fe310_uart_t *uart, const fe310_uart_config_t *cfg);
void fe310_uart_set_baudrate(const fe310_uart_t *uart, uint32_t baudrate);
void fe310_uart_write_byte(const fe310_uart_t *uart, uint8_t byte);
bool fe310_uart_try_write_byte(const fe310_uart_t *uart, uint8_t byte);
bool fe310_uart_read_byte(const fe310_uart_t *uart, uint8_t *byte);
void fe310_uart_write(const fe310_uart_t *uart, const void *buf, size_t len);
void fe310_uart_write_string(const fe310_uart_t *uart, const char *s);
uint32_t fe310_uart_pending(const fe310_uart_t *uart);

#endif
