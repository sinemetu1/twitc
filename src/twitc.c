#include "twitc.h"
#include "twitc_reply.h"

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <oauth.h>

size_t
stream_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize;

    realsize = size * nmemb;
    printf("author: %s\n", twitc_parse_json_author(contents));

    return realsize;
}

size_t
twitc_get_memory_callback(void *contents, size_t size,
                          size_t nmemb, void *userp)
{
   size_t realsize;
   struct MemoryStruct *mem;

   realsize = size * nmemb;
   mem = (struct MemoryStruct *)userp;

   mem->memory = realloc(mem->memory, mem->size + realsize + 1);
   if(mem->memory == NULL) {
     fprintf(stderr, "not enough memory (realloc returned NULL)\n");
     return 0;
   }

   memcpy(&(mem->memory[mem->size]), contents, realsize);
   mem->size += realsize;
   mem->memory[mem->size] = 0;

   return realsize;
}

int
twitc_do_curl(char *http_hdr, char *req_url,
              struct MemoryStruct *chunk, char *post_data,
              size_t (*callback)(void*, size_t, size_t, void*))
{
    CURL *curl = curl_easy_init();
    if (!curl) { return -1; }
    CURLcode curl_res;
    struct curl_slist* slist = NULL;

    slist = curl_slist_append(slist, http_hdr);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    curl_easy_setopt(curl, CURLOPT_URL, req_url);

    if (post_data != NULL) {
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        // Debug:
        /*fprintf(stderr, "Curl is:\n");*/
        /*fprintf(stderr, "curl --request 'POST' '%s' --data '%s' --header '%s' --verbose\n",*/
                /*req_url, post_data, http_hdr);*/
    }
    // Debug:
    /*fprintf(stderr, "Curl is:\n");*/
    /*fprintf(stderr, "curl --get '%s' --header '%s' --verbose\n", req_url, http_hdr);*/

    if (chunk == NULL) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    } else {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
    }
    /*curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);*/

    // DO the request
    curl_res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (curl_res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(curl_res));
        return -1;
    }

    /* we're done with libcurl, so clean it up */ 
    if (slist) { curl_slist_free_all(slist); }
    curl_global_cleanup();

    return 0;
}

int
twitc_oauth_request(const char *url,
                    const char *req_c_key, const char *req_c_secret,
                    char *user_token, char *user_secret,
                    struct MemoryStruct *chunk, int isPost,
                    size_t (*callback)(void*, size_t, size_t, void*))
{
    printf("Requesting url...%s\n", url);

    char *auth_params       = NULL;
    char auth_header[2048];
    char *non_auth_params   = NULL;
    char *final_url         = NULL;
    char *temp_url          = NULL;
    char **argv             = NULL;
    int argc = oauth_split_url_parameters(url, &argv);
    char *http_method;

    if (isPost == TWITC_OAUTH_POST) {
        http_method = "POST";
    } else {
        http_method = "GET";
    }

    temp_url = oauth_sign_array2(&argc, &argv,
            NULL, OA_HMAC, http_method,
            req_c_key, req_c_secret, user_token, user_secret);

    if (isPost == TWITC_OAUTH_POST) {
        int total_url_length = strlen(argv[0]) + 1;
        final_url = malloc(total_url_length);
        if (final_url == NULL) {
            fprintf(stderr, "not enough memory (malloc returned NULL)\n");
            exit(1);
        }
        strcpy(final_url, argv[0]);
    } else {
        final_url = oauth_serialize_url_sep(argc, 0, argv, "&", 1);
    }

    auth_params = oauth_serialize_url_sep(argc, 1, argv, ", ", 6);
    sprintf(auth_header, "Authorization: OAuth %s", auth_params);

    if (isPost == TWITC_OAUTH_POST) {
        non_auth_params = oauth_serialize_url_sep(argc, 1, argv, "&", 1 );
    }

    twitc_do_curl(auth_header, final_url, chunk, non_auth_params,
            callback);

    if (final_url) { free(final_url); }
    if (temp_url) { free(temp_url); }
    if (auth_params) { free(auth_params); }
    if (non_auth_params) { free(non_auth_params); }
    oauth_free_array(&argc, &argv);

    return 0;
}

