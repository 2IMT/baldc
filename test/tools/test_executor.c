#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"

static void ensure_size(char** buffer, size_t* size, size_t atleast) {
    if (*size == 0) {
        *size = 100;
    }

    while (*size < atleast) {
        size_t new_size = *size * 2;
        *buffer = realloc(*buffer, new_size);
        *size = new_size;
    }
}

int main(int argc, char** argv) {
    printf("test_executor: starting\n");

    if (argc <= 1) {
        printf("test_executor: no tests to run\n");
        return 0;
    }

    const char* count_arg = " test_count";
    size_t count_arg_len = strlen(count_arg);

    size_t exec_count = (size_t)(argc - 1);
    char** execs = argv + 1;

    size_t test_count = 0;
    size_t buffer_size = 100;
    char* buffer = malloc(buffer_size);
    for (size_t i = 0; i < exec_count; i++) {
        size_t exec_len = strlen(execs[i]);
        size_t len = exec_len + count_arg_len + 1;
        ensure_size(&buffer, &buffer_size, len);
        strcpy(buffer, execs[i]);
        strcpy(buffer + exec_len, count_arg);

        FILE* file = popen(buffer, "r");
        if (file == NULL) {
            fprintf(stderr, "test_executor: error: failed to run test file `%s`\n",
                execs[i]);
        }
        size_t value = 0;
        if (fscanf(file, "%zu", &value) == EOF) {
            fprintf(stderr,
                "test_executor: error: failed to retrieve test count for test "
                "file `%s`\n",
                execs[i]);
            pclose(file);
            return 1;
        }
        test_count += value;
        pclose(file);
    }

    printf("test_executor: collected %zu tests\n", test_count);

    char temp[1024];
    char test_ran_buf[TEST_RAN_LEN];
    size_t failed_count = 0;
    for (size_t i = 0; i < exec_count; i++) {
        printf("test_executor: running test file `%s`\n", execs[i]);
        FILE* file = popen(execs[i], "r");
        if (file == NULL) {
            fprintf(stderr, "test_executor: error: failed to run test file `%s`\n",
                execs[i]);
        }

        size_t read_count = fread(test_ran_buf, 1, TEST_RAN_LEN, file);
        if (ferror(file)) {
            fprintf(stderr,
                "text_executor: error: failed to read pipe of test file "
                "`%s`\n",
                execs[i]);
            pclose(file);
            return 1;
        }
        buffer[read_count] = '\0';

        if (read_count != TEST_RAN_LEN || memcmp(test_ran_buf, test_ran_buf, TEST_RAN_LEN) != 0) {
            fprintf(stderr,
                "test_executor: error: test file `%s` ran unsuccessfully\n", execs[i]);
            pclose(file);
            return 1;
        }

        while (true) {
            size_t read_count = fread(temp, 1, sizeof(temp), file);
            if (ferror(file)) {
                fprintf(stderr,
                    "text_executor: failed to read pipe of test file "
                    "`%s`\n",
                    execs[i]);
                pclose(file);
                return 1;
            }
            if (read_count == 0) {
                break;
            }
            printf("%.*s", (int)read_count, temp);
        }

        failed_count += WEXITSTATUS(pclose(file));
    }

    free(buffer);

    if (failed_count > 0) {
        printf("test_executor: fail: %zu out of %zu failed\n", failed_count, test_count);
        return 1;
    }

    printf("test_executor: all %zu tests passed\n", test_count);

    return 0;
}
