#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "engine.h"
#include "weapon.h"
#include "player.h"

static int num_of_players = 4;

static SDL_Color player_colors[4] = {
    {.r = 128, .g = 0, .b = 0, .a = 255},
    {.r = 51, .g = 153, .b = 51, .a = 255},
    {.r = 0, .g = 0, .b = 253, .a = 255},
    {.r = 255, .g = 153, .b = 0, .a = 255}};

static player *players = NULL;

void init_Players()
{
    player_keybinds keysets[4] = {{.up = SDL_SCANCODE_W, .down = SDL_SCANCODE_S, .left = SDL_SCANCODE_A, .right = SDL_SCANCODE_D, .shoot = SDL_SCANCODE_E},
                                  {.up = SDL_SCANCODE_UP, .down = SDL_SCANCODE_DOWN, .left = SDL_SCANCODE_LEFT, .right = SDL_SCANCODE_RIGHT, .shoot = SDL_SCANCODE_RSHIFT},
                                  {.up = SDL_SCANCODE_I, .down = SDL_SCANCODE_K, .left = SDL_SCANCODE_J, .right = SDL_SCANCODE_L, .shoot = SDL_SCANCODE_O},
                                  {.up = SDL_SCANCODE_T, .down = SDL_SCANCODE_G, .left = SDL_SCANCODE_F, .right = SDL_SCANCODE_H, .shoot = SDL_SCANCODE_Y}};
    players = realloc(players, num_of_players * sizeof(player));

    for (size_t i = 0; i < num_of_players; i++)
    {
        players[i].playerID = i;
        players[i].playerName = i;
        players[i].player_speed = 125;
        players[i].directionX = 1;
        players[i].directionY = 0;
        players[i].aimX = 1;
        players[i].aimY = 0;
        players[i].hitbox.h = 48;
        players[i].hitbox.w = 48;
        SDL_Rect spawn = get_random_spawn(players[i].hitbox.w, players[i].hitbox.h);
        players[i].posX = spawn.x;
        players[i].posY = spawn.y;
        players[i].hitbox.x = (int)players[i].posX;
        players[i].hitbox.y = (int)players[i].posY;
        change_weapon(&players[i], 1);
        players[i].keybinds = keysets[i];
        players[i].score = 0;
        players[i].player_color = &player_colors[i];
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

void update_Players()
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        players[i].current_weapon->fire_timer += eng_get()->deltaTime;

        update_Player_Respawn(&players[i]);
        move_Player(&players[i]);
        check_player_weapon_pickup_collision(&players[i]);
    }
}

void move_Player(player *p)
{
    double deltaTime = eng_get()->deltaTime;

    if (!p->isAlive)
    {
        return;
    }

    double dx = p->directionX;
    double dy = p->directionY;

    int w, h = 0;
    SDL_GetWindowSize(eng_get()->window, &w, &h);

    // NORMALIZACE
    double len = sqrt(dx * dx + dy * dy);
    if (len > 0)
    {
        dx /= len;
        dy /= len;
    }

    float stepX = (dx * p->player_speed * deltaTime);
    float stepY = (dy * p->player_speed * deltaTime);

    SDL_Rect predict;

    // X osa
    float newX = p->posX + stepX;

    predict = p->hitbox;
    predict.x = (int)newX;

    int collision = map_collides_rect(&predict) != 0 ? 1 : 0;

    if (predict.x < 0 || (predict.x + predict.w) > w)
    {
        collision = 1;
    }

    for (size_t j = 0; j < num_of_players && !collision; j++)
    {
        if (p->playerID == j || !players[j].isAlive)
            continue;

        if (SDL_HasIntersection(&predict, &players[j].hitbox))
            collision = 1;
    }

    if (!collision)
    {
        p->posX = newX;
        p->hitbox.x = (int)p->posX;
    }

    // Y osa
    float newY = p->posY + stepY;

    predict = p->hitbox;
    predict.y = (int)newY;

    collision = map_collides_rect(&predict) != 0 ? 1 : 0;

    if (predict.y < 0 || (predict.y + predict.h) > h)
    {
        collision = 1;
    }

    for (size_t j = 0; j < num_of_players && !collision; j++)
    {
        if (p->playerID == j || !players[j].isAlive)
            continue;

        if (SDL_HasIntersection(&predict, &players[j].hitbox))
            collision = 1;
    }

    if (!collision)
    {
        p->posY = newY;
        p->hitbox.y = (int)p->posY;
    }

    if (p->directionX != 0 || p->directionY != 0)
    {
        p->aimX = p->directionX;
        p->aimY = p->directionY;
    }
}

void render_Players()
{
    SDL_Renderer *renderer = eng_get()->renderer;

    for (size_t i = 0; i < num_of_players; i++)
    {
        if (!players[i].isAlive)
        {
            continue;
        }
        SDL_SetRenderDrawColor(renderer, players[i].player_color->r, players[i].player_color->g, players[i].player_color->b, players[i].player_color->a);
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
        // printf("Player %d shot from %s:%d\n", p->playerID, p->current_weapon->weapon_name, p->current_ammo_in_weapon);
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

void update_Player_Respawn(player *p)
{
    if (p->isAlive)
    {
        return;
    }

    p->respawn_timer_elapsed += eng_get()->deltaTime;

    if (p->respawn_timer_elapsed >= p->respawn_timer)
    {
        respawn_Player(p->playerID);
    }
}

void respawn_Player(size_t i)
{
    SDL_Rect spawn = get_random_spawn(players[i].hitbox.w, players[i].hitbox.h);

    players[i].isAlive = 1;
    players[i].respawn_timer_elapsed = 0.0f;

    players[i].posX = spawn.x;
    players[i].posY = spawn.y;
    players[i].hitbox.x = (int)players[i].posX;
    players[i].hitbox.y = (int)players[i].posY;
}

SDL_Rect get_random_spawn(int player_w, int player_h)
{
    int w = 0, h = 0;
    SDL_GetWindowSize(eng_get()->window, &w, &h);

    SDL_Rect spawn;

    int i = 0;

    do
    {
        spawn.w = player_w;
        spawn.h = player_h;
        spawn.x = rand() % (w - player_w);
        spawn.y = rand() % (h - player_h);

        i++;
        if (i > 1000)
        {
            spawn.x = 50;
            spawn.y = 50;
            break;
        }
    } while (map_collides_rect(&spawn) != 0 || interects_with_any_player(&spawn) != 0);

    // printf("Spawn: x=%d y=%d w=%d h=%d\n", spawn.x, spawn.y, spawn.w, spawn.h);

    return spawn;
}

int interects_with_any_player(SDL_Rect *rect)
{
    for (size_t i = 0; i < num_of_players; i++)
    {
        if (!players[i].isAlive)
        {
            continue;
        }

        if (SDL_HasIntersection(rect, &players[i].hitbox))
        {
            return 1;
        }
    }

    return 0;
}

void check_player_weapon_pickup_collision(player *p)
{
    sceneManager *scm = scm_get_scm();
    if (SDL_HasIntersection(&p->hitbox, &scm->weapon_pickup.rect) && scm->weapon_pickup.isActive)
    {
        change_weapon(p, scm->weapon_pickup.weaponID);
        p->current_ammo_in_weapon = p->current_weapon->max_ammo;
        clear_weapon_pickup();

        printf("Player %d picked up weapon %s\n", p->playerID, p->current_weapon->weapon_name);
    }
}
