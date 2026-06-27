#include "drivers/spi.h"

#include "fe310/fe310_bitfields.h"
#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

static uint32_t spi_sckdiv(uint32_t input_hz, uint32_t speed_hz)
{
    if (speed_hz == 0u) {
        return 0u;
    }

    uint32_t div = input_hz / (2u * speed_hz);
    return (div == 0u) ? 0u : (div - 1u);
}

void fe310_spi_set_speed(const fe310_spi_t *spi, uint32_t speed_hz)
{
    fe310_write32(spi->base + FE310_SPI_SCKDIV, spi_sckdiv(spi->input_hz, speed_hz));
}

void fe310_spi_init(const fe310_spi_t *spi, const fe310_spi_config_t *cfg)
{
    fe310_spi_set_speed(spi, cfg->speed_hz);
    fe310_write32(spi->base + FE310_SPI_SCKMODE, (uint32_t)cfg->mode);
    fe310_write32(spi->base + FE310_SPI_CSID, cfg->cs_id);
    fe310_write32(spi->base + FE310_SPI_CSDEF, cfg->cs_active_low ? UINT32_MAX : 0u);
    fe310_write32(spi->base + FE310_SPI_CSMODE, FE310_SPI_CSMODE_AUTO);
    fe310_write32(spi->base + FE310_SPI_FMT,
                  FE310_SPI_FMT_PROTO_SINGLE |
                  FE310_SPI_FMT_ENDIAN_MSB |
                  FE310_SPI_FMT_LEN(8));
    fe310_write32(spi->base + FE310_SPI_TXMARK, 1u);
    fe310_write32(spi->base + FE310_SPI_RXMARK, 0u);
}

void fe310_spi_select(const fe310_spi_t *spi)
{
    fe310_write32(spi->base + FE310_SPI_CSMODE, FE310_SPI_CSMODE_HOLD);
}

void fe310_spi_deselect(const fe310_spi_t *spi)
{
    fe310_write32(spi->base + FE310_SPI_CSMODE, FE310_SPI_CSMODE_AUTO);
}

uint8_t fe310_spi_transfer_byte(const fe310_spi_t *spi, uint8_t tx)
{
    while ((fe310_read32(spi->base + FE310_SPI_TXDATA) & FE310_SPI_TXDATA_FULL) != 0u) {
    }
    fe310_write32(spi->base + FE310_SPI_TXDATA, tx);

    uint32_t rx;
    do {
        rx = fe310_read32(spi->base + FE310_SPI_RXDATA);
    } while ((rx & FE310_SPI_RXDATA_EMPTY) != 0u);

    return (uint8_t)(rx & 0xffu);
}

void fe310_spi_transfer(const fe310_spi_t *spi, const uint8_t *tx, uint8_t *rx, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        uint8_t out = tx == 0 ? 0xffu : tx[i];
        uint8_t in = fe310_spi_transfer_byte(spi, out);
        if (rx != 0) {
            rx[i] = in;
        }
    }
}

bool fe310_spi_read_jedec_id(const fe310_spi_t *spi, uint8_t id[3])
{
    uint8_t cmd = 0x9fu;
    fe310_spi_select(spi);
    (void)fe310_spi_transfer_byte(spi, cmd);
    id[0] = fe310_spi_transfer_byte(spi, 0xffu);
    id[1] = fe310_spi_transfer_byte(spi, 0xffu);
    id[2] = fe310_spi_transfer_byte(spi, 0xffu);
    fe310_spi_deselect(spi);

    return id[0] != 0x00u && id[0] != 0xffu;
}
