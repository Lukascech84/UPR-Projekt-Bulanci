#pragma once
#include <SDL2/SDL.h>

typedef struct
{
    SDL_KeyCode up;
    SDL_KeyCode down;
    SDL_KeyCode left;
    SDL_KeyCode right;
} player_keybinds;

typedef struct
{
    int directionY;
    int directionX;
    SDL_Rect hitbox;
    player_keybinds keybinds;
} player;

player *init_Players(player *p, int num_of_players);

void clear_Players(player *p);

void move_Players(player *p, int num_of_players, int speed, double deltaTime);

void render_Players(player *p, int num_of_players, SDL_Renderer *renderer);

void input_Players(player *p, int num_of_players, SDL_Event event);

void resize_Players(player *p, int num_of_players, int new_h, int new_w);