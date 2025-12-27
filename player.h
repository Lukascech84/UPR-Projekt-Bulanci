#pragma once
#include <SDL2/SDL.h>
#include "engine.h"
#include "weapon.h"
#include "sceneManager.h"

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
    float respawn_timer;
    float respawn_timer_elapsed;
    SDL_Rect hitbox;
    weapon *current_weapon;
    player_keybinds keybinds;
} player;

void *init_Players();

void clear_Players();

int get_Num_Of_Players();

player *get_Players();

void move_Players();

void render_Players();

void input_Players();

void resize_Players(int new_h, int new_w);

void shoot_Player(player *p);

void kill_Player(int);

void update_Players_Respawn();

void respawn_Player(size_t);