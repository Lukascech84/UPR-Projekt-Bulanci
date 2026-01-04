#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#include "sceneManager.h"
#include "engine.h"
#include "ui.h"
#include "weapon.h"
#include "player.h"

static scene scenes[MAX_SCENES];
static sceneManager scm = {.current_Scene = NULL};

static AfterGameStats aftStats[MAX_NUMBER_OF_PLAYERS];

scene create_menu_scene()
{
    scene s = {0};

    s.scene_index = SCENE_MENU;
    s.scene_name = "Menu";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "assets/maps/00_Menu/menu.jpg";
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
        .isActive = 1,
    };
    s.colMap.width = 0;
    s.colMap.height = 0;
    s.colMap.tiles = NULL;

    return s;
}

scene create_start_screen_scene()
{
    scene s = {0};

    s.scene_index = SCENE_START_SCREEN;
    s.scene_name = "Start screen";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "assets/maps/00_Menu/menu.jpg";

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
    s.buttons[2] = (button){
        .isActive = 1,
        .button = {25, 150, 75, 50},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "<-",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 150, 75, 50},
        },
        .onClick = on_decrease_player_count,
        .state = BUTTON_NORMAL};
    s.buttons[3] = (button){
        .isActive = 1,
        .button = {250, 150, 75, 50},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "->",
            .text_color = {255, 255, 255, 255},
            .text_box = {250, 150, 75, 50},
        },
        .onClick = on_increase_player_count,
        .state = BUTTON_NORMAL};
    s.buttons[4] = (button){
        .isActive = 1,
        .button = {25, 225, 75, 50},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "<-",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 225, 75, 50},
        },
        .onClick = on_decrease_map_index,
        .state = BUTTON_NORMAL};
    s.buttons[5] = (button){
        .isActive = 1,
        .button = {250, 225, 75, 50},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "->",
            .text_color = {255, 255, 255, 255},
            .text_box = {250, 225, 75, 50},
        },
        .onClick = on_increase_max_index,
        .state = BUTTON_NORMAL};
    s.buttons[6] = (button){
        .isActive = 1,
        .button = {25, 300, 75, 50},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "<-",
            .text_color = {255, 255, 255, 255},
            .text_box = {25, 300, 75, 50},
        },
        .onClick = on_decrease_timer,
        .state = BUTTON_NORMAL};
    s.buttons[7] = (button){
        .isActive = 1,
        .button = {250, 300, 75, 50},
        .button_color = {100, 100, 100, 255},
        .hover_color = {140, 140, 140, 255},
        .pressed_color = {180, 180, 180, 255},
        .textField = {
            .text = "->",
            .text_color = {255, 255, 255, 255},
            .text_box = {250, 300, 75, 50},
        },
        .onClick = on_increase_timer,
        .state = BUTTON_NORMAL};
    s.textFields[0] = (textField){
        .text_color = {255, 255, 255, 255},
        .text = "Select Map",
        .text_box_color = {0, 0, 0, 150},
        .text_box = {512 - 150, 0, 300, 50},
        .isActive = 1,
        .isEditable = 0};
    s.textFields[1] = (textField){
        .text_color = {255, 255, 255, 255},
        .text = "",
        .number_value = get_Num_Of_Players_Pointer(),
        .text_box_color = {0, 0, 0, 150},
        .text_box = {125, 150, 100, 50},
        .isActive = 1,
        .isEditable = 0,
    };
    s.textFields[2] = (textField){
        .text_color = {255, 255, 255, 255},
        .text = "",
        .number_value = &scm_get_scm()->selected_map_index,
        .text_box_color = {0, 0, 0, 150},
        .text_box = {125, 225, 100, 50},
        .isActive = 1,
        .isEditable = 0,
    };
    s.textFields[3] = (textField){
        .text_color = {255, 255, 255, 255},
        .text = "",
        .number_value = &scm_get_scm()->timer,
        .text_box_color = {0, 0, 0, 150},
        .text_box = {125, 300, 100, 50},
        .isActive = 1,
        .isEditable = 0,
    };
    s.colMap.width = 0;
    s.colMap.height = 0;
    s.colMap.tiles = NULL;

    return s;
}

scene create_settings_scene()
{
    scene s = {0};

    s.scene_index = SCENE_SETTINGS;
    s.scene_name = "Settings";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "assets/maps/00_Menu/menu.jpg";

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

    s.scene_index = SCENE_LEADERBOARD;
    s.scene_name = "Leaderboard";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "assets/maps/00_Menu/menu.jpg";

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
        .text = "Leaderboard",
        .text_box_color = {0, 0, 0, 150},
        .text_box = {512 - 200, 0, 400, 50},
        .isActive = 1};
    s.colMap.width = 0;
    s.colMap.height = 0;
    s.colMap.tiles = NULL;

    return s;
}

