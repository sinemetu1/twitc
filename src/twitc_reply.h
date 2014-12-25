/**
 * @brief Methods for handling replies from Twitter's api.
 * @file reply.h
 * @author Sam Garrett <samdgarrett@gmail.com>
 */

#ifndef TWITC_REPLY_H
#define TWITC_REPLY_H

#define TWITC_REPLY_USER_KEY       "user"
#define TWITC_REPLY_SN_KEY         "screen_name"
#define TWITC_REPLY_STATUS         "status"
#define TWITC_REPLY_ERRORS         "errors"
#define TWITC_REPLY_ERRORS_MESSAGE "message"

/**
 * Parses a json reply and gets the author.
 *
 * @param contents a char* is expected in json format
 * @return the author
 */
char *twitc_parse_json_author(void *contents);

/**
 * Takes a reply from twitters authentication endpoints and
 * parses the token and secret values.
 *
 * @param reply the reply from the authentication endpoint
 * @param token a token that will be updated with the replied value
 *   NOTE: realloc'd so will need to be freed by caller
 * @param secret a secret that will be updated with the replied value
 *   NOTE: realloc'd so will need to be freed by caller
 * @return 0 == success and 1 == failure.
 */
int twitc_parse_reply(char *reply, char **token, char **secret);

/**
 * Returns 0 or 1 depdending on if the contents contains an error.
 *
 * @param contents a char* is expected in json format
 * @return 0 == success and 1 == failure.
 */
int twitc_reply_has_error(void *contents);

#endif
