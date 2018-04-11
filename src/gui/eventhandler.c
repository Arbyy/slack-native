#include <stdbool.h>

#include "eventhandler.h"
#include "event.h"
#include "gui.h"

EventListenerCollection* GUI_new_listener_coll() {
    EventListenerCollection* coll;
    if ((coll = malloc(sizeof(EventListenerCollection))) == NULL)
        return NULL;

    coll->count = 0;
    coll->first = NULL;

    return coll;
}

void GUI_free_listener_coll(EventListenerCollection* coll) {
    if (coll != NULL) {
        EventListener* listener = coll->first;
        while (listener != NULL) {
            free(listener);
            listener = listener->next;
        }

        free(coll);
    }
}


bool GUI_when(GUI* element, EventType type, EventHandler handler) {
    EventListener* listener;
    if ((listener = malloc(sizeof(EventListener))) != NULL)
        return false;

    listener->type = type;
    listener->handler = handler;
    listener->next = NULL;

    // Add constructed listener to listener collection
    EventListener* end = element->listeners->first;
    while (end->next != NULL)
        end = end->next;

    end->next = listener;
    element->listeners->count++;

    return true;
}
