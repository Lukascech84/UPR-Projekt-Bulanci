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

typedef struct player
{
    int playerID;
    int player_speed;
    int directionX;
    int directionY;
    int isAlive;
    int respawn_timer;
    SDL_Rect hitbox;
    weapon *current_weapon;
    player_keybinds keybinds;
} player;

void *init_Players();

void clear_Players();

int get_Num_Of_Players();

player *get_Players();

void move_Players(double deltaTime);

void render_Players(SDL_Renderer *renderer);

void input_Players(SDL_Renderer *);

void resize_Players(int new_h, int new_w);

void shoot_Player(player *p, SDL_Renderer *);

void kill_Player(int);