#ifndef FE310_DRIVER_SPI_H
#define FE310_DRIVER_SPI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uintptr_t base;
    uint32_t input_hz;
} fe310_spi_t;

typedef enum {
    FE310_SPI_MODE0 = 0,
    FE310_SPI_MODE1 = 1,
    FE310_SPI_MODE2 = 2,
    FE310_SPI_MODE3 = 3,
} fe310_spi_mode_t;

typedef struct {
    uint32_t speed_hz;
    fe310_spi_mode_t mode;
    uint8_t cs_id;
    bool cs_active_low;
} fe310_spi_config_t;

void fe310_spi_init(const fe310_spi_t *spi, const fe310_spi_config_t *cfg);
void fe310_spi_set_speed(const fe310_spi_t *spi, uint32_t speed_hz);
void fe310_spi_select(const fe310_spi_t *spi);
void fe310_spi_deselect(const fe310_spi_t *spi);
uint8_t fe310_spi_transfer_byte(const fe310_spi_t *spi, uint8_t tx);
void fe310_spi_transfer(const fe310_spi_t *spi, const uint8_t *tx, uint8_t *rx, size_t len);
bool fe310_spi_read_jedec_id(const fe310_spi_t *spi, uint8_t id[3]);

#endif
