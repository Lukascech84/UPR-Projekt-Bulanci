#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_BULLETS 256

typedef struct player player;

typedef struct
{
    int playerID;
    SDL_Rect hitbox;
    float bullet_velocity;
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
    int weaponID;
    char weapon_name[50];
    int num_of_bullets;
    // int bullet_spread;
    float bullet_velocity;
    float fire_rate;
    float fire_timer;
    int max_ammo;

    SDL_Texture *bullet_texture;
    SDL_Texture *weapon_texture;
} weapon;

weapon *get_weapon(int);

void change_weapon(player *, int);

void init_bullet();

void spawn_bullet(player *);

void render_bullet();

void update_bullet();

void destroy_bullet(int);