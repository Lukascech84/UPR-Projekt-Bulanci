#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct
{
    int isActive;
    SDL_Rect button;
    SDL_Color button_color;
    char text[64];
    SDL_Color text_color;
    SDL_Rect text_size;
    SDL_Texture *text_texture;

} button;

void init_ui();

void load_ui(SDL_Renderer *);

void render_ui(SDL_Renderer *);

void clear_ui();

void init_buttons();

void load_buttons(SDL_Renderer *);

void render_buttons(SDL_Renderer *);

void clear_buttons();

void create_button_text_texture(SDL_Renderer *, button *);