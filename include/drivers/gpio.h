#ifndef FE310_DRIVER_GPIO_H
#define FE310_DRIVER_GPIO_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uintptr_t base;
} fe310_gpio_t;

typedef enum {
    FE310_GPIO_MODE_INPUT,
    FE310_GPIO_MODE_INPUT_PULLUP,
    FE310_GPIO_MODE_OUTPUT,
    FE310_GPIO_MODE_DISABLED,
} fe310_gpio_mode_t;

typedef enum {
    FE310_GPIO_IRQ_RISE,
    FE310_GPIO_IRQ_FALL,
    FE310_GPIO_IRQ_HIGH,
    FE310_GPIO_IRQ_LOW,
} fe310_gpio_irq_mode_t;

void fe310_gpio_set_mode(const fe310_gpio_t *gpio, uint8_t pin, fe310_gpio_mode_t mode);
void fe310_gpio_write(const fe310_gpio_t *gpio, uint8_t pin, bool value);
void fe310_gpio_toggle(const fe310_gpio_t *gpio, uint8_t pin);
bool fe310_gpio_read(const fe310_gpio_t *gpio, uint8_t pin);
void fe310_gpio_enable_iof(const fe310_gpio_t *gpio, uint8_t pin, uint8_t iof);
void fe310_gpio_disable_iof(const fe310_gpio_t *gpio, uint8_t pin);
void fe310_gpio_irq_configure(const fe310_gpio_t *gpio, uint8_t pin, fe310_gpio_irq_mode_t mode, bool enable);
void fe310_gpio_irq_clear(const fe310_gpio_t *gpio, uint8_t pin, fe310_gpio_irq_mode_t mode);
bool fe310_gpio_irq_pending(const fe310_gpio_t *gpio, uint8_t pin, fe310_gpio_irq_mode_t mode);

#endif
