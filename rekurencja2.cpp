#include <iostream>
#include "sdl_draw_01.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int NO_LIMIT = 0x7fffffff;

bool processEvents2(SDL_Event &zdarzenie);
void rysunekRekurencyjny2(pp_draw_01::DrawContext context, int x, int y, int size, int n, int &cnt, int limit);
void eventsLoop(const SDL_Window *window);

using namespace std;
using namespace pp_draw_01;

int rekurencja2() {
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


    // Główna pętla programu
    SDL_Event zdarzenie;
    bool wyjscie = false;
    for (int nr = 0; !wyjscie; ++nr) {
        // przetwarzanie zdarzeń, zwraca true jeśli program ma zostać zamknięty
        wyjscie = processEvents2(zdarzenie);


        context.color = 0xff;
        int cnt = 0; // licznik wywołań funkcji rekurencyjnej (używany w limicie rysowanych okręgów)
        rysunekRekurencyjny2(context, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_HEIGHT*7/8, 5, cnt, nr);


        // Rysowanie na ekranie (odświerzenie obrazu)
        SDL_UpdateWindowSurface(window);

        // Ograniczenie częstotliwości powtórzeń pętli w celu ograniczenia obciążenia CPU
        SDL_Delay(10); // 10 ms => 100 razy na sekundę
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void eventsLoop(SDL_Window *window) {
    SDL_Event zdarzenie;
    bool wyjscie = false;
    for (int nr = 0; !wyjscie; ++nr) {
        // przetwarzanie zdarzeń, zwraca true jeśli program ma zostać zamknięty
        wyjscie = processEvents2(zdarzenie);


        // Rysowanie na ekranie (odświerzenie obrazu)
        SDL_UpdateWindowSurface(window);

        // Ograniczenie częstotliwości powtórzeń pętli w celu ograniczenia obciążenia CPU
        SDL_Delay(100); // 100 ms => 10 razy na sekundę
    }
}

void rysunekRekurencyjny2(DrawContext context, int x, int y, int size, int n, int &cnt, int limit) {
    ++cnt;
    if (n > 0 && size > 3 && cnt<limit) {
        circle(context, x, y, size*7/8);
        int delta = size / 4;
        rysunekRekurencyjny2(context, x - delta, y - delta, size / 2, n-1, cnt, limit);
        rysunekRekurencyjny2(context, x - delta, y + delta, size / 2, n-1, cnt, limit);
        rysunekRekurencyjny2(context, x + delta, y - delta, size / 2, n-1, cnt, limit);
        rysunekRekurencyjny2(context, x + delta, y + delta, size / 2, n-1, cnt, limit);
    }
}

bool processEvents2(SDL_Event &zdarzenie) {
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
