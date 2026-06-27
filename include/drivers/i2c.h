#ifndef FE310_DRIVER_I2C_H
#define FE310_DRIVER_I2C_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uintptr_t base;
    uint32_t input_hz;
} fe310_i2c_t;

typedef enum {
    FE310_I2C_OK = 0,
    FE310_I2C_ERR_NACK = -1,
    FE310_I2C_ERR_TIMEOUT = -2,
    FE310_I2C_ERR_ARB_LOST = -3,
    FE310_I2C_ERR_ARG = -4,
} fe310_i2c_status_t;

typedef struct {
    uint32_t bus_hz;
    bool irq_enable;
} fe310_i2c_config_t;

void fe310_i2c_init(const fe310_i2c_t *i2c, const fe310_i2c_config_t *cfg);
fe310_i2c_status_t fe310_i2c_probe(const fe310_i2c_t *i2c, uint8_t addr7);
fe310_i2c_status_t fe310_i2c_write(const fe310_i2c_t *i2c, uint8_t addr7, const uint8_t *data, size_t len);
fe310_i2c_status_t fe310_i2c_read(const fe310_i2c_t *i2c, uint8_t addr7, uint8_t *data, size_t len);
fe310_i2c_status_t fe310_i2c_write_read(const fe310_i2c_t *i2c, uint8_t addr7,
                                         const uint8_t *wr, size_t wr_len,
                                         uint8_t *rd, size_t rd_len);

#endif
