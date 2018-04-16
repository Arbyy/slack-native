#ifndef _UTILS_H
#define _UTILS_H

#include "../cJSON/cJSON.h"
#include <stddef.h>
#include <curl/curl.h>

/*
 * A basic data structure to hold the contents of a returned libcurl call.
 * This should fed in as an argument to CURLOPT_WRITEDATA.
 */
typedef struct slacknet_data_buffer_t {
    char* buffer;
    size_t size;
} SlacknetDataBuffer;

/*
 * The standard libcurl CURLOPT_WRITEFUNCTION callback to be used by slacknet
 * code. It assumes you use a SlacknetDataBuffer struct in the 
 * CURLOPT_WRITEDATA option along with this function.
 * 
 * All this function does is receive the incoming data, and append it to the
 * buffer, reallocating a larger size and increasing the size field as it goes.
 * 
 * TODO: Come back to this and make it far more efficient
 */
size_t
slacknet_write_callback(void* ptr, size_t size, size_t nmemb, void* userdata);

/*
 * A basic struct to represent one single URL parameter as it would be used in
 * a GET request.
 */
typedef struct slacknet_url_parameter_t {
    char* name;
    char* data;
} SlacknetURLParameter;

/*
 * Generates a string from an array of SlacknetURLParameters. The string will be
 * formatted such as to typically suit the format expected by URL GET parameters
 * or POST parameters as per RFC-3986.
 *
 * NOTE: The resulting char* from this function is allocated on the heap, and
 * must be freed.
 */ 
char*
slacknet_format_params_array(SlacknetURLParameter params[], size_t params_size);

/*
 * Creates an array of SlacknetURLParameters from an array of strings. This
 * works by taking in alternating parameter names and parameter values. For
 * example, the input ["param1", "value1", "param2", "value2"] would produce
 * an array of two SlacknetURLParameters where the 'name' fields would be
 * "param1" and "param2" respectively, and the data fields would be "value1"
 * and "value2" respectively.
 *
 * This function will return NULL if the params_size variable is an odd number.
 *
 * NOTE: The resulting SlacknetURLParameter* from this function is allocated on
 * the heap, and must be freed (The strings within it are fine, however).
 */
SlacknetURLParameter*
slacknet_create_param_array(char** params, size_t params_size);

/*
 * A function that will add standard URL GET parameters to a base URL. It
 * accepts the original URL, as well as an array of SlacknetURLParameters.
 *
 * NOTE: The resulting char* from this function is allocated on the heap, and
 * must be freed.
 */
char*
slacknet_paramaterize_url(char* origurl,
                          SlacknetURLParameter params[],
                          size_t params_size);

/*
 * A conveniance function that just sends a GET request to that URL. If you wish
 * to use URL paramaters, slacknet_paramterize_url can create a URL with the get
 * paramaters built in. The results of the POST request are put into the data
 * argument.
 *
 * This function returns the CURLcode returned by curl_easy_perform for error
 * detection.
 *
 * NOTE: This function expects that the file "cacert.pem" exists in the same
 * directory as the executable. This is used to verify the SSL certificates
 * presented by websites.
 */ 
CURLcode slacknet_send_get(char* url, SlacknetDataBuffer* data);

/*
 * A conveniance function that just sends a POST request to the URL with
 * those params used. The params can be generated using the function
 * slacknet_format_params_array. The results of the POST request are put
 * into the data argument.
 *
 * The params are URL encoded into the POST request.
 *
 * This function returns the CURLcode returned by curl_easy_perform for error
 * detection.
 *
 * NOTE: This function expects that the file "cacert.pem" exists in the same
 * directory as the executable. This is used to verify the SSL certificates
 * presented by websites.
 */ 
CURLcode slacknet_send_post(char* url, char* params, SlacknetDataBuffer* data);

/*
 * A conveniance function similar to that of slacknet_send_post, however it
 * accepts a cJSON object, and will serialize it before sending it as the body
 * of the POST request.
 *
 * This function also requires the Slack API auth token seperate from the JSON
 * body as per the Slack API rules. More info on why this is can be found on
 * https://api.slack.com/web.
 *
 * The token argument is optional, if it is set to NULL, the Authorization
 * header will be omitted from the request entirely.
 *
 * NOTE: Just like slacknet_send_post, this function assumes the existence of
 * the file "cacert.pem" in the same directory as the executable.
 */ 
CURLcode
slacknet_send_post_json(char* url, char* token, cJSON* params,
                        SlacknetDataBuffer* data);

#endif
