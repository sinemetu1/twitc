/* file: minunit.h
 * taken and modified from http://www.jera.com/techinfo/jtns/jtn002.html#Source_Code
 */

#include <stdlib.h>
#include <stdarg.h>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)

#define mu_run_test(file, line, func, test) do {                                     \
        char *message = test();                                                      \
        tests_run++;                                                                 \
        if (message) {                                                               \
            int ret_length = 1024;                                                   \
            char *toRet = malloc(ret_length);                                        \
            memset(toRet, 0, ret_length);                                            \
            sprintf(toRet, "Failed %s at line %d: %s", __FILE__, __LINE__, message); \
            return toRet;                                                            \
        }                                                                            \
    } while (0)

/**
 * Tests actual for equality with processed string.
 */
#define mu_assert_str(message, actual, format, ...) do {    \
        char local[BUFSIZ] = "";                            \
        snprintf(local, sizeof(local), format, __VA_ARGS__);\
        int cmp = strcmp(local, actual);                    \
        if (cmp != 0) {                                     \
            printf("%s != %s\n", local, actual);            \
            return message;                                 \
        }                                                   \
    } while (0)

/**
 * Tests actual for containment of processed string.
 */
#define mu_assert_strcspn(message, actual, format, ...) do {\
        char local[BUFSIZ] = "";                            \
        snprintf(local, BUFSIZ, format, __VA_ARGS__);       \
        char *cmp = strstr(actual, local);                  \
        if (cmp == NULL) {                                  \
            printf("didn't find %s in actual\n", local);    \
            printf("actual: %s\n", actual);                 \
            return message;                                 \
        }                                                   \
    } while (0)

extern int tests_run;
