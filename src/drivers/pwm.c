#include "drivers/pwm.h"

#include "fe310/fe310_bitfields.h"
#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

static uintptr_t cmp_offset(uint8_t channel)
{
    return FE310_PWM_CMP0 + ((uintptr_t)(channel & 0x3u) * 4u);
}

void fe310_pwm_init(const fe310_pwm_t *pwm, const fe310_pwm_config_t *cfg)
{
    uint32_t config = FE310_PWM_CFG_SCALE(cfg->scale) | FE310_PWM_CFG_ZEROCMP;
    if (cfg->center_aligned) {
        config |= FE310_PWM_CFG_CMP_CENTER(1);
        config |= FE310_PWM_CFG_CMP_CENTER(2);
        config |= FE310_PWM_CFG_CMP_CENTER(3);
    }

    fe310_write32(pwm->base + FE310_PWM_CFG, 0u);
    fe310_write32(pwm->base + FE310_PWM_COUNT, 0u);
    fe310_write32(pwm->base + FE310_PWM_CMP0, 0xffffu);
    fe310_write32(pwm->base + FE310_PWM_CMP1, 0u);
    fe310_write32(pwm->base + FE310_PWM_CMP2, 0u);
    fe310_write32(pwm->base + FE310_PWM_CMP3, 0u);
    fe310_write32(pwm->base + FE310_PWM_CFG, config);
    (void)cfg->frequency_hz;
}

void fe310_pwm_start(const fe310_pwm_t *pwm)
{
    fe310_set_bits32(pwm->base + FE310_PWM_CFG, FE310_PWM_CFG_ENALWAYS);
}

void fe310_pwm_stop(const fe310_pwm_t *pwm)
{
    fe310_clear_bits32(pwm->base + FE310_PWM_CFG, FE310_PWM_CFG_ENALWAYS | FE310_PWM_CFG_ENONESHOT);
}

void fe310_pwm_set_period_ticks(const fe310_pwm_t *pwm, uint16_t ticks)
{
    fe310_write32(pwm->base + FE310_PWM_CMP0, ticks == 0u ? 1u : ticks);
}

void fe310_pwm_set_compare(const fe310_pwm_t *pwm, uint8_t channel, uint16_t compare)
{
    if (channel > 3u) {
        return;
    }
    fe310_write32(pwm->base + cmp_offset(channel), compare);
}

void fe310_pwm_set_duty_permille(const fe310_pwm_t *pwm, uint8_t channel, uint16_t permille)
{
    if (channel == 0u || channel > 3u) {
        return;
    }
    if (permille > 1000u) {
        permille = 1000u;
    }

    uint32_t period = fe310_read32(pwm->base + FE310_PWM_CMP0) & 0xffffu;
    uint32_t cmp = (period * permille) / 1000u;
    fe310_pwm_set_compare(pwm, channel, (uint16_t)cmp);
}

uint32_t fe310_pwm_count(const fe310_pwm_t *pwm)
{
    return fe310_read32(pwm->base + FE310_PWM_COUNT);
}

bool fe310_pwm_irq_pending(const fe310_pwm_t *pwm, uint8_t channel)
{
    if (channel > 3u) {
        return false;
    }
    return (fe310_read32(pwm->base + FE310_PWM_CFG) & FE310_PWM_CFG_CMP_IP(channel)) != 0u;
}

void fe310_pwm_irq_clear(const fe310_pwm_t *pwm, uint8_t channel)
{
    if (channel > 3u) {
        return;
    }
    fe310_clear_bits32(pwm->base + FE310_PWM_CFG, FE310_PWM_CFG_CMP_IP(channel));
}
