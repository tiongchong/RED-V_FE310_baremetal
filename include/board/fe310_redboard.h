#ifndef BOARD_FE310_REDBOARD_H
#define BOARD_FE310_REDBOARD_H

#include <stdint.h>

#include "fe310/fe310_memory_map.h"

#ifndef BOARD_CPU_HZ
#define BOARD_CPU_HZ 16000000UL
#endif

#ifndef BOARD_UART_BAUD
#define BOARD_UART_BAUD 115200UL
#endif

#define BOARD_NAME                  "FE310 RedBoard"
#define BOARD_CORE_HZ               ((uint32_t)BOARD_CPU_HZ)
#define BOARD_PERIPH_HZ             ((uint32_t)BOARD_CPU_HZ)
#define BOARD_RTC_HZ                32768UL

#define BOARD_CLI_UART_BASE         FE310_UART0_BASE
#define BOARD_CLI_UART_BAUD         ((uint32_t)BOARD_UART_BAUD)

#define BOARD_GPIO_BASE             FE310_GPIO0_BASE
#define BOARD_USER_LED_GPIO         5u

/* Verify these against your RED-V/FE310 RedBoard revision schematic. */
#define BOARD_I2C0_SDA_GPIO         12u
#define BOARD_I2C0_SCL_GPIO         13u
#define BOARD_SPI1_SCK_GPIO         3u
#define BOARD_SPI1_MISO_GPIO        4u
#define BOARD_SPI1_MOSI_GPIO        2u
#define BOARD_SPI1_CS0_GPIO         5u

#define BOARD_DEFAULT_I2C_HZ        100000u
#define BOARD_DEFAULT_SPI_HZ        1000000u
#define BOARD_DEFAULT_PWM_HZ        1000u

#endif
