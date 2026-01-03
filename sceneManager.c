#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "sceneManager.h"
#include "engine.h"
#include "ui.h"
#include "weapon.h"
#include "player.h"

#define MAX_SCENES 6
#define MAX_TEXTFIELDS_PER_SCENE 10

static scene scenes[MAX_SCENES];
static sceneManager scm = {.current_Scene = NULL};

scene create_menu_scene()
{
    scene s = {0};

    s.scene_index = 0;
    s.scene_name = "Menu";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "./assets/maps/00_Menu/bombici.jpg";
    s.buttons[0] = (button){
        .isActive = 1,
        .button = {25, 50, 200, 75},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "Start",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 50, 200, 75}},
        .onClick = on_start,
        .state = BUTTON_NORMAL};
    s.buttons[1] = (button){
        .isActive = 1,
        .button = {25, 175, 300, 75},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "Settings",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 175, 300, 75},

        },
        .onClick = on_settings,
        .state = BUTTON_NORMAL};
    s.buttons[2] = (button){
        .isActive = 1,
        .button = {25, 300, 400, 75},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "Leaderboard",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 300, 400, 75},
        },
        .onClick = on_leaderboard,
        .state = BUTTON_NORMAL};
    s.buttons[3] = (button){
        .isActive = 1,
        .button = {25, 425, 200, 75},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "Exit",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 425, 200, 75},
        },
        .onClick = on_quit,
        .state = BUTTON_NORMAL};
    s.textFields[0] = (textField){
        .text_color = {255, 255, 255, 255},
        .text = "Welcome to Bulanci!",
        .text_box_color = {0, 0, 0, 150},
        .text_box = {512 - 250, 0, 500, 50},
        .isActive = 1};
    s.colMap.width = 0;
    s.colMap.height = 0;
    s.colMap.tiles = NULL;

    return s;
}

scene create_start_screen_scene()
{
    scene s = {0};

    s.scene_index = 1;
    s.scene_name = "Start screen";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "./assets/maps/00_Menu/bombici.jpg";

    s.buttons[0] = (button){
        .isActive = 1,
        .button = {25, 50, 300, 75},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "Start Game",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 50, 300, 75},
        },
        .onClick = on_start_game,
        .state = BUTTON_NORMAL};
    s.buttons[1] = (button){
        .isActive = 1,
        .button = {25, 425, 200, 75},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "Back",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 425, 200, 75},
        },
        .onClick = on_back,
        .state = BUTTON_NORMAL};
    s.textFields[0] = (textField){
        .text_color = {255, 255, 255, 255},
        .text = "Select Map",
        .text_box_color = {0, 0, 0, 150},
        .text_box = {512 - 150, 0, 300, 50},
        .isActive = 1};
    s.colMap.width = 0;
    s.colMap.height = 0;
    s.colMap.tiles = NULL;

    return s;
}

scene create_settings_scene()
{
    scene s = {0};

    s.scene_index = 2;
    s.scene_name = "Settings";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "./assets/maps/00_Menu/bombici.jpg";

    s.buttons[0] = (button){
        .isActive = 1,
        .button = {25, 425, 200, 75},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "Back",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 425, 200, 75},
        },
        .onClick = on_back,
        .state = BUTTON_NORMAL};
    s.textFields[0] = (textField){
        .text_color = {255, 255, 255, 255},
        .text = "Settings",
        .text_box_color = {0, 0, 0, 150},
        .text_box = {512 - 150, 0, 300, 50},
        .isActive = 1};
    s.colMap.width = 0;
    s.colMap.height = 0;
    s.colMap.tiles = NULL;

    return s;
}

scene create_leaderboard_scene()
{
    scene s = {0};

    s.scene_index = 3;
    s.scene_name = "Leaderboard";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "./assets/maps/00_Menu/bombici.jpg";

    s.buttons[0] = (button){
        .isActive = 1,
        .button = {25, 425, 200, 75},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "Back",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 425, 200, 75},
        },
        .onClick = on_back,
        .state = BUTTON_NORMAL};
    s.textFields[0] = (textField){
        .text_color = {255, 255, 255, 255},
        .text = "Select Map",
        .text_box_color = {0, 0, 0, 150},
        .text_box = {512 - 150, 0, 300, 50},
        .isActive = 1};
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

scene create_map2_scene()
{
    scene s = {0};

    s.scene_index = 1;
    s.scene_name = "Na dobrou noc";
    s.have_players = 1;
    s.respawn_timer = 5.0f;
    s.bg_texture_address = "./assets/maps/02_Na_Dobrou_Noc/na-dobrou-noc-upraveny.jpg";
    s.colMap.width = 32;
    s.colMap.height = 18;
    static int map2[] = {
        1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2,
        0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 2, 2,
        0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    s.colMap.tiles = map2;

    return s;
}

void init_scenes()
{
    scenes[0] = create_menu_scene();
    scenes[1] = create_start_screen_scene();
    scenes[2] = create_settings_scene();
    scenes[3] = create_leaderboard_scene();
    scenes[4] = create_map1_scene();
    scenes[5] = create_map2_scene();

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
    scm.last_scene_index = scm.current_Scene->scene_index;

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

    for (size_t y = 0; y < colMap->height; y++)
    {
        for (size_t x = 0; x < colMap->width; x++)
        {
            if (colMap->tiles[y * colMap->width + x] == 0)
            {
                continue;
            }

            SDL_Rect colBox = {.h = TILE_SIZE, .w = TILE_SIZE, .x = x * TILE_SIZE, .y = y * TILE_SIZE};

            if (colMap->tiles[y * colMap->width + x] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
            }

            SDL_RenderFillRect(renderer, &colBox);
        }
    }
}
