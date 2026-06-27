#include "cli/cli_print.h"

#include <stdarg.h>
#include <stdio.h>

void cli_print(EmbeddedCli *cli, const char *s)
{
    embeddedCliPrint(cli, s);
}

void cli_printf(EmbeddedCli *cli, const char *fmt, ...)
{
    char buf[192];
    va_list ap;

    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    if (n < 0) {
        embeddedCliPrint(cli, "ERR format");
        return;
    }

    buf[sizeof(buf) - 1u] = '\0';
    embeddedCliPrint(cli, buf);
}

void cli_print_hex8(EmbeddedCli *cli, uint8_t value)
{
    cli_printf(cli, "0x%02x", value);
}

void cli_print_hex32(EmbeddedCli *cli, uint32_t value)
{
    cli_printf(cli, "0x%08lx", (unsigned long)value);
}
