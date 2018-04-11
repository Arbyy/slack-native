#ifndef _UTILS_H
#define _UTILS_H

#include <stddef.h>

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
 * Creates an array of SlacknetURLParameters from an array of strings. This
 * works by taking in alternating parameter names and parameter values. For
 * example, the input ["param1", "value1", "param2", "value2"] would produce
 * an array of two SlacknetURLParameters where the 'name' fields would be
 * "param1" and "param2" respectively, and the data fields would be "value1"
 * and "value2" respectively.
 *
 * This function will return NULL if the params_size variable is an odd number.
 */
SlacknetURLParameter* slacknet_create_param_array(char** params, size_t params_size);

/*
 * A function that will add standard URL GET parameters to a base URL. It
 * accepts the original URL, as well as an array of SlacknetURLParameters.
 */
char* slacknet_paramaterize_url(char* origurl,
                                SlacknetURLParameter params[],
                                size_t params_size);

#endif
