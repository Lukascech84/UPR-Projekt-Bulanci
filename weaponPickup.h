#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "engine.h"
#include "ui.h"
#include "collisionMap.h"

#define WEAPON_PICKUP_SIZE 32
#define WEAPON_PICKUP_RESPAWN_TIME 10.0f

typedef struct
{
    SDL_Rect rect;
    int isActive;
    int weaponID;
    float respawn_timer;
    float respawn_timer_elapsed;
} weaponPickup;

void spawn_weapon_pickup();

void render_weapon_pickup();

void update_weapon_pickup_respawn_timer();

void clear_weapon_pickup();