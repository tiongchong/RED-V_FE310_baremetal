#ifndef FE310_CLI_PORT_H
#define FE310_CLI_PORT_H

#include "embedded_cli.h"

void fe310_cli_init(void);
void fe310_cli_poll(void);
EmbeddedCli *fe310_cli_instance(void);

#endif
