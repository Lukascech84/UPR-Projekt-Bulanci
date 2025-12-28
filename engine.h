#pragma once
#include <SDL2/SDL.h>

typedef struct
{
    int running;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int window_w;
    int window_h;
    double deltaTime;
    Uint64 lastTicks;
} engine;

engine *eng_get();

int eng_init(char *title, int w, int h);

void eng_run();

void eng_quit();