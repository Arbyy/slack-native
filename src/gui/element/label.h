#ifndef _GUI_ELEMENT_LABEL_H
#define _GUI_ELEMENT_LABEL_H

#include <SDL2/SDL_ttf.h>

typedef struct label_data_t {
    char* label;
    TTF_Font* font;
} LabelData;

/*
 * Creates a new label element. By default, uses the default label font set by
 * `GUI_label_set_default_font`, unless `GUI_label_set_font` has been called on
 * the returned instance from this function.
 *
 * A default font must be set before any labels are created.
 */
GUI* GUI_make_label(int x, int y, int width, int height, char* label);

/*
 * Sets the font instance that all labels will use by default unless they have
 * had their font set to something else.
 */
void GUI_label_set_default_font(TTF_Font* font);

/*
 * Sets a custom font for a single label, separate from the default font. The
 * font is not freed when any label using it is destroyed, so it must be freed
 * manually.
 */
void GUI_label_set_font(GUI* label, TTF_Font* font);

#endif
