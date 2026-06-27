#include "cli/cli_port.h"

#include "board/board.h"
#include "cli/cli_commands.h"
#include "drivers/uart.h"

#define CLI_BUFFER_BYTES 4096u

static CLI_UINT cli_buffer[BYTES_TO_CLI_UINTS(CLI_BUFFER_BYTES)];
static EmbeddedCli *cli_instance;

static void cli_write_char(EmbeddedCli *cli, char c)
{
    (void)cli;
    fe310_uart_write_byte(&board_cli_uart, (uint8_t)c);
}

static void cli_unknown_command(EmbeddedCli *cli, CliCommand *command)
{
    (void)command;
    embeddedCliPrint(cli, "ERR unknown command; try help");
}

void fe310_cli_init(void)
{
    EmbeddedCliConfig *config = embeddedCliDefaultConfig();
    config->invitation = "FE310> ";
    config->rxBufferSize = 128;
    config->cmdBufferSize = 160;
    config->historyBufferSize = 256;
    config->maxBindingCount = 24;
    config->enableAutoComplete = true;
    config->cliBuffer = cli_buffer;
    config->cliBufferSize = CLI_BUFFER_BYTES;

    cli_instance = embeddedCliNew(config);
    if (cli_instance == 0) {
        board_write("embedded-cli allocation failed\r\n");
        while (1) {
        }
    }

    cli_instance->writeChar = cli_write_char;
    cli_instance->onCommand = cli_unknown_command;
    fe310_cli_register_commands(cli_instance);
    embeddedCliProcess(cli_instance);
}

void fe310_cli_poll(void)
{
    uint8_t c;
    while (fe310_uart_read_byte(&board_cli_uart, &c)) {
        embeddedCliReceiveChar(cli_instance, (char)c);
    }
    embeddedCliProcess(cli_instance);
}

EmbeddedCli *fe310_cli_instance(void)
{
    return cli_instance;
}
