/*

Super simple main file just to test all of the dependencies.

*/

#include <stdbool.h>
#include <stdlib.h>
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
#include "gui/layout/split.h"
#include "gui/eventhandler.h"
#include "gui/style.h"
#include "gui/text.h"
#include "gui/gui.h"

#include "xz/xz_config.h"
#include "xz/xz.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

int main(int argc, char* args[]) {
    /* const char teststr[] = "This is 本当にテストです。 ｗｉｄｅ normal."; */
    /* printf("Testing splitting the string \"%s\" into individual chars\n", teststr); */

    /* char buf[5]; */
    /* const char* saveptr; */
    /* utf8_next_char(teststr, buf, &saveptr); */

    /* while (buf[0] != '\0') { */
    /*     printf("%s\n", buf); */

    /*     utf8_next_char(NULL, buf, &saveptr); */
    /* } */

    /* printf("Finished splitting the string\n"); */

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

            // XXX test embedded font extraction
            xz_crc32_init();
            xz_crc64_init();

            GUI_font_init();
            TTF_Font* notosans = TTF_OpenFont("NotoSans-Regular.ttf", 16);
            TTF_Font* notojp = TTF_OpenFont("NotoSansCJKjp-Regular.otf", 16);
            FontFamily* ff = GUI_create_font_family(2, (TTF_Font*[]){notosans, notojp});

            GUI_label_set_default_font_family(ff);

            GUI* frame = GUI_split_layout(GUI_make_frame(0, 0, 640, 480),
                                          VERTICAL, LEFT, 220, PIXELS, false);

            GUI* sidebar = GUI_simple_layout(GUI_make_frame(0, 0, 0, 0));
            sidebar->style = malloc(sizeof(GUIStyle));
            sidebar->style->bg = 0xFF333333;
            GUI_add_element(frame, sidebar);

            GUI* content = GUI_simple_layout(GUI_make_frame(0, 0, 0, 0));
            GUI_add_element(frame, content);

            GUI_add_element(content, GUI_make_button(20, 20, 100, 30, ""));
            /* GUI_add_element(frame, GUI_make_button(200, 54, 180, 238, "test2")); */
            /* GUI_add_element(frame, GUI_make_button(38, 209, 60, 24, "test3")); */
            GUI_add_element(content, GUI_make_label(50, 350, 300, 24, "これは日本語のテストストリング。"));
            GUI_add_element(content, GUI_make_label(50, 366, 300, 24, "This is a test string in English."));
            GUI_add_element(content, GUI_make_label(50, 382, 300, 24, "This is a テスト ｍｉｘｅｄ string."));
            GUI_prepare(frame);

            SDL_Rect inRect = {300, 300, 200, 100};
            SDL_SetTextInputRect(&inRect);
            SDL_StartTextInput();
            SDL_Event event;
            while (1) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT)
                        goto quit;

                    else if (event.type == SDL_WINDOWEVENT &&
                        (event.window.event == SDL_WINDOWEVENT_RESIZED ||
                         event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED))
                        // Obtain a new window surface whenever the window is resized
                        surface = SDL_GetWindowSurface(window);

                    else if (event.type == SDL_TEXTINPUT) {
                        printf("Text input: '%s'\n\n", event.text.text);
                    }

                    else if (event.type == SDL_TEXTEDITING) {
                        printf("Text edited:\n"
                               "    Composition: '%s'\n"
                               "    Cursor: %i\n"
                               "    Selection length: %i\n\n",
                               event.edit.text, event.edit.start, event.edit.length);
                    }

                    GUI_SDL_event_handler(frame, &event);
                }

                // GUI test stuff
                bool updated = GUI_update(frame);
                if (updated) {
                    SDL_FillRect(surface, NULL, 0);

                    SDL_BlitSurface(frame->surface, NULL, surface, NULL);
                    SDL_UpdateWindowSurface(window);
                }

                SDL_Delay(15);
            }

        quit:
            GUI_free(frame);
            GUI_free_font_family(ff);
            TTF_CloseFont(notosans);
            TTF_CloseFont(notojp);
            GUI_font_destroy();
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    if (curl) {
        curl_easy_cleanup(curl);
    }

    return 0;
}
