#ifndef FE310_TEST_UTILS_H
#define FE310_TEST_UTILS_H

#include <stdbool.h>
#include <stdint.h>

const char *test_arg_value(int argc, char **argv, const char *key);
uint32_t test_arg_u32(int argc, char **argv, const char *key, uint32_t default_value);
bool test_arg_bool(int argc, char **argv, const char *key, bool default_value);
int test_pass(void);
int test_fail(void);

#endif
