#include "drivers/clint.h"

#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

uint64_t fe310_clint_get_mtime(void)
{
    return fe310_read64_consistent(FE310_CLINT_BASE + FE310_CLINT_MTIME,
                                   FE310_CLINT_BASE + FE310_CLINT_MTIME + 4u);
}

void fe310_clint_set_mtimecmp(uint64_t value)
{
    uintptr_t base = FE310_CLINT_BASE + FE310_CLINT_MTIMECMP;
    fe310_write32(base, UINT32_MAX);
    fe310_write32(base + 4u, (uint32_t)(value >> 32));
    fe310_write32(base, (uint32_t)value);
}

void fe310_clint_disable_mtimecmp(void)
{
    fe310_clint_set_mtimecmp(UINT64_MAX);
}

void fe310_clint_set_msip(bool pending)
{
    fe310_write32(FE310_CLINT_BASE + FE310_CLINT_MSIP, pending ? 1u : 0u);
}
