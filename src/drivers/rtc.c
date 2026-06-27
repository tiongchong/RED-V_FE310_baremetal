#include "drivers/rtc.h"

#include "fe310/fe310_bitfields.h"
#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

void fe310_rtc_start(uint8_t scale)
{
    fe310_write32(FE310_AON_BASE + FE310_AON_RTCCFG,
                  FE310_AON_CFG_SCALE(scale) | FE310_AON_CFG_ENALWAYS);
}

uint64_t fe310_rtc_count(void)
{
    return fe310_read64_consistent(FE310_AON_BASE + FE310_AON_RTCLO,
                                   FE310_AON_BASE + FE310_AON_RTCHI);
}

void fe310_rtc_set_compare(uint32_t value)
{
    fe310_write32(FE310_AON_BASE + FE310_AON_RTCCMP0, value);
}

bool fe310_rtc_irq_pending(void)
{
    return (fe310_read32(FE310_AON_BASE + FE310_AON_RTCCFG) & FE310_AON_CFG_IP0) != 0u;
}

void fe310_rtc_irq_clear(void)
{
    fe310_clear_bits32(FE310_AON_BASE + FE310_AON_RTCCFG, FE310_AON_CFG_IP0);
}
