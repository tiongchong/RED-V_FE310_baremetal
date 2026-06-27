#ifndef FE310_DRIVER_PWM_H
#define FE310_DRIVER_PWM_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uintptr_t base;
    uint32_t input_hz;
} fe310_pwm_t;

typedef struct {
    uint32_t frequency_hz;
    uint8_t scale;
    bool center_aligned;
} fe310_pwm_config_t;

void fe310_pwm_init(const fe310_pwm_t *pwm, const fe310_pwm_config_t *cfg);
void fe310_pwm_start(const fe310_pwm_t *pwm);
void fe310_pwm_stop(const fe310_pwm_t *pwm);
void fe310_pwm_set_period_ticks(const fe310_pwm_t *pwm, uint16_t ticks);
void fe310_pwm_set_compare(const fe310_pwm_t *pwm, uint8_t channel, uint16_t compare);
void fe310_pwm_set_duty_permille(const fe310_pwm_t *pwm, uint8_t channel, uint16_t permille);
uint32_t fe310_pwm_count(const fe310_pwm_t *pwm);
bool fe310_pwm_irq_pending(const fe310_pwm_t *pwm, uint8_t channel);
void fe310_pwm_irq_clear(const fe310_pwm_t *pwm, uint8_t channel);

#endif
