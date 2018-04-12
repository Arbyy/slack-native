#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "eventhandler.h"
#include "event.h"
#include "gui.h"


void GUI_free(GUI* elem) {
    if (elem->child != NULL)
        GUI_free(elem->child);
    if (elem->next != NULL)
        GUI_free(elem->next);

    GUI_free_listener_coll(elem->listeners);
    SDL_FreeSurface(elem->surface);

    if (elem->auxfree != NULL)
        elem->auxfree(elem->aux);
    if (elem->aux != NULL)
        free(elem->aux);
    free(elem);
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
    gui->surface = NULL;
    gui->auxfree = NULL;
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
    if (gui->listeners == NULL ||
        gui->surface == NULL) {
        GUI_free(gui);
        return NULL;
    }

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
    SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, 0xFF, 0xFF, 0xFF));

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
    // There are no child elements yet
    if (parent->child == NULL) {
        parent->child = element;
        return;
    }

    // Find the last child element added
    GUI* last_child = parent->child;
    while (last_child->next != NULL)
        last_child = last_child->next;

    // Add the element to the end child
    last_child->next = element;
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
