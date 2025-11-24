#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    int scene_index;
    char scene_name[50];

    // Map data

} scene;

typedef struct
{
    scene current_Scene;

} sceneManager;

void load_scene(sceneManager *scm, int scene_index, scene scenes[])
{
    scm->current_Scene = scenes[scene_index];
}