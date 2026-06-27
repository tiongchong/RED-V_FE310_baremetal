#include "board/board.h"
#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/spi.h"

void board_configure_spi1_pins(void);

static const fe310_spi_t spi1 = {
    .base = FE310_SPI1_BASE,
    .input_hz = BOARD_PERIPH_HZ,
};

static void init_spi1(int argc, char **argv)
{
    fe310_spi_config_t cfg = {
        .speed_hz = test_arg_u32(argc, argv, "hz", BOARD_DEFAULT_SPI_HZ),
        .mode = FE310_SPI_MODE0,
        .cs_id = (uint8_t)test_arg_u32(argc, argv, "cs", 0u),
        .cs_active_low = true,
    };
    board_configure_spi1_pins();
    fe310_spi_init(&spi1, &cfg);
}

int test_spi_loopback(EmbeddedCli *cli, int argc, char **argv)
{
    uint8_t pattern[4] = { 0x55u, 0xaau, 0x00u, 0xffu };
    uint8_t rx[4] = { 0u, 0u, 0u, 0u };

    init_spi1(argc, argv);
    cli_print(cli, "wire SPI1 MOSI to MISO before running this test");
    fe310_spi_select(&spi1);
    fe310_spi_transfer(&spi1, pattern, rx, sizeof(pattern));
    fe310_spi_deselect(&spi1);

    cli_printf(cli, "rx %02x %02x %02x %02x", rx[0], rx[1], rx[2], rx[3]);
    for (unsigned int i = 0; i < sizeof(pattern); ++i) {
        if (rx[i] != pattern[i]) {
            return test_fail();
        }
    }
    return test_pass();
}

int test_spi_flash_id(EmbeddedCli *cli, int argc, char **argv)
{
    uint8_t id[3] = { 0u, 0u, 0u };

    init_spi1(argc, argv);
    cli_print(cli, "expects external SPI flash on SPI1 CS0");

    if (!fe310_spi_read_jedec_id(&spi1, id)) {
        cli_printf(cli, "jedec id invalid: %02x %02x %02x", id[0], id[1], id[2]);
        return test_fail();
    }

    cli_printf(cli, "jedec id: %02x %02x %02x", id[0], id[1], id[2]);
    return test_pass();
}