int
twitc_oauth_request_token(const char *req_c_key, const char *req_c_secret,
                          char **token, char **secret)
{
    int toRet = 0;
    struct MemoryStruct reply;
    reply.memory = malloc(1);
    reply.size = 0;
    if (reply.memory == NULL) {
        fprintf(stderr, "not enough memory (malloc returned NULL)\n");
        exit(1);
    }
    
    twitc_oauth_request(REQUEST_TOKEN_URL, req_c_key,
            req_c_secret, NULL, NULL, &reply, TWITC_OAUTH_GET,
            &twitc_get_memory_callback);

    if (reply.memory) {
        if (twitc_parse_reply(reply.memory, token, secret)) {
            toRet = 1;
        }
    } else {
        printf("Request token failed\n");
        toRet = -1;
    }

    if (reply.memory) { free(reply.memory); }

    return toRet;
}

int
twitc_oauth_authorize_token(char *token, char **pin)
{
    char *url_and_token = malloc(BUFSIZ);
    char *my_pin = malloc(BUFSIZ);
    if (url_and_token == NULL || my_pin == NULL) {
        fprintf(stderr, "not enough memory (malloc returned NULL)\n");
        exit(1);
    }

    sprintf(url_and_token, "%s?%s=%s", AUTHORIZE_URL, TWITC_OAUTH_TOKEN, token);
    printf("Go to this url in a browser to authorize this app:\n%s\n",
            url_and_token);
    
    printf("Now input the pin that you have obtained:\n");
    scanf("%s", my_pin);

    int size = strlen(my_pin);
    pin = realloc(pin, (size + 1) * sizeof(char));
    memcpy(pin, &my_pin, size + 1);

    printf("You entered %s.\n", *pin);

    free(my_pin);
    free(url_and_token);
    return 1;
}

int
twitc_oauth_access_token(const char *req_c_key, const char *req_c_secret,
                         char *token, char *secret, char* pin,
                         char **access_token, char **access_secret)
{
    int toRet = 0;
    char url_and_token[BUFSIZ];
    struct MemoryStruct reply;
    reply.memory = malloc(1);
    reply.size = 0; 

    if (reply.memory == NULL) {
        fprintf(stderr, "not enough memory (malloc returned NULL)\n");
        exit(1);
    }

    sprintf(url_and_token, "%s?%s=%s&%s=%s", ACCESS_TOKEN_URL,
            TWITC_OAUTH_TOKEN, token,
            TWITC_OAUTH_VERIFIER, pin);
    twitc_oauth_request(url_and_token, req_c_key, req_c_secret,
            token, secret, &reply, TWITC_OAUTH_GET,
            &twitc_get_memory_callback);

    if (reply.memory) {
        if (twitc_parse_reply(reply.memory, access_token, access_secret)) {
            toRet = 1;
        }
    } else {
        printf("Request token failed\n");
        toRet = 1;
    }

    if (reply.memory) { free(reply.memory); }

    return toRet;
}

int
twitc_check_memory_for_errors(struct MemoryStruct* mem_struct)
{
    if (!mem_struct ||
        !mem_struct->memory ||
        twitc_reply_has_error(mem_struct->memory) == 1) {
        return 1;
    }
    return 0;
}

struct MemoryStruct*
twitc_oauth_twitter(const char *a_url, const char *req_c_key,
                    const char *req_c_secret, char *user_token,
                    char *user_secret, int isPost,
                    size_t (*callback)(void*, size_t, size_t, void*))
{
    char url_and_token[BUFSIZ];
    struct MemoryStruct* reply = malloc(sizeof(*reply));
    if (reply == NULL) {
        fprintf(stderr, "not enough memory (malloc returned NULL)\n");
        exit(1);
    }
    reply->memory = malloc(1);
    if (reply->memory == NULL) {
        fprintf(stderr, "not enough memory (malloc returned NULL)\n");
        exit(1);
    }
    reply->size = 0;

    sprintf(url_and_token, "%s", a_url);
    
    twitc_oauth_request(url_and_token, req_c_key, req_c_secret,
            user_token, user_secret, reply, isPost,
            callback);

    return reply;
}
