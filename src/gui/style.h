#ifndef _GUI_STYLE_H
#define _GUI_STYLE_H

#include <stdint.h>

#include <SDL2/SDL.h>

/*
 * Colours used by a single GUI element.
 *
 * NOTE: all colours use #AARRGGBB format.
 */
typedef struct gui_style_t {
    uint32_t bg, fg;
} GUIStyle;

/*
 * The colour scheme used by the entire GUI by default, unless overridden.
 */
typedef struct gui_theme_t {
    // TODO: actually fill out some of the colour properties that will be needed
    uint32_t sidebar_bg, sidebar_fg;
    uint32_t chatarea_bg, chatarea_fg;
} GUITheme;


static inline uint32_t GUI_map_colour(SDL_PixelFormat* format, uint32_t colour) {
    uint8_t a = (colour >> 24) & 0xFF, r = (colour >> 16) & 0xFF,
        g = (colour >> 8) & 0xFF, b = colour & 0xFF;

    return SDL_MapRGBA(format, r, g, b, a);
}

#endif
