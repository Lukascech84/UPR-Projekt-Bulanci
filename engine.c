#include "engine.h"

static engine eng;

void eng_init(SDL_Window *w, SDL_Renderer *r, int w_w, int w_h)
{
    eng.window = w;
    eng.renderer = r;
    eng.window_w = w_w;
    eng.window_h = w_h;
    eng.deltaTime = 0.0;
    eng.lastTicks = SDL_GetPerformanceCounter();
}

engine *eng_get(void)
{
    return &eng;
}