#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../xz/xz_config.h"
#include "../xz/xz.h"

#include "unifont.h"
#include "unicode.h"
#include "text.h"

static uint8_t* unifont_raw;
static TTF_Font* unifont;

bool GUI_init_font(void) {
    // Decompress everything in a single run
    struct xz_dec* unifont_dec = xz_dec_init(XZ_SINGLE, 0);

    // Define size of compressed and extracted data + create buffers
    struct xz_buf unifont_buf;
    unifont_buf.in = _UNIFONT_XZ_ENCODED;
    unifont_buf.in_pos = 0;
    unifont_buf.in_size = _UNIFONT_XZ_SIZE;
    unifont_buf.out = malloc(_UNIFONT_RAW_SIZE);
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

    // Create file-global buffer variable so we can free the buffer later
    unifont_raw = unifont_buf.out;

    // Create a font from the extracted memory contents
    SDL_RWops* unifont_mem = SDL_RWFromMem(unifont_buf.out, _UNIFONT_RAW_SIZE);
    if (unifont_mem == NULL)
        return false;

    unifont = TTF_OpenFontRW(unifont_mem, true, 16);

    // Returns true unless there was a font with TTF_OpenFontRW()
    return unifont != NULL;
}

void GUI_destroy_font(void) {
    TTF_CloseFont(unifont);
    unifont = NULL;

    free(unifont_raw);
    unifont_raw = NULL;
}

FontFamily* GUI_create_font_family(unsigned int num, TTF_Font** fonts) {
    FontFamily* family = malloc(sizeof(FontFamily));
    if (family == NULL)
        return NULL;

    family->num = num;
    family->fonts = malloc(sizeof(TTF_Font*) * num);
    if (family->fonts == NULL) {
        free(family);
        return NULL;
    }

    memcpy(family->fonts, fonts, sizeof(TTF_Font*) * num);

    return family;
}

void GUI_free_font_family(FontFamily* family) {
    free(family->fonts);
    free(family);
}

/*
 * Takes a utf-16 character, and returns a pointer to the first font encountered
 * in the supplied font family (or the backup font) that contains the character.
 *
 * Returns the embedded font if the font family is NULL.
 */
static TTF_Font* get_suitable_font_from_family(FontFamily* family, uint16_t c) {
    if (family == NULL)
        return unifont;

    for (size_t i = 0; i < family->num; i++)
        if (TTF_GlyphIsProvided(family->fonts[i], c))
            return family->fonts[i];

    return unifont;
}

TextCollection* GUI_collect_text(FontFamily* family, const char* text) {
    // The return linked list
    TextCollection *coll = NULL, *end = NULL;

    // Collects all of the adjacent characters that use the same font
    TTF_Font* sf_font = NULL;
    unsigned int sf_buf_size = 256, sf_size = 0;
    char* sf_buf = calloc(sizeof(char), sf_buf_size);
    if (sf_buf == NULL)
        return NULL;

    // Iterates over each UTF-8 character in the string
    char buf[5];
    const char* saveptr;
    utf8_next_char(text, buf, &saveptr);

    while (buf[0] != '\0') {
        // Find the first font that contains the current Unicode character
        TTF_Font* next_font =
            get_suitable_font_from_family(family, utf8_char_to_cp(buf));

        // Create a new entry in the linked list each time we change fonts
        if (next_font != sf_font) {
            if (coll == NULL) {
                // Nothing has been created yet (i.e. this is the first loop)
                coll = malloc(sizeof(TextCollection));
                end = coll;

                coll->next = NULL;
                coll->font = NULL;
                coll->text = NULL;

                sf_font = next_font;
            } else {
                // We are appending a new linked list item
                end->font = sf_font;
                end->text = malloc(sizeof(char) * (sf_size + 1));
                end->text[sf_size] = '\0';
                strncpy(end->text, sf_buf, sf_size);

                memset(sf_buf, 0, sf_buf_size);
                sf_size = 0;
                sf_font = next_font;

                end->next = malloc(sizeof(TextCollection));
                end = end->next;
                end->next = NULL;
                end->font = NULL;
                end->text = NULL;
            }
        }

        // Copy into the temporary "same font" buffer
        strcpy(sf_buf + sf_size, buf);
        sf_size += strlen(buf);

        // Increase size of sf_buf if we are within one char of overflow
        // (also zeroes new section)
        if (sf_buf_size - sf_size <= 5) {
            unsigned int sf_buf_nsize = sf_buf_size * 2;
            sf_buf = realloc(sf_buf, sizeof(char) * 2);
            // TODO check return of realloc for NULL
            memset(sf_buf + sf_buf_size, 0, sf_buf_size);
            sf_buf_size = sf_buf_nsize;
        }

        utf8_next_char(NULL, buf, &saveptr);
    }

    // If we ran into the end of the string, but there is data in the temporary
    // buffer, write it to the end linked list item
    if (sf_size > 0) {
        end->font = sf_font;
        end->text = malloc(sizeof(char) * (sf_size + 1));
        end->text[sf_size] = '\0';
        strncpy(end->text, sf_buf, sf_size);
    }

    free(sf_buf);
    return coll;
}

void GUI_free_text_collection(TextCollection* coll) {
    TextCollection* current = coll;
    while (current != NULL) {
        TextCollection* next = current->next;
        free(current->text);
        free(current);
        current = next;
    }
}

void GUI_size_text_collection(TextCollection* coll,
                              unsigned int* width, unsigned int* height) {
    int twidth = 0, theight = 0;
    *width = 0;
    *height = 0;
    TextCollection* current = coll;
    while (current != NULL) {
        TTF_SizeUTF8(current->font, current->text, &twidth, &theight);

        *width += twidth;
        *height = *height > (unsigned int) theight ?
            *height : (unsigned int) theight;

        current = current->next;
    }
}

SDL_Surface* GUI_draw_text_collection(TextCollection* coll, SDL_Color colour) {
    unsigned int width = 0, height = 0, twidth = 0;
    GUI_size_text_collection(coll, &width, &height);

    SDL_Surface* surf = SDL_CreateRGBSurface(0, width, height, 32,
                                             0xFF000000,
                                             0x00FF0000,
                                             0x0000FF00,
                                             0x000000FF);

    TextCollection* current = coll;
    while (current != NULL) {
        SDL_Surface* text =
            TTF_RenderUTF8_Blended(current->font, current->text, colour);

        if (!text) {
            fprintf(stderr, "Error printing section \"%s\" in text collection:\n%s\n",
                    current->text, TTF_GetError());
            continue; // Let's be optimistic about rendering the remaining text
        }

        // Center text vertically and place to the right of text rendered before
        SDL_Rect dest;
        dest.x = twidth;
        dest.y = (height - text->h) / 2;
        dest.w = text->w;
        dest.h = text->h;

        SDL_BlitSurface(text, NULL, surf, &dest);
        SDL_FreeSurface(text);

        twidth += text->w;

        current = current->next;
    }

    return surf;
}
