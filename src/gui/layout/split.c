#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "../eventhandler.h"
#include "../gui.h"
#include "split.h"


static void paint(GUI* this) {
    // Clear surface (note that split panes have no style)
    SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, 0xFF, 0, 0));

    if (this->child != NULL) {
        // The top / left surface always starts at the top left corner.
        SDL_BlitSurface(this->child->surface, NULL, this->surface, NULL);

        if (this->child->next != NULL) {
            GUI* next = this->child->next;

            SDL_Rect dest = { .w = next->width, .h = next->height };

            // Move second element to end of first
            if (((SplitLayoutData*) this->aux)->axis == HORIZONTAL) {
                dest.x = 0;
                dest.y = this->child->height;
            } else {
                dest.x = this->child->width;
                dest.y = 0;
            }

            SDL_BlitSurface(next->surface, NULL, this->surface, &dest);
        }
    }

    this->dirty = false;
}


static void* resize_children(GUI* this, void* data) {
    SplitLayoutData* sdata = this->aux;

    // Obtain the GUI element that will be set to a fixed size
    GUI* fixed_elem =
        sdata->fixed_side == LEFT ? this->child : this->child->next;
    // Obtain the GUI element that will expand to fill remaining space
    GUI* expanded_elem =
        sdata->fixed_side == LEFT ? this->child->next : this->child;

    // Used to find the size of the expanded element
    int fixed_elem_size, frame_size;

    if (fixed_elem != NULL) {
        // Pointer to the axis that we need to calculate the size of
        int* adjust_axis;

        // Axis parallel with the split is always 100% of the size of the frame
        if (sdata->axis == HORIZONTAL) {
            fixed_elem->width = this->width;
            frame_size = this->height;
            adjust_axis = &fixed_elem->height;
        } else {
            fixed_elem->height = this->height;
            frame_size = this->width;
            adjust_axis = &fixed_elem->width;
        }

        if (sdata->unit == PERCENT) {
            fixed_elem_size = floor((double) frame_size / 100 *
                                    sdata->fixed_size);
            *adjust_axis = fixed_elem_size;
        } else { // units are pixels
            fixed_elem_size = sdata->fixed_size;
            *adjust_axis = fixed_elem_size;
        }

        // Set x and y to 0, as the mouse event handler in this file handles
        // shifting coordinates instead
        fixed_elem->x = 0;
        fixed_elem->y = 0;

        // Make the fixed element resize its children, too
        GUI_trigger(fixed_elem, RESIZED, NULL);
    }

    if (expanded_elem != NULL) {
        // The expanded element gets the remainder of the space (that which
        // wasn't used by the fixed element)
        if (sdata->axis == HORIZONTAL) {
            expanded_elem->width = this->width;
            expanded_elem->height = frame_size - fixed_elem_size;
        } else {
            expanded_elem->height = this->height;
            expanded_elem->width = frame_size - fixed_elem_size;
        }

        // Set x and y to 0, as the mouse event handler in this file handles
        // shifting coordinates instead
        expanded_elem->x = 0;
        expanded_elem->y = 0;

        // Notify the element that it has been resized
        GUI_trigger(expanded_elem, RESIZED, NULL);
    }

    return NULL;
}


/*
 * Tracks down what child the mouse was or is over (if any), and if it was/is,
 * forwards the mouse event to it.
 */
static void generic_mouse_event_forward(GUI* this, void* data, EventType e) {
    if (this->child == NULL || this->child->next == NULL)
        return;

    SplitLayoutData* sdata = this->aux;

    MouseData* mdata = malloc(sizeof(MouseData));
    if (mdata == NULL)
        return;

    // Duplicate mouse data
    memcpy(mdata, data, sizeof(MouseData));

    if (sdata->axis == HORIZONTAL) {
        if (mdata->y < this->child->height)
            GUI_trigger(this->child, e, mdata);
        else {
            mdata->y -= this->child->height;
            mdata->lasty -= this->child->height;
            GUI_trigger(this->child->next, e, mdata);
        }
    } else {
        if (mdata->x < this->child->width)
            GUI_trigger(this->child, e, mdata);
        else {
            mdata->x -= this->child->width;
            mdata->lastx -= this->child->width;
            GUI_trigger(this->child->next, e, mdata);
        }
    }

    free(mdata);
}


static void* mouse_moved(GUI* this, void* data) {
    generic_mouse_event_forward(this, data, MOUSE_MOVED);
}

static void* mouse_clicked(GUI* this, void* data) {
    generic_mouse_event_forward(this, data, CLICKED);
}

static void* mouse_released(GUI* this, void* data) {
    generic_mouse_event_forward(this, data, RELEASED);
}


GUI* GUI_split_layout(GUI* frame, SplitAxis axis, SplitSide fixed_side,
                      double fixed_size, SplitSizeUnit unit, bool adjustable) {
    frame->paint = paint;

    // Add layout data
    frame->aux = malloc(sizeof(SplitLayoutData));
    if (frame->aux != NULL) {
        SplitLayoutData* data = frame->aux;

        data->axis = axis;
        data->fixed_side = fixed_side;
        data->fixed_size = fixed_size;
        data->unit = unit;
        data->adjustable = adjustable;
    }

    // Add event listeners
    GUI_when(frame, RESIZED, resize_children);
    GUI_when(frame, MOUSE_MOVED, mouse_moved);
    GUI_when(frame, CLICKED, mouse_clicked);
    GUI_when(frame, RELEASED, mouse_released);

    return frame;
}
