#include <string.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../gui.h"
#include "label.h"


// Set this with GUI_label_set_default_font
static TTF_Font* default_font = NULL;


static void paint(GUI* this) {
    // Clear surface
    SDL_FillRect(this->surface, NULL, 0);

    LabelData* data = this->aux;

    // Render text
    SDL_Color colour = {0, 0, 0};
    SDL_Surface* text = TTF_RenderUTF8_Blended(data->font, data->label, colour);

    if (!text) {
        printf("SDL_ttf error rendering while rendering label:\n%s\n", TTF_GetError());
    } else {
        SDL_Rect* dest = malloc(sizeof(SDL_Rect));

        if (dest != NULL) {
            // Center text in label area
            dest->x = this->surface->w / 2 - text->w / 2;
            dest->y = this->surface->h / 2 - text->h / 2;
            dest->w = text->w;
            dest->h = text->h;

            SDL_BlitSurface(text, NULL, this->surface, dest);
            SDL_FreeSurface(text);

            free(dest);
        }
    }

    this->dirty = false;
}

static void auxfree(void* aux) {
    LabelData* data = aux;
    free(data->label);
}

GUI* GUI_make_label(int x, int y, int width, int height, char* label) {
    GUI* this = GUI_alloc_generic(width, height);
    this->x = x;
    this->y = y;

    this->type = LABEL;
    this->paint = paint;

    this->aux = malloc(sizeof(LabelData));
    LabelData* data = this->aux;

    // Font points to default font by default
    data->font = default_font;

    // Copy supplied label into aux struct
    data->label = malloc(sizeof(char) * (strlen(label) + 1));
    if (data->label != NULL)
        strcpy(data->label, label);

    // Set function for cleaning up the aux struct label field
    this->auxfree = auxfree;

    // We haven't been rendered before
    this->dirty = true;

    return this;
}


void GUI_label_set_default_font(TTF_Font* font) {
    if (font != NULL)
        default_font = font;
}


void GUI_label_set_font(GUI* label, TTF_Font* font) {
    if (font != NULL)
        ((LabelData*) label->aux)->font = font;
}
