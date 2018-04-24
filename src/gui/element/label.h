#ifndef _GUI_ELEMENT_LABEL_H
#define _GUI_ELEMENT_LABEL_H

#include <SDL2/SDL_ttf.h>

#include "../text.h"
#include "../gui.h"

typedef struct label_data_t {
    char* label;
    FontFamily* family;
    TextCollection* coll;
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
 * Sets the font family that all labels will use by default unless they have had
 * their font family set to something else.
 */
void GUI_label_set_default_font_family(FontFamily* family);

/*
 * Sets a custom font family for a single label, separate from the default font
 * family. The family is not freed when any label using it is destroyed, so it
 * must be freed manually after all labels that use it have been destroyed.
 */
void GUI_label_set_font_family(GUI* label, FontFamily* family);

#endif
