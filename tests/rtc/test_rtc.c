#include "board/board.h"
#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/rtc.h"
#include "drivers/timer.h"

int test_rtc_tick(EmbeddedCli *cli, int argc, char **argv)
{
    uint8_t scale = (uint8_t)test_arg_u32(argc, argv, "scale", 0u);
    uint32_t ms = test_arg_u32(argc, argv, "ms", 100u);

    fe310_rtc_start(scale);
    uint64_t a = fe310_rtc_count();
    fe310_timer_delay_ms(ms, BOARD_RTC_HZ);
    uint64_t b = fe310_rtc_count();

    cli_printf(cli, "rtc count %lu -> %lu", (unsigned long)a, (unsigned long)b);
    return b != a ? test_pass() : test_fail();
}
