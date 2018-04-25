#ifndef _GUI_GUI_H
#define _GUI_GUI_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "event.h"
#include "style.h"

typedef enum element_t {
    FRAME,
    LABEL,
    BUTTON,
    INPUT,
} ElementType;

typedef struct gui_t GUI;
/* Function called on elements to paint them, taking an argument, which is the
   element to paint */
typedef void (*PaintFunc)(GUI* this);
/* Function called when GUI structs have an *aux struct which has fields that
   need freeing (NOTE: auxfree must not free the aux struct itself) */
typedef void (*AuxFreeFunc)(void* aux);

struct gui_t {
    int x,y, width,height;
    ElementType type;
    EventListenerCollection *listeners;

    GUIStyle *style;

    SDL_Surface *surface;

    bool dirty;
    PaintFunc paint;

    GUI *parent, *child, *previous, *next;

    /* Element type specific data */
    AuxFreeFunc auxfree;
    void *aux;
};


/*
 * Frees a GUI element tree (i.e. frees all children as well), and frees the
 * *aux field of GUI elements as well.
 *
 * NOTE: If the *aux field contains any sub-structures that must be freed, this
 * function will not free them. They must be manually freed first. This is
 * _usually_ handled by the elements themselves by adding an auxfree function to
 * the GUI struct.
 */
void GUI_free(GUI* elem);

/*
 * Allocates a GUI structure and allocates or sets to null all fields. This
 * shouldn't ever be needed in user code, and should probably only be used
 * internally. Returns null if anything went wrong.
 */
GUI* GUI_alloc_generic(int width, int height);

/*
 * Creates a GUI* struct, and makes it an element of type FRAME. This function
 * is intended to be used to create the root element that all other GUI elements
 * are placed within, using GUI_add_element.
 *
 * If this function returns a null pointer, it means that the frame construction
 * process failed somewhere.
 */
GUI* GUI_make_frame(int x, int y, int width, int height);

/*
 * Adds a new element as a child of the supplied parent. This works by finding
 * the first child of the parent, then continually searching child->next until
 * the end of the child chain is reached, upon which the new element is added.
 */
void GUI_add_element(GUI* parent, GUI* element);

/*
 * Removes the element from its place amongst the children of its parent. Note
 * that you don't need to specify the parent of the element to remove it. This
 * function also frees the element being removed, along with its children, so
 * be sure to save any information that these elements hold that you need before
 * calling this function.
 */
void GUI_remove_element(GUI* element);

/*
 * Calls the paint function pointer on the supplied element, as well as all
 * children. This should be called by any function that determines that an
 * element's state is dirty.
 *
 * The function works from the bottom up, painting parents after children, until
 * it reaches the element supplied.
 */
void GUI_paint(GUI* element);

/*
 * Called on a root element, this function recursively checks children to see if
 * their `dirty` property has been set to true. If it has, it calls `GUI_paint`
 * on that element, which draws all of its children. If the child `GUI_update`
 * returns true, the parent is also redrawn. In this way, the whole display can
 * be updated while redrawing as few buffers as possible.
 */
bool GUI_update(GUI* root);

/*
 * Called on a freshly constructed GUI, before it is rendered for the first
 * time. Does some basic setup, such as calling some event handlers to allow
 * layouts to work themselves out.
 */
void GUI_prepare(GUI* root);

#endif
