#include "board/board.h"
#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/pwm.h"
#include "drivers/timer.h"

static const fe310_pwm_t pwm0 = {
    .base = FE310_PWM0_BASE,
    .input_hz = BOARD_PERIPH_HZ,
};

static void init_pwm(int argc, char **argv)
{
    fe310_pwm_config_t cfg = {
        .frequency_hz = test_arg_u32(argc, argv, "hz", BOARD_DEFAULT_PWM_HZ),
        .scale = (uint8_t)test_arg_u32(argc, argv, "scale", 0u),
        .center_aligned = test_arg_bool(argc, argv, "center", false),
    };
    fe310_pwm_init(&pwm0, &cfg);
    fe310_pwm_set_period_ticks(&pwm0, (uint16_t)test_arg_u32(argc, argv, "period", 1000u));
}

int test_pwm_duty(EmbeddedCli *cli, int argc, char **argv)
{
    uint8_t channel = (uint8_t)test_arg_u32(argc, argv, "channel", 1u);
    uint16_t duty = (uint16_t)test_arg_u32(argc, argv, "duty", 500u);

    init_pwm(argc, argv);
    fe310_pwm_set_duty_permille(&pwm0, channel, duty);
    fe310_pwm_start(&pwm0);
    cli_printf(cli, "pwm0 channel=%u duty_permille=%u", channel, duty);
    return (channel >= 1u && channel <= 3u && duty <= 1000u) ? test_pass() : test_fail();
}

int test_pwm_count(EmbeddedCli *cli, int argc, char **argv)
{
    init_pwm(argc, argv);
    fe310_pwm_start(&pwm0);
    uint32_t a = fe310_pwm_count(&pwm0);
    fe310_timer_delay_ms(test_arg_u32(argc, argv, "ms", 10u), BOARD_RTC_HZ);
    uint32_t b = fe310_pwm_count(&pwm0);
    cli_printf(cli, "pwm0 count %lu -> %lu", (unsigned long)a, (unsigned long)b);
    return a != b ? test_pass() : test_fail();
}
