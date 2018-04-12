#include <SDL2/SDL.h>

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


GUI* GUI_simple_layout(GUI* frame) {
    frame->paint = paint;
    return frame;
}
