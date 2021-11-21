/*
 *  Podczas zajęć nie jest wymagana znajomość tego pliku, znajdują się tu różne testowe implementacje
 *  funkcji rysujących (głownie okręgi i elipsy).
 *
 *  Do wykonywania ćwiczeń wystarczy korzystać z funkcji których deklaracje dostępne są w pliku sdl_draw_01.h
 *
 *  Dla zainteresowanych pozostaje możliwość przejrzenia oraz modyfikacji zamieszczonego tu kodu.
 *
 *  Źródła:
 *      https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 *      https://pl.wikipedia.org/wiki/Algorytm_Bresenhama
 *
 */
#include "sdl_draw_01.h"

namespace pp_draw_01 {

    void _put8(DrawContext & context, const int x1, const int y1);
    void _put16(DrawContext & context, const int x1, const int y1);
    void _put24(DrawContext & context, const int x1, const int y1);
    void _put32(DrawContext & context, const int x1, const int y1);
    void _drawCirclePoints(DrawContext & context, int sx, int sy, int dx, int dy);
    void _drawCirclePoints2(DrawContext & context, int sx, int sy, int dx, int dy);
    void ellipseSWH(DrawContext & context, Sint32 xs, Sint32 ys, Sint32 rx, Sint32 ry);
    void circleBres1(DrawContext & context, int xc, int yc, int r);
    void circleBres2(DrawContext & context, int xc, int yc, int r);

    void put_pixel(DrawContext & context, const int x1, const int y1) {
        switch (context.surface->format->BytesPerPixel) {
            case 1:
                _put8(context, x1, y1);
                break;
            case 2:
                _put16(context, x1, y1);
                break;
            case 3:
                _put24(context, x1, y1);
                break;
            case 4:
                _put32(context, x1, y1);
                break;
        }

    }

    void put_pixel(SDL_Surface *surface, const int x1, const int y1, const Uint32 color) {
        DrawContext context;
        context.surface = surface;
        context.color = color;
        put_pixel(context, x1, y1);
    }

    void line(DrawContext & context, const int x1, const int y1, const int x2, const int y2)
    {
        // zmienne pomocnicze
        int d, dx, dy, ai, bi, xi, yi;
        int x = x1, y = y1;
        // ustalenie kierunku rysowania
        if (x1 < x2)
        {
            xi = 1;
            dx = x2 - x1;
        }
        else
        {
            xi = -1;
            dx = x1 - x2;
        }
        // ustalenie kierunku rysowania
        if (y1 < y2)
        {
            yi = 1;
            dy = y2 - y1;
        }
        else
        {
            yi = -1;
            dy = y1 - y2;
        }
        // pierwszy piksel
        put_pixel(context, x, y);
        // oś wiodąca OX
        if (dx > dy)
        {
            ai = (dy - dx) * 2;
            bi = dy * 2;
            d = bi - dx;
            // pętla po kolejnych x
            while (x != x2)
            {
                // test współczynnika
                if (d >= 0)
                {
                    x += xi;
                    y += yi;
                    d += ai;
                }
                else
                {
                    d += bi;
                    x += xi;
                }
                put_pixel(context, x, y);
            }
        }
            // oś wiodąca OY
        else
        {
            ai = ( dx - dy ) * 2;
            bi = dx * 2;
            d = bi - dy;
            // pętla po kolejnych y
            while (y != y2)
            {
                // test współczynnika
                if (d >= 0)
                {
                    x += xi;
                    y += yi;
                    d += ai;
                }
                else
                {
                    d += bi;
                    y += yi;
                }
                put_pixel(context, x, y);
            }
        }
    }

    void ellipse(DrawContext & context, Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2)
    {
        Sint32 xs = (x1 + x2) >> 1;
        Sint32 ys = (y1 + y2) >> 1;
        Sint32 rx = (x2 - x1) >> 1;
        Sint32 ry = (y2 - y1) >> 1;
        if(rx < 0) rx = -rx;
        if(ry < 0) ry = -ry;
        ellipseSWH(context, xs, ys, rx, ry);
    }

    void circle(DrawContext & context, int x, int y, int d) {
        if (d % 2 == 1) {
            circleBres1(context, x, y, d / 2);
        } else {
            circleBres2(context, x, y, d / 2 - 1);
        }
    }



#define put(x, y) put_pixel(context, (x), (y))

    void _put8(DrawContext & context, const int x1, const int y1) {
        static_cast<Uint8 *>(context.surface->pixels)[context.surface->w * y1 + x1] = context.color;
    }

    void _put16(DrawContext & context, const int x1, const int y1) {
        static_cast<Uint16 *>(context.surface->pixels)[context.surface->w * y1 + x1] = context.color;
    }

