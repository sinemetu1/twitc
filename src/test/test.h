/**
 * @file test.h
 * @author Sam Garrett <samdgarrett@gmail.com>
 */
#ifndef TEST_TWITC_H
#define TEST_TWITC_H

#include <curl/curl.h>

void print_tracked_params(void);
void free_tracked_params(void);

/**
 * NOTE: will not work if using in multi-threaded tests
 *       because of global vars below.
 */
#define test_curl_easy_setopt(handle, option, paremeter, track) do {               \
            curl_easy_setopt(handle, option, paremeter);                           \
            int length = strlen(paremeter) + 1;                                    \
            if (track == NULL) {                                                   \
                track = malloc(length);                                            \
                if (track == NULL) {                                               \
                    fprintf(stderr, "not enough memory (malloc returned NULL)\n"); \
                    exit(1);                                                       \
                }                                                                  \
            } else {                                                               \
                track = realloc(track, length);                                    \
                if (track == NULL) {                                               \
                    fprintf(stderr, "not enough memory (realloc returned NULL)\n");\
                    exit(1);                                                       \
                }                                                                  \
            }                                                                      \
            snprintf(track, length, "%s", paremeter);                              \
        } while (0)

/**
 * Can't really have a drop-in replacement for curl_easy_perform
 * since it's return value is used, thus this one with an extra param.
 */
#define test_curl_easy_perform(handle, curl_res) do { \
            test_curl_perform_called++;               \
            curl_res = CURLE_OK;                      \
        } while (0)

int test_curl_perform_called = 0;
char *test_curl_url;
char *test_curl_post_data;
char *test_curl_header;

void
print_tracked_params(void)
{
    printf("perform_called: %d\ncalled_url: %s\nposted_data: %s\ntest_curl_header: %s\n",
            test_curl_perform_called, test_curl_url, test_curl_post_data,
            test_curl_header);
}

void
free_tracked_params(void)
{
    if (test_curl_url) { free(test_curl_url); }
    if (test_curl_post_data) { free(test_curl_post_data); }
    if (test_curl_header) { free(test_curl_header); }
}

#endif
