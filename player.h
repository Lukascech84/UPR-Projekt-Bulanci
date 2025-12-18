#pragma once
#include <SDL2/SDL.h>
#include "weapon.h"

typedef struct
{
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
    SDL_Scancode shoot;
} player_keybinds;

typedef struct
{
    int playerID;
    int directionY;
    int directionX;
    SDL_Rect hitbox;
    weapon *current_weapon;
    player_keybinds keybinds;
} player;

void *init_Players();

void clear_Players();

void move_Players(double deltaTime);

void render_Players(SDL_Renderer *renderer);

void input_Players(SDL_Renderer *);

void resize_Players(int new_h, int new_w);

void shoot_Players(player *p, SDL_Renderer *);