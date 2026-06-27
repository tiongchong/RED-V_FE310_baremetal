#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/otp.h"

#include <stdio.h>

int test_otp_read(EmbeddedCli *cli, int argc, char **argv)
{
    uint32_t offset = test_arg_u32(argc, argv, "offset", 0u);
    uint32_t len = test_arg_u32(argc, argv, "len", 16u);

    if (len > 64u) {
        len = 64u;
    }

    uint8_t buf[64];
    fe310_otp_status_t rc = fe310_otp_read(offset, buf, len);
    if (rc != FE310_OTP_OK) {
        cli_printf(cli, "otp read rc=%d", (int)rc);
        return test_fail();
    }

    for (uint32_t i = 0; i < len; i += 16u) {
        char line[80];
        unsigned int pos = 0;
        pos += (unsigned int)snprintf(line + pos, sizeof(line) - pos, "%08lx:", (unsigned long)(offset + i));
        for (uint32_t j = 0; j < 16u && i + j < len; ++j) {
            pos += (unsigned int)snprintf(line + pos, sizeof(line) - pos, " %02x", buf[i + j]);
        }
        cli_print(cli, line);
    }

    return test_pass();
}
