#ifndef FE310_DRIVER_WDOG_H
#define FE310_DRIVER_WDOG_H

#include <stdint.h>

void fe310_wdog_disable(void);
void fe310_wdog_start(uint32_t compare, uint8_t scale, int reset_enable);
void fe310_wdog_feed(void);
uint32_t fe310_wdog_count(void);

#endif
