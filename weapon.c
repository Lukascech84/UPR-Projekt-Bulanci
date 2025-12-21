#include "weapon.h"
#include "player.h"

static bullet fired_bullets[MAX_BULLETS];

static int num_of_weapons = 2;
static weapon weapons[2] = {{.weapon_name = "Pistol", .num_of_bullets = 1, .bullet_velocity = 5, .weapon_texture = NULL},
                            {.weapon_name = "AK-47", .num_of_bullets = 1, .bullet_velocity = 10, .weapon_texture = NULL}};

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

void init_bullet()
{
    for (size_t i = 0; i < MAX_BULLETS; i++)
    {
        fired_bullets[i].active = 0;
    }
}

void spawn_bullet(SDL_Renderer *renderer, player *p)
{
    for (size_t i = 0; i < MAX_BULLETS; i++)
    {
        if (fired_bullets[i].active == 0)
        {
            fired_bullets[i].playerID = p->playerID;
            fired_bullets[i].active = 1;
            fired_bullets[i].lastDirectionX = p->directionX;
            fired_bullets[i].lastDirectionY = p->directionY;
            fired_bullets[i].directionX = p->directionX;
            fired_bullets[i].directionY = p->directionY;
            // fired_bullets[i].bullet_spread = 0;
            fired_bullets[i].bullet_texture = p->current_weapon->bullet_texture;
            fired_bullets[i].bullet_velocity = p->current_weapon->bullet_velocity;
            fired_bullets[i].hitbox.h = 20;
            fired_bullets[i].hitbox.w = 20;
            fired_bullets[i].hitbox.x = p->hitbox.x;
            fired_bullets[i].hitbox.y = p->hitbox.y;
            return;
        }
    }
}

void render_bullet(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
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

        fired_bullets[i].hitbox.x += (fired_bullets[i].directionX * fired_bullets[i].bullet_velocity);
        fired_bullets[i].hitbox.y += (fired_bullets[i].directionY * fired_bullets[i].bullet_velocity);

        for (size_t j = 0; j < get_Num_Of_Players(); j++)
        {
            if (fired_bullets[i].playerID == j || !get_Players()[j].isAlive)
            {
                continue;
            }

            if (SDL_HasIntersection(&fired_bullets[i].hitbox, &get_Players()[j].hitbox))
            {
                printf("Player %ld got killed by player %d\n", j, fired_bullets[i].playerID);
                kill_Player(j);
                destroy_bullet(i);
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
}