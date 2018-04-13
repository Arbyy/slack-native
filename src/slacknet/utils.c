#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

size_t 
slacknet_write_callback(void* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t realsize = size * nmemb;

    SlacknetDataBuffer* mem = (SlacknetDataBuffer*) userdata;

    mem->buffer = realloc(mem->buffer, mem->size + realsize + 1);

    if (mem->buffer) {
        memcpy(&(mem->buffer[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->buffer[mem->size] = 0;
    }

    return realsize;
}

SlacknetURLParameter*
slacknet_create_param_array(char** params, size_t params_size) {
    if ((params_size%2) != 0) {
        return NULL;
    }

    unsigned int num_params = params_size / 2;
    
    SlacknetURLParameter* retval = malloc(num_params *
                                          sizeof(SlacknetURLParameter));

    bool isname = true;
    SlacknetURLParameter current;
    unsigned int i;
    for (i = 0; i <= params_size - 1; ++i) {
        if (isname) {
            current.name = params[i];
        } else {
            current.data = params[i];

            retval[i/2] = current;

            memset(&current, 0, sizeof(current));
        }
        isname = !isname;
    }
    
    return retval;
}

char*
slacknet_format_params_array(SlacknetURLParameter params[],
                             size_t params_size) {
    // Add (params_size - 1) to account for the '&' between parameters
    unsigned int totalsize = (params_size - 1);
    unsigned int i;
    for (i = 0; i <= params_size - 1; ++i) {
        // Add 1 to account for the '=' between the parameter name and data
        totalsize += (strlen(params[i].name)) + (strlen(params[i].data)) + 1;
    }
    // For the null terminator
    totalsize += 1;

    char* retval = malloc(totalsize);
    memset(retval, 0, totalsize);
    for (i = 0; i <= params_size - 1; ++i) {
        strcat(retval, params[i].name);
        strcat(retval, "=");
        strcat(retval, params[i].data);

        if (i != (params_size - 1)) {
            strcat(retval, "&");
        }
    }

    return retval;
}

char*
slacknet_paramaterize_url(char* origurl,
                          SlacknetURLParameter params[],
                          size_t params_size) {
    size_t origsize = strlen(origurl);
    char* paramstr = slacknet_format_params_array(params, params_size);
    // Add 1 for the '?' that comes before the params
    char* retval = malloc(origsize + strlen(paramstr) + 1);
    strcpy(retval, origurl);
    strcat(retval, "?");
    strcat(retval, paramstr);

    return retval;
}

CURLcode slacknet_send_post(char* url, char* params, SlacknetDataBuffer* data) {
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(params));
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "./cacert.pem");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, slacknet_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)data);
    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return result;
}

CURLcode
slacknet_send_post_json(char* url, char* token, cJSON* params,
                        SlacknetDataBuffer* data) {
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-type: application/json");
    char* authheader;
    if (token) {
        // 23 because that is the size of "Authorization: Bearer " exactly.
        // Add 1 specifically for the null terminator
        authheader = malloc(23 + strlen(token) + 1);
        strcpy(authheader, "Authorization: Bearer ");
        strcat(authheader, token);
        headers = curl_slist_append(headers, authheader);
    }
    headers = curl_slist_append(headers, "charsets: utf-8");

    char* jsonbody = cJSON_PrintUnformatted(params);

    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(jsonbody));
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonbody);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "./cacert.pem");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, slacknet_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)data);
    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    curl_slist_free_all(headers);
    free(jsonbody);
    free(authheader);

    return result;
}
