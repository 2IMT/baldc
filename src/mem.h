#ifndef BALDC_MEM_H
#define BALDC_MEM_H

#include <stdlib.h>

#define BC_MALLOC(n)          malloc(n)
#define BC_CALLOC(n, size)    calloc(n, size)
#define BC_REALLOC(ptr, size) realloc(ptr, size)
#define BC_FREE(ptr)          free(ptr)

void bc_mem_check(void* ptr);

struct bc_mem_arena_block {
    void* data;
    struct bc_mem_arena_block* next;
    size_t size;
    size_t used;
};

struct bc_mem_arena {
    struct bc_mem_arena_block* first;
    struct bc_mem_arena_block* curr;
    size_t block_size;
};

struct bc_mem_arena_block bc_mem_arena_block_new(size_t block_size);

struct bc_mem_arena bc_mem_arena_new(size_t block_size);

void* bc_mem_arena_alloc(struct bc_mem_arena* arena, size_t size);

void* bc_mem_arena_alloc_aligned(
    struct bc_mem_arena* arena, size_t size, size_t alignment);

#define BC_MEM_ARENA_ALLOC_TYPE(arena, ty) \
    ((ty*)bc_mem_arena_alloc_aligned((arena), sizeof(ty), _Alignof(ty)))

void* bc_mem_arena_calloc(struct bc_mem_arena* arena, size_t n, size_t size);

void* bc_mem_arena_calloc_aligned(
    struct bc_mem_arena* arena, size_t n, size_t size, size_t alignment);

#define BC_MEM_ARENA_CALLOC_TYPE(arena, ty, n) \
    ((ty*)bc_mem_arena_calloc_aligned((arena), n, sizeof(ty), _Alignof(ty)))

void bc_mem_arena_free(struct bc_mem_arena arena);

#endif
