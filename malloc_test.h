/** @file
 * @author A. Ż. <azewierzejew@gmail.com>
 * @date 30.05.2019
 */

#ifndef MALLOC_TEST_H
#define MALLOC_TEST_H

#include <stddef.h>

void *_test_calloc(size_t number_of_elements, size_t size, const char *file, int line);

void *_test_malloc(size_t size, char *const file, int line);

void *_test_realloc(void *const ptr, size_t size, char *const file, int line);

void _test_free(void *const ptr);

int _mock_main(int argc, char **argv, int real_main());


#ifndef MALLOC_TEST_LOCAL_INCLUDE

#ifdef calloc
#undef calloc
#endif /* calloc */
#define calloc(num, size) _test_calloc(num, size, __FILE__, __LINE__)

#ifdef malloc
#undef malloc
#endif /* malloc */
#define malloc(size) _test_malloc(size, __FILE__, __LINE__)

#ifdef realloc
#undef realloc
#endif /* realloc */
#define realloc(ptr, size) _test_realloc(ptr, size, __FILE__, __LINE__)

#ifdef free
#undef free
#endif /* free */
#define free(ptr) _test_free(ptr)

#define main() _real_main(); \
int main(int argc, char **argv) { \
    _mock_main(argc, argv, _real_main); \
} \
int _real_main()

#endif /* MALLOC_TEST_LOCAL_INCLUDE */


#endif /* MALLOC_TEST_H */
