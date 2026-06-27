#include "board/board.h"
#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/timer.h"
#include "drivers/uart.h"

int test_uart_poll(EmbeddedCli *cli, int argc, char **argv)
{
    const char *message = test_arg_value(argc, argv, "message");
    if (message == 0) {
        message = "uart polling path OK";
    }

    fe310_uart_write_string(&board_cli_uart, "UART direct write: ");
    fe310_uart_write_string(&board_cli_uart, message);
    fe310_uart_write_string(&board_cli_uart, "\r\n");
    cli_print(cli, "UART polling write completed");
    return test_pass();
}

int test_uart_rx_timeout(EmbeddedCli *cli, int argc, char **argv)
{
    uint32_t ms = test_arg_u32(argc, argv, "ms", 3000u);
    uint64_t start = fe310_timer_now_ticks();
    uint64_t timeout = fe310_timer_ticks_from_us(ms * 1000u, BOARD_RTC_HZ);
    uint8_t byte = 0;

    cli_printf(cli, "send one byte within %lu ms", (unsigned long)ms);
    while ((fe310_timer_now_ticks() - start) < timeout) {
        if (fe310_uart_read_byte(&board_cli_uart, &byte)) {
            cli_printf(cli, "rx=0x%02x '%c'", byte, (byte >= 32u && byte < 127u) ? byte : '.');
            return test_pass();
        }
    }

    cli_print(cli, "timeout waiting for UART byte");
    return test_fail();
}
