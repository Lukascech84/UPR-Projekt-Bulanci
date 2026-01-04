#pragma once
#include <SDL2/SDL.h>

#define TILE_SIZE 32 // v pixelech

typedef struct
{
    int width;
    int height;
    int *tiles;

} collisionMap;

int map_collides_rect(SDL_Rect *rect);
