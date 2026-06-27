#include "board/board.h"

void board_configure_i2c0_pins(void)
{
    fe310_gpio_enable_iof(&board_gpio0, BOARD_I2C0_SDA_GPIO, 0);
    fe310_gpio_enable_iof(&board_gpio0, BOARD_I2C0_SCL_GPIO, 0);
}

void board_configure_spi1_pins(void)
{
    fe310_gpio_enable_iof(&board_gpio0, BOARD_SPI1_SCK_GPIO, 0);
    fe310_gpio_enable_iof(&board_gpio0, BOARD_SPI1_MISO_GPIO, 0);
    fe310_gpio_enable_iof(&board_gpio0, BOARD_SPI1_MOSI_GPIO, 0);
    fe310_gpio_enable_iof(&board_gpio0, BOARD_SPI1_CS0_GPIO, 0);
}
