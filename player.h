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

player *init_Players(player *p, int num_of_players)
{
    player_keybinds keysets[4] = {{.up = SDLK_w, .down = SDLK_s, .left = SDLK_a, .right = SDLK_d},
                                  {.up = SDLK_UP, .down = SDLK_DOWN, .left = SDLK_LEFT, .right = SDLK_RIGHT},
                                  {.up = SDLK_i, .down = SDLK_k, .left = SDLK_j, .right = SDLK_l},
                                  {.up = SDLK_t, .down = SDLK_g, .left = SDLK_f, .right = SDLK_h}};
    p = realloc(p, num_of_players * sizeof(player));
    if (!p)
    {
        return NULL;
    }

    for (size_t i = 0; i < num_of_players; i++)
    {
        p[i].directionX = 0;
        p[i].directionY = 0;
        p[i].hitbox.h = 100;
        p[i].hitbox.w = 100;
        p[i].hitbox.x = (1.5f * i) * 100;
        p[i].hitbox.y = 100;
        p[i].keybinds = keysets[i];
    }

    return p;
}

void clear_Players(player *p)
{
    free(p);
}

void move_Players(player *p, int num_of_players, int speed, double deltaTime)
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        SDL_Rect predict = p[i].hitbox;
        predict.x += p[i].directionX * speed * deltaTime;
        predict.y += p[i].directionY * speed * deltaTime;

        int collision = 0;

        for (size_t j = 0; j < num_of_players; j++)
        {
            if (i == j)
            {
                continue;
            }

            if (SDL_HasIntersection(&predict, &p[j].hitbox))
            {
                collision = 1;
                break;
            }
        }

        if (!collision)
        {
            p[i].hitbox = predict;
        }
    }
}

void render_Players(player *p, int num_of_players, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (size_t i = 0; i < num_of_players; i++)
    {
        SDL_RenderFillRect(renderer, &p[i].hitbox);
    }
}

void input_Players(player *p, int num_of_players, SDL_Event event)
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        SDL_Keycode code = event.key.keysym.sym;
        if (event.type == SDL_KEYDOWN)
        {
            if (code == p[i].keybinds.up)
            {
                p[i].directionY = -1;
            }
            else if (code == p[i].keybinds.down)
            {
                p[i].directionY = 1;
            }
            else if (code == p[i].keybinds.left)
            {
                p[i].directionX = -1;
            }
            else if (code == p[i].keybinds.right)
            {
                p[i].directionX = 1;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            if (code == p[i].keybinds.up)
            {
                p[i].directionY = 0;
            }
            else if (code == p[i].keybinds.down)
            {
                p[i].directionY = 0;
            }
            else if (code == p[i].keybinds.left)
            {
                p[i].directionX = 0;
            }
            else if (code == p[i].keybinds.right)
            {
                p[i].directionX = 0;
            }
        }
    }
}
