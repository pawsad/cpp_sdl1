/*
 * Przykładowy program demonstrujący użycie biblioteki SDL2 w celu obsługi zdarzeń (myszy i klawiatury) oraz rysowania.
 *
 * Wymagana biblioteka SDL2 musi być zainstalowana i widoczna przez środowisko programistyczne:
 *
 * Windows: W przypadku korzystania z MSYS (zgodnie z instrukcją), w konsoli MSYS:
 *
 *      pacman -S --noconfirm --needed mingw-w64-x86_64-SDL2
 *
 * Linux: W przypadku korzystania z systemu z rodziny Ubuntu, w terminalu:
 *
 *      sudo apt-get install -y libsdl2-dev
 *
 */
#include <iostream>
#include "sdl_draw_01.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

using namespace std;
using namespace pp_draw_01;

int Test0()
{
    SDL_Window* window = nullptr;
    DrawContext context;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "could not initialize sdl2: " << SDL_GetError() << endl;
        return 1;
    }
    window = SDL_CreateWindow(
            "PP Hello SDL2 - Test 0",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
    );
    if (window == nullptr) {
        cerr << "could not create window: " << SDL_GetError() << endl;
        return 1;
    }

    context.surface = SDL_GetWindowSurface(window);
    SDL_FillRect(context.surface, nullptr, SDL_MapRGB(context.surface->format, 0xFF, 0xFF, 0xFF));


    // KOD UMIESZCZONY W TYM MIEJSCU JEST WYKONYWANY TYLKO RAZ, PO URUCHOMIENIU PROGRAMU

    // testowe rysowanie okręgów przy użyciu różnych implementacji rysowania okręgu i elipsy
    TestDraw(context);


    // Rysowanie na ekranie (odświerzenie obrazu)
    SDL_UpdateWindowSurface(window);

    // Główna pętla programu
    SDL_Event zdarzenie;
    bool wyjscie = false;
    while( !wyjscie )
    {
        while( SDL_PollEvent( & zdarzenie ) )
        {
            // Zdarzenia systemowe
            if( zdarzenie.type == SDL_QUIT )
            {
                wyjscie = true;
            }

            // Zdarzenia Klawiatury
            if( zdarzenie.type == SDL_KEYDOWN )
            {
                // KOD UMIESZCZONY W TYM MIEJSCU jest wykonywany po każdym naciśnięciu klawisza

                if( zdarzenie.key.keysym.sym == SDLK_q ) {
                    cout << "Key [Q] => QUIT" << endl << "Bye, bye!" << endl;
                    wyjscie = true;
                }

            }
        }

        // Ograniczenie częstotliwości powtórzeń pętli w celu ograniczenia obciążenia CPU
        SDL_Delay(100); // 100 ms => 10 razy na sekundę
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int Test1()
{
    SDL_Window* window = nullptr;
    DrawContext context;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "could not initialize sdl2: " << SDL_GetError() << endl;
        return 1;
    }
    window = SDL_CreateWindow(
            "PP Hello SDL2 - Test 1",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
    );
    if (window == nullptr) {
        cerr << "could not create window: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Rect rect = {10,10,SCREEN_WIDTH-20,SCREEN_HEIGHT-20};
    context.surface = SDL_GetWindowSurface(window);
    SDL_FillRect(context.surface, &rect, SDL_MapRGB(context.surface->format, 0xFF, 0xFF, 0xFF));


    // KOD UMIESZCZONY W TYM MIEJSCU JEST WYKONYWANY TYLKO RAZ, PO URUCHOMIENIU PROGRAMU

    // testowe rysowanie okręgów przy użyciu różnych implementacji rysowania okręgu i elipsy
    //TestDraw(context);

    context.color = 0;
    ellipse(context, 100,100, 700,500);


    // Główna pętla programu
    SDL_Event zdarzenie, poczatek;
    bool wyjscie = false;
    while( !wyjscie )
    {
        while( SDL_PollEvent( & zdarzenie ) )
        {
            // Zdarzenia systemowe
            if( zdarzenie.type == SDL_QUIT )
            {
                wyjscie = true;
            }

            // Zdarzenia Klawiatury
            if( zdarzenie.type == SDL_KEYDOWN )
            {
                // KOD UMIESZCZONY W TYM MIEJSCU jest wykonywany po każdym naciśnięciu klawisza

                if( zdarzenie.key.keysym.sym == SDLK_q ) {
                    cout << "Key [Q] => QUIT" << endl << "Bye, bye!" << endl;
                    wyjscie = true;
                }

                if( zdarzenie.key.keysym.sym == 'c' ) {
                    cout << "Key [C] => CLEAN" << endl;
                    SDL_FillRect(context.surface, NULL, SDL_MapRGB(context.surface->format, 0xFF, 0xFF, 0xFF));
                }
            }

            // Zdarzenia myszy
            if( zdarzenie.type == SDL_MOUSEMOTION )
            {
                // KOD UMIESZCZONY W TYM MIEJSCU jest wykonywany po każdej zmianie pozycji kursora myszy

                Uint32 color = SDL_MapRGB(context.surface->format, 0xAA, 0xDD, 0xFF);
                put_pixel(context.surface, zdarzenie.motion.x, zdarzenie.motion.y, color);
            }

            if( zdarzenie.type == SDL_MOUSEBUTTONDOWN )
            {
                // KOD UMIESZCZONY W TYM MIEJSCU jest wykonywany po każdym naciśnięciu przycisku myszy

                // kolor
                switch(zdarzenie.button.button) {
                    case  SDL_BUTTON_LEFT:
                        context.color = SDL_MapRGB(context.surface->format, 0xDD, 0x00, 0x00);
                        break;
                    case  SDL_BUTTON_RIGHT:
                        context.color = SDL_MapRGB(context.surface->format, 0x00, 0xBB, 0x00);
                        break;
                    default:
                        context.color = SDL_MapRGB(context.surface->format, 0x00, 0x00, 0xFF);
                        break;
                }

                circle(context, zdarzenie.button.x, zdarzenie.button.y, 11);

                poczatek = zdarzenie;
            }

            if( zdarzenie.type == SDL_MOUSEBUTTONUP )
            {
                // KOD UMIESZCZONY W TYM MIEJSCU jest wykonywany po każdym zwolnieniu przycisku myszy

                rect.x = zdarzenie.button.x-2;
                rect.y = zdarzenie.button.y-2;
                rect.w = 5;
                rect.h = 5;
                SDL_FillRect(context.surface, &rect, context.color);

                line(context, poczatek.motion.x, poczatek.motion.y, zdarzenie.motion.x, zdarzenie.motion.y);
            }

        }

        // Rysowanie na ekranie (odświerzenie obrazu)
        SDL_UpdateWindowSurface(window);

        // Ograniczenie częstotliwości odświerzania
        SDL_Delay(50); // 50 ms => 20 FPS (klatek na sekundę)
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


int main(int argc, char ** argv)
{
    return Test1();
}
