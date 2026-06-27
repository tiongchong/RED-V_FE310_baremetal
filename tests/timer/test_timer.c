#include "board/board.h"
#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/timer.h"
#include "fe310/mmio.h"

static volatile uint32_t timer_irq_seen;

static void timer_callback(void *context)
{
    (void)context;
    ++timer_irq_seen;
}

int test_timer_delay(EmbeddedCli *cli, int argc, char **argv)
{
    uint32_t ms = test_arg_u32(argc, argv, "ms", 100u);
    uint64_t before = fe310_timer_now_ticks();
    fe310_timer_delay_ms(ms, BOARD_RTC_HZ);
    uint64_t after = fe310_timer_now_ticks();
    uint64_t elapsed = after - before;
    cli_printf(cli, "mtime elapsed ticks=%lu", (unsigned long)elapsed);
    return elapsed >= fe310_timer_ticks_from_us(ms * 1000u, BOARD_RTC_HZ) ? test_pass() : test_fail();
}

int test_timer_irq(EmbeddedCli *cli, int argc, char **argv)
{
    uint32_t ms = test_arg_u32(argc, argv, "ms", 50u);
    uint64_t deadline = fe310_timer_now_ticks() + fe310_timer_ticks_from_us(ms * 1000u, BOARD_RTC_HZ);
    uint64_t timeout = fe310_timer_now_ticks() + fe310_timer_ticks_from_us((ms + 100u) * 1000u, BOARD_RTC_HZ);

    timer_irq_seen = 0;
    fe310_timer_clear_callback_fired();
    fe310_timer_register_callback(timer_callback, 0);
    fe310_timer_set_mtimecmp(deadline);
    fe310_enable_machine_timer_interrupt();
    fe310_enable_global_interrupts();

    while (!fe310_timer_callback_fired() && fe310_timer_now_ticks() < timeout) {
    }

    fe310_disable_machine_timer_interrupt();
    fe310_timer_disable_mtimecmp();

    cli_printf(cli, "timer_irq_seen=%lu", (unsigned long)timer_irq_seen);
    return timer_irq_seen != 0u ? test_pass() : test_fail();
}
