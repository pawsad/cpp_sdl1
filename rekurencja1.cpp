#include <iostream>
#include "sdl_draw_01.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool processEvents1(SDL_Event &zdarzenie);
void rysunekRekurencyjny1(pp_draw_01::DrawContext context, int x, int y, int size, int n);

using namespace std;
using namespace pp_draw_01;

int rekurencja1() {
    SDL_Window *window = nullptr;
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


    // KOD UMIESZCZONY W TYM MIEJSCU JEST WYKONYWANY TYLKO RAZ, PO URUCHOMIENIU PROGRAMU
    SDL_FillRect(context.surface, nullptr, SDL_MapRGB(context.surface->format, 0xFF, 0xFF, 0xFF));

    context.color = 0xffff00;
    rysunekRekurencyjny1(context, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_HEIGHT * 7 / 8, 3);


    // Rysowanie na ekranie (odświerzenie obrazu)
    SDL_UpdateWindowSurface(window);

    // Główna pętla programu
    SDL_Event zdarzenie;
    bool wyjscie = false;
    for (int nr = 0; !wyjscie; ++nr) {
        // przetwarzanie zdarzeń, zwraca true jeśli program ma zostać zamknięty
        wyjscie = processEvents1(zdarzenie);


        context.color = 0x0000ff;
        rysunekRekurencyjny1(context, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_HEIGHT * 7 / 8, nr / 10);


        // Rysowanie na ekranie (odświerzenie obrazu)
        SDL_UpdateWindowSurface(window);

        // Ograniczenie częstotliwości powtórzeń pętli w celu ograniczenia obciążenia CPU
        SDL_Delay(100); // 100 ms => 10 razy na sekundę
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void rysunekRekurencyjny1(DrawContext context, int x, int y, int size, int n) {
    if (n > 0 && size > 3) {
        circle(context, x, y, size);
        int delta = size / 4;
        rysunekRekurencyjny1(context, x - delta, y - delta, size / 2, n - 1);
        rysunekRekurencyjny1(context, x - delta, y + delta, size / 2, n - 1);
        rysunekRekurencyjny1(context, x + delta, y - delta, size / 2, n - 1);
        rysunekRekurencyjny1(context, x + delta, y + delta, size / 2, n - 1);
    }
}

bool processEvents1(SDL_Event &zdarzenie) {
    bool wyjscie = false;
    while (SDL_PollEvent(&zdarzenie)) {
        // Zdarzenia systemowe
        if (zdarzenie.type == SDL_QUIT) {
            wyjscie = true;
        }

        // Zdarzenia Klawiatury
        if (zdarzenie.type == SDL_KEYDOWN) {
            // KOD UMIESZCZONY W TYM MIEJSCU jest wykonywany po każdym naciśnięciu klawisza

            if (zdarzenie.key.keysym.sym == SDLK_q) {
                cout << "Key [Q] => QUIT" << endl << "Bye, bye!" << endl;
                wyjscie = true;
            }

        }
    }
    return wyjscie;
}
