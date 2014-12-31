/* basic tests for twitc */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include "minunit.h"
#include "twitc.h"
#include "twitc_reply.h"

extern int test_curl_perform_called;
extern char *test_curl_url;
extern char *test_curl_post_data;
extern char *test_curl_header;

int tests_run = 0;

void free_tracked_params(void);
char * test_twitc_oauth_request_token(void);
char * test_twitc_oauth_access_token(void);
char * test_twitc_oauth_twitter(void);
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

    free_tracked_params();

    return result != 0;
}

char *
twitc_all_tests(void)
{
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_oauth_request_token);
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_oauth_access_token);
    mu_run_test(__FILE__, __LINE__, __func__, test_twitc_oauth_twitter);
    return 0;
}

char *
test_twitc_oauth_request_token(void)
{
    printf("\nChecking test_twitc_oauth_request_token...\n\n");
    int cache_count = test_curl_perform_called;

    char *token = NULL;
    char *secret = NULL;
    char *c_key = "C";
    char *c_secret = "D";

    twitc_oauth_request_token(c_key, c_secret, &token, &secret);

    mu_assert("verify called", (cache_count+1) == test_curl_perform_called);
    mu_assert_str("verify called url", test_curl_url, "%s?", REQUEST_TOKEN_URL);
    mu_assert_strcspn("verify oauth_consumer_key header",
            test_curl_header, "%s=\"%s\"", "oauth_consumer_key", "C");
    mu_assert_str("verify no post_data", test_curl_post_data, "%s", "(null)");

    if (token) { free(token); }
    if (secret) { free(secret); }

    return 0;
}

char *
test_twitc_oauth_access_token(void)
{
    printf("\nChecking test_twitc_oauth_access_token...\n\n");
    int cache_count = test_curl_perform_called;

    char *access_token = "access_token";
    char *access_secret = "access_secret";
    char *pin = "XYZ";
    char *token = "A";
    char *secret = "B";
    char *c_key = "C";
    char *c_secret = "D";

    twitc_oauth_access_token(c_key, c_secret, token, secret, pin, &access_token,
            &access_secret);

    mu_assert("verify called", (cache_count+1) == test_curl_perform_called);
    mu_assert_str("verify called url", test_curl_url, "%s?", ACCESS_TOKEN_URL);
    mu_assert_strcspn("verify oauth_consumer_key header",
            test_curl_header, "%s=\"%s\"", "oauth_consumer_key", "C");
    mu_assert_strcspn("verify oauth_token header",
            test_curl_header, "%s=\"%s\"", TWITC_OAUTH_TOKEN, token);
    mu_assert_str("verify no post_data", test_curl_post_data, "%s", "(null)");

    return 0;
}

char *
test_twitc_oauth_twitter(void)
{
    printf("\nChecking test_twitc_oauth_twitter...\n\n");
    int cache_count = test_curl_perform_called;

    CURL *curl = curl_easy_init(); // used for checking escaping in post_data
    struct MemoryStruct *reply;
    int has_errors;
    char tweet_url[BUFSIZ] = "";
    char *token = "A";
    char *secret = "B";
    char *c_key = "C";
    char *c_secret = "D";
    int randNumber = rand();
    char *extra_params = "testing=extra_params";
    char *post_string = "hello d'oh!";
    char *escaped_post_string;

    reply = twitc_oauth_twitter(TEST_TIMELINE_URL,
                c_key, c_secret, token, secret, TWITC_OAUTH_GET,
                &twitc_get_memory_callback);

    has_errors = twitc_check_memory_for_errors(reply);
    mu_assert("should be able to do twitter oauth", has_errors == 0);
    mu_assert("verify called", (cache_count+1) == test_curl_perform_called);
    mu_assert_str("verify called url", test_curl_url, "%s?", TEST_TIMELINE_URL);
    mu_assert_strcspn("verify oauth_consumer_key header",
            test_curl_header, "%s=\"%s\"", "oauth_consumer_key", "C");
    mu_assert_strcspn("verify oauth_token header",
            test_curl_header, "%s=\"%s\"", TWITC_OAUTH_TOKEN, token);
    mu_assert_str("verify no post_data", test_curl_post_data, "%s", "(null)");

    if (reply->memory) { free(reply->memory); }
    if (reply) { free(reply); }

    // try with POST to timeline
    snprintf(tweet_url, sizeof(tweet_url),
            "%s?%s&%s=%s%d", USER_STATUS_URL, extra_params, TWITC_REPLY_STATUS,
            post_string, randNumber);
    reply = twitc_oauth_twitter(tweet_url,
                c_key, c_secret, token, secret, TWITC_OAUTH_POST,
                &twitc_get_memory_callback);
    has_errors = twitc_check_memory_for_errors(reply);
    mu_assert("should be able to do POST with twitter oauth", has_errors == 0);
    mu_assert("verify called", (cache_count+2) == test_curl_perform_called);
    mu_assert_str("verify called url", test_curl_url, "%s", USER_STATUS_URL);
    mu_assert_strcspn("verify oauth_consumer_key header",
            test_curl_header, "%s=\"%s\"", "oauth_consumer_key", "C");
    mu_assert_strcspn("verify oauth_token header",
            test_curl_header, "%s=\"%s\"", TWITC_OAUTH_TOKEN, token);
    escaped_post_string = curl_easy_escape(curl, post_string, strlen(post_string));
    mu_assert_str("verify post_data", test_curl_post_data, "%s=%s%d&%s",
            TWITC_REPLY_STATUS, escaped_post_string,
            randNumber, extra_params);

    if (escaped_post_string) { free(escaped_post_string); }
    if (reply->memory) { free(reply->memory); }
    if (reply) { free(reply); }
    curl_easy_cleanup(curl);

    return 0;
}
