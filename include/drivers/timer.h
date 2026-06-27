#ifndef FE310_DRIVER_TIMER_H
#define FE310_DRIVER_TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef void (*fe310_timer_callback_t)(void *context);

uint64_t fe310_timer_now_ticks(void);
uint64_t fe310_timer_ticks_from_us(uint32_t us, uint32_t timer_hz);
void fe310_timer_delay_ticks(uint64_t ticks);
void fe310_timer_delay_us(uint32_t us, uint32_t timer_hz);
void fe310_timer_delay_ms(uint32_t ms, uint32_t timer_hz);
void fe310_timer_set_mtimecmp(uint64_t ticks);
void fe310_timer_disable_mtimecmp(void);
void fe310_timer_register_callback(fe310_timer_callback_t callback, void *context);
void fe310_timer_irq_handler(void);
bool fe310_timer_callback_fired(void);
void fe310_timer_clear_callback_fired(void);

#endif
