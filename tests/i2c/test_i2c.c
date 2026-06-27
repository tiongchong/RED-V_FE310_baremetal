#include "board/board.h"
#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/i2c.h"

void board_configure_i2c0_pins(void);

static const fe310_i2c_t i2c0 = {
    .base = FE310_I2C0_BASE,
    .input_hz = BOARD_PERIPH_HZ,
};

static void init_i2c(int argc, char **argv)
{
    fe310_i2c_config_t cfg = {
        .bus_hz = test_arg_u32(argc, argv, "hz", BOARD_DEFAULT_I2C_HZ),
        .irq_enable = false,
    };
    board_configure_i2c0_pins();
    fe310_i2c_init(&i2c0, &cfg);
}

int test_i2c_scan(EmbeddedCli *cli, int argc, char **argv)
{
    uint32_t start = test_arg_u32(argc, argv, "start", 0x08u);
    uint32_t end = test_arg_u32(argc, argv, "end", 0x77u);
    uint32_t found = 0;

    init_i2c(argc, argv);
    cli_printf(cli, "scan 0x%02lx..0x%02lx", (unsigned long)start, (unsigned long)end);

    for (uint32_t addr = start; addr <= end && addr < 0x80u; ++addr) {
        if (fe310_i2c_probe(&i2c0, (uint8_t)addr) == FE310_I2C_OK) {
            cli_printf(cli, "found 0x%02lx", (unsigned long)addr);
            ++found;
        }
    }

    cli_printf(cli, "devices=%lu", (unsigned long)found);
    return found == 0u ? test_fail() : test_pass();
}

int test_i2c_probe(EmbeddedCli *cli, int argc, char **argv)
{
    uint8_t addr = (uint8_t)test_arg_u32(argc, argv, "addr", 0x50u);
    init_i2c(argc, argv);

    fe310_i2c_status_t rc = fe310_i2c_probe(&i2c0, addr);
    cli_printf(cli, "probe addr=0x%02x rc=%d", addr, (int)rc);
    return rc == FE310_I2C_OK ? test_pass() : test_fail();
}
