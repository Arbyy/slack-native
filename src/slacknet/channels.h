#ifndef _CHANNELS_H
#define _CHANNELS_H

#include "../cJSON/cJSON.h"
#include <curl/curl.h>
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>

/*
 * Calls the channels.archive Slack API method with the supplied token and
 * channel id. The returned JSON will be stored in the supplied retval, and the
 * CURLcode resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.archive
 */ 
CURLcode slacknet_channels_archive(char* token, char* channel, cJSON** retval);

/*
 * Calls the channels.create Slack API method with the supplied token, channel
 * name and (the optional) validate flag. The returned JSON will be stored in
 * the supplied retval, and the CURLcode resulting from the CURL call will be
 * returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.create
 */ 
CURLcode
slacknet_channels_create(char* token, char* name,
                         bool validate, cJSON** retval);

/*
 * A data structure to hold all of the arguments for the channels.history Slack
 * API method. More information is on the documentation for the associated
 * function: slacknet_channels_history.
 */
typedef struct slacknet_channels_history_args_t {
    char* token;
    char* channel;
    int count; // Optional
    bool inclusive; // Optional
    time_t latest; // Optional
    time_t oldest; // Optional
    bool unreads; // Optional
} SlacknetChannelsHistoryArgs;

/*
 * The default values for the optional fields of the SlacknetChannelsHistoryArgs
 * struct. These default values are lifted from the API page of the
 * channels.history method. The value for 'latest' had to be a magic value
 * because a constant can't represent the current time.
 */ 
static const int _SLACKNET_CHANNELS_HISTORY_COUNT_DEFAULT = 100;
static const bool _SLACKNET_CHANNELS_HISTORY_INCLUSIVE_DEFAULT = false;
static const time_t _SLACKNET_CHANNELS_HISTORY_LATEST_DEFAULT = 0x1337;
static const time_t _SLACKNET_CHANNELS_HISTORY_OLDEST_DEFAULT = 0;
static const bool _SLACKNET_CHANNELS_HISTORY_UNREADS_DEFAULT = false;

/*
 * A basic function that initializes a SlacknetChannelsHistoryArgs struct with
 * its default values.
 */
SlacknetChannelsHistoryArgs
slacknet_channels_history_init_args(char* token, char* channel);

/*
 * Calls the channels.history Slack API method with the supplied args data
 * structure. The optional fields will be omitted from the request if left as
 * their defaults (therefore using the defaults as described on the Slack API).
 * The returned JSON will be stored in the supplied retval, and the CURLcode
 * resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.history
 */ 
CURLcode
slacknet_channels_history(SlacknetChannelsHistoryArgs* params, cJSON** retval);

/*
 * Calls the channels.info Slack API method with the supplied token, channel and
 * (the optional) include_locale arguments. The returned JSON will be stored in
 * the supplied retval, and the CURLcode resulting from the CURL call will be
 * returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.info
 */ 
CURLcode
slacknet_channels_info(char* token, char* channel, bool lcle, cJSON** retval);

/*
 * Calls the channels.invite Slack API method with the supplied token, channel
 * and user arguments. The returned JSON will be stored in the supplied retval,
 * and the CURLcode resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.invite
 */ 
CURLcode
slacknet_channels_invite(char* token, char* chnl, char* usr, cJSON** retval);

/*
 * Calls the channels.join Slack API method with the supplied token, name and
 * (the optional) validate arguments. The returned JSON will be stored in the
 * supplied retval, and the CURLcode resulting from the CURL call will be
 * returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.join
 */ 
CURLcode
slacknet_channels_join(char* token, char* name, bool validate, cJSON** retval);

/*
 * Calls the channels.kick Slack API method with the supplied token, channel and
 * the user. The returned JSON will be stored in the supplied retval, and the
 * CURLcode resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.kick
 */
CURLcode
slacknet_channels_kick(char* token, char* channel, char* user, cJSON** retval);

