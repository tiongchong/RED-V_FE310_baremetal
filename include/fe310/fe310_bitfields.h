#ifndef FE310_BITFIELDS_H
#define FE310_BITFIELDS_H

#include <stdint.h>

#define BIT(n)                    (UINT32_C(1) << (n))

/* UART */
#define FE310_UART_TXDATA_FULL    BIT(31)
#define FE310_UART_RXDATA_EMPTY   BIT(31)
#define FE310_UART_TXCTRL_TXEN    BIT(0)
#define FE310_UART_TXCTRL_NSTOP   BIT(1)
#define FE310_UART_TXCTRL_TXCNT(v) (((uint32_t)(v) & 0x7u) << 16)
#define FE310_UART_RXCTRL_RXEN    BIT(0)
#define FE310_UART_RXCTRL_RXCNT(v) (((uint32_t)(v) & 0x7u) << 16)
#define FE310_UART_IE_TXWM        BIT(0)
#define FE310_UART_IE_RXWM        BIT(1)
#define FE310_UART_IP_TXWM        BIT(0)
#define FE310_UART_IP_RXWM        BIT(1)

/* SPI */
#define FE310_SPI_TXDATA_FULL     BIT(31)
#define FE310_SPI_RXDATA_EMPTY    BIT(31)
#define FE310_SPI_CSMODE_AUTO     0u
#define FE310_SPI_CSMODE_HOLD     2u
#define FE310_SPI_CSMODE_OFF      3u
#define FE310_SPI_FMT_PROTO_SINGLE 0u
#define FE310_SPI_FMT_ENDIAN_MSB  0u
#define FE310_SPI_FMT_ENDIAN_LSB  BIT(2)
#define FE310_SPI_FMT_DIR_RX      0u
#define FE310_SPI_FMT_DIR_TX      BIT(3)
#define FE310_SPI_FMT_LEN(v)      (((uint32_t)(v) & 0xFu) << 16)

/* I2C OpenCores-compatible controller */
#define FE310_I2C_CONTROL_EN      BIT(7)
#define FE310_I2C_CONTROL_IEN     BIT(6)
#define FE310_I2C_CMD_STA         BIT(7)
#define FE310_I2C_CMD_STO         BIT(6)
#define FE310_I2C_CMD_RD          BIT(5)
#define FE310_I2C_CMD_WR          BIT(4)
#define FE310_I2C_CMD_ACK         BIT(3)
#define FE310_I2C_CMD_IACK        BIT(0)
#define FE310_I2C_STATUS_RXACK    BIT(7)
#define FE310_I2C_STATUS_BUSY     BIT(6)
#define FE310_I2C_STATUS_AL       BIT(5)
#define FE310_I2C_STATUS_TIP      BIT(1)
#define FE310_I2C_STATUS_IF       BIT(0)

/* PWM */
#define FE310_PWM_CFG_SCALE(v)    ((uint32_t)(v) & 0xFu)
#define FE310_PWM_CFG_STICKY      BIT(8)
#define FE310_PWM_CFG_ZEROCMP     BIT(9)
#define FE310_PWM_CFG_DEGLITCH    BIT(10)
#define FE310_PWM_CFG_ENALWAYS    BIT(12)
#define FE310_PWM_CFG_ENONESHOT   BIT(13)
#define FE310_PWM_CFG_CMP_CENTER(ch) (BIT(16 + (ch)))
#define FE310_PWM_CFG_CMP_GANG(ch)   (BIT(24 + (ch)))
#define FE310_PWM_CFG_CMP_IP(ch)     (BIT(28 + (ch)))

/* PRCI */
#define FE310_PRCI_HFROSCCFG_DIV(v)  ((uint32_t)(v) & 0x3Fu)
#define FE310_PRCI_HFROSCCFG_TRIM(v) (((uint32_t)(v) & 0x1Fu) << 16)
#define FE310_PRCI_HFROSCCFG_EN      BIT(30)
#define FE310_PRCI_HFROSCCFG_READY   BIT(31)
#define FE310_PRCI_HFXOSCCFG_EN      BIT(30)
#define FE310_PRCI_HFXOSCCFG_READY   BIT(31)
#define FE310_PRCI_PLLCFG_R(v)       ((uint32_t)(v) & 0x7u)
#define FE310_PRCI_PLLCFG_F(v)       (((uint32_t)(v) & 0x3Fu) << 4)
#define FE310_PRCI_PLLCFG_Q(v)       (((uint32_t)(v) & 0x3u) << 10)
#define FE310_PRCI_PLLCFG_SEL        BIT(16)
#define FE310_PRCI_PLLCFG_REFSEL     BIT(17)
#define FE310_PRCI_PLLCFG_BYPASS     BIT(18)
#define FE310_PRCI_PLLCFG_LOCK       BIT(31)
#define FE310_PRCI_PLLOUTDIV_DIV(v)  ((uint32_t)(v) & 0x3Fu)
#define FE310_PRCI_PLLOUTDIV_BY1     BIT(8)

/* AON watchdog/RTC */
#define FE310_AON_WDOGKEY_VALUE      UINT32_C(0x51F15E)
#define FE310_AON_WDOGFEED_VALUE     UINT32_C(0xD09F00D)
#define FE310_AON_CFG_SCALE(v)       ((uint32_t)(v) & 0xFu)
#define FE310_AON_CFG_RSTEN          BIT(8)
#define FE310_AON_CFG_ZEROCMP        BIT(9)
#define FE310_AON_CFG_ENALWAYS       BIT(12)
#define FE310_AON_CFG_IP0            BIT(28)

#endif
