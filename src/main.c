/*

Super simple main file just to test all of the dependencies.

*/

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <curl/curl.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libwebsockets/libwebsockets.h>
#undef main

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
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(window);
            SDL_Delay(2000);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    if (curl) {
        curl_easy_cleanup(curl);
    }

    return 0;
}
