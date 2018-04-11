/*
 * The underlying functionality of GUI element events. User code should use
 * `eventhandler.h` functions, rather than interacting with this directly.
 */
#ifndef _GUI_EVENT_H
#define _GUI_EVENT_H

/*
 * Event types used both internally and by user code
 */
typedef enum events_t {
    /* External use */
    MOUSE_ENTER,
    MOUSE_EXIT,
    CLICKED,
    KEY_PRESSED,

    /* Internal use */
    RESIZED
} EventType;

/*
 * All event listeners receive and return void pointers, allowing all event
 * handler types to use the same functions when being passed around.
 */
typedef struct gui_t GUI; /* See gui.h */
typedef void* (*EventHandler)(GUI*, void*);

/*
 * A single event handler.
 */
typedef struct event_listener_t {
    EventType type;
    EventHandler handler;
    EventHandler *next;
} EventListener;

/*
 * Linked list of event handlers. May one day have extra information here.
 */
typedef struct event_listener_coll_t {
    unsigned int count;
    EventListener *first;
} EventListenerCollection;

#endif
