#include <iostream>

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#endif
#include "sdl_draw_01.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

using namespace std;
using namespace pp_draw_01;


int Test1()
{
    SDL_Window* window = nullptr;
    DrawContext context;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow(
            "hello_sdl2",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
    );
    if (window == nullptr) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Rect rect = {10,10,SCREEN_WIDTH-20,SCREEN_HEIGHT-20};
    context.surface = SDL_GetWindowSurface(window);
    SDL_FillRect(context.surface, NULL, SDL_MapRGB(context.surface->format, 0xFF, 0xFF, 0xFF));


    // testowe rysowanie
    TestDraw(context);


    SDL_Event zdarzenie;
    bool wyjscie = false;
    while( !wyjscie )
    {
        while( SDL_PollEvent( & zdarzenie ) )
        {
            if( zdarzenie.type == SDL_QUIT )
            {
                wyjscie = true;
            }

            if( zdarzenie.type == SDL_MOUSEMOTION )
            {
                context.color = SDL_MapRGB(context.surface->format, 0x0, 0x0, 0xff);
                put_pixel(context, zdarzenie.motion.x, zdarzenie.motion.y);
            }

            if( zdarzenie.type == SDL_MOUSEBUTTONDOWN )
            {
                /*
                rect.x = zdarzenie.motion.x-1;
                rect.y = zdarzenie.motion.y-1;
                rect.w = 3;
                rect.h = 3;

                if(zdarzenie.button.button == SDL_BUTTON_LEFT)
                    SDL_FillRect(context.surface, &rect, SDL_MapRGB(context.surface->format, 0xff, 0xff, 0x0));
                else
                    if(zdarzenie.button.button == SDL_BUTTON_RIGHT)
                    SDL_FillRect(context.surface, &rect, SDL_MapRGB(context.surface->format, 0x00, 0xff, 0x0));
                else
                    SDL_FillRect(context.surface, &rect, SDL_MapRGB(context.surface->format, 0xff, 0x00, 0x0));
                // */

                if(zdarzenie.button.button == SDL_BUTTON_LEFT)
                {
                    context.color = SDL_MapRGB(context.surface->format, 0xff, 0xff, 0x00);
                    ellipseSWH(context, zdarzenie.motion.x, zdarzenie.motion.y, 5, 5);
                }
                else
                if(zdarzenie.button.button == SDL_BUTTON_RIGHT)
                {
                    context.color = SDL_MapRGB(context.surface->format, 0x00, 0xff, 0x00);
                    ellipseSWH(context, zdarzenie.motion.x, zdarzenie.motion.y, 5, 5);
                }
                else
                {
                    context.color = SDL_MapRGB(context.surface->format, 0xff, 0x00, 0x00);
                    ellipseSWH(context, zdarzenie.motion.x, zdarzenie.motion.y, 5, 5);
                }

            }



            SDL_UpdateWindowSurface(window);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


int main()
{
    return Test1();
}
