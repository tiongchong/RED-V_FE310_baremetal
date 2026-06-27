#include "drivers/timer.h"

#include "drivers/clint.h"

static fe310_timer_callback_t timer_callback;
static void *timer_callback_context;
static volatile bool timer_fired;

uint64_t fe310_timer_now_ticks(void)
{
    return fe310_clint_get_mtime();
}

uint64_t fe310_timer_ticks_from_us(uint32_t us, uint32_t timer_hz)
{
    return ((uint64_t)us * timer_hz + 999999u) / 1000000u;
}

void fe310_timer_delay_ticks(uint64_t ticks)
{
    uint64_t start = fe310_timer_now_ticks();
    while ((fe310_timer_now_ticks() - start) < ticks) {
    }
}

void fe310_timer_delay_us(uint32_t us, uint32_t timer_hz)
{
    fe310_timer_delay_ticks(fe310_timer_ticks_from_us(us, timer_hz));
}

void fe310_timer_delay_ms(uint32_t ms, uint32_t timer_hz)
{
    for (uint32_t i = 0; i < ms; ++i) {
        fe310_timer_delay_us(1000u, timer_hz);
    }
}

void fe310_timer_set_mtimecmp(uint64_t ticks)
{
    fe310_clint_set_mtimecmp(ticks);
}

void fe310_timer_disable_mtimecmp(void)
{
    fe310_clint_disable_mtimecmp();
}

void fe310_timer_register_callback(fe310_timer_callback_t callback, void *context)
{
    timer_callback = callback;
    timer_callback_context = context;
    timer_fired = false;
}

void fe310_timer_irq_handler(void)
{
    fe310_timer_disable_mtimecmp();
    timer_fired = true;
    if (timer_callback != 0) {
        timer_callback(timer_callback_context);
    }
}

bool fe310_timer_callback_fired(void)
{
    return timer_fired;
}

void fe310_timer_clear_callback_fired(void)
{
    timer_fired = false;
}
