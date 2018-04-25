#ifndef _GUI_FONTFAMILY_H
#define _GUI_FONTFAMILY_H

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct font_family_t {
    unsigned int num;
    TTF_Font** fonts;
} FontFamily;

typedef struct text_collection_t TextCollection;
struct text_collection_t {
    TTF_Font* font;
    char* text;
    TextCollection* next;
};

/**
 * GUI_init_font() - Initialises the font system (decompresses embedded font).
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
bool GUI_init_font(void);

/**
 * GUI_destroy_font() - Destroys all font state and buffers.
 *
 * This cleans up everything to do with the font system, and should only really
 * be called when the program is being closed. No other font functions besides
 * GUI_font_init() should be called after this has been called.
 */
void GUI_destroy_font(void);

/**
 * GUI_create_font_family() - Create a new collection of fonts.
 * @num:        The number of fonts in the following TTF_Font* array
 * @fonts:      An array of TTF_Font* fonts to use
 *
 * A font family defines a group of fonts that should be tried, in order, to
 * to render each glyph in a given string that uses that font family. If a glyph
 * can't be found in any font in the family, then the backup embedded GNU
 * Unifont will be used, which should realistically never have any problems
 * finding the glyph.
 *
 * This function copies the array of pointers to a separate array that is
 * contained within the FontFamily struct. This means that the FontFamily is not
 * reliant on the original pointer array, but is reliant on the fonts that each
 * pointer refers to remaining valid.
 *
 * Note that creating and freeing font families does not handle destroying the
 * fonts that they use.
 *
 * This function returns NULL if any errors occurred.
 */
FontFamily* GUI_create_font_family(unsigned int num, TTF_Font** fonts);

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
 * GUI_collect_text() - Sorts a string into groups drawable with a single font.
 * @family:     The family of fonts to try, in order, for each glyph
 * @text:       The string to split into font groups
 *
 * Groups adjacent characters together that can be drawn with the associated
 * font. Characters will always prefer to be drawn by the font in the family
 * closest to the top of the font stack.
 *
 * If @family is NULL, the text will be collected into a single collection which
 * uses the embedded font (GNU Unifont).
 *
 * By grouping text together this way, it reduces font draw operations
 * (especially when there is only one font needed for the entire text), and it
 * allows similar characters to have correct font kerning applied, by drawing
 * them in one operation, rather than as separate characters.
 */
TextCollection* GUI_collect_text(FontFamily* family, const char* text);

/**
 * GUI_free_text_collection() - Frees all data inside a text collection
 *                              (excluding the fonts themselves).
 * @coll:       The text collection to free
 *
 * Frees a text collection structure, and the text buffer that each collection
 * item contains, but does not do anything with the fonts that each item refers
 * to.
 */
void GUI_free_text_collection(TextCollection* coll);

/**
 * GUI_size_text() - Returns the size of a text collection if rendered on a
 *                   single line.
 * @coll:       The text collection to measure
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
void GUI_size_text_collection(TextCollection* coll,
                              unsigned int* width, unsigned int* height);

/**
 * GUI_draw_text() - Draws the supplied text collection to a new surface.
 * @coll:       The collection of strings to draw
 * @colour:     The colour to draw the string in
 *
 * Draws the supplied @coll to a new SDL_Surface that is the size of the text
 * that it contains. The entire string is drawn on a single line. Due to the
 * mixing and matching of fonts that may occur, different fonts may have
 * different heights. This function just centers each string block vertically in
 * a rather naive attempt to alleviate the issue.
 *
 * The worst case for mixing fonts is where a font family uses a font size that
 * is nearly halfway between two power of two numbers, and a glyph is found that
 * must be rendered using the embedded GNU Unifont. Because the GNU Unifont is a
 * 16pt bitmap font, it is only really suitable to be drawn at PO2 font sizes
 * >=16pt. Therefore, there will be a rather large size mismatch between the
 * fonts in the font family and the Unifont in this case.
 */
SDL_Surface* GUI_draw_text_collection(TextCollection* coll, SDL_Color colour);

#endif
