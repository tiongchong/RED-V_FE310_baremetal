#ifndef FE310_DRIVER_PLIC_H
#define FE310_DRIVER_PLIC_H

#include <stdbool.h>
#include <stdint.h>

void fe310_plic_set_priority(uint32_t irq, uint32_t priority);
void fe310_plic_enable(uint32_t irq);
void fe310_plic_disable(uint32_t irq);
bool fe310_plic_is_pending(uint32_t irq);
void fe310_plic_set_threshold(uint32_t threshold);
uint32_t fe310_plic_claim(void);
void fe310_plic_complete(uint32_t irq);

#endif
