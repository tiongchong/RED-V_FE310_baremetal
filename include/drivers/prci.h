#ifndef FE310_DRIVER_PRCI_H
#define FE310_DRIVER_PRCI_H

#include <stdbool.h>
#include <stdint.h>

void fe310_prci_enable_hfxosc(void);
bool fe310_prci_hfxosc_ready(void);
void fe310_prci_select_hfxosc_bypass(void);
void fe310_prci_configure_pll_320mhz_from_16mhz(void);
uint32_t fe310_prci_raw_pllcfg(void);
uint32_t fe310_prci_raw_hfxosccfg(void);
uint32_t fe310_prci_raw_hfrosccfg(void);

#endif
