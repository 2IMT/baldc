#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"

typedef bool(test_func)(void);

static const char* _test_name = NULL;
static const char* _test_file_name = NULL;

#define TEST_BEGIN(name) \
    bool name(void) { \
        _test_name = #name; \
        printf("%s: running test `%s`\n", _test_file_name, _test_name);

#define TEST_END \
    _test_name = NULL; \
    return true; \
    }

#define TESTS(...) \
    int main(int argc, char** argv) { \
        _test_file_name = argv[0]; \
        test_func* tests[] = { __VA_ARGS__ }; \
        size_t count = sizeof(tests) / sizeof(tests[0]); \
        if (argc > 1) { \
            if (strcmp(argv[1], "test_count") == 0) { \
                printf("%zu\n", count); \
                return 0; \
            } else { \
                return 1; \
            } \
        } \
        printf(TEST_RAN); \
        int failed = 0; \
        for (size_t i = 0; i < count; ++i) { \
            if (!tests[i]()) { \
                failed++; \
            } \
        } \
        return failed; \
    }

#define TEST_ASSERT(expr) \
    if (!(expr)) { \
        TEST_FAIL("assertion `" #expr "` failed"); \
    }

#define TEST_FAIL(reason) \
    do { \
        printf("%s: fail: test `%s`:\n", _test_file_name, _test_name); \
        printf("%s:     %s:%d: %s\n", _test_file_name, __FILE__, __LINE__, \
            reason); \
        return false; \
    } while (0)
