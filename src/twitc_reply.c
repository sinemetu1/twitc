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
  char oauth_token_chk[BUFSIZ];
  char oauth_secret_chk[BUFSIZ];
  int oauth_token_len  = 0;
  int oauth_secret_len = 0;
  char *delim = "&";
  char *reply_token;

  snprintf(oauth_token_chk, sizeof(oauth_token_chk),
          "%s=", TWITC_OAUTH_TOKEN);
  snprintf(oauth_secret_chk, sizeof(oauth_secret_chk),
          "%s=", TWITC_OAUTH_TOKEN_SECRET);
  oauth_token_len = strlen(oauth_token_chk) - 1;
  oauth_secret_len = strlen(oauth_secret_chk) - 1;

  reply_token = strtok(reply, delim);
  while (reply_token != NULL) {
      int is_oauth_token =
          strncmp(reply_token, oauth_token_chk, oauth_token_len) == 0;
      int is_oauth_token_secret =
          strncmp(reply_token, oauth_secret_chk, oauth_secret_len) == 0;

      if (is_oauth_token && !is_oauth_token_secret) {
          char *a_token = reply_token + oauth_token_len + 1;
          *token = strdup(a_token);
      } else if (is_oauth_token_secret) {
          char *a_secret = reply_token + oauth_secret_len + 1;
          *secret = strdup(a_secret);
      }

      /* continue with other tokens */
      reply_token = strtok(NULL, delim);
  }

  return success;
}
