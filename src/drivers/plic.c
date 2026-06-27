#include "drivers/plic.h"

#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

void fe310_plic_set_priority(uint32_t irq, uint32_t priority)
{
    fe310_write32(FE310_PLIC_BASE + FE310_PLIC_PRIORITY + irq * 4u, priority & 0x7u);
}

void fe310_plic_enable(uint32_t irq)
{
    uintptr_t reg = FE310_PLIC_BASE + FE310_PLIC_ENABLE + ((irq / 32u) * 4u);
    fe310_set_bits32(reg, UINT32_C(1) << (irq % 32u));
}

void fe310_plic_disable(uint32_t irq)
{
    uintptr_t reg = FE310_PLIC_BASE + FE310_PLIC_ENABLE + ((irq / 32u) * 4u);
    fe310_clear_bits32(reg, UINT32_C(1) << (irq % 32u));
}

bool fe310_plic_is_pending(uint32_t irq)
{
    uintptr_t reg = FE310_PLIC_BASE + FE310_PLIC_PENDING + ((irq / 32u) * 4u);
    return (fe310_read32(reg) & (UINT32_C(1) << (irq % 32u))) != 0u;
}

void fe310_plic_set_threshold(uint32_t threshold)
{
    fe310_write32(FE310_PLIC_BASE + FE310_PLIC_THRESHOLD, threshold & 0x7u);
}

uint32_t fe310_plic_claim(void)
{
    return fe310_read32(FE310_PLIC_BASE + FE310_PLIC_CLAIM);
}

void fe310_plic_complete(uint32_t irq)
{
    fe310_write32(FE310_PLIC_BASE + FE310_PLIC_CLAIM, irq);
}
