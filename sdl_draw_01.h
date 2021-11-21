#ifndef PP_SDL_01_SDL_DRAW_01_H
#define PP_SDL_01_SDL_DRAW_01_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

namespace pp_draw_01 {


    struct DrawContext {
        SDL_Surface *surface = nullptr;
        Uint32 color = 0xffffffff;
    };

    void TestDraw(DrawContext & context);

    // Base function to use:
    void put_pixel(SDL_Surface *surface, const int x1, const int y1, const Uint32 color);
    void put_pixel(DrawContext & context, const int x1, const int y1);
    void line(DrawContext & context, const Sint32 x1, const Sint32 y1, const Sint32 x2, const Sint32 y2);

    // Example functions to draw circe / ellipse
    void ellipse(DrawContext & context, Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2);
    void circle(DrawContext & context, int x, int y, int diameter);

} // namespace

#endif //PP_SDL_01_SDL_DRAW_01_H
