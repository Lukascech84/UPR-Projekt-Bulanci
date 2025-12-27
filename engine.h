#pragma once
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    int window_w;
    int window_h;
    double deltaTime;
    Uint64 lastTicks;
} engine;

void eng_init(SDL_Window *w, SDL_Renderer *r, int w_w, int w_h);

engine *eng_get();