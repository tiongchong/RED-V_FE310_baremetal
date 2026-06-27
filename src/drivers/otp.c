#include "drivers/otp.h"

#include <string.h>

#include "fe310/fe310_memory_map.h"

bool fe310_otp_range_valid(uint32_t offset, size_t len)
{
    return offset <= FE310_OTP_MEM_SIZE && len <= (size_t)(FE310_OTP_MEM_SIZE - offset);
}

uint8_t fe310_otp_read8(uint32_t offset)
{
    if (!fe310_otp_range_valid(offset, 1u)) {
        return 0xffu;
    }
    return *(volatile const uint8_t *)((uintptr_t)FE310_OTP_MEM_BASE + offset);
}

fe310_otp_status_t fe310_otp_read(uint32_t offset, void *buf, size_t len)
{
    if (buf == 0 || !fe310_otp_range_valid(offset, len)) {
        return FE310_OTP_ERR_RANGE;
    }

    uint8_t *out = (uint8_t *)buf;
    for (size_t i = 0; i < len; ++i) {
        out[i] = fe310_otp_read8(offset + (uint32_t)i);
    }
    return FE310_OTP_OK;
}

fe310_otp_status_t fe310_otp_program_guarded(uint32_t offset, uint8_t value, const char *confirm)
{
    (void)offset;
    (void)value;
    (void)confirm;

#if defined(FE310_ENABLE_OTP_PROGRAM_TEST) && FE310_ENABLE_OTP_PROGRAM_TEST
    if (confirm == 0 || strcmp(confirm, "YES_BURN_OTP") != 0) {
        return FE310_OTP_ERR_LOCKED;
    }
    return FE310_OTP_ERR_UNSUPPORTED;
#else
    return FE310_OTP_ERR_LOCKED;
#endif
}
