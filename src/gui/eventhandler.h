/*
 * Provides functions that allow event triggers and handlers to be registered.
 */
#ifndef _GUI_EVENTHANDLER_H
#define _GUI_EVENTHANDLER_H

#include <stdbool.h>

#include "gui.h"
#include "event.h"


/*
 * Creates a new, empty array of event listeners for a GUI element.
 */
EventListenerCollection* GUI_new_listener_coll();

/*
 * Does the opposite of above...
 */
void GUI_free_listener_coll(EventListenerCollection* coll);

/*
 * Appends the supplied event handler to the event handler array for the
 * supplied event. Returns false if the event handler could not be added.
 */
bool GUI_when(GUI* element, EventType type, EventHandler handler);

/*
 * Called by external code when it detects conditions where a GUI event should
 * be triggered.
 *
 * The root element is the element that initially received the event, such as a
 * button receiving a button click event. It is then up to that element to
 * to decide whether or not to propagate the event by calling this function
 * again on its children.
 *
 * Returns a boolean stating whether or not any event handlers were found and
 * fired.
 */
bool GUI_trigger(GUI* root, EventType event);

#endif
