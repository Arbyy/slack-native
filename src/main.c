/*

Super simple main file just to test all of the dependencies.

*/

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
#include "gui/layout/simple.h"
#include "gui/eventhandler.h"
#include "gui/gui.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
        window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
            GUI* frame = GUI_simple_layout(GUI_make_frame(0, 0, 640, 480));
            GUI_add_element(frame, GUI_make_button(20, 20, 100, 30, "test"));

            // TODO: move GUI related event listeners to a separate function,
            // probably in eventhandler.h
            SDL_Event event;
            int last_mousex = -1, last_mousey = -1;
            while (1) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT)
                        goto quit;
                    if (event.type == SDL_MOUSEMOTION ||
                        event.type == SDL_MOUSEBUTTONDOWN ||
                        event.type == SDL_MOUSEBUTTONUP) {
                        MouseData* data = malloc(sizeof(MouseData));
                        if (data != NULL) {
                            SDL_GetMouseState(&data->x, &data->y);
                            data->lastx = last_mousex;
                            data->lasty = last_mousey;
                            last_mousex = data->x;
                            last_mousey = data->y;

                            switch (event.type) {
                            case SDL_MOUSEMOTION:
                                GUI_trigger(frame, MOUSE_MOVED, data);
                                break;
                            case SDL_MOUSEBUTTONDOWN:
                                GUI_trigger(frame, CLICKED, data);
                                break;
                            case SDL_MOUSEBUTTONUP:
                                GUI_trigger(frame, RELEASED, data);
                                break;
                            }

                            free(data);
                        }
                    }
                }

                // GUI test stuff
                bool updated = GUI_update(frame);
                if (updated) {
                    SDL_FillRect(surface, NULL, 0);

                    SDL_BlitSurface(frame->surface, NULL, surface, NULL);
                    SDL_UpdateWindowSurface(window);

                    printf("GUI redraw\n");
                }

                SDL_Delay(16);
            }

        quit:
            GUI_free(frame);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    if (curl) {
        curl_easy_cleanup(curl);
    }

    return 0;
}
