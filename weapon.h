#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_BULLETS 256

typedef struct player player;

typedef struct
{
    int playerID;
    SDL_Rect hitbox;
    int bullet_velocity;
    int directionX;
    int directionY;
    int lastDirectionX;
    int lastDirectionY;
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

void init_bullet();

void spawn_bullet(SDL_Renderer *, player *);

void render_bullet(SDL_Renderer *);

void update_bullet();

void destroy_bullet(int);