    void _put24(DrawContext & context, const int x1, const int y1) {
        *static_cast<Uint16 *>(static_cast<void *>( static_cast<Uint8 *>(context.surface->pixels) +
                                                    (context.surface->w * y1 + x1) * 3     )) = Uint16(context.color);
        *static_cast<Uint8 * >(static_cast<void *>( static_cast<Uint8 *>(context.surface->pixels) +
                                                    (context.surface->w * y1 + x1) * 3 + 2 )) = Uint8(
                context.color >> 16);
    }

    void _put32(DrawContext & context, const int x1, const int y1) {
        static_cast<Uint32 *>(context.surface->pixels)[context.surface->w * y1 + x1] = context.color;
    }

    void _drawCirclePoints(DrawContext & context, int sx, int sy, int dx, int dy) {
        put(sx + dx, sy + dy);
        put(sx - dx, sy + dy);
        put(sx + dx, sy - dy);
        put(sx - dx, sy - dy);
        put(sx + dy, sy + dx);
        put(sx - dy, sy + dx);
        put(sx + dy, sy - dx);
        put(sx - dy, sy - dx);
    }

    void _drawCirclePoints2(DrawContext & context, int sx, int sy, int dx, int dy) {
        put(sx + dx + 1, sy + dy + 1);
        put(sx - dx, sy + dy + 1);
        put(sx + dx + 1, sy - dy);
        put(sx - dx, sy - dy);
        put(sx + dy + 1, sy + dx + 1);
        put(sx - dy, sy + dx + 1);
        put(sx + dy + 1, sy - dx);
        put(sx - dy, sy - dx);
    }

    void ellipseSWH(DrawContext & context, Sint32 xs, Sint32 ys, Sint32 rx, Sint32 ry)
    {
        Sint32 x = 0, y = ry;
        Sint32 e = 0, e1, e2;
        Sint32 rx2 = rx * rx, ry2 = ry * ry;
        Sint32 fx = 0, fy = rx2 * ry;

        while(fx <= fy)
        {
            put(xs+x, ys+y);
            put(xs+x, ys-y);
            put(xs-x, ys+y);
            put(xs-x, ys-y);
            e1 = e  + (fx << 1) + ry2;
            e2 = e1 - (fy << 1) + rx2;
            x++; fx += ry2;
            if(e1 + e2 >= 0)
            {
                e = e2; y--; fy -= rx2;
            }
            else
            {
                e = e1;
            }
        }

        while(y >= 0)
        {
            put(xs+x, ys+y);
            put(xs+x, ys-y);
            put(xs-x, ys+y);
            put(xs-x, ys-y);
            e1 = e  - (fy << 1) + rx2;
            e2 = e1 + (fx << 1) + ry2;
            y--; fy -= rx2;
            if(e1 + e2 < 0)
            {
                e = e2; x++; fx += ry2;
            }
            else
            {
                e = e1;
            }
        }
    }

    void ellipseSWH2(DrawContext & context, Sint32 xs, Sint32 ys, Sint32 rx, Sint32 ry) {
        Sint32 x = 0, y = ry;
        Sint32 x1 = x + 1, y1 = y + 1;
        Sint32 e = 0, e1, e2;
        Sint32 rx2 = rx * rx, ry2 = ry * ry;
        Sint32 fx = 0, fy = rx2 * ry;

        while (fx <= fy) {
            put(xs + x1, ys + y1);
            put(xs + x1, ys - y);
            put(xs - x, ys + y1);
            put(xs - x, ys - y);
            e1 = e + (fx << 1) + ry2;
            e2 = e1 - (fy << 1) + rx2;
            x++;
            x1++;
            fx += ry2;
            if (e1 + e2 >= 0) {
                e = e2;
                y--;
                y1--;
                fy -= rx2;
            } else {
                e = e1;
            }
        }

        while (y >= 0) {
            put(xs + x1, ys + y1);
            put(xs + x1, ys - y);
            put(xs - x, ys + y1);
            put(xs - x, ys - y);
            e1 = e - (fy << 1) + rx2;
            e2 = e1 + (fx << 1) + ry2;
            y--;
            y1--;
            fy -= rx2;
            if (e1 + e2 < 0) {
                e = e2;
                x++;
                x1++;
                fx += ry2;
            } else {
                e = e1;
            }
        }
    }

