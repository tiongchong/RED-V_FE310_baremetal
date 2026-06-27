#include "board/board.h"
#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/gpio.h"
#include "drivers/timer.h"

int test_gpio_output(EmbeddedCli *cli, int argc, char **argv)
{
    uint8_t pin = (uint8_t)test_arg_u32(argc, argv, "pin", BOARD_USER_LED_GPIO);
    uint32_t value = test_arg_u32(argc, argv, "value", 1u);
    uint32_t blink = test_arg_u32(argc, argv, "blink", 0u);
    uint32_t ms = test_arg_u32(argc, argv, "ms", 100u);

    fe310_gpio_set_mode(&board_gpio0, pin, FE310_GPIO_MODE_OUTPUT);

    if (blink != 0u) {
        for (uint32_t i = 0; i < blink; ++i) {
            fe310_gpio_write(&board_gpio0, pin, true);
            fe310_timer_delay_ms(ms, BOARD_RTC_HZ);
            fe310_gpio_write(&board_gpio0, pin, false);
            fe310_timer_delay_ms(ms, BOARD_RTC_HZ);
        }
        cli_printf(cli, "gpio%u blinked %lu times", pin, (unsigned long)blink);
        return test_pass();
    }

    fe310_gpio_write(&board_gpio0, pin, value != 0u);
    cli_printf(cli, "gpio%u output=%lu", pin, (unsigned long)(value != 0u));
    return test_pass();
}

int test_gpio_input(EmbeddedCli *cli, int argc, char **argv)
{
    uint8_t pin = (uint8_t)test_arg_u32(argc, argv, "pin", BOARD_USER_LED_GPIO);
    bool pullup = test_arg_bool(argc, argv, "pullup", false);

    fe310_gpio_set_mode(&board_gpio0, pin, pullup ? FE310_GPIO_MODE_INPUT_PULLUP : FE310_GPIO_MODE_INPUT);
    cli_printf(cli, "gpio%u input=%u", pin, fe310_gpio_read(&board_gpio0, pin) ? 1u : 0u);
    return test_pass();
}
