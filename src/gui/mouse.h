#ifndef _GUI_MOUSE_H
#define _GUI_MOUSE_H

#include <stdbool.h>

#include "event.h"
#include "gui.h"

/*
 * Returns whether or not the current mouse coordinates are inside an element.
 */
extern inline bool mouse_event_is_inside(GUI* element, MouseData* event) {
    // x and y coordinates of far side of element (i.e. bottom right corner)
    int element_rx = element->x + element->width,
        element_ry = element->y + element->height;

    return (element->x <= event->x && element->y <= event->y &&
            element_rx > event->x && element_ry > event->y);
}

/*
 * Returns whether or not the last mouse coordinates were inside an element.
 */
extern inline bool mouse_event_was_inside(GUI* element, MouseData* event) {
    // x and y coordinates of far side of element (i.e. bottom right corner)
    int element_rx = element->x + element->width,
        element_ry = element->y + element->height;

    return (element->x <= event->lastx && element->y <= event->lasty &&
            element_rx > event->lastx && element_ry > event->lasty);
}

#endif
