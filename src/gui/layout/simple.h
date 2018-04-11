/*
 * The simplest layout manager - this simply places elements in wherever their
 * x and y coordinates specify, and doesn't reflow when the container is resized
 * at all.
 *
 * This layout manager is so simple, it isn't really a layout manager at all.
 *
 * If the frame is made larger, the elements will not move, and there will be an
 * empty space to the right/bottom of the elements, and if the frame is made
 * smaller, elements will extend out of view.
 */
#ifndef _GUI_LAYOUT_SIMPLE_H
#define _GUI_LAYOUT_SIMPLE_H

#include "../gui.h"

/*
 * Takes a GUI frame, such as the output from `GUI_make_frame`, and changes the
 * paint function to be layout aware. Returns the input GUI frame to make
 * variable assignment easier.
 */
GUI* GUI_simple_layout(GUI* frame);

#endif
