#include "board/board.h"
#include "cli/cli_port.h"

int main(void)
{
    board_init();
    fe310_cli_init();

    board_write("\r\nFE310 RedBoard driver lab ready\r\n");
    board_write("Type \"help\" or \"test list\".\r\n");

    while (1) {
        fe310_cli_poll();
    }
}
