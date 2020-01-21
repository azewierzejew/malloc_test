/** @file
 * @author A. Ż. <azewierzejew@gmail.com>
 * @date 30.05.2019
 */
#define MALLOC_TEST_LOCAL_INCLUDE

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static size_t error_counter = 0;
static size_t ERROR_NUM = -1;
static bool FAIL_ALL_AFTER = false; // By default fail once.

static bool count_up(const char *file, int line) {
    if (error_counter++ == ERROR_NUM || (FAIL_ALL_AFTER && error_counter > ERROR_NUM)) {
        fprintf(stderr, "MOCK ALLOC REACHED IN %s:%d\n", file, line);
        return false;
    }

    return true;
}

void *_test_calloc(size_t number_of_elements, size_t size, const char *file, int line) {
    if (!count_up(file, line)) {
        return NULL;
    }

    return calloc(number_of_elements, size);
}

void *_test_malloc(size_t size, char *const file, int line) {
    if (!count_up(file, line)) {
        return NULL;
    }

    return malloc(size);
}

void *_test_realloc(void *const ptr, size_t size, char *const file, int line) {
    if (!count_up(file, line)) {
        return NULL;
    }

    return realloc(ptr, size);
}

void _test_free(void *const ptr) {
    free(ptr);
}


int _mock_main(int argc, char **argv, int real_main()) {
    printf("%d\n", argc);
    size_t num = -1;
    if (argc >= 2) {
        errno = 0;
        num = strtoul(argv[1], 0, 0);
        if (errno != 0) {
            fprintf(stderr, "Error while parsing: %s\n", strerror(errno));
            return 1;
        }
    }
    ERROR_NUM = num;

    if (argc >= 3 && strcmp(argv[2], "--fail-all-after") == 0) {
        FAIL_ALL_AFTER = true;
    }

    return real_main();
}
