#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef enum
{
    BUTTON_NORMAL,
    BUTTON_HOVER,
    BUTTON_PRESSED
} ButtonState;

typedef void (*ButtonCallback)();

typedef struct
{
    int isActive;

    SDL_Rect text_box;
    SDL_Rect text_pos;
    SDL_Color text_box_color;
    SDL_Color text_color;

    char text[128];

    SDL_Texture *text_texture;
} textField;

typedef struct
{
    int isActive;

    SDL_Rect button;

    SDL_Color button_color;
    SDL_Color hover_color;
    SDL_Color pressed_color;

    textField textField;

    ButtonState state;
    ButtonCallback onClick;
} button;

typedef struct
{
    char *playerName;
    int playerID;
    SDL_Rect bg_pos;
    SDL_Rect score_text_pos;
    int score;
    SDL_Rect weapon_text_pos;
    int weapon;
    SDL_Rect ammo_text_pos;
    int ammo;

    SDL_Texture *score_text_texture;
    SDL_Texture *weapon_text_texture;
    SDL_Texture *ammo_text_texture;
    SDL_Color text_color;
} scoreCounter;

void init_ui();

void load_ui();

void render_ui();

void clear_ui();

void load_buttons();

void render_buttons();

void clear_buttons();

void load_textfields();

void render_textfields();

void clear_textfields();

void create_textfield_texture(textField *);

int is_mouse_over_button(button *, int, int);

void update_buttons(SDL_Event *);

void init_scoreCounter();

void render_scoreCounter();

void update_scoreCounter();

void clear_scoreCounter();

void create_scoreCounter_score_text_texture(scoreCounter *);

void create_scoreCounter_weapon_text_texture(scoreCounter *);

void create_scoreCounter_ammo_text_texture(scoreCounter *);

// MENU SCENE CALLBACKS
void on_start();

void on_settings();

void on_leaderboard();

void on_quit();

// START SCENE CALLBACKS
void on_start_game();

void on_back();