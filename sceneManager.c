#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sceneManager.h"

void load_scene(sceneManager *scm, int scene_index, scene scenes[])
{
    scm->current_Scene = &scenes[scene_index];
}

int load_textures(scene scenes[], int num_of_scenes, SDL_Renderer *renderer)
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

void destroy_textures(scene scenes[], int num_of_scenes)
{
    for (size_t i = 0; i < num_of_scenes; i++)
    {
        SDL_DestroyTexture(scenes[i].bg_texture);
    }
}