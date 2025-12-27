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

    SDL_Rect button;

    SDL_Color button_color;
    SDL_Color hover_color;
    SDL_Color pressed_color;

    char text[64];
    SDL_Color text_color;

    SDL_Texture *text_texture;
    SDL_Rect text_size;

    ButtonState state;
    ButtonCallback onClick;
} button;

void init_ui();

void load_ui();

void render_ui();

void clear_ui();

void load_buttons();

void render_buttons();

void clear_buttons();

void create_button_text_texture(button *);

int is_mouse_over_button(button *, int, int);

void update_buttons(SDL_Event *);

void on_start_game();

void on_quit();