#include "drivers/wdog.h"

#include "fe310/fe310_bitfields.h"
#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

static void wdog_unlock(void)
{
    fe310_write32(FE310_AON_BASE + FE310_AON_WDOGKEY, FE310_AON_WDOGKEY_VALUE);
}

void fe310_wdog_disable(void)
{
    wdog_unlock();
    fe310_write32(FE310_AON_BASE + FE310_AON_WDOGCFG, 0u);
}

void fe310_wdog_start(uint32_t compare, uint8_t scale, int reset_enable)
{
    uint32_t cfg = FE310_AON_CFG_SCALE(scale) | FE310_AON_CFG_ZEROCMP | FE310_AON_CFG_ENALWAYS;
    if (reset_enable != 0) {
        cfg |= FE310_AON_CFG_RSTEN;
    }

    wdog_unlock();
    fe310_write32(FE310_AON_BASE + FE310_AON_WDOGCMP0, compare);
    wdog_unlock();
    fe310_write32(FE310_AON_BASE + FE310_AON_WDOGCFG, cfg);
}

void fe310_wdog_feed(void)
{
    wdog_unlock();
    fe310_write32(FE310_AON_BASE + FE310_AON_WDOGFEED, FE310_AON_WDOGFEED_VALUE);
}

uint32_t fe310_wdog_count(void)
{
    return fe310_read32(FE310_AON_BASE + FE310_AON_WDOGCOUNT);
}
