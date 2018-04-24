#ifndef _GUI_UNICODE_H
#define _GUI_UNICODE_H

#include <stdint.h>

/**
 * utf8_next_char() - Extractx unicode characters from a string
 * @str:        The string to iterate over
 * @buf:        The char buffer to store chars in (should be 5 chars long)
 * @saveptr:    A pointer to an unused char pointer, used internally
 *
 * This function inserts into @buf a string representing a single UTF-8 encoded
 * character each time it is called, or NULL if the end of the string has been
 * reached.
 *
 * This function works on the same principle as the POSIX function strtok_r(),
 * where on the first call, str and saveptr should be provided, and on
 * subsequent invocations on the same string, only saveptr should be passed in.
 *
 * This system makes this function reentrant, and it will work as long as the
 * string that was initially passed in is not freed or moved between calls.
 *
 * NOTE: This function assumes that the string is correct (i.e. no null
 * terminators / string ending in the middle of multibyte characters).
 */
void utf8_next_char(const char* str, char* buf, const char** saveptr);

/**
 * utf8_char_to_cp() - Converts a single UTF-8 character to a code point
 * @u8:         The character (as a char array)
 *
 * Takes a single UTF-8 character (as a byte string), and converts it to a
 * Unicode character point. This character point can then be used in
 * conversions to other Unicode representations as an intermediate value.
 *
 * Returns 0 if the lead byte is unrecognised.
 */
uint32_t utf8_char_to_cp(const char* u8);

#endif
