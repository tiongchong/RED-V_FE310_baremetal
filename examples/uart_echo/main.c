#include "board/board.h"
#include "drivers/uart.h"

int main(void)
{
    board_init();
    board_write("UART echo ready\r\n");

    while (1) {
        uint8_t c;
        if (fe310_uart_read_byte(&board_cli_uart, &c)) {
            fe310_uart_write_byte(&board_cli_uart, c);
        }
    }
}
