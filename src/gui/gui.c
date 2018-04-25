#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "eventhandler.h"
#include "event.h"
#include "style.h"
#include "gui.h"


static void GUI_free_single(GUI* elem) {
    GUI_free_listener_coll(elem->listeners);
    SDL_FreeSurface(elem->surface);

    if (elem->auxfree != NULL)
        elem->auxfree(elem->aux);

    free(elem->style);
    free(elem->aux);
    free(elem);
}

void GUI_free(GUI* elem) {
    if (elem->child != NULL)
        GUI_free(elem->child);
    if (elem->next != NULL)
        GUI_free(elem->next);

    GUI_free_single(elem);
}

static void* resize_surface(GUI* this, void* data) {
    SDL_FreeSurface(this->surface);
    this->surface = NULL;

    this->surface = SDL_CreateRGBSurface(0, this->width, this->height, 32,
                                         0xFF000000,
                                         0x00FF0000,
                                         0x0000FF00,
                                         0x000000FF);

    // This element must be redrawn, as we replaced its surface with a new one
    this->dirty = true;

    return NULL;
}

GUI* GUI_alloc_generic(int width, int height) {
    GUI* gui;
    if ((gui = malloc(sizeof(GUI))) == NULL)
        return NULL;

    // Clear dimensions
    gui->x = 0;
    gui->y = 0;
    gui->width = width;
    gui->height = height;

    // Clear all pointer fields
    gui->listeners = NULL;
    gui->previous = NULL;
    gui->surface = NULL;
    gui->auxfree = NULL;
    gui->parent = NULL;
    gui->style = NULL;
    gui->paint = NULL;
    gui->child = NULL;
    gui->next = NULL;
    gui->aux = NULL;

    // Allocate pointer fields
    gui->listeners = GUI_new_listener_coll();

    gui->surface = SDL_CreateRGBSurface(0, width, height, 32,
                                        0xFF000000,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF);

    // If any allocations failed, clean up everything
    if (gui->listeners == NULL || gui->surface == NULL) {
        GUI_free(gui);
        return NULL;
    }

    // Add default event listeners
    GUI_when(gui, RESIZED, resize_surface);

    // Set dirty because the new GUI element hasn't been rendered yet
    gui->dirty = true;

    return gui;
}


/*
 * Non-content-aware default frame paint function (it paints all children with
 * no specified destination rectangle).
 */
static void fallback_frame_paint(GUI* this) {
    // Clear surface
    if (this->style != NULL)
        SDL_FillRect(this->surface, NULL, GUI_map_colour(this->surface->format,
                                                         this->style->bg));
    else
        SDL_FillRect(this->surface, NULL,
                     SDL_MapRGB(this->surface->format, 0xFF, 0xFF, 0xFF));

    GUI* child = this->child;
    while (child != NULL) {
        SDL_BlitSurface(child->surface, NULL, this->surface, NULL);
        child = child->next;
    }

    this->dirty = false;
}

GUI* GUI_make_frame(int x, int y, int width, int height) {
    GUI* frame = GUI_alloc_generic(width, height);

    frame->type = FRAME;
    frame->paint = fallback_frame_paint;

    return frame;
}


void GUI_add_element(GUI* parent, GUI* element) {
    if (parent->child == NULL) {
        // There are no child elements yet
        parent->child = element;
        element->parent = parent;
        return;
    }

    // Find the last child element added
    GUI* last_child = parent->child;
    while (last_child->next != NULL)
        last_child = last_child->next;

    // Add the element to the end child
    last_child->next = element;
    element->previous = last_child;
    element->parent = parent;
}


void GUI_remove_element(GUI* element) {
    // Remove all children as well
    GUI* child = element->child;
    while (child != NULL) {
        GUI_remove_element(child);
        child = child->next;
    }

    if (element->previous != NULL) {
        // Join previous and next children, if they exist
        element->previous->next = element->next;

        if (element->next != NULL) {
            element->next->previous = element->previous;
        }
    } else if (element->parent != NULL) {
        // Make the parent's second child the first child
        element->parent->child = element->next;

        if (element->next != NULL) {
            // Make next element first element in the list
            element->next->previous = NULL;
        }
    }

    GUI_free_single(element);

    // Make parent dirty, so it redraws without this element (also prevents a
    // segfault, apparently)
    if (element->parent != NULL)
        element->parent->dirty = true;
}


void GUI_paint(GUI* element) {
    // Paint children before painting the parent
    GUI* child = element->child;
    while (child != NULL) {
        GUI_paint(child);
        child = child->next;
    }

    element->paint(element);
}


bool GUI_update(GUI* root) {
    // If this element is dirty, paint all children, then paint this element
    if (root->dirty) {
        GUI_paint(root);
        return true;
    }

    bool dirty = false;
    GUI* child = root->child;

    // If any child is dirty, the parent is also dirty
    while (child != NULL) {
        dirty |= GUI_update(child);
        child = child->next;
    }

    if (dirty)
        root->paint(root);

    return dirty;
}


void GUI_prepare(GUI* root) {
    // Allows layout managers to update before the GUI is drawn for the first
    // time. Otherwise, the window would have to be resized before the GUI is
    // drawn properly...
    GUI_trigger(root, RESIZED, NULL);
}
