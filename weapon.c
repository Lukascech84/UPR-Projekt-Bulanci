#include <stdio.h>
#include <SDL2/SDL.h>
#include "engine.h"
#include "weapon.h"
#include "player.h"

static bullet fired_bullets[MAX_BULLETS];

static int num_of_weapons = 2;
static weapon weapons[2] = {{.weaponID = 0, .weapon_name = "Pistol", .num_of_bullets = 1, .bullet_velocity = 250.0f, .fire_rate = 1.0f, .fire_timer = 0.0f, .max_ammo = -1, .weapon_texture = NULL},
                            {.weaponID = 1, .weapon_name = "AK-47", .num_of_bullets = 1, .bullet_velocity = 350.0f, .fire_rate = 0.4f, .fire_timer = 0.0f, .max_ammo = 15, .weapon_texture = NULL}};

weapon *get_weapon(int index)
{
    if (index >= num_of_weapons || index < 0)
    {
        return NULL;
    }
    else
    {
        return &weapons[index];
    }
}

void change_weapon(player *p, int w)
{
    p->current_weapon = &weapons[w];
    p->current_ammo_in_weapon = weapons[w].max_ammo;
}

void init_bullet()
{
    for (size_t i = 0; i < MAX_BULLETS; i++)
    {
        fired_bullets[i].active = 0;
    }
}

void spawn_bullet(player *p)
{
    SDL_Renderer *renderer = eng_get()->renderer;

    for (size_t i = 0; i < MAX_BULLETS; i++)
    {
        if (fired_bullets[i].active == 0)
        {
            fired_bullets[i].playerID = p->playerID;
            fired_bullets[i].active = 1;
            fired_bullets[i].directionX = p->aimX;
            fired_bullets[i].directionY = p->aimY;
            // fired_bullets[i].bullet_spread = 0;
            fired_bullets[i].bullet_texture = p->current_weapon->bullet_texture;
            fired_bullets[i].bullet_velocity = p->current_weapon->bullet_velocity;
            fired_bullets[i].hitbox.h = 7;
            fired_bullets[i].hitbox.w = 7;
            fired_bullets[i].posX = p->aimX == 0 ? p->hitbox.x + p->hitbox.w / 2 : p->hitbox.x + (p->aimX == -1 ? 0 : p->hitbox.w);
            fired_bullets[i].posY = p->aimY == 0 ? p->hitbox.y + p->hitbox.h / 2 : p->hitbox.y + (p->aimY == -1 ? 0 : p->hitbox.h);
            fired_bullets[i].hitbox.x = (int)fired_bullets[i].posX;
            fired_bullets[i].hitbox.y = (int)fired_bullets[i].posY;
            return;
        }
    }
}

void render_bullet()
{
    SDL_Renderer *renderer = eng_get()->renderer;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (size_t i = 0; i < MAX_BULLETS; i++)
    {
        if (fired_bullets[i].active)
        {
            SDL_RenderFillRect(renderer, &fired_bullets[i].hitbox);
        }
    }
}

void update_bullet()
{
    for (size_t i = 0; i < MAX_BULLETS; i++)
    {
        if (!fired_bullets[i].active)
        {
            continue;
        }

        double deltaTime = eng_get()->deltaTime;

        double dx = fired_bullets[i].directionX;
        double dy = fired_bullets[i].directionY;

        double len = sqrt(dx * dx + dy * dy);
        if (len > 0)
        {
            dx /= len;
            dy /= len;
        }

        fired_bullets[i].posX += (dx * fired_bullets[i].bullet_velocity * deltaTime);
        fired_bullets[i].posY += (dy * fired_bullets[i].bullet_velocity * deltaTime);

        fired_bullets[i].hitbox.x = (int)fired_bullets[i].posX;
        fired_bullets[i].hitbox.y = (int)fired_bullets[i].posY;

        if (map_collides_rect(&fired_bullets[i].hitbox) == 1)
        {
            destroy_bullet(i);
        }

        for (size_t j = 0; j < get_Num_Of_Players(); j++)
        {
            if (fired_bullets[i].playerID == j || !get_Players()[j].isAlive)
            {
                continue;
            }

            if (SDL_HasIntersection(&fired_bullets[i].hitbox, &get_Players()[j].hitbox))
            {
                get_Players()[fired_bullets[i].playerID].score++;
                kill_Player(j);
                destroy_bullet(i);
                printf("Player %ld got killed by player %d with score %d\n", j, fired_bullets[i].playerID, get_Players()[fired_bullets[i].playerID].score);
                break;
            }
        }
    }
}

void destroy_bullet(int i)
{
    fired_bullets[i].active = 0;
    fired_bullets[i].directionX = 0;
    fired_bullets[i].directionY = 0;
    // fired_bullets[i].bullet_spread = 0;
    fired_bullets[i].bullet_texture = NULL;
    fired_bullets[i].bullet_velocity = 0;
    fired_bullets[i].hitbox.h = 0;
    fired_bullets[i].hitbox.w = 0;
    fired_bullets[i].hitbox.x = 0;
    fired_bullets[i].hitbox.y = 0;
    fired_bullets[i].posX = 0;
    fired_bullets[i].posY = 0;
}