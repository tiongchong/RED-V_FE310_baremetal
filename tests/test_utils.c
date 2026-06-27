#include "cli/test_utils.h"

#include <stdlib.h>
#include <string.h>

static bool split_key_value(const char *arg, const char *key, const char **value)
{
    size_t key_len = strlen(key);
    if (strncmp(arg, key, key_len) != 0 || arg[key_len] != '=') {
        return false;
    }
    *value = &arg[key_len + 1u];
    return true;
}

const char *test_arg_value(int argc, char **argv, const char *key)
{
    for (int i = 0; i < argc; ++i) {
        const char *value = 0;
        if (split_key_value(argv[i], key, &value)) {
            return value;
        }
    }
    return 0;
}

uint32_t test_arg_u32(int argc, char **argv, const char *key, uint32_t default_value)
{
    const char *value = test_arg_value(argc, argv, key);
    if (value == 0) {
        return default_value;
    }
    return (uint32_t)strtoul(value, 0, 0);
}

bool test_arg_bool(int argc, char **argv, const char *key, bool default_value)
{
    const char *value = test_arg_value(argc, argv, key);
    if (value == 0) {
        return default_value;
    }

    return strcmp(value, "1") == 0 ||
           strcmp(value, "true") == 0 ||
           strcmp(value, "yes") == 0 ||
           strcmp(value, "on") == 0;
}

int test_pass(void)
{
    return 0;
}

int test_fail(void)
{
    return -1;
}
