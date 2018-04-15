#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "../element/label.h"
#include "../eventhandler.h"
#include "../gui.h"
#include "button.h"


static void paint(GUI* this) {
    // Clear surface
    SDL_FillRect(this->surface, NULL, 0);

    uint32_t fill_colour;
    if (((ButtonData*) this->aux)->mousedown) {
        fill_colour = SDL_MapRGB(this->surface->format, 0xFF, 0, 0);
    } else if (((ButtonData*) this->aux)->mouseover) {
        fill_colour = SDL_MapRGB(this->surface->format, 0x55, 0x55, 0x55);
    } else {
        fill_colour = SDL_MapRGB(this->surface->format, 0x33, 0x33, 0x33);
    }

    SDL_FillRect(this->surface, NULL, fill_colour);

    // Blit label onto button
    SDL_BlitSurface(this->child->surface, NULL, this->surface, NULL);

    this->dirty = false;
}

static void* set_mouseover(GUI* this, void* data) {
    ButtonData* bdata = this->aux;
    bdata->mouseover = true;
    this->dirty = true;
    return NULL;
}

static void* clear_mouseover(GUI* this, void* data) {
    ButtonData* bdata = this->aux;
    bdata->mouseover = false;
    bdata->mousedown = false; // Cancel click
    this->dirty = true;
    return NULL;
}

static void* set_mousedown(GUI* this, void* data) {
    ButtonData* bdata = this->aux;
    bdata->mousedown = true;
    this->dirty = true;
    return NULL;
}

static void* clear_mousedown(GUI* this, void* data) {
    // TODO: add actions when the mouse is released
    ButtonData* bdata = this->aux;
    bdata->mousedown = false;
    this->dirty = true;
    return NULL;
}

GUI* GUI_make_button(int x, int y, int width, int height, char* label) {
    GUI* this = GUI_alloc_generic(width, height);
    this->x = x;
    this->y = y;

    this->type = BUTTON;
    this->paint = paint;

    this->aux = malloc(sizeof(ButtonData));
    ButtonData* data = this->aux;

    // Clear aux fields
    data->mouseover = false;
    data->mousedown = false;

    // Create label and add it to the button
    GUI* label_elem = GUI_make_label(x, y, width, height, label);
    this->child = label_elem;

    // Add default event listeners
    GUI_when(this, MOUSE_ENTERED, set_mouseover);
    GUI_when(this, MOUSE_EXITED, clear_mouseover);
    GUI_when(this, CLICKED, set_mousedown);
    GUI_when(this, RELEASED, clear_mousedown);

    // We haven't been rendered before
    this->dirty = true;

    return this;
}
