#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "sceneManager.h"
#include "engine.h"
#include "ui.h"

#define MAX_SCENES 2
#define MAX_BUTTONS_PER_SCENE 10

static scene scenes[MAX_SCENES];
static sceneManager scm = {.current_Scene = NULL};

scene create_menu_scene()
{
    scene s = {0};

    s.scene_index = 0;
    s.scene_name = "Menu";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "./assets/maps/00_Menu/menu_test.png";

    s.buttons[0] = (button){
        .isActive = 1,
        .button = {412, 238, 200, 100},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .text = "Play",
        .text_color = {255, 255, 255, 255},
        .onClick = on_start_game,
        .state = BUTTON_NORMAL};
    s.buttons[1] = (button){
        .isActive = 1,
        .button = {25, 476 - 25, 200, 100},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .text = "Exit",
        .text_color = {255, 255, 255, 255},
        .onClick = on_quit,
        .state = BUTTON_NORMAL};
    s.colMap.width = 0;
    s.colMap.height = 0;
    s.colMap.tiles = NULL;

    return s;
}

scene create_map1_scene()
{
    scene s = {0};

    s.scene_index = 1;
    s.scene_name = "Grass";
    s.have_players = 1;
    s.respawn_timer = 5.0f;
    s.bg_texture_address = "./assets/maps/01_Grass/map1_scaled.png";
    s.colMap.width = 17;
    s.colMap.height = 10;
    static int map1[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    s.colMap.tiles = map1;

    return s;
}

void init_scenes()
{
    scenes[0] = create_menu_scene();
    scenes[1] = create_map1_scene();

    scm.current_Scene = &scenes[0];
}

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
    clear_ui();
    scm.current_Scene = &scenes[scene_index];
    printf("Loading scene: %s\n", scm_get_scene(scene_index)->scene_name);
    load_ui();
}

int scm_load_textures()
{
    SDL_Renderer *renderer = eng_get()->renderer;

    for (size_t i = 0; i < MAX_SCENES; i++)
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
    for (size_t i = 0; i < MAX_SCENES; i++)
    {
        SDL_DestroyTexture(scenes[i].bg_texture);
    }
}

void scm_render_collisionMap()
{
    collisionMap *colMap = &scm.current_Scene->colMap;

    SDL_Renderer *renderer = eng_get()->renderer;

    SDL_SetRenderDrawColor(renderer, 255, 0, 128, 100);

    for (size_t y = 0; y < colMap->height; y++)
    {
        for (size_t x = 0; x < colMap->width; x++)
        {
            if (colMap->tiles[y * colMap->width + x] == 0)
            {
                continue;
            }

            SDL_Rect colBox = {.h = TILE_SIZE, .w = TILE_SIZE, .x = x * TILE_SIZE, .y = y * TILE_SIZE};

            SDL_RenderFillRect(renderer, &colBox);
        }
    }
}