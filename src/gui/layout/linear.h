/*
 * This layout manager lays out each successive element in a line, either
 * horizontally or vertically, depending on the alignment specified.
 *
 * Each element's width or height, whichever is the opposite of the alignment
 * axis, is set to 100% of the parent frame's dimension.
 *
 * For example, for a vertically aligned linear layout, each element would be
 * laid out underneath the elements added before it, and each element's width
 * would be 100% of the frame's width. Each element's height is whatever it has
 * specified.
 */
#ifndef _GUI_LAYOUT_LINEAR_H
#define _GUI_LAYOUT_LINEAR_H

#include "../gui.h"

typedef enum linear_alignment_t {
    HORIZONTAL,
    VERTICAL
} LinearAlignment;

/*
 * Added to the *aux field of a frame to store the alignment the paint function
 * should use when rendering.
 */
typedef struct linear_layout_data_t {
    LinearAlignment alignment;
} LinearLayoutData;

/*
 * Alters the paint function of a frame, as well as adding a tiny data structure
 * to *aux which tells the paint function what alignment to use when rendering.
 *
 * Attaches to the resize event, so that the non-aligned axis of children can be
 * maximised automatically.
 */
GUI* GUI_linear_layout(GUI* frame, LinearAlignment alignment);

#endif
