#include "weapon.h"

bullet fired_bullets[MAX_BULLETS];

void init_bullet(bullet *fired_bullets)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        fired_bullets[i].active = 0;
    }
}

void spawn_bullet(SDL_Renderer *renderer, int x_pos, int y_pos, weapon current_weapon)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (fired_bullets[i].active == 0)
        {
            fired_bullets[i].active = 1;
            // fired_bullets[i].bullet_spread = 0;
            fired_bullets[i].bullet_texture = current_weapon.bullet_texture;
            fired_bullets[i].bullet_velocity = current_weapon.bullet_velocity;
            fired_bullets[i].hitbox.h = 20;
            fired_bullets[i].hitbox.w = 20;
            fired_bullets[i].hitbox.x = x_pos;
            fired_bullets[i].hitbox.y = y_pos;
            return;
        }
    }
}