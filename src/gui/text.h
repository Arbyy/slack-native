#ifndef _GUI_FONTFAMILY_H
#define _GUI_FONTFAMILY_H

#include <SDL2/SDL_ttf.h>

typedef struct fontfamily_t {
    unsigned int num;
    TTF_Font** fonts;
} FontFamily;

/**
 * GUI_font_init() - Initialises the font system (decompresses embedded font).
 *
 * This must be called before any other text related functions are called, but
 * xz_crc32_init() and xz_crc64_init() must be called before this is called. If
 * these two functions haven't been called before this is called, the xz library
 * will _always_ think that the archive is corrupt, and error.
 *
 * This function decompresses and loads the font contained in unifont.h as the
 * backup font for all font families (as in it will be used for any glyphs that
 * aren't supplied by any other fonts in the font family).
 *
 * Returns false if the embedded GNU Unifont could not be loaded for any reason.
 */
bool GUI_font_init(void);

/**
 * GUI_font_destroy() - Destroys all font state and buffers.
 *
 * This cleans up everything to do with the font system, and should only really
 * be called when the program is being closed. No other font functions besides
 * GUI_font_init() should be called after this has been called.
 */
void GUI_font_destroy(void);

/**
 * GUI_create_font_family() - Create a new collection of fonts.
 * @fonts:      An array of TTF_Font* fonts to use
 *
 * A font family defines a group of fonts that should be tried, in order, to
 * to render each glyph in a given string that uses that font family. If a glyph
 * can't be found in any font in the family, then the backup embedded GNU
 * Unifont will be used, which should realistically never have any problems
 * finding the glyph.
 *
 * Note that creating and freeing font families does not handle destroying the
 * fonts that they use.
 *
 * This function returns NULL if any errors occurred.
 */
FontFamily* GUI_create_font_family(TTF_Font** fonts);

/**
 * GUI_free_font_family() - Frees a FontFamily structure.
 * @family:     The font family structure to free
 *
 * Frees the supplied font family structure, and its TTF_Font pointer array.
 *
 * Note that creating and freeing font families does not handle destroying the
 * fonts that they use.
 */
void GUI_free_font_family(FontFamily* family);

/**
 * GUI_size_text() - Returns the size of a UTF-8 string, rendered using the
 *                   supplied font family.
 * @family:     The font family to use to check character dimensions
 * @text:       A UTF-8 encoded string containing the text to measure
 * @width:      A pointer to where the returned string width will be stored
 * @height:     A pointer to where the returned string height will be stored
 *
 * Returns the width and height of the supplied text, rendered on a single line,
 * Using the fonts in the supplied font family.
 *
 * If there is an invalid glyph in the string, this function will likely return
 * an inaccurate result, as the TTF_SizeText() function called by this function
 * will return a width and height of -1 for non-existent glyphs.
 */
void GUI_size_text(FontFamily* family, const char* text,
                   unsigned int* width, unsigned int* height);

/**
 * GUI_draw_text() - Draws the supplied UTF-8 encoded string to a new surface.
 * @family:     The font family to use to draw the text
 * @text:       The UTF-8 encoded string containing the text to draw
 *
 * Draws the supplied string to a new SDL_Surface that is the size of the text
 * that it contains. The entire string is drawn on a single line, and the font
 * for each glyph is determined by finding the first font in the family that
 * contains that glyph. This means that a single string could be rendered in
 * multiple fonts, however, this function does make some attempt to line up
 * adjoining characters from different fonts in a way that looks decent.
 *
 * The worst case for mixing fonts is where a font family uses a font size that
 * is nearly halfway between two power of two numbers, and a glyph is found that
 * must be rendered using the embedded GNU Unifont. Because the GNU Unifont is a
 * 16pt bitmap font, it is only really suitable to be drawn at PO2 font sizes
 * >=16pt. Therefore, there will be a rather large size mismatch between the
 * fonts in the font family and the Unifont in this case.
 */
SDL_Surface* GUI_draw_text(FontFamily* family, const char* text);

#endif
