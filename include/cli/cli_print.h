#ifndef FE310_CLI_PRINT_H
#define FE310_CLI_PRINT_H

#include <stdint.h>

#include "embedded_cli.h"

void cli_print(EmbeddedCli *cli, const char *s);
void cli_printf(EmbeddedCli *cli, const char *fmt, ...);
void cli_print_hex8(EmbeddedCli *cli, uint8_t value);
void cli_print_hex32(EmbeddedCli *cli, uint32_t value);

#endif
