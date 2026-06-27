#ifndef FE310_DRIVER_CLINT_H
#define FE310_DRIVER_CLINT_H

#include <stdbool.h>
#include <stdint.h>

uint64_t fe310_clint_get_mtime(void);
void fe310_clint_set_mtimecmp(uint64_t value);
void fe310_clint_disable_mtimecmp(void);
void fe310_clint_set_msip(bool pending);

#endif
