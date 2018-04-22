#include "channels.h"
#include "utils.h"

CURLcode slacknet_channels_archive(char* token, char* channel, cJSON** retval) {
    char* params[] = {
        "token",
        token,
        "channel",
        channel
    };

    SlacknetURLParameter* paramarray = slacknet_create_param_array(params, 4);
    // The size is 2 because an create_param_array will reduce an array of 4
    // strings into 2 individual URL parameters.
    char* paramstr = slacknet_format_params_array(paramarray, 2);

    SlacknetDataBuffer data = {0};
    CURLcode res = slacknet_send_post("https://slack.com/api/channels.archive",
                                      paramstr,
                                      &data);

    *retval = cJSON_Parse(data.buffer);

    free(paramarray);
    free(paramstr);
    
    return res;
}

CURLcode
slacknet_channels_create(char* token, char* name,
                         bool validate, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "name", name);
    if (validate) {
        cJSON_AddBoolToObject(params, "validate", validate);
    }

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.create",
                                token,
                                params,
                                &data);

    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);
    
    return res;
}

SlacknetChannelsHistoryArgs
slacknet_channels_history_init_args(char* token, char* channel) {
    SlacknetChannelsHistoryArgs retval;
    retval.token = token;
    retval.channel = channel;
    retval.count = _SLACKNET_CHANNELS_HISTORY_COUNT_DEFAULT;
    retval.inclusive = _SLACKNET_CHANNELS_HISTORY_INCLUSIVE_DEFAULT;
    retval.latest = _SLACKNET_CHANNELS_HISTORY_LATEST_DEFAULT;
    retval.oldest = _SLACKNET_CHANNELS_HISTORY_OLDEST_DEFAULT;
    retval.unreads = _SLACKNET_CHANNELS_HISTORY_UNREADS_DEFAULT;
    return retval;
}

CURLcode
slacknet_channels_history(SlacknetChannelsHistoryArgs* params, cJSON** retval) {
    // Start at 2 because the 'token' and 'channel' arguments are required 
    int count = 2;

    if (params->count != _SLACKNET_CHANNELS_HISTORY_COUNT_DEFAULT) {
        count++;
    }
    if (params->inclusive != _SLACKNET_CHANNELS_HISTORY_INCLUSIVE_DEFAULT) {
        count++;
    }
    if (params->latest != _SLACKNET_CHANNELS_HISTORY_LATEST_DEFAULT) {
        count++;
    }
    if (params->oldest != _SLACKNET_CHANNELS_HISTORY_OLDEST_DEFAULT) {
        count++;
    }
    if (params->unreads != _SLACKNET_CHANNELS_HISTORY_UNREADS_DEFAULT) {
        count++;
    }

    // Count * 2 because we have to allow for the key name as well as the values
    char** paramarray = malloc((count*2) * sizeof(char*));
    paramarray[0] = "token";
    paramarray[1] = params->token;
    paramarray[2] = "channel";
    paramarray[3] = params->channel;

    int counter = 4;
    if (params->count != _SLACKNET_CHANNELS_HISTORY_COUNT_DEFAULT) {
        paramarray[counter] = "count";
        counter++;

        int length = snprintf(NULL, 0, "%d", params->count);
        char str[length + 1];
        snprintf(str, length + 1, "%d", params->count);
        paramarray[counter] = str;
        counter++;
    }
    if (params->inclusive != _SLACKNET_CHANNELS_HISTORY_INCLUSIVE_DEFAULT) {
        paramarray[counter] = "inclusive";
        counter++;

        if (params->inclusive) {
            paramarray[counter] = "1";
        } else {
            paramarray[counter] = "0";
        }
        counter++;
    }
    if (params->latest != _SLACKNET_CHANNELS_HISTORY_LATEST_DEFAULT) {
        paramarray[counter] = "latest";
        counter++;

        int length = snprintf(NULL, 0, "%ld", params->latest);
        char str[length + 1];
        snprintf(str, length + 1, "%ld", params->latest);
        paramarray[counter] = str;
        counter++;
    }
    if (params->oldest != _SLACKNET_CHANNELS_HISTORY_OLDEST_DEFAULT) {
        paramarray[counter] = "oldest";
        counter++;

        int length = snprintf(NULL, 0, "%ld", params->oldest);
        char str[length + 1];
        snprintf(str, length + 1, "%ld", params->oldest);
        paramarray[counter] = str;
        counter++;
    }
    if (params->unreads != _SLACKNET_CHANNELS_HISTORY_UNREADS_DEFAULT) {
        paramarray[counter] = "unreads";
        counter++;

        if (params->unreads) {
            paramarray[counter] = "1";
        } else {
            paramarray[counter] = "0";
        }
        counter++;
    }

    SlacknetURLParameter* urlarray =
        slacknet_create_param_array(paramarray, count*2);
    char* finalurl =
        slacknet_paramaterize_url("https://slack.com/api/channels.history",
                                  urlarray, count);

    SlacknetDataBuffer data = {0};
    CURLcode res = slacknet_send_get(finalurl, &data);
    *retval = cJSON_Parse(data.buffer);

    free(urlarray);
    free(finalurl);
    
    return res;
}

