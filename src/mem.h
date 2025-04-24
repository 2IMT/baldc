#ifndef BALDC_MEM_H
#define BALDC_MEM_H

#include <stdlib.h>

#define BC_MALLOC(n)          malloc(n)
#define BC_CALLOC(n, size)    calloc(n, size)
#define BC_REALLOC(ptr, size) realloc(ptr, size)
#define BC_FREE(ptr)          free(ptr)

void bc_mem_check(void* ptr);

#endif
