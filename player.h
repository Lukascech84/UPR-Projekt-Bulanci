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
    weapon current_weapon;
    player_keybinds keybinds;
} player;

player *init_Players(player *p, int num_of_players, weapon weapons[]);

void clear_Players(player *p);

void move_Players(player *p, int num_of_players, int speed, double deltaTime);

void render_Players(player *p, int num_of_players, SDL_Renderer *renderer);

void input_Players(player *p, int num_of_players, SDL_Renderer *);

void resize_Players(player *p, int num_of_players, int new_h, int new_w);

void shoot_Players(player *p, SDL_Renderer *);