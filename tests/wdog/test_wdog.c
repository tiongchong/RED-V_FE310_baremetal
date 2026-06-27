#include <string.h>

#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/timer.h"
#include "drivers/wdog.h"
#include "board/board.h"

int test_wdog_feed(EmbeddedCli *cli, int argc, char **argv)
{
    uint32_t cmp = test_arg_u32(argc, argv, "cmp", 0x10000u);
    uint8_t scale = (uint8_t)test_arg_u32(argc, argv, "scale", 8u);

    fe310_wdog_start(cmp, scale, 0);
    fe310_timer_delay_ms(test_arg_u32(argc, argv, "ms", 10u), BOARD_RTC_HZ);
    fe310_wdog_feed();
    cli_printf(cli, "wdog count=%lu fed", (unsigned long)fe310_wdog_count());
    fe310_wdog_disable();
    return test_pass();
}

int test_wdog_reset(EmbeddedCli *cli, int argc, char **argv)
{
    const char *confirm = test_arg_value(argc, argv, "confirm");
    if (confirm == 0 || strcmp(confirm, "YES_RESET") != 0) {
        cli_print(cli, "refusing reset; pass confirm=YES_RESET");
        return test_fail();
    }

    cli_print(cli, "watchdog reset armed");
    fe310_wdog_start(test_arg_u32(argc, argv, "cmp", 1000u),
                     (uint8_t)test_arg_u32(argc, argv, "scale", 0u),
                     1);
    while (1) {
    }
}
