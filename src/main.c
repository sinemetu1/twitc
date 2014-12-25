#include "twitc.h"
#include "twitc_reply.h"

#include <stdio.h>

int
main(void)
{
    char *token = NULL;
    char *secret = NULL;
    char *c_key = getenv(CONSUMER_KEY);
    char *c_secret = getenv(CONSUMER_SECRET);
    char *access_token = NULL;
    char *access_secret = NULL;
    struct MemoryStruct* reply;
    char *pin = malloc(1);
    if (pin == NULL) {
        fprintf(stderr, "not enough memory (malloc returned NULL)\n");
        exit(1);
    }

    // get token and secret
    twitc_oauth_request_token(c_key, c_secret, &token, &secret);

    // authorize the token
    twitc_oauth_authorize_token(token, &pin);

    // get an access token
    twitc_oauth_access_token(c_key, c_secret, token, secret,
            pin, &access_token, &access_secret);

    free(pin);

    // do twitter stream call
    reply = twitc_oauth_twitter(USER_STREAM_URL,
            c_key, c_secret, access_token, access_secret, TWITC_OAUTH_GET,
            &twitc_get_memory_callback);
    int has_errors = twitc_check_memory_for_errors(reply);
    
    if (has_errors == 1) {
        printf("Call to %s failed :(", USER_STREAM_URL);
    } else {
        printf("Success! Reply was: %s", reply->memory);
    }

    if (reply->memory) { free(reply->memory); }
    free(reply);

    return 0;
}
