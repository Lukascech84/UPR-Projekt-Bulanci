#include <SDL2/SDL.h>
#include <stdio.h>
#include "weaponPickup.h"
#include "sceneManager.h"
#include "engine.h"
#include "weapon.h"
#include "player.h"

void spawn_weapon_pickup()
{
    sceneManager *scm = scm_get_scm();

    if (scm->weapon_pickup.isActive)
    {
        return;
    }

    int random_weaponID = rand() % (get_num_of_weapons() - 1) + 1;

    SDL_Rect spawn = get_random_spawn(32, 32);

    scm->weapon_pickup.weaponID = random_weaponID;
    scm->weapon_pickup.rect.x = spawn.x;
    scm->weapon_pickup.rect.y = spawn.y;
    scm->weapon_pickup.rect.w = WEAPON_PICKUP_SIZE;
    scm->weapon_pickup.rect.h = WEAPON_PICKUP_SIZE;
    scm->weapon_pickup.respawn_timer = WEAPON_PICKUP_RESPAWN_TIME;
    scm->weapon_pickup.respawn_timer_elapsed = 0.0f;
    scm->weapon_pickup.isActive = 1;

    printf("Spawned weapon pickup %s at x=%d y=%d\n", get_weapon(random_weaponID)->weapon_name, spawn.x, spawn.y);
}

void render_weapon_pickup()
{
    sceneManager *scm = scm_get_scm();

    if (!scm->weapon_pickup.isActive)
    {
        return;
    }

    SDL_Renderer *renderer = eng_get()->renderer;
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &scm->weapon_pickup.rect);
}

void update_weapon_pickup_respawn_timer()
{
    sceneManager *scm = scm_get_scm();

    if (scm->weapon_pickup.isActive)
    {
        return;
    }

    // printf("Time remaining %.2f\n", scm->weapon_pickup.respawn_timer - scm->weapon_pickup.respawn_timer_elapsed);

    if (scm->weapon_pickup.respawn_timer_elapsed >= scm->weapon_pickup.respawn_timer)
    {
        spawn_weapon_pickup();
    }

    scm->weapon_pickup.respawn_timer_elapsed += eng_get()->deltaTime;
}

void clear_weapon_pickup()
{
    sceneManager *scm = scm_get_scm();

    scm->weapon_pickup.isActive = 0;
    scm->weapon_pickup.weaponID = -1;
    scm->weapon_pickup.respawn_timer_elapsed = 0.0f;
    scm->weapon_pickup.respawn_timer = WEAPON_PICKUP_RESPAWN_TIME;
}
