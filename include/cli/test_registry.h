#ifndef FE310_TEST_REGISTRY_H
#define FE310_TEST_REGISTRY_H

#include <stddef.h>

#include "embedded_cli.h"

typedef int (*fe310_test_fn_t)(EmbeddedCli *cli, int argc, char **argv);

typedef struct {
    const char *name;
    const char *help;
    fe310_test_fn_t run;
} fe310_test_case_t;

const fe310_test_case_t *fe310_test_registry(size_t *count);
const fe310_test_case_t *fe310_test_find(const char *name);

#endif
