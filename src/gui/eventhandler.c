#include "eventhandler.h"


EventListenerCollection* GUI_new_listener_coll() {
    EventListenerCollection* coll;
    if ((coll = malloc(sizeof(EventListenerCollection))) == NULL)
        return NULL;

    coll->count = 0;
    coll->listeners = NULL;

    return coll;
}
