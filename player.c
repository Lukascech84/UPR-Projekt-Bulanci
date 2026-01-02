#include <SDL2/SDL.h>
#include <math.h>
#include "engine.h"
#include "weapon.h"
#include "player.h"

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
        players[i].playerName = i;
        players[i].player_speed = 200;
        players[i].directionX = 1;
        players[i].directionY = 0;
        players[i].aimX = 1;
        players[i].aimY = 0;
        players[i].hitbox.h = 48;
        players[i].hitbox.w = 48;
        players[i].hitbox.x = i * 100 + 200;
        players[i].hitbox.y = 350;
        change_weapon(&players[i], 1);
        players[i].keybinds = keysets[i];
        players[i].score = 0;
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

void move_Players()
{
    double deltaTime = eng_get()->deltaTime;

    for (size_t i = 0; i < num_of_players; i++)
    {
        if (!players[i].isAlive)
            continue;

        double dx = players[i].directionX;
        double dy = players[i].directionY;

        int w, h = 0;
        SDL_GetWindowSize(eng_get()->window, &w, &h);

        // NORMALIZACE
        double len = sqrt(dx * dx + dy * dy);
        if (len > 0)
        {
            dx /= len;
            dy /= len;
        }

        int stepX = (int)(dx * players[i].player_speed * deltaTime);
        int stepY = (int)(dy * players[i].player_speed * deltaTime);

        SDL_Rect predict;

        // X osa
        predict = players[i].hitbox;
        predict.x += stepX;

        int collision = map_collides_rect(&predict);

        if (predict.x < 0 || predict.x > w)
        {
            collision = 1;
        }

        for (size_t j = 0; j < num_of_players && !collision; j++)
        {
            if (i == j || !players[j].isAlive)
                continue;

            if (SDL_HasIntersection(&predict, &players[j].hitbox))
                collision = 1;
        }

        if (!collision)
            players[i].hitbox.x = predict.x;

        // Y osa
        predict = players[i].hitbox;
        predict.y += stepY;

        collision = map_collides_rect(&predict);

        if (predict.y < 0 || predict.x > h)
        {
            collision = 1;
        }

        for (size_t j = 0; j < num_of_players && !collision; j++)
        {
            if (i == j || !players[j].isAlive)
                continue;

            if (SDL_HasIntersection(&predict, &players[j].hitbox))
                collision = 1;
        }

        if (!collision)
        {
            players[i].hitbox.y = predict.y;
        }

        if (players[i].directionX != 0 || players[i].directionY != 0)
        {
            players[i].aimX = players[i].directionX;
            players[i].aimY = players[i].directionY;
        }

        players[i].current_weapon->fire_timer += deltaTime;
    }
}

void render_Players()
{
    SDL_Renderer *renderer = eng_get()->renderer;

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

void input_Players()
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
            shoot_Player(&players[i]);
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

void shoot_Player(player *p)
{
    if (p->current_weapon->fire_timer >= p->current_weapon->fire_rate)
    {
        spawn_bullet(p);
        p->current_weapon->fire_timer = 0.0f;
        if (p->current_ammo_in_weapon > 0)
        {
            p->current_ammo_in_weapon--;
        }
        printf("Player %d shot from %s:%d\n", p->playerID, p->current_weapon->weapon_name, p->current_ammo_in_weapon);
        if (p->current_ammo_in_weapon == 0)
        {
            change_weapon(p, 0);
        }
    }
}

void kill_Player(int i)
{
    players[i].isAlive = 0;
    players[i].respawn_timer_elapsed = 0.0f;
    players[i].respawn_timer = scm_get_scm()->current_Scene->respawn_timer;
}

void update_Players_Respawn()
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        if (players[i].isAlive)
        {
            continue;
        }

        players[i].respawn_timer_elapsed += eng_get()->deltaTime;

        if (players[i].respawn_timer_elapsed >= players[i].respawn_timer)
        {
            respawn_Player(i);
        }
    }
}

void respawn_Player(size_t i)
{
    players[i].isAlive = 1;
    players[i].respawn_timer_elapsed = 0.0f;

    players[i].hitbox.x = i * 100 + 200;
    players[i].hitbox.y = 300;
}