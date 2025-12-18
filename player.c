#include <SDL2/SDL.h>
#include "player.h"

static int num_of_players = 4;
static float player_speed = 200;

static player *players = NULL;

void *init_Players()
{
    player_keybinds keysets[4] = {{.up = SDL_SCANCODE_W, .down = SDL_SCANCODE_S, .left = SDL_SCANCODE_A, .right = SDL_SCANCODE_D, .shoot = SDL_SCANCODE_E},
                                  {.up = SDL_SCANCODE_UP, .down = SDL_SCANCODE_DOWN, .left = SDL_SCANCODE_LEFT, .right = SDL_SCANCODE_RIGHT, .shoot = SDL_SCANCODE_RSHIFT},
                                  {.up = SDL_SCANCODE_I, .down = SDL_SCANCODE_K, .left = SDL_SCANCODE_J, .right = SDL_SCANCODE_L, .shoot = SDL_SCANCODE_O},
                                  {.up = SDL_SCANCODE_T, .down = SDL_SCANCODE_G, .left = SDL_SCANCODE_F, .right = SDL_SCANCODE_H, .shoot = SDL_SCANCODE_SPACE}};
    players = realloc(players, num_of_players * sizeof(player));
    if (!players)
    {
        return NULL;
    }

    for (size_t i = 0; i < num_of_players; i++)
    {
        players[i].playerID = i;
        players[i].directionX = 0;
        players[i].directionY = 0;
        players[i].hitbox.h = 100;
        players[i].hitbox.w = 100;
        players[i].hitbox.x = (1.5f * i) * 100;
        players[i].hitbox.y = 100;
        players[i].current_weapon = get_weapon(0);
        players[i].keybinds = keysets[i];
    }
}

void clear_Players()
{
    free(players);
    players = NULL;
}

void move_Players(double deltaTime)
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        SDL_Rect predict = players[i].hitbox;
        predict.x += players[i].directionX * player_speed * deltaTime;
        predict.y += players[i].directionY * player_speed * deltaTime;

        int collision = 0;

        for (size_t j = 0; j < num_of_players; j++)
        {
            if (i == j)
            {
                continue;
            }

            if (SDL_HasIntersection(&predict, &players[j].hitbox))
            {
                collision = 1;
                break;
            }
        }

        if (!collision)
        {
            players[i].hitbox = predict;
        }
    }
}

void render_Players(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (size_t i = 0; i < num_of_players; i++)
    {
        SDL_RenderFillRect(renderer, &players[i].hitbox);
    }
}

void input_Players(SDL_Renderer *renderer)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    for (size_t i = 0; i < num_of_players; i++)
    {
        // Y
        if (state[players[i].keybinds.up])
            players[i].directionY = -1;
        else if (state[players[i].keybinds.down])
            players[i].directionY = 1;
        else
            players[i].directionY = 0;

        // X
        if (state[players[i].keybinds.left])
            players[i].directionX = -1;
        else if (state[players[i].keybinds.right])
            players[i].directionX = 1;
        else
            players[i].directionX = 0;

        // Shoot
        if (state[players[i].keybinds.shoot])
            shoot_Players(&players[i], renderer);
    }
}

void resize_Players(int new_h, int new_w)
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        players[i].hitbox.h = new_h;
        players[i].hitbox.w = new_w;
    }
}

void shoot_Players(player *p, SDL_Renderer *renderer)
{
    printf("Player %d shot from %s.\n", p->playerID, p->current_weapon->weapon_name);
    spawn_bullet(renderer, p->hitbox.x, p->hitbox.y, *p->current_weapon);
}
