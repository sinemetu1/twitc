/* file: minunit.h */
/* taken from http://www.jera.com/techinfo/jtns/jtn002.html#Source_Code */
#include <stdlib.h>
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(file, line, func, test) do { \
        char *message = test(); \
        int ret_length = 1024; \
        char *toRet = malloc(ret_length); \
        memset(toRet, 0, ret_length); \
        tests_run++; \
        if (message) { \
            sprintf(toRet, "Failed %s at line %d: %s", __FILE__, __LINE__, message); \
            return toRet; \
        } \
    } while (0)
extern int tests_run;
