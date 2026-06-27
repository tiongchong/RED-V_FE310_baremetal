#include "cli/test_registry.h"

#include <string.h>

int test_gpio_output(EmbeddedCli *cli, int argc, char **argv);
int test_gpio_input(EmbeddedCli *cli, int argc, char **argv);
int test_uart_poll(EmbeddedCli *cli, int argc, char **argv);
int test_uart_rx_timeout(EmbeddedCli *cli, int argc, char **argv);
int test_i2c_scan(EmbeddedCli *cli, int argc, char **argv);
int test_i2c_probe(EmbeddedCli *cli, int argc, char **argv);
int test_spi_loopback(EmbeddedCli *cli, int argc, char **argv);
int test_spi_flash_id(EmbeddedCli *cli, int argc, char **argv);
int test_pwm_duty(EmbeddedCli *cli, int argc, char **argv);
int test_pwm_count(EmbeddedCli *cli, int argc, char **argv);
int test_timer_delay(EmbeddedCli *cli, int argc, char **argv);
int test_timer_irq(EmbeddedCli *cli, int argc, char **argv);
int test_otp_read(EmbeddedCli *cli, int argc, char **argv);
int test_rtc_tick(EmbeddedCli *cli, int argc, char **argv);
int test_wdog_feed(EmbeddedCli *cli, int argc, char **argv);
int test_wdog_reset(EmbeddedCli *cli, int argc, char **argv);
int test_plic_basic(EmbeddedCli *cli, int argc, char **argv);
int test_aon_backup(EmbeddedCli *cli, int argc, char **argv);

static const fe310_test_case_t tests[] = {
    { "aon.backup", "Write/read one AON backup register", test_aon_backup },
    { "gpio.input", "Read a GPIO input pin", test_gpio_input },
    { "gpio.output", "Drive or blink a GPIO output pin", test_gpio_output },
    { "i2c.probe", "Probe one I2C 7-bit address", test_i2c_probe },
    { "i2c.scan", "Scan I2C0 for devices", test_i2c_scan },
    { "otp.read", "Read FE310 OTP memory bytes", test_otp_read },
    { "plic.basic", "Configure PLIC priority/enable state", test_plic_basic },
    { "pwm.count", "Start PWM and verify counter movement", test_pwm_count },
    { "pwm.duty", "Configure PWM duty on channel 1..3", test_pwm_duty },
    { "rtc.tick", "Start RTC and verify the count advances", test_rtc_tick },
    { "spi.flash-id", "Read JEDEC ID through SPI1 external wiring", test_spi_flash_id },
    { "spi.loopback", "SPI1 MOSI-to-MISO loopback transfer", test_spi_loopback },
    { "timer.delay", "Busy-wait using CLINT mtime", test_timer_delay },
    { "timer.irq", "Validate machine timer interrupt", test_timer_irq },
    { "uart.poll", "Print through CLI UART polling path", test_uart_poll },
    { "uart.rx-timeout", "Wait for one UART byte within timeout", test_uart_rx_timeout },
    { "wdog.feed", "Start and feed watchdog without reset", test_wdog_feed },
    { "wdog.reset", "Guarded watchdog reset test", test_wdog_reset },
};

const fe310_test_case_t *fe310_test_registry(size_t *count)
{
    *count = sizeof(tests) / sizeof(tests[0]);
    return tests;
}

const fe310_test_case_t *fe310_test_find(const char *name)
{
    size_t count = 0;
    const fe310_test_case_t *registry = fe310_test_registry(&count);

    for (size_t i = 0; i < count; ++i) {
        if (strcmp(registry[i].name, name) == 0) {
            return &registry[i];
        }
    }

    return 0;
}
