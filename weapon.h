#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_BULLETS 256

typedef struct
{
    SDL_Rect hitbox;
    int bullet_velocity;
    // int bullet_spread;
    int active;
    SDL_Texture *bullet_texture;
} bullet;

typedef struct
{
    char weapon_name[50];
    int num_of_bullets;
    // int bullet_spread;
    int bullet_velocity;

    SDL_Texture *bullet_texture;
    SDL_Texture *weapon_texture;
} weapon;

weapon *get_weapon(int);

void init_bullet(bullet *);

void spawn_bullet(SDL_Renderer *, int, int, weapon);