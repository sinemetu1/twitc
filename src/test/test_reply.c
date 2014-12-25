/* basic tests for reply */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "twitc_reply.h"

int tests_run = 0;

char *test_twitc_parse_json(void);
char *test_twitc_reply_has_error(void);
char *reply_all_tests(void);

int
main(void)
{
    char *result = reply_all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED :)\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}

char
*test_twitc_parse_json(void)
{
    printf("\nChecking test_twitc_parse_json...\n\n");

    const char* author = "John Cage";
    char contents[BUFSIZ];
    char *json;

    sprintf(contents, "{ \"%s\": { \"%s\": \"%s\" } }",
            TWITC_REPLY_USER_KEY, TWITC_REPLY_SN_KEY, author);
    json = twitc_parse_json_author((void *)contents);

    mu_assert("should parse json", json);

    return 0;
}

char
*test_twitc_reply_has_error(void)
{
    printf("\nChecking twitc_reply_has_error...\n\n");

    char contents[BUFSIZ];
    int has_error;
    int doesnt_have_error;

    sprintf(contents, "{ \"%s\": { \"%s\": \"an error occurred\" } }",
            TWITC_REPLY_ERRORS, TWITC_REPLY_ERRORS_MESSAGE);

    has_error = twitc_reply_has_error((void *)contents);
    mu_assert("should indicate error for error json", has_error == 1);

    // negative check, no error
    sprintf(contents, "{ \"%s\": { \"%s\": \"John Cage\" } }",
            TWITC_REPLY_USER_KEY, TWITC_REPLY_SN_KEY);

    doesnt_have_error = twitc_reply_has_error((void *)contents);
    mu_assert("should indicate error for error json", doesnt_have_error == 0);

    return 0;
}

char
*reply_all_tests(void)
{
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_parse_json);
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_reply_has_error);
    return 0;
}
