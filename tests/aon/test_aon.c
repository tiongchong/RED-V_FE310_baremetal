#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/aon.h"

int test_aon_backup(EmbeddedCli *cli, int argc, char **argv)
{
    uint8_t index = (uint8_t)test_arg_u32(argc, argv, "index", 0u);
    uint32_t value = test_arg_u32(argc, argv, "value", 0x5aa55aa5u);

    fe310_aon_backup_write(index, value);
    uint32_t readback = fe310_aon_backup_read(index);
    cli_printf(cli, "backup%u wrote=0x%08lx read=0x%08lx",
               index,
               (unsigned long)value,
               (unsigned long)readback);
    return readback == value ? test_pass() : test_fail();
}