/*
 * Calls the channels.leave Slack API method with the supplied token and
 * channel. The returned JSON will be stored in the supplied retval, and the
 * CURLcode resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.leave
 */ 
CURLcode slacknet_channels_leave(char* token, char* channel, cJSON** retval);

/*
 * A data structure to hold all of the arguments for the channels.list Slack API
 * method. More information is on the documentation for the associated function:
 * slacknet_channels_list.
 */
typedef struct slacknet_channels_list_args_t {
    char* token;
    char* cursor; // Optional
    bool exclude_archived; // Optional
    bool exclude_members; // Optional
    int limit; // Optional
} SlacknetChannelsListArgs;

/*
 * The default values for the optional fields of the SlacknetChannelsHistoryArgs
 * struct. These default values are lifted from the API page of the
 * channels.history method. The value for 'latest' had to be a magic value
 * because a constant can't represent the current time.
 */
static char* _SLACKNET_CHANNELS_LIST_CURSOR_DEFAULT = "default_val";
static const bool _SLACKNET_CHANNELS_LIST_EXCLUDE_ARCHIVED_DEFAULT = false;
static const bool _SLACKNET_CHANNELS_LIST_EXCLUDE_MEMBERS_DEFAULT = false;
static const int _SLACKNET_CHANNELS_LIST_LIMIT_DEFAULT = 20;

/*
 * A basic function that initializes a SlacknetChannelsListArgs struct with its
 * default values.
 */
SlacknetChannelsListArgs slacknet_channels_list_init_args(char* token);

/*
 * Calls the channels.list Slack API method with the supplied args data
 * structure. The optional fields will be omitted from the request if left as
 * their defaults (therefore using the defaults as described on the Slack API).
 * The returned JSON will be stored in the supplied retval, and the CURLcode
 * resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.list
 */ 
CURLcode
slacknet_channels_list(SlacknetChannelsListArgs* params, cJSON** retval);

/*
 * Calls the channels.mark Slack API method with the supplied token, channel,
 * and ts. The returned JSON will be stored in the supplied retval, and the
 * CURLcode resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.mark
 */
CURLcode
slacknet_channels_mark(char* token, char* channel, time_t ts, cJSON** retval);

/*
 * Calls the channels.rename Slack API method with the supplied token, channel,
 * name and (the optional) validate flag. The returned JSON will be stored in
 * the supplied retval, and the CURLcode resulting from the CURL call will be
 * returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.rename
 */ 
CURLcode
slacknet_channels_rename(char* token, char* channel, char* name,
                         bool validate, cJSON** retval);

/*
 * Calls the channels.replies Slack API method with the supplied token, channel,
 * and thread_ts arguments. The returned JSON will be stored in the supplied
 * retval, and the CURLcode resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.replies
 */ 
CURLcode
slacknet_channels_replies(char* token, char* chnl, time_t ts, cJSON** retval);

/*
 * Calls the channels.setPurpose Slack API method with the supplied token,
 * channel and purpose. The returned JSON will be stored in the supplied retval,
 * and the CURLcode resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.setPurpose
 */ 
CURLcode
slacknet_channels_set_purpose(char* token, char* channel,
                              char* purpose, cJSON** retval);

/*
 * Calls the channels.setTopic Slack API method with the supplied token,
 * channel and topic. The returned JSON will be stored in the supplied retval,
 * and the CURLcode resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.setTopic
 */ 
CURLcode
slacknet_channels_set_topic(char* token, char* channel,
                            char* topic, cJSON** retval);

/*
 * Calls the channels.unarchive Slack API method with the supplied token and
 * channel. The returned JSON will be stored in the supplied retval, and the
 * CURLcode resulting from the CURL call will be returned.
 *
 * For more information on how this API method is used, go to
 * https://api.slack.com/methods/channels.unarchive
 */ 
CURLcode
slacknet_channels_unarchive(char* token, char* channel, cJSON** retval);

#endif
