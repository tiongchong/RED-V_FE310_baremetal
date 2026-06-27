#include "drivers/aon.h"

#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

void fe310_aon_unlock(void)
{
    fe310_write32(FE310_AON_BASE + FE310_AON_PMUKEY, 0x51f15e);
}

uint32_t fe310_aon_backup_read(uint8_t index)
{
    return fe310_read32(FE310_AON_BASE + FE310_AON_BACKUP0 + ((uintptr_t)(index & 0x0fu) * 4u));
}

void fe310_aon_backup_write(uint8_t index, uint32_t value)
{
    fe310_write32(FE310_AON_BASE + FE310_AON_BACKUP0 + ((uintptr_t)(index & 0x0fu) * 4u), value);
}
