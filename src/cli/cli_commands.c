#include "cli/cli_commands.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "board/board.h"
#include "cli/cli_print.h"
#include "cli/test_registry.h"
#include "drivers/gpio.h"
#include "drivers/prci.h"
#include "drivers/timer.h"
#include "drivers/uart.h"

#define MAX_CLI_TOKENS 16u

static int tokens_to_argv(char *args, char **argv, uint16_t max_tokens)
{
    uint16_t count = embeddedCliGetTokenCount(args);
    if (count > max_tokens) {
        count = max_tokens;
    }

    for (uint16_t i = 0; i < count; ++i) {
        argv[i] = embeddedCliGetTokenVariable(args, (uint16_t)(i + 1u));
    }

    return (int)count;
}

static void cmd_info(EmbeddedCli *cli, char *args, void *context)
{
    (void)args;
    (void)context;
    cli_printf(cli, "board=%s", BOARD_NAME);
    cli_printf(cli, "core_hz=%lu periph_hz=%lu uart_baud=%lu",
               (unsigned long)board_core_hz(),
               (unsigned long)board_periph_hz(),
               (unsigned long)BOARD_CLI_UART_BAUD);
    cli_printf(cli, "mtime=%lu", (unsigned long)fe310_timer_now_ticks());
    cli_printf(cli, "prci hfrosc=0x%08lx hfxosc=0x%08lx pll=0x%08lx",
               (unsigned long)fe310_prci_raw_hfrosccfg(),
               (unsigned long)fe310_prci_raw_hfxosccfg(),
               (unsigned long)fe310_prci_raw_pllcfg());
}

static void cmd_led(EmbeddedCli *cli, char *args, void *context)
{
    (void)context;
    char *argv[MAX_CLI_TOKENS];
    int argc = tokens_to_argv(args, argv, MAX_CLI_TOKENS);

    if (argc == 0 || strcmp(argv[0], "toggle") == 0) {
        fe310_gpio_toggle(&board_gpio0, BOARD_USER_LED_GPIO);
        cli_print(cli, "OK led toggled");
        return;
    }

    if (strcmp(argv[0], "on") == 0) {
        fe310_gpio_write(&board_gpio0, BOARD_USER_LED_GPIO, true);
        cli_print(cli, "OK led on");
        return;
    }

    if (strcmp(argv[0], "off") == 0) {
        fe310_gpio_write(&board_gpio0, BOARD_USER_LED_GPIO, false);
        cli_print(cli, "OK led off");
        return;
    }

    cli_print(cli, "usage: led [on|off|toggle]");
}

static void cmd_test(EmbeddedCli *cli, char *args, void *context)
{
    (void)context;
    char *argv[MAX_CLI_TOKENS];
    int argc = tokens_to_argv(args, argv, MAX_CLI_TOKENS);

    if (argc == 0 || strcmp(argv[0], "list") == 0) {
        size_t count = 0;
        const fe310_test_case_t *tests = fe310_test_registry(&count);
        for (size_t i = 0; i < count; ++i) {
            cli_printf(cli, "%s - %s", tests[i].name, tests[i].help);
        }
        return;
    }

    if (strcmp(argv[0], "run") == 0) {
        if (argc < 2) {
            cli_print(cli, "usage: test run <name> [key=value ...]");
            return;
        }

        const fe310_test_case_t *test = fe310_test_find(argv[1]);
        if (test == 0) {
            cli_print(cli, "ERR test not found");
            return;
        }

        cli_printf(cli, "RUN %s", test->name);
        int rc = test->run(cli, argc - 2, &argv[2]);
        cli_printf(cli, "%s %s rc=%d", rc == 0 ? "PASS" : "FAIL", test->name, rc);
        return;
    }

    cli_print(cli, "usage: test list | test run <name> [key=value ...]");
}

static void cmd_reboot(EmbeddedCli *cli, char *args, void *context)
{
    (void)cli;
    (void)args;
    (void)context;

    typedef void (*rom_entry_t)(void);
    rom_entry_t reset = (rom_entry_t)0x00001000u;
    reset();
}

void fe310_cli_register_commands(EmbeddedCli *cli)
{
    (void)embeddedCliAddBinding(cli, (CliCommandBinding){
        .name = "info",
        .help = "Print board, clock, timer, and PRCI status",
        .tokenizeArgs = true,
        .context = 0,
        .binding = cmd_info,
    });

    (void)embeddedCliAddBinding(cli, (CliCommandBinding){
        .name = "led",
        .help = "Control built-in LED: led [on|off|toggle]",
        .tokenizeArgs = true,
        .context = 0,
        .binding = cmd_led,
    });

    (void)embeddedCliAddBinding(cli, (CliCommandBinding){
        .name = "test",
        .help = "Run hardware tests: test list | test run <name> [key=value ...]",
        .tokenizeArgs = true,
        .context = 0,
        .binding = cmd_test,
    });

    (void)embeddedCliAddBinding(cli, (CliCommandBinding){
        .name = "reboot",
        .help = "Jump to FE310 mask ROM reset entry",
        .tokenizeArgs = true,
        .context = 0,
        .binding = cmd_reboot,
    });
}
