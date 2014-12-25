/**
 * @brief Methods for interacting with Twitter's api.
 * @file twitc.h
 * @author Sam Garrett <samdgarrett@gmail.com>
 */
#ifndef TWITC_H
#define TWITC_H

#include <stdlib.h>

// twitc version
#define LIBTWITC_VERSION "0.0.1"
#define LIBTWITC_VERSION_MAJOR 0
#define LIBTWITC_VERSION_MINOR 0
#define LIBTWITC_VERSION_MICRO 1

#define TWITC_OAUTH_TOKEN    "oauth_token"
#define TWITC_OAUTH_VERIFIER "oauth_verifier"

#define TWITC_OAUTH_GET  1
#define TWITC_OAUTH_POST 0

#define USER_STREAM_URL   "https://userstream.twitter.com/1.1/user.json"
#define USER_STATUS_URL   "https://api.twitter.com/1.1/statuses/update.json"
#define TEST_TIMELINE_URL "https://api.twitter.com/1.1/statuses/user_timeline.json"
#define REQUEST_TOKEN_URL "https://api.twitter.com/oauth/request_token?oauth_callback=oob"
#define AUTHORIZE_URL     "https://api.twitter.com/oauth/authorize"
#define ACCESS_TOKEN_URL  "https://api.twitter.com/oauth/access_token"

#define CONSUMER_KEY        "TWITC_KEY"
#define CONSUMER_SECRET     "TWITC_SECRET"
#define TWITC_ACCESS_TOKEN  "TWITC_ACCESS_TOKEN"
#define TWITC_ACCESS_SECRET "TWITC_ACCESS_SECRET"

struct MemoryStruct {
   char *memory;
   size_t size;
};

/**
 * Just an example of a stream processing callback.
 *
 * @param contents actual contents
 * @param size of each member
 * @param nmemb number of members
 * @param userp a pointer to a MemoryStruct, probably not used
 *   if streaming though.
 * @return the number of bytes written
 */
size_t stream_callback(void *contents, size_t size, size_t nmemb, void *userp);

/**
 * A callback function used in CURLOPT_WRITEFUNCTION where the userp
 * structure (a MemoryStruct) gets the contents written to it.
 *
 * @param contents actual contents
 * @param size of each member
 * @param nmemb number of members
 * @param userp a pointer to a MemoryStruct
 * @return the number of bytes written
 */
size_t twitc_get_memory_callback(void *contents, size_t size,
                                 size_t nmemb, void *userp);

/**
 * A wrapper around a basic curl call.
 *
 * @param http_hdr the http header string
 * @param req_url the url to request
 * @param chunk a pointer to a MemoryStruct that is written to if passed in
 * @param post_data data to be posted to the url
 * @param callback a callback function similar to \ref twitc_get_memory_callback()
 * @return -1 if failed, 0 if success
 */
int twitc_do_curl(char *http_hdr, char *req_url,
        struct MemoryStruct *chunk, char *post_data,
        size_t (*callback)(void*, size_t, size_t, void*));

/**
 * Request an token and secret based on the req_c_key and req_c_secret.
 *
 * See: https://dev.twitter.com/oauth/reference/post/oauth/request_token
 *
 * @param req_c_key consumer key
 * @param req_c_secret consumer secret
 * @param token a token that will be set once the reply from twitter is received
 * @param secret a secret that will be set once the reply from twitter is received
 * @return 0 == success and 1 == failure.
 */
int twitc_oauth_request_token(const char *req_c_key, const char *req_c_secret,
                              char **token, char **secret);

/**
 * This method prints out an authorization url for the passed in token.
 * A user would need to visit this url to get a pin to authorize the token.
 *
 * See: https://dev.twitter.com/oauth/reference/get/oauth/authorize
 *
 * @param token a token that is used to get the proper url for authorization
 * @param pin that is used once the user has visited the authorzation url.
 *   NOTE: pin is realloc'd and will need to be freed by the caller.
 * @return 0 == success and 1 == failure.
 */
int twitc_oauth_authorize_token(char *token, char **pin);

/**
 * Requests an oauth access_token and access_secret based on an already
 * fetched token and secret. 
 *
 * NOTE: Also uses pin as the oauth_verifier param.
 *
 * See: https://dev.twitter.com/oauth/reference/post/oauth/access_token
 *
 * @param req_c_key consumer key
 * @param req_c_secret consumer secret
 * @param token a token that will be set once the reply from twitter is received
 * @param secret a secret that will be set once the reply from twitter is received
 * @param pin that is used once the user has visited the authorzation url
 * @param access_token will be set by making a call to verify with the pin
 * @param access_secret will be set by making a call to verify with the pin
 * @return 0 == success and 1 == failure.
 */
int twitc_oauth_access_token(const char *req_c_key, const char *req_c_secret,
                             char *token, char *secret, char* pin,
                             char **access_token, char **access_secret);


/**
 * Makes a request to twitters api with an authorized consumer token/secret
 * and a user token/secret set.
 *
 * See: https://dev.twitter.com/rest/public
 *
 * @param a_url the url for the request to twitter
 * @param req_c_key consumer key
 * @param req_c_secret consumer secret
 * @param user_token a user token obtained via twitc_oauth_access_token()
 * @param user_secret a user token obtained via twitc_oauth_access_token()
 * @param isPost either OAUTH_GET or OAUTH_POST depending on the type of request
 * @param callback a callback function \ref twitc_get_memory_callback()
 *   stream_callback() or similar.
 * @return a pointer to a MemoryStruct containing the reply from twitter. Caller
 * should free.
 */
struct MemoryStruct* 
twitc_oauth_twitter(const char *a_url, const char *req_c_key,
                    const char *req_c_secret, char *user_token,
                    char *user_secret, int isPost,
                    size_t (*callback)(void*, size_t, size_t, void*));

/**
 * A helper function for checking if a MemoryStruct (usually used for the reply
 * from a twitter feed call) contains errors.
 * See \ref twitc_reply.h twitc_reply_has_error()
 *
 * @param mem_struct
 * @return 0 if no errors and 1 if has errors
 */
int twitc_check_memory_for_errors(struct MemoryStruct* mem_struct);

int twitc_oauth_request(const char *url, const char *req_c_key,
                        const char *req_c_secret, char *user_token,
                        char *user_secret, struct MemoryStruct *chunk,
                        int isPost,
                        size_t (*callback)(void*, size_t, size_t, void*));

#endif
