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
 * Copies the data in `original` into a new struct (make sure to free the return
 * value of this when you're done), and offsets the mouse coordinates so that
 * the position 0, 0 is the very top left of this frame.
 */
static MouseData* generate_offset_mousedata(GUI* this, MouseData* original) {
    MouseData* mdata = malloc(sizeof(MouseData));
    if (mdata == NULL)
        return NULL;

    // Copy supplied event data to new struct
    memcpy(mdata, original, sizeof(MouseData));

    // Create relative coordinates
    mdata->x -= this->x;
    mdata->y -= this->y;
    mdata->lastx -= this->x;
    mdata->lasty -= this->y;

    return mdata;
}


/*
 * Finds all elements that either the current or last mouse position are under,
 * and triggers the `MOUSE_MOVED` event on them, with the position of the
 * frame pointed to by *this subtracted from the mouse coordinates.
 */
static void* mouse_moved(GUI* this, void* data) {
    MouseData* mdata = generate_offset_mousedata(this, (MouseData*) data);

    if (mdata != NULL) {
        GUI* child = this->child;
        while (child != NULL) {
            GUI_trigger_mouse_events(child, mdata);

            child = this->next;
        }
    }

    free(mdata);
}


static void* mouse_clicked(GUI* this, void* data) {
    MouseData* mdata = generate_offset_mousedata(this, (MouseData*) data);

    if (mdata != NULL) {
        GUI* child = this->child;
        while (child != NULL) {
            if (mouse_event_is_inside(child, mdata)) {
                GUI_trigger(child, CLICKED, mdata);
            }

            child = this->next;
        }
    }

    free(mdata);
}


static void* mouse_released(GUI* this, void* data) {
    MouseData* mdata = generate_offset_mousedata(this, (MouseData*) data);

    if (mdata != NULL) {
        GUI* child = this->child;
        while (child != NULL) {
            if (mouse_event_is_inside(child, mdata)) {
                GUI_trigger(child, RELEASED, mdata);
            }

            child = this->next;
        }
    }

    free(mdata);
}


GUI* GUI_simple_layout(GUI* frame) {
    frame->paint = paint;

    GUI_when(frame, MOUSE_MOVED, mouse_moved);
    GUI_when(frame, CLICKED, mouse_clicked);
    GUI_when(frame, RELEASED, mouse_released);

    return frame;
}
