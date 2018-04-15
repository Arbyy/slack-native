/*
 * This layout manager has only two children. The layout can either be split
 * horizontally, or vertically.
 *
 * The size of one of the children can be set to be a fixed percentage or pixel
 * value. This means that when the window is resized, it will be constrained to
 * this size, and the other child will expand to fill the rest of the available
 * space.
 *
 * The split can also be set to be adjustable, meaning that the user can drag
 * the divider to resize the two sides. This works by setting the size of the
 * children to a percentage, meaning that when the whole window is resized, the
 * size of the children will scale proportionally.
 */
#ifndef _GUI_LAYOUT_SPLIT_H
#define _GUI_LAYOUT_SPLIT_H

#include <stdbool.h>

#include "../gui.h"

/*
 * Represents whether the two children should be stacked on top of each other,
 * or next to each other.
 */
typedef enum split_axis_t {
    HORIZONTAL,
    VERTICAL
} SplitAxis;

/*
 * Used to reference either the first or second child. Note that top is
 * equivalent to left, and bottom is equivalent to right, as the layout can
 * only be split horizontally or vertically, but not both simultaneously.
 */
typedef enum split_side_t {
    LEFT,
    RIGHT,
    TOP = LEFT,
    BOTTOM = RIGHT
} SplitSide;

/*
 * Used to specify whether or not the fixed child's size is a constant size or
 * a percentage of the total size of the frame.
 */
typedef enum split_size_unit_t {
    PIXELS,
    PERCENT
} SplitSizeUnit;

/*
 * Added to the *aux field of the frame to store the split axis, fixed-size
 * child, fixed-size child size, and whether or not the split size is adjustable
 */
typedef struct split_layout_data_t {
    SplitAxis axis;
    SplitSide fixed_side;
    double fixed_size;
    SplitSizeUnit unit;
    bool adjustable;
} SplitLayoutData;


/*
 * Attaches event listeners to handle resizing children when the frame is
 * resized, in correlation with the size constraints passed to this function.
 *
 * If more than two elements are added to a split layout, the additional items
 * will be ignored.
 *
 * NOTE: adjustable split layouts are not yet implemented.
 */
GUI* GUI_split_layout(GUI* frame, SplitAxis axis, SplitSide fixed_side,
                      double fixed_size, SplitSizeUnit unit, bool adjustable);

#endif
