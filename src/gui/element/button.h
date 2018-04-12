#ifndef _GUI_ELEMENT_BUTTON_H
#define _GUI_ELEMENT_BUTTON_H

#include <stdbool.h>

#include "../gui.h"

typedef struct button_data_t {
    char* label;
    bool mouseover, mousedown; /* Controls rendering effects */
} ButtonData;

/*
 * Creates a new button element.
 *
 * By default, the button has event listeners for `MOUSE_ENTER`, `MOUSE_EXIT`
 * and `CLICKED`, which change the way that the button appears for the next
 * render.
 *
 * This function copies `label`, so if the argument passed to this is
 * dynamically allocated, it will need to be freed manually, outside of this
 * function.
 */
GUI* GUI_make_button(int x, int y, int width, int height, char* label);

#endif
