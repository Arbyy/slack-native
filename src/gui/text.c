#include <stdint.h>

#include <SDL2/SDL_ttf.h>
#include "xz/xz_config.h"
#include "xz/xz.h"

#include "unifont.h"
#include "text.h"

static uint8_t* unifont_buf;
static TTF_Font* unifont;

bool GUI_font_init(void) {
    // Decompress everything in a single run
    struct xz_dec* unifont_dec = xz_dec_init(XZ_SINGLE, 0);

    // Define size of compressed and extracted data + create buffers
    unifont_buf = malloc(_UNIFONT_RAW_SIZE);
    if (unifont_buf == NULL)
        return false;

    struct xz_buf unifont_buf;
    unifont_buf.in = _UNIFONT_XZ_ENCODED;
    unifont_buf.in_pos = 0;
    unifont_buf.in_size = _UNIFONT_XZ_SIZE;
    unifont_buf.out = unifont_buf;
    unifont_buf.out_pos = 0;
    unifont_buf.out_size = _UNIFONT_RAW_SIZE;

    // Execute .xz decompression
    if (xz_dec_run(unifont_dec, &unifont_buf) != XZ_STREAM_END) {
        // An error occurred decompressing the font data
        free(unifont_buf.out);
        xz_dec_end(unifont_dec);
        return false;
    }

    xz_dec_end(unifont_dec);

    // Create a font from the extracted memory contents
    SDL_RWops* unifont_mem = SDL_RWFromMem(unifont_buf.out, _UNIFONT_RAW_SIZE);
    if (unifont_mem == NULL)
        return false;

    unifont = TTF_OpenFontRW(unifont_mem, true, 16);

    // Returns true unless there was a font with TTF_OpenFontRW()
    return unifont != NULL;
}
