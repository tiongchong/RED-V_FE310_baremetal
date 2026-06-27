#include "drivers/prci.h"
#include "fe310/fe310_bitfields.h"
#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

void fe310_prci_enable_hfxosc(void)
{
    fe310_set_bits32(FE310_PRCI_BASE + FE310_PRCI_HFXOSCCFG, FE310_PRCI_HFXOSCCFG_EN);
    while (!fe310_prci_hfxosc_ready()) {
    }
}

bool fe310_prci_hfxosc_ready(void)
{
    return (fe310_read32(FE310_PRCI_BASE + FE310_PRCI_HFXOSCCFG) & FE310_PRCI_HFXOSCCFG_READY) != 0u;
}

void fe310_prci_select_hfxosc_bypass(void)
{
    fe310_prci_enable_hfxosc();
    fe310_write32(FE310_PRCI_BASE + FE310_PRCI_PLLCFG,
                  FE310_PRCI_PLLCFG_REFSEL | FE310_PRCI_PLLCFG_BYPASS | FE310_PRCI_PLLCFG_SEL);
}

void fe310_prci_configure_pll_320mhz_from_16mhz(void)
{
    fe310_prci_enable_hfxosc();
    fe310_write32(FE310_PRCI_BASE + FE310_PRCI_PLLOUTDIV, FE310_PRCI_PLLOUTDIV_BY1);
    fe310_write32(FE310_PRCI_BASE + FE310_PRCI_PLLCFG,
                  FE310_PRCI_PLLCFG_REFSEL |
                  FE310_PRCI_PLLCFG_R(1) |
                  FE310_PRCI_PLLCFG_F(79) |
                  FE310_PRCI_PLLCFG_Q(2));
    while ((fe310_read32(FE310_PRCI_BASE + FE310_PRCI_PLLCFG) & FE310_PRCI_PLLCFG_LOCK) == 0u) {
    }
    fe310_set_bits32(FE310_PRCI_BASE + FE310_PRCI_PLLCFG, FE310_PRCI_PLLCFG_SEL);
}

uint32_t fe310_prci_raw_pllcfg(void)
{
    return fe310_read32(FE310_PRCI_BASE + FE310_PRCI_PLLCFG);
}

uint32_t fe310_prci_raw_hfxosccfg(void)
{
    return fe310_read32(FE310_PRCI_BASE + FE310_PRCI_HFXOSCCFG);
}

uint32_t fe310_prci_raw_hfrosccfg(void)
{
    return fe310_read32(FE310_PRCI_BASE + FE310_PRCI_HFROSCCFG);
}
