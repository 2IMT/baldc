#include "tools/test.h"

#include <stdint.h>
#include <stddef.h>

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

TEST_BEGIN(test_arena_alloc_aligned) {
    size_t block_size = 256;
    struct bc_mem_arena arena = bc_mem_arena_new(block_size);

    char* ptr1 = bc_mem_arena_alloc_aligned(&arena, 7, 16);
    TEST_ASSERT(arena.first->data == ptr1);
    TEST_ASSERT(arena.first->used == 7);

    char* ptr2 = bc_mem_arena_alloc_aligned(&arena, 7, 16);
    TEST_ASSERT((char*)arena.first->data + 16 == ptr2);
    TEST_ASSERT(arena.first->used == 16 + 7);

    char* ptr3 = bc_mem_arena_alloc_aligned(&arena, 10, 4);
    TEST_ASSERT((char*)arena.first->data + 16 + 8 == ptr3);
    TEST_ASSERT(arena.first->used == 16 + 8 + 10);

    char* ptr4 =
        bc_mem_arena_alloc_aligned(&arena, block_size - 16 - 8 - 10, 16);
    TEST_ASSERT(arena.first != arena.curr);
    TEST_ASSERT(arena.first->next != NULL);
    TEST_ASSERT(arena.first->next == arena.curr);
    TEST_ASSERT(arena.curr->data == ptr4);
    TEST_ASSERT(arena.curr->used == block_size - 16 - 8 - 10);

    char* ptr5 = bc_mem_arena_alloc_aligned(&arena, 384, 64);
    TEST_ASSERT(arena.first->next != NULL);
    TEST_ASSERT(arena.first->next != arena.curr);
    TEST_ASSERT(arena.first->next->next != NULL);
    TEST_ASSERT(arena.first->next->next == arena.curr);
    TEST_ASSERT(arena.curr->data == ptr5);
    TEST_ASSERT(arena.curr->size == 512);
    TEST_ASSERT(arena.curr->used == 384);

    bc_mem_arena_free(arena);

    TEST_END;
}

TEST_BEGIN(test_arena_calloc) {
    size_t block_size = 64 * sizeof(int32_t);
    struct bc_mem_arena arena = bc_mem_arena_new(block_size);

    int32_t* ptr = bc_mem_arena_calloc(&arena, 64, sizeof(int32_t));
    for (size_t i = 0; i < 64; i++) {
        TEST_ASSERT(ptr[i] == 0);
    }

    bc_mem_arena_free(arena);

    TEST_END;
}

TEST_BEGIN(test_arena_calloc_aligned) {
    size_t block_size = 65 * sizeof(int32_t);
    struct bc_mem_arena arena = bc_mem_arena_new(block_size);

    char* ptr1 = bc_mem_arena_calloc(&arena, 1, sizeof(char));
    TEST_ASSERT(*ptr1 == 0);

    int32_t* ptr2 = bc_mem_arena_calloc_aligned(
        &arena, 64, sizeof(int32_t), _Alignof(int32_t));
    TEST_ASSERT(ptr1 + 4 == (char*)ptr2);
    for (size_t i = 0; i < 64; i++) {
        TEST_ASSERT(ptr2[i] == 0);
    }

    TEST_END;
}

TESTS(test_arena_new, test_arena_alloc, test_arena_alloc_aligned,
    test_arena_calloc, test_arena_calloc_aligned)
