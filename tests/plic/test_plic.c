#include "cli/cli_print.h"
#include "cli/test_utils.h"
#include "drivers/plic.h"

int test_plic_basic(EmbeddedCli *cli, int argc, char **argv)
{
    uint32_t irq = test_arg_u32(argc, argv, "irq", 3u);
    uint32_t priority = test_arg_u32(argc, argv, "priority", 1u);

    fe310_plic_set_threshold(0u);
    fe310_plic_set_priority(irq, priority);
    fe310_plic_enable(irq);

    cli_printf(cli, "plic irq=%lu priority=%lu pending=%u",
               (unsigned long)irq,
               (unsigned long)priority,
               fe310_plic_is_pending(irq) ? 1u : 0u);

    fe310_plic_disable(irq);
    return test_pass();
}
