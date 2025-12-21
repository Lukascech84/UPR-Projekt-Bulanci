#include <SDL2/SDL.h>
#include "player.h"
#include "sceneManager.h"

static int num_of_players = 4;

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
        players[i].player_speed = 200;
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

int get_Num_Of_Players()
{
    return num_of_players;
}

player *get_Players()
{
    return players;
}

void move_Players(double deltaTime)
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        SDL_Rect predict = players[i].hitbox;
        predict.x += players[i].directionX * players[i].player_speed * deltaTime;
        predict.y += players[i].directionY * players[i].player_speed * deltaTime;

        int collision = 0;

        for (size_t j = 0; j < num_of_players; j++)
        {
            if (i == j || !players[j].isAlive)
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
        if (!players[i].isAlive)
        {
            continue;
        }

        SDL_RenderFillRect(renderer, &players[i].hitbox);
    }
}

void input_Players(SDL_Renderer *renderer)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    for (size_t i = 0; i < num_of_players; i++)
    {
        if (!players[i].isAlive)
        {
            continue;
        }

        // Y
        if (state[players[i].keybinds.up])
            players[i].directionY = -1;
        else if (state[players[i].keybinds.down])
            players[i].directionY = 1;
        else if (players[i].directionY != 0)
        {
            players[i].directionY = 0;
        }

        // X
        if (state[players[i].keybinds.left])
            players[i].directionX = -1;
        else if (state[players[i].keybinds.right])
            players[i].directionX = 1;
        else if (players[i].directionX != 0)
        {
            players[i].directionX = 0;
        }

        // Shoot
        if (state[players[i].keybinds.shoot])
            shoot_Player(&players[i], renderer);
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

void shoot_Player(player *p, SDL_Renderer *renderer)
{
    printf("Player %d shot from %s.\n", p->playerID, p->current_weapon->weapon_name);
    spawn_bullet(renderer, p);
}

void kill_Player(int i)
{
    players[i].isAlive = 0;
    players[i].respawn_timer_elapsed = 0.0f;
    players[i].respawn_timer = scm_get_scm()->current_Scene->respawn_timer;
}

void update_Players_Respawn(double deltaTime)
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        if (players[i].isAlive)
        {
            continue;
        }

        players[i].respawn_timer_elapsed += deltaTime;

        if (players[i].respawn_timer_elapsed >= players[i].respawn_timer)
        {
            respawn_Player(i);
        }
    }
}

void respawn_Player(int i)
{
    players[i].isAlive = 1;
    players[i].respawn_timer_elapsed = 0.0f;

    players[i].hitbox.x = (1.5f * i) * 100;
    players[i].hitbox.y = 100;
}