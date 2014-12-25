#include "twitc_reply.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oauth.h>
#include <json-c/json.h>

char*
twitc_parse_json_author(void *contents)
{
    json_object *tweet_obj;
    json_object *user_json;
    json_object *author_json;

    tweet_obj = json_tokener_parse((char *) contents);
    user_json = json_object_object_get(tweet_obj, TWITC_REPLY_USER_KEY);

    author_json = json_object_object_get(user_json, TWITC_REPLY_SN_KEY);
    const char* author = json_object_get_string(author_json);

    json_object_put(tweet_obj);

    return (char *) author;
}

int
twitc_reply_has_error(void *contents)
{
    json_object *json_obj;
    json_object *errors_obj;
    json_bool has_errors;

    json_obj = json_tokener_parse((char *) contents);
    has_errors = json_object_object_get_ex(json_obj, TWITC_REPLY_ERRORS, &errors_obj);

    json_object_put(json_obj);
    if (has_errors) {
        return 1;
    } else {
        return 0;
    }
}

int
twitc_parse_reply(char *reply, char **token, char **secret)
{
  int success = 1;
  char **rv = NULL;
  int rc;

  rc = oauth_split_url_parameters(reply, &rv);
  qsort(rv, rc, sizeof(char *), oauth_cmpstringp);

  if (rc >= 1) {
    success = 0;
    /*printf("rv[0]: %s\n", rv[0]);*/
    /*printf("rv[1]: %s\n", rv[1]);*/
    /*printf("rv[2]: %s\n", rv[2]);*/
    if (!strncmp(rv[0], "oauth_token=", 11)
        && !strncmp(rv[1], "oauth_token_secret=", 18)) {
        if (token) {
            *token = strdup(&(rv[0][12]));
        }
        if (secret) {
            *secret = strdup(&(rv[1][19]));
        }
    } else if (!strncmp(rv[1], "oauth_token=", 11)
        && !strncmp(rv[2], "oauth_token_secret=", 18)) {
        if (token) {
            *token = strdup(&(rv[1][12]));
        }
        if (secret) {
            *secret = strdup(&(rv[2][19]));
        }
    }
  }

  if (rv) { free(rv); }

  return success;
}
