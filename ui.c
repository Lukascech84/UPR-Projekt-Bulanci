#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "ui.h"
#include "sceneManager.h"

#define FONT_ADDRESS "./assets/fonts/Jersey_Pixelated/Jersey10-Regular.ttf"

button loaded_buttons_in_scene[MAX_BUTTONS_PER_SCENE];

TTF_Font *font = NULL;

void init_ui()
{
    font = TTF_OpenFont(FONT_ADDRESS, 72);
    if (!font)
    {
        printf("Font load error: %s\n", TTF_GetError());
        return;
    }

    init_buttons();
}

void load_ui(SDL_Renderer *renderer)
{
    load_buttons(renderer);
}

void render_ui(SDL_Renderer *renderer)
{
    render_buttons(renderer);
}

void clear_ui()
{
    clear_buttons();
}

void init_buttons()
{
    for (size_t i = 0; i < MAX_BUTTONS_PER_SCENE; i++)
    {
        loaded_buttons_in_scene[i].isActive = 0;

        loaded_buttons_in_scene[i].button.h = 0;
        loaded_buttons_in_scene[i].button.w = 0;
        loaded_buttons_in_scene[i].button.x = 0;
        loaded_buttons_in_scene[i].button.y = 0;
        loaded_buttons_in_scene[i].button_color.a = 0;
        loaded_buttons_in_scene[i].button_color.r = 0;
        loaded_buttons_in_scene[i].button_color.g = 0;
        loaded_buttons_in_scene[i].button_color.b = 0;

        strcpy(loaded_buttons_in_scene[i].text, "");

        loaded_buttons_in_scene[i].text_color.a = 0;
        loaded_buttons_in_scene[i].text_color.r = 0;
        loaded_buttons_in_scene[i].text_color.g = 0;
        loaded_buttons_in_scene[i].text_color.b = 0;
        loaded_buttons_in_scene[i].text_texture = NULL;
    }
}

void load_buttons(SDL_Renderer *renderer)
{
    for (size_t i = 0; i < MAX_BUTTONS_PER_SCENE; i++)
    {
        if (scm_get_scm()->current_Scene->buttons[i].isActive == 1)
        {
            loaded_buttons_in_scene[i] = scm_get_scm()->current_Scene->buttons[i];

            create_button_text_texture(renderer, &loaded_buttons_in_scene[i]);
        }
    }
}

void render_buttons(SDL_Renderer *renderer)
{
    for (size_t i = 0; i < MAX_BUTTONS_PER_SCENE; i++)
    {
        if (!loaded_buttons_in_scene[i].isActive)
        {
            continue;
        }

        SDL_SetRenderDrawColor(renderer, loaded_buttons_in_scene[i].button_color.r, loaded_buttons_in_scene[i].button_color.g, loaded_buttons_in_scene[i].button_color.b, loaded_buttons_in_scene[i].button_color.a);

        SDL_RenderFillRect(renderer, &loaded_buttons_in_scene[i].button);

        if (loaded_buttons_in_scene[i].text_texture)
        {
            SDL_RenderCopy(
                renderer,
                loaded_buttons_in_scene[i].text_texture,
                NULL,
                &loaded_buttons_in_scene[i].text_size);
        }
    }
}

void clear_buttons()
{
    for (size_t i = 0; i < MAX_BUTTONS_PER_SCENE; i++)
    {
        if (loaded_buttons_in_scene[i].text_texture)
        {
            SDL_DestroyTexture(loaded_buttons_in_scene[i].text_texture);
            loaded_buttons_in_scene[i].text_texture = NULL;
        }

        memset(&loaded_buttons_in_scene[i], 0, sizeof(button));
    }
}

void create_button_text_texture(SDL_Renderer *renderer, button *btn)
{
    if (btn->text_texture)
    {
        SDL_DestroyTexture(btn->text_texture);
        btn->text_texture = NULL;
    }

    if (btn->text[0] == '\0')
    {
        return;
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, btn->text, btn->text_color);
    if (!surface)
    {
        printf("Error creating surface for text");
        return;
    }

    btn->text_texture = SDL_CreateTextureFromSurface(renderer, surface);

    btn->text_size.w = surface->w;
    btn->text_size.h = surface->h;

    SDL_FreeSurface(surface);

    if (!btn->text_texture)
    {
        printf("Error creating texture for text");
        return;
    }

    btn->text_size.x = btn->button.x + (btn->button.w - btn->text_size.w) / 2;
    btn->text_size.y = btn->button.y + (btn->button.h - btn->text_size.h) / 2;
}