#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    int scene_index;
    char scene_name[50];

    // Map data
    char *bg_texture_address;
    SDL_Texture *bg_texture;

} scene;

typedef struct
{
    scene *current_Scene;

} sceneManager;

void load_scene(sceneManager *scm, int scene_index, scene scenes[]);

int load_textures(scene scenes[], int num_of_scenes, SDL_Renderer *renderer);

void destroy_textures(scene scenes[], int num_of_scenes);