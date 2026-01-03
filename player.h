#pragma once
#include <SDL2/SDL.h>
#include "engine.h"
#include "weapon.h"
#include "sceneManager.h"
#include "collisionMap.h"

#define MAX_NUMBER_OF_PLAYERS 4

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
    char *playerName;
    int player_speed;
    float posX;
    float posY;
    int directionX;
    int directionY;
    int aimX;
    int aimY;
    int isAlive;
    float respawn_timer;
    float respawn_timer_elapsed;
    int current_ammo_in_weapon;
    int score;
    SDL_Color *player_color;
    SDL_Rect hitbox;
    weapon *current_weapon;
    player_keybinds keybinds;
} player;

void init_Players();

void clear_Players();

int get_Num_Of_Players();

player *get_Players();

void update_Players();

void move_Player(player *);

void render_Players();

void input_Players();

void resize_Players(int new_h, int new_w);

void shoot_Player(player *);

void kill_Player(int);

void update_Player_Respawn(player *);

void respawn_Player(size_t);

SDL_Rect get_random_spawn(int, int);

int interects_with_any_player(SDL_Rect *);

void check_player_weapon_pickup_collision(player *);