#ifndef FE310_MMIO_H
#define FE310_MMIO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static inline uint32_t fe310_read32(uintptr_t addr)
{
    return *(volatile uint32_t *)addr;
}

static inline void fe310_write32(uintptr_t addr, uint32_t value)
{
    *(volatile uint32_t *)addr = value;
}

static inline void fe310_set_bits32(uintptr_t addr, uint32_t mask)
{
    fe310_write32(addr, fe310_read32(addr) | mask);
}

static inline void fe310_clear_bits32(uintptr_t addr, uint32_t mask)
{
    fe310_write32(addr, fe310_read32(addr) & ~mask);
}

static inline void fe310_write_mask32(uintptr_t addr, uint32_t mask, uint32_t value)
{
    uint32_t reg = fe310_read32(addr);
    reg = (reg & ~mask) | (value & mask);
    fe310_write32(addr, reg);
}

static inline uint64_t fe310_read64_consistent(uintptr_t lo_addr, uintptr_t hi_addr)
{
    uint32_t hi0;
    uint32_t lo;
    uint32_t hi1;

    do {
        hi0 = fe310_read32(hi_addr);
        lo = fe310_read32(lo_addr);
        hi1 = fe310_read32(hi_addr);
    } while (hi0 != hi1);

    return ((uint64_t)hi1 << 32) | lo;
}

static inline void fe310_fence(void)
{
    __asm__ volatile("fence" ::: "memory");
}

static inline void fe310_wait_for_interrupt(void)
{
    __asm__ volatile("wfi");
}

static inline void fe310_enable_global_interrupts(void)
{
    const uintptr_t mask = (uintptr_t)0x8u;
    __asm__ volatile("csrs mstatus, %0" :: "r"(mask) : "memory");
}

static inline void fe310_disable_global_interrupts(void)
{
    const uintptr_t mask = (uintptr_t)0x8u;
    __asm__ volatile("csrc mstatus, %0" :: "r"(mask) : "memory");
}

static inline void fe310_enable_machine_timer_interrupt(void)
{
    const uintptr_t mask = (uintptr_t)0x80u;
    __asm__ volatile("csrs mie, %0" :: "r"(mask) : "memory");
}

static inline void fe310_disable_machine_timer_interrupt(void)
{
    const uintptr_t mask = (uintptr_t)0x80u;
    __asm__ volatile("csrc mie, %0" :: "r"(mask) : "memory");
}

static inline void fe310_enable_machine_external_interrupt(void)
{
    const uintptr_t mask = (uintptr_t)0x800u;
    __asm__ volatile("csrs mie, %0" :: "r"(mask) : "memory");
}

static inline void fe310_disable_machine_external_interrupt(void)
{
    const uintptr_t mask = (uintptr_t)0x800u;
    __asm__ volatile("csrc mie, %0" :: "r"(mask) : "memory");
}

#endif
