/*

Super simple main file just to test all of the dependencies.

*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <curl/curl.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libwebsockets/libwebsockets.h>
#undef main

#include "gui/element/button.h"
#include "gui/element/label.h"
#include "gui/layout/simple.h"
#include "gui/eventhandler.h"
#include "gui/style.h"
#include "gui/gui.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

int main(int argc, char* args[]) {
    lws_set_log_level(LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);

    unsigned testavcodec = avcodec_version();
    unsigned testavformat = avformat_version();

    printf("AVCodec Version: %i\n", testavcodec);
    printf("AVFormat Version: %i\n", testavformat);

    avcodec_register_all();

    CURL* curl = curl_easy_init();

    SDL_Window* window = NULL;
    SDL_Surface* surface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Errors");
    } else {
        uint32_t window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
        window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);

        if (window == NULL) {
            printf("Errors, yo");
        } else {
            int imgflags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP;
            if (!(IMG_Init(imgflags) & imgflags)) {
                printf("SDL_image errored out man\n%s\n", IMG_GetError());
            }

            if (TTF_Init() == -1) {
                printf("SDL_ttf errored out man\n%s\n", TTF_GetError());
            }

            surface = SDL_GetWindowSurface(window);

            // GUI init
            TTF_Font* default_font = TTF_OpenFont("NotoSans-Regular.ttf", 14);
            GUI_label_set_default_font(default_font);

            GUI* frame = GUI_simple_layout(GUI_make_frame(0, 0, 640, 480));
            frame->style = malloc(sizeof(GUIStyle));
            frame->style->bg = 0xFFDDDDDD;
            printf("%i\n", GUI_map_colour(frame->surface->format, 0x00FF00));

            GUI_add_element(frame, GUI_make_button(20, 20, 100, 30, "test"));
            GUI_add_element(frame, GUI_make_button(200, 54, 180, 238, "test2"));
            GUI_add_element(frame, GUI_make_button(38, 209, 60, 24, "test3"));
            GUI_add_element(frame, GUI_make_label(50, 350, 150, 24, "This is a test string."));


            SDL_Event event;
            while (1) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT)
                        goto quit;

                    if (event.type == SDL_WINDOWEVENT &&
                        (event.window.event == SDL_WINDOWEVENT_RESIZED ||
                         event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED))
                        // Obtain a new window surface whenever the window is resized
                        surface = SDL_GetWindowSurface(window);

                    GUI_SDL_event_handler(frame, &event);
                }

                // GUI test stuff
                bool updated = GUI_update(frame);
                if (updated) {
                    SDL_FillRect(surface, NULL, 0);

                    SDL_BlitSurface(frame->surface, NULL, surface, NULL);
                    SDL_UpdateWindowSurface(window);
                }

                SDL_Delay(16);
            }

        quit:
            GUI_free(frame);
            TTF_CloseFont(default_font);
            default_font = NULL;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    if (curl) {
        curl_easy_cleanup(curl);
    }

    return 0;
}
