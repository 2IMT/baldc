#include "tools/test.h"

#include "mem.h"

TEST_BEGIN(test_arena_new) {
    size_t block_size = 1024;
    struct bc_mem_arena arena = bc_mem_arena_new(block_size);

    TEST_ASSERT(arena.block_size == block_size);
    TEST_ASSERT(arena.first != NULL && arena.curr != NULL);
    TEST_ASSERT(arena.first->size == block_size);
    TEST_ASSERT(arena.curr == arena.first);

    bc_mem_arena_free(arena);

    TEST_END;
}

TEST_BEGIN(test_arena_alloc) {
    size_t block_size = 256;
    struct bc_mem_arena arena = bc_mem_arena_new(block_size);

    char* ptr1 = bc_mem_arena_alloc(&arena, 128);
    TEST_ASSERT(arena.first->used == 128);

    char* ptr2 = bc_mem_arena_alloc(&arena, 64);
    TEST_ASSERT(arena.first->used == 128 + 64);

    TEST_ASSERT((char*)ptr1 + 128 == (char*)ptr2);

    bc_mem_arena_alloc(&arena, 65);
    TEST_ASSERT(arena.first != arena.curr);
    TEST_ASSERT(arena.first->used == 128 + 64);
    TEST_ASSERT(arena.curr->used == 65);

    bc_mem_arena_alloc(&arena, 257);
    TEST_ASSERT(arena.curr->size == 256 * 2);
    TEST_ASSERT(arena.curr->used == 257);

    bc_mem_arena_alloc(&arena, 200);
    TEST_ASSERT(arena.curr->used = 257 + 200);

    bc_mem_arena_free(arena);

    block_size = 1024;
    arena = bc_mem_arena_new(block_size);

    bc_mem_arena_alloc(&arena, block_size);
    bc_mem_arena_alloc(&arena, block_size);
    TEST_ASSERT(arena.first != arena.curr);
    TEST_ASSERT(arena.first->next == arena.curr);
    TEST_ASSERT(arena.curr->next == NULL);
    TEST_ASSERT(arena.first->used == block_size);
    TEST_ASSERT(arena.curr->used == block_size);

    bc_mem_arena_free(arena);

    TEST_END;
}

TESTS(test_arena_new, test_arena_alloc)