scene create_end_screen_scene()
{
    scene s = {0};

    s.scene_index = SCENE_END_SCREEN;
    s.scene_name = "End Screen";
    s.have_players = 0;
    s.respawn_timer = 0.0f;
    s.bg_texture_address = "assets/maps/00_Menu/menu.jpg";

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
        .onClick = on_menu,
        .state = BUTTON_NORMAL};
    s.textFields[0] = (textField){
        .text_color = {255, 255, 255, 255},
        .text = "End of the Game!",
        .text_box_color = {0, 0, 0, 150},
        .text_box = {512 - 250, 0, 500, 50},
        .isActive = 1};
    s.colMap.width = 0;
    s.colMap.height = 0;
    s.colMap.tiles = NULL;

    return s;
}

scene create_map1_scene()
{
    scene s = {0};

    s.scene_index = SCENE_MAP_TEST;
    s.scene_name = "Grass";
    s.have_players = 1;
    s.respawn_timer = 5.0f;
    s.bg_texture_address = "assets/maps/01_Grass/map1_scaled.png";
    s.colMap.width = 32;
    s.colMap.height = 18;
    static int map1[] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2,
        2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2,
        2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2,
        2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
        2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
        2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
        2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    s.colMap.tiles = map1;

    return s;
}

scene create_map2_scene()
{
    scene s = {0};

    s.scene_index = SCENE_MAP_DOBROU_NOC;
    s.scene_name = "Na dobrou noc";
    s.have_players = 1;
    s.respawn_timer = 5.0f;
    s.bg_texture_address = "assets/maps/02_Na_Dobrou_Noc/na-dobrou-noc-upraveny.jpg";
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
    scenes[SCENE_MENU] = create_menu_scene();
    scenes[SCENE_START_SCREEN] = create_start_screen_scene();
    scenes[SCENE_SETTINGS] = create_settings_scene();
    scenes[SCENE_LEADERBOARD] = create_leaderboard_scene();
    scenes[SCENE_END_SCREEN] = create_end_screen_scene();
    scenes[SCENE_MAP_TEST] = create_map1_scene();
    scenes[SCENE_MAP_DOBROU_NOC] = create_map2_scene();

    scm.current_Scene = &scenes[0];
    scm.selected_map_index = 1;
    scm.timer = 180;
    scm.timer_elapsed = 0;
    scm.isTimerRunning = 0;
    scm.num_of_active_players_in_last_game = 0;
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
    scm.current_Scene->scene_index = scene_index;
    printf("Loading scene: %s[%d]\n", scm_get_scene(scene_index)->scene_name, scene_index);
    load_ui();
}

int scm_load_textures()
{
    SDL_Renderer *renderer = eng_get()->renderer;

    for (int i = 0; i < MAX_SCENES; i++)
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
    for (int i = 0; i < MAX_SCENES; i++)
    {
        SDL_DestroyTexture(scenes[i].bg_texture);
    }
}

void scm_render_collisionMap()
{
    collisionMap *colMap = &scm.current_Scene->colMap;

    SDL_Renderer *renderer = eng_get()->renderer;

    for (int y = 0; y < colMap->height; y++)
    {
        for (int x = 0; x < colMap->width; x++)
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

void scm_start_timer()
{
    scm.timer_elapsed = 0;
    scm.isTimerRunning = 1;
}

void scm_update_timer()
{
    if (!scm.isTimerRunning)
    {
        return;
    }
    if (scm.timer_elapsed >= scm.timer)
    {
        scm.isTimerRunning = 0;
        stop_game();
        return;
    }

    scm.timer_elapsed += eng_get()->deltaTime;
    // printf("Timer: %.2f / %d\n", scm.timer_elapsed, scm.timer);
}

void scm_save_stats_after_game()
{
    sceneManager *scm = scm_get_scm();
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    AfterGameStats *stats = get_AftStats();

    scm->num_of_active_players_in_last_game = get_Num_Of_Players();

    player *p = get_Players();
    for (int i = 0; i < get_Num_Of_Players(); i++)
    {
        stats[i].wasPlayerActive = 1;
        stats[i].playerID = i;
        strcpy(stats[i].playerName, p[i].playerName);
        stats[i].score = p[i].score;
    }

    FILE *stats_file = fopen(STATS_FILE, "a");
    if (stats_file == NULL)
    {
        printf("Error opening stats file, results will not be saved\n");
        return;
    }

    fprintf(stats_file, "MATCH ON MAP %s [%02d-%02d-%d-%02d:%02d]\n", scm->current_Scene->scene_name, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    for (int i = 0; i < get_Num_Of_Players(); i++)
    {
        fprintf(stats_file, "Player[%d] with name %s ended with score %d\n", i, stats[i].playerName, stats[i].score);
    }
    fprintf(stats_file, "----------------------------------------------------\n");

    fclose(stats_file);
}

void scm_init_after_game_stats_array()
{
    AfterGameStats *stats = get_AftStats();
    for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++)
    {
        stats[i].playerID = 0;
        strcpy(stats[i].playerName, "");
        stats[i].score = 0;
        stats[i].wasPlayerActive = 0;
    }
}

AfterGameStats *get_AftStats()
{
    return aftStats;
}
