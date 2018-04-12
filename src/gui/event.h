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
    /* Mouse */
    MOUSE_ENTERED,
    MOUSE_MOVED,
    MOUSE_EXITED,
    CLICKED,
    RELEASED,
    /* Keyboard */
    KEY_PRESSED,
    KEY_RELEASED,

    /* Internal use */
    RESIZED
} EventType;

/*
 * All event listeners receive and return void pointers, allowing all event
 * handler types to use the same functions when being passed around.
 */
typedef struct gui_t GUI; /* See gui.h */
typedef void* (*EventHandler)(GUI* this, void* data);

/*
 * A single event handler.
 */
typedef struct event_listener_t EventListener;
struct event_listener_t {
    EventType type;
    EventHandler handler;
    EventListener *next;
};

/*
 * Linked list of event handlers. May one day have extra information here.
 */
typedef struct event_listener_coll_t {
    unsigned int count;
    EventListener *first;
} EventListenerCollection;


/* Data supplied to the void* second argument to `EventHandler`s. */

/*
 * Data supplied for all mouse events, namely : `MOUSE_ENTER`, `MOUSE_EXIT`,
 * `CLICKED`, `RELEASED`.
 */
typedef struct mouse_data_t {
    int x, y;   /* used by all mouse events */
    int button; /* only for `CLICKED`/`RELEASED` */
} MouseData;

/*
 * Data supplied for `RESIZED` events.
 */
typedef struct resize_data_t {
    int width, height; // New max width and height of element
} ResizeData;

#endif
