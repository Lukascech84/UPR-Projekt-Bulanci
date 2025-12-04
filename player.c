#include <SDL2/SDL.h>
#include "player.h"

player *init_Players(player *p, int num_of_players)
{
    player_keybinds keysets[4] = {{.up = SDL_SCANCODE_W, .down = SDL_SCANCODE_S, .left = SDL_SCANCODE_A, .right = SDL_SCANCODE_D, .shoot = SDL_SCANCODE_E},
                                  {.up = SDL_SCANCODE_UP, .down = SDL_SCANCODE_DOWN, .left = SDL_SCANCODE_LEFT, .right = SDL_SCANCODE_RIGHT, .shoot = SDL_SCANCODE_RSHIFT},
                                  {.up = SDL_SCANCODE_I, .down = SDL_SCANCODE_K, .left = SDL_SCANCODE_J, .right = SDL_SCANCODE_L, .shoot = SDL_SCANCODE_O},
                                  {.up = SDL_SCANCODE_T, .down = SDL_SCANCODE_G, .left = SDL_SCANCODE_F, .right = SDL_SCANCODE_H, .shoot = SDL_SCANCODE_SPACE}};
    p = realloc(p, num_of_players * sizeof(player));
    if (!p)
    {
        return NULL;
    }

    for (size_t i = 0; i < num_of_players; i++)
    {
        p[i].playerID = i;
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

void input_Players(player *p, int num_of_players)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    for (size_t i = 0; i < num_of_players; i++)
    {
        // Y
        if (state[p[i].keybinds.up])
            p[i].directionY = -1;
        else if (state[p[i].keybinds.down])
            p[i].directionY = 1;
        else
            p[i].directionY = 0;

        // X
        if (state[p[i].keybinds.left])
            p[i].directionX = -1;
        else if (state[p[i].keybinds.right])
            p[i].directionX = 1;
        else
            p[i].directionX = 0;

        // Shoot
        if (state[p[i].keybinds.shoot])
            shoot_Players(p[i]);
    }
}

void resize_Players(player *p, int num_of_players, int new_h, int new_w)
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        p[i].hitbox.h = new_h;
        p[i].hitbox.w = new_w;
    }
}

void shoot_Players(player p)
{
    printf("Player %d shot.\n", p.playerID);
}
