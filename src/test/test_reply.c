/* basic tests for twitc_reply */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "twitc_reply.h"

int tests_run = 0;

char * test_twitc_parse_json(void);
char * test_twitc_reply_has_error(void);
char * test_twitc_reply_has_error_true(void);
char * test_twitc_parse_reply(void);
char * verify_min_params_twitc_parse_reply(void);
char * extra_param_twitc_parse_reply(void);
char * reply_all_tests(void);

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

char *
reply_all_tests(void)
{
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_parse_json);
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_reply_has_error);
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_reply_has_error_true);
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_parse_reply);
    mu_run_test(__FILE__, __LINE__, __func__, extra_param_twitc_parse_reply);
    mu_run_test(__FILE__, __LINE__, __func__, verify_min_params_twitc_parse_reply);

    return 0;
}

char *
test_twitc_parse_json(void)
{
    printf("\nChecking test_twitc_parse_json...\n\n");

    const char *author = "John Cage";
    char *reply_format = "{ \"%s\": { \"%s\": \"%s\" } }";
    char contents[BUFSIZ] = "";
    char *json;
    int doesnt_have_error = 1;

    snprintf(contents, sizeof(contents), reply_format,
            TWITC_REPLY_USER_KEY, TWITC_REPLY_SN_KEY, author);

    json = twitc_parse_json_author(contents);
    mu_assert_str("should parse json", json, "%s", author);

    doesnt_have_error = twitc_reply_has_error(contents);
    mu_assert("should NOT indicate error for json", doesnt_have_error == 0);

    if (json) { free(json); }

    return 0;
}

char *
test_twitc_reply_has_error(void)
{
    printf("\nChecking twitc_reply_has_error...\n\n");

    const char *author = "John Cage";
    char *reply_format = "{ \"%s\": { \"%s\": \"%s\" } }";
    char contents[BUFSIZ] = "";
    int doesnt_have_error = 1;

    snprintf(contents, sizeof(contents), reply_format,
            TWITC_REPLY_USER_KEY, TWITC_REPLY_SN_KEY, author);

    doesnt_have_error = twitc_reply_has_error(contents);
    mu_assert("should NOT indicate error for json", doesnt_have_error == 0);

    return 0;
}

char *
test_twitc_reply_has_error_true(void)
{
    printf("\nChecking test_twitc_reply_has_error_true...\n\n");

    char *reply_format = "{ \"%s\": \"\" }";
    char contents[BUFSIZ] = "";
    int has_error = 0;

    snprintf(contents, sizeof(contents), reply_format,
            TWITC_REPLY_ERRORS);

    has_error = twitc_reply_has_error(contents);
    mu_assert("should indicate error for error json", has_error == 1);

    return 0;
}

char *
test_twitc_parse_reply()
{
    printf("\nChecking test_twitc_parse_reply...\n\n");

    char *token = NULL;
    char *secret = NULL;
    char *format = "%s=%s&%s=%s&oauth_callback_confirmed=true";
    char *expected_token = "aAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB";
    char *expected_secret = "cCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCD";
    char reply[BUFSIZ];
    snprintf(reply, sizeof(reply), format, TWITC_OAUTH_TOKEN, expected_token,
            TWITC_OAUTH_TOKEN_SECRET, expected_secret);

    twitc_parse_reply(reply, &token, &secret);

    mu_assert_str("assert expected_token", token, "%s", expected_token);
    mu_assert_str("assert expected_secret", secret, "%s", expected_secret);

    if (secret) { free(secret); }
    if (token) { free(token); }

    // make sure order doesn't matter
    snprintf(reply, sizeof(reply), format,
            TWITC_OAUTH_TOKEN_SECRET, expected_secret,
            TWITC_OAUTH_TOKEN, expected_token);

    twitc_parse_reply(reply, &token, &secret);

    mu_assert_str("assert expected_token reverse order",
            token, "%s", expected_token);
    mu_assert_str("assert expected_secret reverse order",
            secret, "%s", expected_secret);

    if (secret) { free(secret); }
    if (token) { free(token); }

    return 0;
}

char *
extra_param_twitc_parse_reply()
{
    printf("\nChecking extra_param_twitc_parse_reply...\n\n");

    char *token = NULL;
    char *secret = NULL;
    char *format_extra_param = "%s=%s&%s=%s&oauth_callback_confirmed=true&%s";
    char *expected_token = "aAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB";
    char *expected_secret = "cCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCD";
    char reply[BUFSIZ];

    snprintf(reply, sizeof(reply), format_extra_param,
            TWITC_OAUTH_TOKEN_SECRET, expected_secret,
            TWITC_OAUTH_TOKEN, expected_token,
            "something=extra");

    twitc_parse_reply(reply, &token, &secret);

    mu_assert_str("assert expected_token extra param",
            token, "%s", expected_token);
    mu_assert_str("assert expected_secret extra param",
            secret, "%s", expected_secret);

    if (secret) { free(secret); }
    if (token) { free(token); }

    return 0;
}

char *
verify_min_params_twitc_parse_reply()
{
    printf("\nChecking verify_min_params_twitc_parse_reply...\n\n");

    char *token = NULL;
    char *secret = NULL;
    char *format = "%s=%s";
    char *expected_token = "aAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB";
    char *expected_secret = "cCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCD";
    char reply[BUFSIZ];

    // token only
    snprintf(reply, sizeof(reply), format,
            TWITC_OAUTH_TOKEN, expected_token);
    twitc_parse_reply(reply, &token, &secret);
    mu_assert_str("assert expected_token", token, "%s", expected_token);
    mu_assert("assert secret is not set", secret == NULL);

    if (secret) { free(secret); }
    if (token) {
        free(token);
        token = NULL; // we expect token to be unmodified next
    }

    // secret only
    snprintf(reply, sizeof(reply), format,
            TWITC_OAUTH_TOKEN_SECRET, expected_secret);
    twitc_parse_reply(reply, &token, &secret);
    mu_assert("assert token is not set", token == NULL);
    mu_assert_str("assert expected_secret", secret, "%s", expected_secret);

    if (secret) { free(secret); }
    if (token) { free(token); }

    return 0;
}
