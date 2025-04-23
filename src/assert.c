#include "assert.h"

#ifndef BALDC_RELEASE

#  include <stdio.h>
#  include <stdlib.h>

void bc_assert_fail(const char* file, int line, const char* assertion) {
    fprintf(stderr, "%s:%d: assertion `%s` failed\n", file, line, assertion);
    abort();
}

#endif