    void ellipseSWH3(DrawContext & context, Sint32 xs, Sint32 ys, Sint32 rx, Sint32 ry) {
        Sint32 x = 0, y = ry;
        Sint32 x0 = x + 1, y0 = y + 1;
        Sint32 x1 = x + 1, y1 = y + 1;
        Sint32 e = 0, e1, e2;
        Sint32 rx2 = rx * rx, ry2 = ry * ry;
        Sint32 fx = 0, fy = rx2 * ry;

        while (fx <= fy) {
            put(xs + x1, ys + y1);
            put(xs + x1, ys - y0);
            put(xs - x0, ys + y1);
            put(xs - x0, ys - y0);
            e1 = e + (fx << 1) + ry2;
            e2 = e1 - (fy << 1) + rx2;
            x++;
            x0++;
            x1++;
            fx += ry2;
            if (e1 + e2 >= 0) {
                e = e2;
                y--;
                y0--;
                y1--;
                fy -= rx2;
            } else {
                e = e1;
            }
        }

        while (y >= 0) {
            put(xs + x1, ys + y1);
            put(xs + x1, ys - y0);
            put(xs - x0, ys + y1);
            put(xs - x0, ys - y0);
            e1 = e - (fy << 1) + rx2;
            e2 = e1 + (fx << 1) + ry2;
            y--;
            y0--;
            y1--;
            fy -= rx2;
            if (e1 + e2 < 0) {
                e = e2;
                x++;
                x0++;
                x1++;
                fx += ry2;
            } else {
                e = e1;
            }
        }
    }

    void circleBres1(DrawContext & context, int xc, int yc, int r) {
        if (r < 1) {
            if (r > 0) put(xc, yc);
            return;
        }

        int x = 0, y = r;
        int d = 3 - 2 * r;
        _drawCirclePoints(context, xc, yc, x, y);
        while (y >= x) {
            x++;

            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            } else {
                d = d + 4 * x + 6;
            }
            _drawCirclePoints(context, xc, yc, x, y);
        }
    }

    void circleBres2(DrawContext & context, int sx, int sy, int r) {
        if (r < 1) {
            if (r > 0) {
                put(sx + 1, sy + 1);
                put(sx, sy + 1);
                put(sx + 1, sy);
                put(sx, sy);
            }
            return;
        }

        int x = 0, y = r;
        int d = 3 - 2 * r;
        _drawCirclePoints2(context, sx, sy, x, y);
        while (y >= x) {
            x++;

            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            } else {
                d = d + 4 * x + 6;
            }
            _drawCirclePoints2(context, sx, sy, x, y);
        }
    }

#undef put



    void TestDraw(DrawContext & context)
    {
        context.color = SDL_MapRGB(context.surface->format, 0x00, 0x00, 0xff);
        //ellipseSWH( 80, 100, 0, 0); // 3
        ellipseSWH(context, 100, 100, 1, 1); // 3
        ellipseSWH(context, 120, 100, 2, 2); // 5
        ellipseSWH(context, 140, 100, 3, 3); // 7
        ellipseSWH(context, 160, 100, 4, 4); // 9
        ellipseSWH(context, 160,  50, 4, 4); // 9
        ellipseSWH(context, 180, 100, 5, 5); // 11
        ellipseSWH(context, 200, 100, 6, 6); // 13
        context.color = SDL_MapRGB(context.surface->format, 0x88, 0x00, 0x00);
        //ellipse(100, 140, 101, 141); // 3
        ellipse(context, 120, 140, 122, 142); // 5
        ellipse(context, 140, 140, 143, 143); // 7
        ellipse(context, 160, 140, 164, 144); // 3
        ellipse(context, 180, 140, 185, 145); // 5
        ellipse(context, 200, 140, 206, 146); // 7

        ellipseSWH2(context, 120, 180, 2, 2); // 5
        ellipseSWH2(context, 140, 180, 3, 3); // 7
        ellipseSWH2(context, 160, 180, 4, 4); // 3
        ellipseSWH2(context, 180, 180, 5, 5); // 5
        ellipseSWH2(context, 200, 180, 6, 6); // 7
        ellipseSWH2(context, 300, 180, 32, 32); //

        circleBres1(context,  80, 220, 0); // 3
        circleBres1(context, 100, 220, 1); // 3
        circleBres1(context, 120, 220, 2); // 5
        circleBres1(context, 140, 220, 3); // 7
        circleBres1(context, 160, 220, 4); // 9
        circleBres1(context, 180, 220, 5); // 11
        circleBres1(context, 200, 220, 6); // 13
        circleBres1(context, 300, 220, 32); // 15

        circleBres2(context,  80, 240, 0); // 4
        circleBres2(context, 100, 240, 1); // 4
        circleBres2(context, 120, 240, 2); // 6
        circleBres2(context, 140, 240, 3); // 8
        circleBres2(context, 160, 240, 4); // 10
        circleBres2(context, 180, 240, 5); // 12
        circleBres2(context, 200, 240, 6); // 14
        circleBres2(context, 300, 240, 32); // 16

        circle(context,  80, 260, 0); // 4
        circle(context, 100, 260, 1); // 4
        circle(context, 120, 260, 2); // 6
        circle(context, 140, 260, 3); // 8
        circle(context, 160, 260, 4); // 10
        circle(context, 180, 260, 5); // 12
        circle(context, 200, 260, 6); // 14
        circle(context, 300, 260, 32); // 16
    }


}
