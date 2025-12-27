#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "engine.h"
#include "ui.h"

#define MAX_BUTTONS_PER_SCENE 10

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

} scene;

typedef struct
{
    scene *current_Scene;
    int players_spawned;

} sceneManager;

scene create_menu_scene();

scene create_map1_scene();

void init_scenes();

sceneManager *scm_get_scm();

scene *scm_get_scene(int);

void scm_load_scene(int);

int scm_load_textures();

void scm_destroy_textures();