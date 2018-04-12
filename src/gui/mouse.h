/*
 * Convenience functions for mouse related event handlers.
 */
#ifndef _GUI_MOUSE_H
#define _GUI_MOUSE_H

#include <stdbool.h>

#include "eventhandler.h"
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

/*
 * Generates `MOUSE_MOVED`, `MOUSE_ENTERED` and `MOUSE_EXITED` events for a
 * supplied element, depending on the received `MouseData`.
 *
 * The supplied MouseData should already have had it's coordinates shifted so
 * that 0, 0 aligns with the top left of the parent container of the element
 * supplied.
 */
extern inline bool GUI_trigger_mouse_events(GUI* element, MouseData* event) {
    if (mouse_event_is_inside(element, event) ||
        mouse_event_was_inside(element, event)) {
        GUI_trigger(element, MOUSE_MOVED, event);
    }

    if (mouse_event_is_inside(element, event) &&
        !mouse_event_was_inside(element, event)) {
        GUI_trigger(element, MOUSE_ENTERED, event);
    }

    if (mouse_event_was_inside(element, event) &&
        !mouse_event_is_inside(element, event)) {
        GUI_trigger(element, MOUSE_EXITED, event);
    }
}

#endif
