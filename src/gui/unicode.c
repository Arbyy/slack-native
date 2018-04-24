#include <string.h>
#include <stdint.h>

#include "unicode.h"

void utf8_next_char(const char* str, char* buf, const char** saveptr) {
    // Determine whether or not this is the first call or a continuation
    const char* strptr;
    if (str == NULL)
        strptr = *saveptr;
    else {
        strptr = str;
        *saveptr = str;
    }

    // Prepare return string by filling it with zeroes
    memset(buf, 0, 5);

    // If the string is empty (i.e. we've finished finding chars, bail early)
    if (strptr[0] == '\0')
        return;

    // We're going to reuse utf8_char_to_cp() as a byte counter
    uint32_t code_point = utf8_char_to_cp(strptr);

    // Copy character to return buffer and increment saveptr
    unsigned int bytes =
        code_point < 0x80 ? 1 :
        code_point < 0x800 ? 2 :
        code_point < 0x10000 ? 3 : 4;

    strncpy(buf, strptr, bytes);
    *saveptr += bytes;
}

uint32_t utf8_char_to_cp(const char* u8) {
    const unsigned char* u8c = (unsigned char*) u8;
    unsigned char lb = u8c[0];

    if ((lb & 0x80) == 0)         // Single byte (ASCII) character
        return lb & 0x7F;
    else if ((lb & 0xE0) == 0xC0) // Two byte char
        return (lb & 0x1F) << 6 | (u8c[1] & 0x3F);
    else if ((lb & 0xF0) == 0xE0) // Three bytes
        return (lb & 0xF) << 12 | (u8c[1] & 0x3F) << 6 | (u8c[2] & 0x3F);
    else if ((lb & 0xF8) == 0xF0) // Four bytes
        return (lb & 0x7) << 18 | (u8c[1] & 0x3F) << 12 |
            (u8c[2] & 0x3F) << 6 | (u8c[3] & 0x3F);
    else
        return 0;
}
