#ifndef FE310_DRIVER_AON_H
#define FE310_DRIVER_AON_H

#include <stdint.h>

void fe310_aon_unlock(void);
uint32_t fe310_aon_backup_read(uint8_t index);
void fe310_aon_backup_write(uint8_t index, uint32_t value);

#endif
