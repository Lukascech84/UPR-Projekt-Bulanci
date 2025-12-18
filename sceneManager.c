#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sceneManager.h"

static int num_of_scenes = 2;
static scene scenes[2] = {{.scene_index = 0, .scene_name = "Menu", .have_players = 0, .bg_texture_address = "./assets/maps/00_Menu/menu_test.png"},
                          {.scene_index = 1, .scene_name = "Mapa1", .have_players = 1, .bg_texture_address = "./assets/maps/01_Grass/map1_scaled.png"}};

static sceneManager scm = {.current_Scene = &scenes[0]};

sceneManager *scm_get_scm()
{
    return &scm;
}

scene *scm_get_scene(int index)
{
    return &scenes[index];
}

void scm_load_scene(int scene_index)
{
    scm.current_Scene = &scenes[scene_index];
}

int scm_load_textures(SDL_Renderer *renderer)
{
    for (size_t i = 0; i < num_of_scenes; i++)
    {
        SDL_Surface *background_sur = IMG_Load(scenes[i].bg_texture_address);
        if (background_sur == NULL)
        {
            printf("Error loading image: %s", IMG_GetError());
            return 1;
        }
        SDL_Texture *background_tex = SDL_CreateTextureFromSurface(renderer, background_sur);
        if (background_tex == NULL)
        {
            printf("Error creating texture");
            return 1;
        }
        scenes[i].bg_texture = background_tex;
        SDL_FreeSurface(background_sur);
    }
    return 0;
}

void scm_destroy_textures()
{
    for (size_t i = 0; i < num_of_scenes; i++)
    {
        SDL_DestroyTexture(scenes[i].bg_texture);
    }
}