#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "engine.h"
#include "ui.h"
#include "collisionMap.h"
#include "weaponPickup.h"

#define MAX_BUTTONS_PER_SCENE 10
#define MAX_TEXTFIELDS_PER_SCENE 10

typedef struct
{
    int scene_index;
    char *scene_name;

    // Map data
    float respawn_timer;
    int have_players;
    char *bg_texture_address;
    SDL_Texture *bg_texture;
    button buttons[MAX_BUTTONS_PER_SCENE];
    textField textFields[MAX_TEXTFIELDS_PER_SCENE];
    collisionMap colMap;

} scene;

typedef struct
{
    scene *current_Scene;
    int last_scene_index;

    int players_spawned;

    weaponPickup weapon_pickup;

} sceneManager;

scene create_menu_scene();

scene create_start_screen_scene();

scene create_settings_scene();

scene create_leaderboard_scene();

scene create_map1_scene();

scene create_map2_scene();

void init_scenes();

sceneManager *scm_get_scm();

scene *scm_get_scene(int);

void scm_load_scene(int);

int scm_load_textures();

void scm_destroy_textures();

void scm_render_collisionMap();