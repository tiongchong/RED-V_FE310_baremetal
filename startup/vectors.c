#include <stdint.h>

#include "drivers/plic.h"
#include "drivers/timer.h"
#include "fe310/fe310_irq.h"

#define MCAUSE_INTERRUPT_BIT 0x80000000UL
#define MCAUSE_CAUSE_MASK    0x7FFFFFFFUL
#define MCAUSE_MACHINE_TIMER  7UL
#define MCAUSE_MACHINE_EXT    11UL

typedef void (*fe310_irq_handler_t)(uint32_t irq);

static fe310_irq_handler_t external_irq_handler;

void fe310_register_external_irq_handler(fe310_irq_handler_t handler)
{
    external_irq_handler = handler;
}

__attribute__((weak)) void fe310_unhandled_trap(uint32_t mcause, uint32_t mepc, uint32_t mtval)
{
    (void)mcause;
    (void)mepc;
    (void)mtval;
    while (1) {
        __asm__ volatile("wfi");
    }
}

uintptr_t fe310_trap_handler(uint32_t mcause, uint32_t mepc, uint32_t mtval, void *regs)
{
    (void)regs;

    if ((mcause & MCAUSE_INTERRUPT_BIT) == 0u) {
        fe310_unhandled_trap(mcause, mepc, mtval);
        return (uintptr_t)mepc;
    }

    switch (mcause & MCAUSE_CAUSE_MASK) {
    case MCAUSE_MACHINE_TIMER:
        fe310_timer_irq_handler();
        break;
    case MCAUSE_MACHINE_EXT: {
        uint32_t irq = fe310_plic_claim();
        if (irq != 0u) {
            if (external_irq_handler != 0) {
                external_irq_handler(irq);
            }
            fe310_plic_complete(irq);
        }
        break;
    }
    default:
        fe310_unhandled_trap(mcause, mepc, mtval);
        break;
    }

    return (uintptr_t)mepc;
}
