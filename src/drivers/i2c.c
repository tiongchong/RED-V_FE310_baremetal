#include "drivers/i2c.h"

#include "fe310/fe310_bitfields.h"
#include "fe310/fe310_regs.h"
#include "fe310/mmio.h"

#define I2C_TIMEOUT 100000u

static uint16_t prescale(uint32_t input_hz, uint32_t bus_hz)
{
    if (bus_hz == 0u) {
        return 0u;
    }

    uint32_t value = (input_hz / (5u * bus_hz));
    value = value == 0u ? 0u : value - 1u;
    if (value > 0xffffu) {
        value = 0xffffu;
    }
    return (uint16_t)value;
}

static fe310_i2c_status_t wait_tip_clear(const fe310_i2c_t *i2c)
{
    for (uint32_t i = 0; i < I2C_TIMEOUT; ++i) {
        uint32_t status = fe310_read32(i2c->base + FE310_I2C_CMD_STATUS);
        if ((status & FE310_I2C_STATUS_AL) != 0u) {
            return FE310_I2C_ERR_ARB_LOST;
        }
        if ((status & FE310_I2C_STATUS_TIP) == 0u) {
            return FE310_I2C_OK;
        }
    }

    return FE310_I2C_ERR_TIMEOUT;
}

static fe310_i2c_status_t write_byte(const fe310_i2c_t *i2c, uint8_t byte, uint32_t cmd)
{
    fe310_write32(i2c->base + FE310_I2C_TXRX, byte);
    fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, cmd | FE310_I2C_CMD_WR);

    fe310_i2c_status_t rc = wait_tip_clear(i2c);
    if (rc != FE310_I2C_OK) {
        return rc;
    }

    return (fe310_read32(i2c->base + FE310_I2C_CMD_STATUS) & FE310_I2C_STATUS_RXACK) != 0u
        ? FE310_I2C_ERR_NACK
        : FE310_I2C_OK;
}

static fe310_i2c_status_t read_byte(const fe310_i2c_t *i2c, uint8_t *byte, bool last)
{
    uint32_t cmd = FE310_I2C_CMD_RD;
    if (last) {
        cmd |= FE310_I2C_CMD_ACK | FE310_I2C_CMD_STO;
    }

    fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, cmd);
    fe310_i2c_status_t rc = wait_tip_clear(i2c);
    if (rc != FE310_I2C_OK) {
        return rc;
    }

    *byte = (uint8_t)fe310_read32(i2c->base + FE310_I2C_TXRX);
    return FE310_I2C_OK;
}

void fe310_i2c_init(const fe310_i2c_t *i2c, const fe310_i2c_config_t *cfg)
{
    uint16_t p = prescale(i2c->input_hz, cfg->bus_hz);
    fe310_write32(i2c->base + FE310_I2C_CONTROL, 0u);
    fe310_write32(i2c->base + FE310_I2C_PRESCALE_LO, p & 0xffu);
    fe310_write32(i2c->base + FE310_I2C_PRESCALE_HI, (p >> 8) & 0xffu);
    fe310_write32(i2c->base + FE310_I2C_CONTROL,
                  FE310_I2C_CONTROL_EN | (cfg->irq_enable ? FE310_I2C_CONTROL_IEN : 0u));
}

fe310_i2c_status_t fe310_i2c_probe(const fe310_i2c_t *i2c, uint8_t addr7)
{
    fe310_i2c_status_t rc = write_byte(i2c, (uint8_t)(addr7 << 1), FE310_I2C_CMD_STA | FE310_I2C_CMD_STO);
    return rc;
}

fe310_i2c_status_t fe310_i2c_write(const fe310_i2c_t *i2c, uint8_t addr7, const uint8_t *data, size_t len)
{
    if (data == 0 && len != 0u) {
        return FE310_I2C_ERR_ARG;
    }

    fe310_i2c_status_t rc = write_byte(i2c, (uint8_t)(addr7 << 1), FE310_I2C_CMD_STA);
    if (rc != FE310_I2C_OK) {
        fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
        return rc;
    }

    for (size_t i = 0; i < len; ++i) {
        uint32_t cmd = (i + 1u == len) ? FE310_I2C_CMD_STO : 0u;
        rc = write_byte(i2c, data[i], cmd);
        if (rc != FE310_I2C_OK) {
            fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
            return rc;
        }
    }

    if (len == 0u) {
        fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
    }

    return FE310_I2C_OK;
}

fe310_i2c_status_t fe310_i2c_read(const fe310_i2c_t *i2c, uint8_t addr7, uint8_t *data, size_t len)
{
    if (data == 0 || len == 0u) {
        return FE310_I2C_ERR_ARG;
    }

    fe310_i2c_status_t rc = write_byte(i2c, (uint8_t)((addr7 << 1) | 1u), FE310_I2C_CMD_STA);
    if (rc != FE310_I2C_OK) {
        fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
        return rc;
    }

    for (size_t i = 0; i < len; ++i) {
        rc = read_byte(i2c, &data[i], i + 1u == len);
        if (rc != FE310_I2C_OK) {
            fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
            return rc;
        }
    }

    return FE310_I2C_OK;
}

fe310_i2c_status_t fe310_i2c_write_read(const fe310_i2c_t *i2c, uint8_t addr7,
                                         const uint8_t *wr, size_t wr_len,
                                         uint8_t *rd, size_t rd_len)
{
    if ((wr == 0 && wr_len != 0u) || (rd == 0 && rd_len != 0u)) {
        return FE310_I2C_ERR_ARG;
    }

    fe310_i2c_status_t rc = write_byte(i2c, (uint8_t)(addr7 << 1), FE310_I2C_CMD_STA);
    if (rc != FE310_I2C_OK) {
        fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
        return rc;
    }

    for (size_t i = 0; i < wr_len; ++i) {
        rc = write_byte(i2c, wr[i], 0u);
        if (rc != FE310_I2C_OK) {
            fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
            return rc;
        }
    }

    if (rd_len == 0u) {
        fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
        return FE310_I2C_OK;
    }

    rc = write_byte(i2c, (uint8_t)((addr7 << 1) | 1u), FE310_I2C_CMD_STA);
    if (rc != FE310_I2C_OK) {
        fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
        return rc;
    }

    for (size_t i = 0; i < rd_len; ++i) {
        rc = read_byte(i2c, &rd[i], i + 1u == rd_len);
        if (rc != FE310_I2C_OK) {
            fe310_write32(i2c->base + FE310_I2C_CMD_STATUS, FE310_I2C_CMD_STO);
            return rc;
        }
    }

    return FE310_I2C_OK;
}
