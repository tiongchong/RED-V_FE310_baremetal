#include "drivers/gpio.h"

#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

static uint32_t pin_mask(uint8_t pin)
{
    return UINT32_C(1) << pin;
}

static uintptr_t irq_ie_offset(fe310_gpio_irq_mode_t mode)
{
    switch (mode) {
    case FE310_GPIO_IRQ_RISE:
        return FE310_GPIO_RISE_IE;
    case FE310_GPIO_IRQ_FALL:
        return FE310_GPIO_FALL_IE;
    case FE310_GPIO_IRQ_HIGH:
        return FE310_GPIO_HIGH_IE;
    case FE310_GPIO_IRQ_LOW:
    default:
        return FE310_GPIO_LOW_IE;
    }
}

static uintptr_t irq_ip_offset(fe310_gpio_irq_mode_t mode)
{
    switch (mode) {
    case FE310_GPIO_IRQ_RISE:
        return FE310_GPIO_RISE_IP;
    case FE310_GPIO_IRQ_FALL:
        return FE310_GPIO_FALL_IP;
    case FE310_GPIO_IRQ_HIGH:
        return FE310_GPIO_HIGH_IP;
    case FE310_GPIO_IRQ_LOW:
    default:
        return FE310_GPIO_LOW_IP;
    }
}

void fe310_gpio_set_mode(const fe310_gpio_t *gpio, uint8_t pin, fe310_gpio_mode_t mode)
{
    uint32_t mask = pin_mask(pin);

    fe310_clear_bits32(gpio->base + FE310_GPIO_IOF_EN, mask);

    switch (mode) {
    case FE310_GPIO_MODE_INPUT:
        fe310_clear_bits32(gpio->base + FE310_GPIO_OUTPUT_EN, mask);
        fe310_set_bits32(gpio->base + FE310_GPIO_INPUT_EN, mask);
        fe310_clear_bits32(gpio->base + FE310_GPIO_PUE, mask);
        break;
    case FE310_GPIO_MODE_INPUT_PULLUP:
        fe310_clear_bits32(gpio->base + FE310_GPIO_OUTPUT_EN, mask);
        fe310_set_bits32(gpio->base + FE310_GPIO_INPUT_EN, mask);
        fe310_set_bits32(gpio->base + FE310_GPIO_PUE, mask);
        break;
    case FE310_GPIO_MODE_OUTPUT:
        fe310_clear_bits32(gpio->base + FE310_GPIO_INPUT_EN, mask);
        fe310_set_bits32(gpio->base + FE310_GPIO_OUTPUT_EN, mask);
        break;
    case FE310_GPIO_MODE_DISABLED:
    default:
        fe310_clear_bits32(gpio->base + FE310_GPIO_INPUT_EN, mask);
        fe310_clear_bits32(gpio->base + FE310_GPIO_OUTPUT_EN, mask);
        fe310_clear_bits32(gpio->base + FE310_GPIO_PUE, mask);
        break;
    }
}

void fe310_gpio_write(const fe310_gpio_t *gpio, uint8_t pin, bool value)
{
    if (value) {
        fe310_set_bits32(gpio->base + FE310_GPIO_OUTPUT_VAL, pin_mask(pin));
    } else {
        fe310_clear_bits32(gpio->base + FE310_GPIO_OUTPUT_VAL, pin_mask(pin));
    }
}

void fe310_gpio_toggle(const fe310_gpio_t *gpio, uint8_t pin)
{
    uint32_t mask = pin_mask(pin);
    fe310_write32(gpio->base + FE310_GPIO_OUTPUT_VAL,
                  fe310_read32(gpio->base + FE310_GPIO_OUTPUT_VAL) ^ mask);
}

bool fe310_gpio_read(const fe310_gpio_t *gpio, uint8_t pin)
{
    return (fe310_read32(gpio->base + FE310_GPIO_INPUT_VAL) & pin_mask(pin)) != 0u;
}

void fe310_gpio_enable_iof(const fe310_gpio_t *gpio, uint8_t pin, uint8_t iof)
{
    uint32_t mask = pin_mask(pin);
    if (iof == 0u) {
        fe310_clear_bits32(gpio->base + FE310_GPIO_IOF_SEL, mask);
    } else {
        fe310_set_bits32(gpio->base + FE310_GPIO_IOF_SEL, mask);
    }
    fe310_set_bits32(gpio->base + FE310_GPIO_IOF_EN, mask);
}

void fe310_gpio_disable_iof(const fe310_gpio_t *gpio, uint8_t pin)
{
    fe310_clear_bits32(gpio->base + FE310_GPIO_IOF_EN, pin_mask(pin));
}

void fe310_gpio_irq_configure(const fe310_gpio_t *gpio, uint8_t pin, fe310_gpio_irq_mode_t mode, bool enable)
{
    uint32_t mask = pin_mask(pin);
    if (enable) {
        fe310_set_bits32(gpio->base + irq_ie_offset(mode), mask);
    } else {
        fe310_clear_bits32(gpio->base + irq_ie_offset(mode), mask);
    }
}

void fe310_gpio_irq_clear(const fe310_gpio_t *gpio, uint8_t pin, fe310_gpio_irq_mode_t mode)
{
    fe310_write32(gpio->base + irq_ip_offset(mode), pin_mask(pin));
}

bool fe310_gpio_irq_pending(const fe310_gpio_t *gpio, uint8_t pin, fe310_gpio_irq_mode_t mode)
{
    return (fe310_read32(gpio->base + irq_ip_offset(mode)) & pin_mask(pin)) != 0u;
}
