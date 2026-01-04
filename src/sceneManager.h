#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "engine.h"
#include "ui.h"
#include "collisionMap.h"
#include "weaponPickup.h"

#define MAX_SCENES 7
#define PLAYABLE_SCENES_START_INDEX 5
#define TIMER_MAX 600
#define TIMER_MIN 30
#define MAX_BUTTONS_PER_SCENE 10
#define MAX_TEXTFIELDS_PER_SCENE 20
#define STATS_FILE "stats.txt"

typedef struct
{
    int wasPlayerActive;
    int playerID;
    char playerName[MAX_PLAYER_NAME_LEN];
    int score;
} AfterGameStats;

typedef enum
{
    SCENE_MENU = 0,
    SCENE_START_SCREEN = 1,
    SCENE_SETTINGS = 2,
    SCENE_LEADERBOARD = 3,
    SCENE_END_SCREEN = 4,
    SCENE_MAP_TEST = 5,
    SCENE_MAP_DOBROU_NOC = 6
} SceneIndex;

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

    int selected_map_index;
    int timer;
    float timer_elapsed;
    int isTimerRunning;
    int num_of_active_players_in_last_game;

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

void scm_start_timer();

void scm_update_timer();

void scm_save_stats_after_game();

void scm_init_after_game_stats_array();

AfterGameStats *get_AftStats();