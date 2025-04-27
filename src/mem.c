#include "mem.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "assert.h"

#define _ALIGN_UP(p, align) (((p) + (align) - 1) & ~((align) - 1))
#define _IS_POWER_OF_TWO(x) (((x) & ((x) - 1)) == 0)

static struct bc_mem_arena_block* _block_alloc(void) {
    struct bc_mem_arena_block* block =
        BC_MALLOC(sizeof(struct bc_mem_arena_block));
    bc_mem_check(block);
    return block;
}

void bc_mem_check(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "FATAL: BAD ALLOCATION\n");
        abort();
    }
}

struct bc_mem_arena_block bc_mem_arena_block_new(size_t block_size) {
    struct bc_mem_arena_block res = {
        .data = BC_MALLOC(block_size),
        .next = NULL,
        .size = block_size,
        .used = 0,
    };
    bc_mem_check(res.data);
    return res;
}

struct bc_mem_arena bc_mem_arena_new(size_t block_size) {
    struct bc_mem_arena_block* block = _block_alloc();
    *block = bc_mem_arena_block_new(block_size);
    struct bc_mem_arena res = {
        .first = block,
        .curr = block,
        .block_size = block_size,
    };
    return res;
}

void* bc_mem_arena_alloc(struct bc_mem_arena* arena, size_t size) {
    return bc_mem_arena_alloc_aligned(arena, size, 1);
}

void* bc_mem_arena_alloc_aligned(
    struct bc_mem_arena* arena, size_t size, size_t alignment) {
    BC_ASSERT(alignment != 0);
    BC_ASSERT(_IS_POWER_OF_TWO(alignment));
    size_t used = _ALIGN_UP(arena->curr->used, alignment);
    if (used > arena->block_size || (arena->block_size - used) < size) {
        struct bc_mem_arena_block* new_block = _block_alloc();
        size_t new_block_size = arena->block_size;
        while (new_block_size < size + alignment - 1) {
            new_block_size *= 2;
        }
        *new_block = bc_mem_arena_block_new(new_block_size);
        void* ptr = new_block->data;
        new_block->used = size;
        arena->curr->next = new_block;
        arena->curr = new_block;
        return ptr;
    }
    void* ptr = (char*)arena->curr->data + used;
    arena->curr->used = used + size;
    return ptr;
}

void* bc_mem_arena_calloc(struct bc_mem_arena* arena, size_t n, size_t size) {
    void* ptr = bc_mem_arena_alloc(arena, n * size);
    memset(ptr, 0, n * size);
    return ptr;
}

void* bc_mem_arena_calloc_aligned(
    struct bc_mem_arena* arena, size_t n, size_t size, size_t alignment) {
    void* ptr = bc_mem_arena_alloc_aligned(arena, n * size, alignment);
    memset(ptr, 0, n * size);
    return ptr;
}

void bc_mem_arena_free(struct bc_mem_arena arena) {
    struct bc_mem_arena_block* curr = arena.first;
    while (curr != NULL) {
        BC_FREE(curr->data);
        struct bc_mem_arena_block* next = curr->next;
        BC_FREE(curr);
        curr = next;
    }
}
