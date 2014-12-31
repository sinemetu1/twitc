#include "twitc_reply.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oauth.h>
#include <json-c/json.h>

static json_object *
parse_wrapper(char *contents)
{
    json_tokener *tok = json_tokener_new();
    json_object *jobj = NULL;
    int stringlen = 0;
    enum json_tokener_error jerr;

    do {
        stringlen = strlen(contents);
        jobj = json_tokener_parse_ex(tok, contents, stringlen);
    } while ((jerr = json_tokener_get_error(tok)) == json_tokener_continue);

    if (jerr != json_tokener_success)
    {
        fprintf(stderr,
                "Error: %s, couldn't parse: %s\n",
                json_tokener_error_desc(jerr), contents);
    }
    if (tok) { json_tokener_free(tok); }
    return jobj;
}

char *
twitc_parse_json_author(char *contents)
{
    json_object *tweet_obj;
    json_object *user_json;
    json_object *author_json;
    char *temp;
    int length = 0;
    char *author = NULL;

    tweet_obj = parse_wrapper(contents);
    if (json_object_object_get_ex(tweet_obj, TWITC_REPLY_USER_KEY, &user_json)) {
        if (json_object_object_get_ex(user_json, TWITC_REPLY_SN_KEY, &author_json)) {
            temp = (char *) json_object_get_string(author_json);
            length = strlen(temp) + 1;
            author = malloc(length);
            snprintf(author, length, "%s", temp);
        }
    }

    json_object_put(tweet_obj);

    return author;
}

int
twitc_reply_has_error(char *contents)
{
    int ret_has_errors = 0;
    json_object *json_obj;

    json_obj = parse_wrapper(contents);

    if (json_object_object_get_ex(json_obj, TWITC_REPLY_ERRORS, NULL)) {
        ret_has_errors = 1;
    } else {
        ret_has_errors = 0;
    }

    json_object_put(json_obj);

    return ret_has_errors;
}

int
twitc_parse_reply(char *reply, char **token, char **secret)
{
  int success = 1;
  char **rv = NULL;
  int rc;

  rc = oauth_split_url_parameters(reply, &rv);
  qsort(rv, rc, sizeof(char *), oauth_cmpstringp);

  if (rc >= 2) {
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

  if (rv) {
      for (int i = 0; i < rc; i++) {
          free(rv[i]);
      }
      free(rv);
  }

  return success;
}