CURLcode
slacknet_channels_info(char* token, char* channel, bool lcle, cJSON** retval) {
    char* localeval;
    if (lcle) {
        localeval = "1";
    } else {
        localeval = "0";
    }
    
    char* paramarray[] = {
        "token",
        token,
        "channel",
        channel,
        "include_locale",
        localeval
    };

    SlacknetURLParameter* urlarray = slacknet_create_param_array(paramarray, 6);
    char* url = slacknet_paramaterize_url("https://slack.com/api/channels.info",
                                          urlarray, 3);

    SlacknetDataBuffer data = {0};
    CURLcode res = slacknet_send_get(url, &data);
    *retval = cJSON_Parse(data.buffer);

    free(urlarray);
    free(url);
    
    return res;
}

CURLcode
slacknet_channels_invite(char* token, char* chnl, char* usr, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "channel", chnl);
    cJSON_AddStringToObject(params, "user", usr);

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.invite",
                                token, params, &data);
    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);

    return res;
}

CURLcode
slacknet_channels_join(char* token, char* name, bool validate, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "name", name);
    if (validate) {
        cJSON_AddBoolToObject(params, "validate", validate);
    }

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.join",
                                token, params, &data);
    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);

    return res;
}

CURLcode
slacknet_channels_kick(char* token, char* channel, char* user, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "channel", channel);
    cJSON_AddStringToObject(params, "user", user);

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.kick",
                                token, params, &data);

    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);

    return res;
}

CURLcode slacknet_channels_leave(char* token, char* channel, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "channel", channel);

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.leave",
                                token, params, &data);

    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);

    return res;
}

SlacknetChannelsListArgs slacknet_channels_list_init_args(char* token) {
    SlacknetChannelsListArgs retval;
    retval.token = token;
    retval.cursor = _SLACKNET_CHANNELS_LIST_CURSOR_DEFAULT;
    retval.exclude_archived = _SLACKNET_CHANNELS_LIST_EXCLUDE_ARCHIVED_DEFAULT;
    retval.exclude_members = _SLACKNET_CHANNELS_LIST_EXCLUDE_MEMBERS_DEFAULT;
    retval.limit = _SLACKNET_CHANNELS_LIST_LIMIT_DEFAULT;
    return retval;
}

