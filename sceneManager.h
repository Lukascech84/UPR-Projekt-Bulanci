#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    int scene_index;
    char scene_name[50];

    // Map data
    int starting_respawn_timer;
    int have_players;
    char *bg_texture_address;
    SDL_Texture *bg_texture;

} scene;

typedef struct
{
    scene *current_Scene;
    int players_spawned;

} sceneManager;

sceneManager *scm_get_scm();

scene *scm_get_scene(int);

void scm_load_scene(int);

int scm_load_textures(SDL_Renderer *);

void scm_destroy_textures();