#ifndef FE310_REGS_H
#define FE310_REGS_H

#include "fe310/fe310_memory_map.h"

/* GPIO */
#define FE310_GPIO_INPUT_VAL      0x00u
#define FE310_GPIO_INPUT_EN       0x04u
#define FE310_GPIO_OUTPUT_EN      0x08u
#define FE310_GPIO_OUTPUT_VAL     0x0Cu
#define FE310_GPIO_PUE            0x10u
#define FE310_GPIO_DS             0x14u
#define FE310_GPIO_RISE_IE        0x18u
#define FE310_GPIO_RISE_IP        0x1Cu
#define FE310_GPIO_FALL_IE        0x20u
#define FE310_GPIO_FALL_IP        0x24u
#define FE310_GPIO_HIGH_IE        0x28u
#define FE310_GPIO_HIGH_IP        0x2Cu
#define FE310_GPIO_LOW_IE         0x30u
#define FE310_GPIO_LOW_IP         0x34u
#define FE310_GPIO_IOF_EN         0x38u
#define FE310_GPIO_IOF_SEL        0x3Cu
#define FE310_GPIO_OUT_XOR        0x40u

/* UART */
#define FE310_UART_TXDATA         0x00u
#define FE310_UART_RXDATA         0x04u
#define FE310_UART_TXCTRL         0x08u
#define FE310_UART_RXCTRL         0x0Cu
#define FE310_UART_IE             0x10u
#define FE310_UART_IP             0x14u
#define FE310_UART_DIV            0x18u

/* SPI/QSPI */
#define FE310_SPI_SCKDIV          0x00u
#define FE310_SPI_SCKMODE         0x04u
#define FE310_SPI_CSID            0x10u
#define FE310_SPI_CSDEF           0x14u
#define FE310_SPI_CSMODE          0x18u
#define FE310_SPI_DELAY0          0x28u
#define FE310_SPI_DELAY1          0x2Cu
#define FE310_SPI_FMT             0x40u
#define FE310_SPI_TXDATA          0x48u
#define FE310_SPI_RXDATA          0x4Cu
#define FE310_SPI_TXMARK          0x50u
#define FE310_SPI_RXMARK          0x54u
#define FE310_SPI_FCTRL           0x60u
#define FE310_SPI_FFMT            0x64u
#define FE310_SPI_IE              0x70u
#define FE310_SPI_IP              0x74u

/* I2C */
#define FE310_I2C_PRESCALE_LO     0x00u
#define FE310_I2C_PRESCALE_HI     0x04u
#define FE310_I2C_CONTROL         0x08u
#define FE310_I2C_TXRX            0x0Cu
#define FE310_I2C_CMD_STATUS      0x10u

/* PWM */
#define FE310_PWM_CFG             0x00u
#define FE310_PWM_COUNT           0x08u
#define FE310_PWM_S               0x10u
#define FE310_PWM_CMP0            0x20u
#define FE310_PWM_CMP1            0x24u
#define FE310_PWM_CMP2            0x28u
#define FE310_PWM_CMP3            0x2Cu

/* CLINT */
#define FE310_CLINT_MSIP          0x0000u
#define FE310_CLINT_MTIMECMP      0x4000u
#define FE310_CLINT_MTIME         0xBFF8u

/* PLIC */
#define FE310_PLIC_PRIORITY       0x000000u
#define FE310_PLIC_PENDING        0x001000u
#define FE310_PLIC_ENABLE         0x002000u
#define FE310_PLIC_CONTEXT        0x200000u
#define FE310_PLIC_THRESHOLD      0x200000u
#define FE310_PLIC_CLAIM          0x200004u

/* PRCI */
#define FE310_PRCI_HFROSCCFG      0x00u
#define FE310_PRCI_HFXOSCCFG      0x04u
#define FE310_PRCI_PLLCFG         0x08u
#define FE310_PRCI_PLLOUTDIV      0x0Cu
#define FE310_PRCI_PROCMONCFG     0xF0u

/* AON watchdog/RTC subset */
#define FE310_AON_WDOGCFG         0x000u
#define FE310_AON_WDOGCOUNT       0x008u
#define FE310_AON_WDOGS           0x010u
#define FE310_AON_WDOGFEED        0x018u
#define FE310_AON_WDOGKEY         0x01Cu
#define FE310_AON_WDOGCMP0        0x020u
#define FE310_AON_RTCCFG          0x040u
#define FE310_AON_RTCLO           0x048u
#define FE310_AON_RTCHI           0x04Cu
#define FE310_AON_RTCS            0x050u
#define FE310_AON_RTCCMP0         0x060u
#define FE310_AON_LFROSCCFG       0x070u
#define FE310_AON_BACKUP0         0x080u
#define FE310_AON_PMUWAKEUPI0     0x100u
#define FE310_AON_PMUKEY          0x14Cu
#define FE310_AON_PMUIE           0x150u
#define FE310_AON_PMUCAUSE        0x154u
#define FE310_AON_PMUSLEEP        0x158u

#endif
