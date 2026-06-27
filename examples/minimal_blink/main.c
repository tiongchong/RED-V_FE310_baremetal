#include "board/board.h"
#include "drivers/gpio.h"
#include "drivers/timer.h"

int main(void)
{
    board_init();
    fe310_gpio_set_mode(&board_gpio0, BOARD_USER_LED_GPIO, FE310_GPIO_MODE_OUTPUT);

    while (1) {
        fe310_gpio_toggle(&board_gpio0, BOARD_USER_LED_GPIO);
        fe310_timer_delay_ms(250, BOARD_RTC_HZ);
    }
}