CURLcode
slacknet_channels_list(SlacknetChannelsListArgs* params, cJSON** retval) {
    // Start at 1 because the 'token' argument is required
    int count = 1;

    if (params->cursor != _SLACKNET_CHANNELS_LIST_CURSOR_DEFAULT) {
        count++;
    }
    if (params->exclude_archived !=
        _SLACKNET_CHANNELS_LIST_EXCLUDE_ARCHIVED_DEFAULT) {
        count++;
    }
    if (params->exclude_members !=
        _SLACKNET_CHANNELS_LIST_EXCLUDE_MEMBERS_DEFAULT) {
        count++;
    }
    if (params->limit != _SLACKNET_CHANNELS_LIST_LIMIT_DEFAULT) {
        count++;
    }

    // Count * 2 because we have to allow for the key name as well as the values
    char* paramarray[(count*2) * sizeof(char*)];
    paramarray[0] = "token";
    paramarray[1] = params->token;

    int counter = 2;
    if (params->cursor != _SLACKNET_CHANNELS_LIST_CURSOR_DEFAULT) {
        paramarray[counter] = "cursor";
        counter++;

        paramarray[counter] = params->cursor;
        counter++;
    }
    if (params->exclude_archived !=
        _SLACKNET_CHANNELS_LIST_EXCLUDE_ARCHIVED_DEFAULT) {
        paramarray[counter] = "exclude_archived";
        counter++;

        if (params->exclude_archived) {
            paramarray[counter] = "1";
        } else {
            paramarray[counter] = "0";
        }
    }
    if (params->exclude_members !=
        _SLACKNET_CHANNELS_LIST_EXCLUDE_MEMBERS_DEFAULT) {
        paramarray[counter] = "exclude_members";
        counter++;

        if (params->exclude_members) {
            paramarray[counter] = "1";
        } else {
            paramarray[counter] = "0";
        }
    }
    if (params->limit != _SLACKNET_CHANNELS_LIST_LIMIT_DEFAULT) {
        paramarray[counter] = "limit";
        counter++;

        int length = snprintf(NULL, 0, "%d", params->limit);
        char str[length + 1];
        snprintf(str, length + 1, "%d", params->limit);
        paramarray[counter] = str;
        counter++;
    }

    SlacknetURLParameter* urlarray =
        slacknet_create_param_array(paramarray, count*2);
    char* finalurl =
        slacknet_paramaterize_url("https://slack.com/api/channels.list",
                                  urlarray, count);

    SlacknetDataBuffer data = {0};
    CURLcode res = slacknet_send_get(finalurl, &data);
    *retval = cJSON_Parse(data.buffer);

    free(urlarray);
    free(finalurl);

    return res;
}

CURLcode
slacknet_channels_mark(char* token, char* channel, time_t ts, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "channel", channel);
    int length = snprintf(NULL, 0, "%ld", ts);
    char str[length + 1];
    snprintf(str, length + 1, "%ld", ts);
    cJSON_AddStringToObject(params, "ts", str);

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.mark",
                                token, params, &data);

    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);

    return res;
}

CURLcode
slacknet_channels_rename(char* token, char* channel, char* name,
                         bool validate, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "channel", channel);
    cJSON_AddStringToObject(params, "name", name);
    if (validate) {
        cJSON_AddBoolToObject(params, "validate", validate);
    }

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.rename",
                                token, params, &data);

    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);

    return res;
}

CURLcode
slacknet_channels_replies(char* token, char* chnl, time_t ts, cJSON** retval) {
    int length = snprintf(NULL, 0, "%ld", ts);
    char tsstring[length + 1];
    snprintf(tsstring, length + 1, "%ld", ts);

    char* paramarray[] = {
        "token",
        token,
        "channel",
        chnl,
        "thread_ts",
        tsstring
    };

    SlacknetURLParameter* urlarray = slacknet_create_param_array(paramarray, 6);
    char* url = slacknet_paramaterize_url("https://slack.com/api/channels.replies",
                                          urlarray, 3);

    SlacknetDataBuffer data = {0};
    CURLcode res = slacknet_send_get(url, &data);
    *retval = cJSON_Parse(data.buffer);

    free(urlarray);
    free(url);

    return res;
}

CURLcode
slacknet_channels_set_purpose(char* token, char* channel,
                              char* purpose, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "channel", channel);
    cJSON_AddStringToObject(params, "purpose", purpose);

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.setPurpose",
                                token, params, &data);

    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);

    return res;
}

CURLcode
slacknet_channels_set_topic(char* token, char* channel,
                            char* topic, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "channel", channel);
    cJSON_AddStringToObject(params, "topic", topic);

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.setTopic",
                                token, params, &data);

    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);

    return res;
}

CURLcode
slacknet_channels_unarchive(char* token, char* channel, cJSON** retval) {
    cJSON* params = cJSON_CreateObject();

    cJSON_AddStringToObject(params, "channel", channel);

    SlacknetDataBuffer data = {0};
    CURLcode res =
        slacknet_send_post_json("https://slack.com/api/channels.unarchive",
                                token, params, &data);

    *retval = cJSON_Parse(data.buffer);

    cJSON_Delete(params);

    return res;
}
