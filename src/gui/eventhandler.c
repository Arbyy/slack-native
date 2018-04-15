#include <stdbool.h>

#include <SDL2/SDL.h>

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
    if ((listener = malloc(sizeof(EventListener))) == NULL)
        return false;

    listener->type = type;
    listener->handler = handler;
    listener->next = NULL;

    // Add constructed listener to listener collection
    if (element->listeners->first == NULL)
        element->listeners->first = listener;
    else {
        EventListener* end = element->listeners->first;
        while (end->next != NULL)
            end = end->next;

        end->next = listener;
    }

    element->listeners->count++;

    return true;
}


bool GUI_trigger(GUI* root, EventType event, void* data) {
    EventListener* listener = root->listeners->first;
    bool found_any = false;
    while(listener != NULL) {
        if (listener->type == event) {
            listener->handler(root, data);
            found_any = true;
        }

        listener = listener->next;
    }

    return found_any;
}


static int last_mousex = -1, last_mousey = -1;
void GUI_SDL_event_handler(GUI* frame, SDL_Event* event) {
    MouseData data;

    switch (event->type) {
    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        SDL_GetMouseState(&data.x, &data.y);
        data.lastx = last_mousex;
        data.lasty = last_mousey;
        last_mousex = data.x;
        last_mousey = data.y;

        switch (event->type) {
        case SDL_MOUSEMOTION:
            GUI_trigger(frame, MOUSE_MOVED, &data);
            break;
        case SDL_MOUSEBUTTONDOWN:
            GUI_trigger(frame, CLICKED, &data);
            break;
        case SDL_MOUSEBUTTONUP:
            GUI_trigger(frame, RELEASED, &data);
            break;
        }

        break;

    case SDL_WINDOWEVENT:
        switch (event->window.event) {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            frame->width = event->window.data1;
            frame->height = event->window.data2;

            GUI_trigger(frame, RESIZED, NULL);
            break;
        }

        break;
    }
}
