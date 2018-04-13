#include "eventhandler.h"
#include "mouse.h"


bool GUI_trigger_mouse_events(GUI* element, MouseData* event) {
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
