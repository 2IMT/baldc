#include "mem.h"

#include <stdio.h>

void bc_mem_check(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "FATAL: BAD ALLOCATION\n");
        abort();
    }
}
