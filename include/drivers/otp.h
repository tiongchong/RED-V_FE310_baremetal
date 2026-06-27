#ifndef FE310_DRIVER_OTP_H
#define FE310_DRIVER_OTP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    FE310_OTP_OK = 0,
    FE310_OTP_ERR_RANGE = -1,
    FE310_OTP_ERR_LOCKED = -2,
    FE310_OTP_ERR_UNSUPPORTED = -3,
} fe310_otp_status_t;

fe310_otp_status_t fe310_otp_read(uint32_t offset, void *buf, size_t len);
uint8_t fe310_otp_read8(uint32_t offset);
bool fe310_otp_range_valid(uint32_t offset, size_t len);
fe310_otp_status_t fe310_otp_program_guarded(uint32_t offset, uint8_t value, const char *confirm);

#endif
