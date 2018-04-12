#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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

    this->dirty = false;
}

static void auxfree(void* aux) {
    ButtonData* data = aux;

    if (data->label != NULL)
        free(data->label);
}

GUI* GUI_make_button(int x, int y, int width, int height, char* label) {
    GUI* this = GUI_alloc_generic(width, height);

    this->type = BUTTON;
    this->paint = paint;

    this->aux = malloc(sizeof(ButtonData));
    ButtonData* data = this->aux;

    // Clear aux fields
    data->mouseover = false;
    data->mousedown = false;

    // Copy supplied label into aux struct
    data->label = malloc(sizeof(char) * (strlen(label) + 1));
    if (data->label != NULL)
        strcpy(data->label, label);

    // Set function for cleaning up the aux struct
    this->auxfree = auxfree;

    // We haven't been rendered before
    this->dirty = true;

    return this;
}
