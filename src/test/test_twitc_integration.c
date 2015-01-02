/* integration tests for twitc */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "minunit.h"
#include "twitc.h"
#include "twitc_reply.h"

extern int test_curl_perform_called;
extern char* test_curl_url;
extern char* test_curl_post_data;
extern char* test_curl_header;

int tests_run = 0;

char * test_twitc_oauth_twitter_integration(void);
char * test_twitc_oauth_authorize_token_integration(void);
char * twitc_all_tests(void);

int
main(void)
{
    srand(time(NULL));
    char *result = twitc_all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED :)\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}

char *
twitc_all_tests(void)
{
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_oauth_authorize_token_integration);
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_oauth_twitter_integration);
    return 0;
}

char *
test_twitc_oauth_twitter_integration(void)
{
    printf("\nChecking test_twitc_oauth_twitter...\n\n");

    struct MemoryStruct *reply;
    int has_errors;
    char tweet_url[BUFSIZ] = "";
    char *token = getenv(TWITC_ACCESS_TOKEN);
    char *secret = getenv(TWITC_ACCESS_SECRET);
    char *c_key = getenv(CONSUMER_KEY);
    char *c_secret = getenv(CONSUMER_SECRET);
    int randNumber = rand();

    reply = twitc_oauth_twitter(TEST_TIMELINE_URL,
                c_key, c_secret, token, secret, TWITC_OAUTH_GET,
                &twitc_get_memory_callback);

    has_errors = twitc_check_memory_for_errors(reply);
    mu_assert("should be able to do twitter oauth", has_errors == 0);

    if (reply->memory) {
        free(reply->memory);
    }

    // try with POST to timeline
    snprintf(tweet_url, sizeof(tweet_url),
            "%s?testing=extra_params&%s=%s%d", USER_STATUS_URL, TWITC_REPLY_STATUS,
            "hello d'oh!", randNumber);
    reply = twitc_oauth_twitter(tweet_url,
                c_key, c_secret, token, secret, TWITC_OAUTH_POST,
                &twitc_get_memory_callback);
    has_errors = twitc_check_memory_for_errors(reply);
    mu_assert("should be able to do POST with twitter oauth", has_errors == 0);

    if (reply->memory) {
        free(reply->memory);
    }
    free(reply);

    return 0;
}

char *
test_twitc_oauth_authorize_token_integration(void)
{
    printf("\nChecking test_twitc_oauth_authorize_token...\n\n");

    char *token = NULL;
    char *secret = NULL;
    char *c_key = getenv(CONSUMER_KEY);
    char *c_secret = getenv(CONSUMER_SECRET);
    char *pin = malloc(1);
    if (pin == NULL) {
        fprintf(stderr, "malloc failed");
        exit(1);
    }

    twitc_oauth_request_token(c_key, c_secret, &token, &secret);

    twitc_oauth_authorize_token(token, &pin);
    mu_assert("should be able to authorize a token", strlen(pin) > 0);

    free(pin);

    return 0;
}
