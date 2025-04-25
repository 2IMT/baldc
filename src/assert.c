#include "assert.h"

#ifdef BALDC_RELEASE

// To avoid empty translation unit on release mode
typedef char _bc_assert_dummy;

#else

#  include <stdio.h>
#  include <stdlib.h>

void bc_assert_fail(const char* file, int line, const char* assertion) {
    fprintf(stderr, "%s:%d: ASSERTION `%s' FAILED\n", file, line, assertion);
    abort();
}

#endif
