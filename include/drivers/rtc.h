#ifndef FE310_DRIVER_RTC_H
#define FE310_DRIVER_RTC_H

#include <stdbool.h>
#include <stdint.h>

void fe310_rtc_start(uint8_t scale);
uint64_t fe310_rtc_count(void);
void fe310_rtc_set_compare(uint32_t value);
bool fe310_rtc_irq_pending(void);
void fe310_rtc_irq_clear(void);

#endif
