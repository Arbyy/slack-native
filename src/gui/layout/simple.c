#include <string.h>

#include <SDL2/SDL.h>

#include "../eventhandler.h"
#include "../mouse.h"
#include "../gui.h"
#include "simple.h"


/*
 * Loops through all children and just blits them onto the surface of the frame.
 * Obeys the childrens' specified x and y position, but does not modify them in
 * any way to prevent them from overflowing off the screen.
 */
static void paint(GUI* this) {
    // Clear surface
SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, 0xFF, 0xFF, 0xFF));

    SDL_Rect* dest = malloc(sizeof(SDL_Rect));

    if (dest != NULL) {
        GUI* child = this->child;
        while (child != NULL) {
            // Literally just stick the GUI element where it tells us to
            dest->x = child->x;
            dest->y = child->y;
            dest->w = child->width;
            dest->h = child->height;

            SDL_BlitSurface(child->surface, NULL, this->surface, dest);

            child = child->next;
        }
    }

    this->dirty = false;
}


/*
 * Finds all elements that either the current or last mouse position are under,
 * and triggers the `MOUSE_MOVED` event on them, with the position of the
 * frame pointed to by *this subtracted from the mouse coordinates.
 */
static void* mouse_moved(GUI* this, void* data) {
    MouseData* mdata = malloc(sizeof(MouseData));

    if (mdata != NULL) {
        // Copy supplied event data to new struct
        memcpy(mdata, data, sizeof(MouseData));

        // Create relative coordinates
        mdata->x -= this->x;
        mdata->y -= this->y;
        mdata->lastx -= this->x;
        mdata->lasty -= this->y;

        GUI* child = this->child;
        while (child != NULL) {
            if (mouse_event_is_inside(child, mdata) ||
                mouse_event_was_inside(child, mdata)) {
                GUI_trigger(child, MOUSE_MOVED, mdata);
            }

            if (mouse_event_is_inside(child, mdata) &&
                !mouse_event_was_inside(child, mdata)) {
                GUI_trigger(child, MOUSE_ENTERED, mdata);
            }

            if (mouse_event_was_inside(child, mdata) &&
                !mouse_event_is_inside(child, mdata)) {
                GUI_trigger(child, MOUSE_EXITED, mdata);
            }

            child = this->next;
        }
    }
}


GUI* GUI_simple_layout(GUI* frame) {
    frame->paint = paint;

    GUI_when(frame, MOUSE_MOVED, mouse_moved);

    return frame;
}
