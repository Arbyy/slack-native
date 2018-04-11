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

    if (elem->aux != NULL)
        free(elem->aux);
    free(elem);
}


GUI* GUI_alloc_generic(int width, int height) {
    GUI* gui;
    if ((gui = malloc(sizeof(GUI))) == NULL)
        return NULL;

    // Clear all pointer fields
    gui->listeners = NULL;
    gui->surface = NULL;
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
static void frame_paint(GUI* this) {
    // yeeeeeaaaahhhhhh
    SDL_FillRect(this->surface, NULL, SDL_MapRGB(this->surface->format, 0xFF, 0, 0));
}

GUI* GUI_make_frame(int x, int y, int width, int height) {
    GUI* frame = GUI_alloc_generic(width, height);

    frame->type = FRAME;
    frame->paint = frame_paint;

    return frame;
}
