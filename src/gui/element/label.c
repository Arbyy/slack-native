#include <string.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../gui.h"
#include "label.h"


// Set this with GUI_label_set_default_font
static FontFamily* default_family = NULL;


static void paint(GUI* this) {
    // Clear surface
    SDL_FillRect(this->surface, NULL, 0);

    LabelData* data = this->aux;

    // Render text
    SDL_Color colour = {0, 0, 0, 255};
    SDL_Surface* text = GUI_draw_text_collection(data->coll, colour);
    //SDL_Surface* text = TTF_RenderUTF8_Blended(data->font, data->label, colour);

    if (!text) {
        printf("SDL_ttf error rendering while rendering label:\n%s\n", TTF_GetError());
    } else {
        // Center text in label area
        SDL_Rect dest;
        dest.x = this->surface->w / 2 - text->w / 2;
        dest.y = this->surface->h / 2 - text->h / 2;
        dest.w = text->w;
        dest.h = text->h;

        SDL_BlitSurface(text, NULL, this->surface, &dest);
        SDL_FreeSurface(text);
    }

    this->dirty = false;
}

static void auxfree(void* aux) {
    LabelData* data = aux;
    GUI_free_text_collection(data->coll);
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
    data->family = default_family;

    // Copy supplied label into aux struct
    data->label = malloc(sizeof(char) * (strlen(label) + 1));
    if (data->label != NULL)
        strcpy(data->label, label);

    // Create text collection using initial label value
    data->coll = GUI_collect_text(data->family, data->label);

    // Set function for cleaning up the aux struct label field
    this->auxfree = auxfree;

    // We haven't been rendered before
    this->dirty = true;

    return this;
}


void GUI_set_label_default_font_family(FontFamily* family) {
    if (family != NULL)
        default_family = family;
}


void GUI_set_label_font_family(GUI* label, FontFamily* family) {
    LabelData* data = label->aux;

    if (family != NULL)
        data->family = family;

    // Create new text collection using new font family
    GUI_free_text_collection(data->coll);
    data->coll = GUI_collect_text(data->family, data->label);
}
