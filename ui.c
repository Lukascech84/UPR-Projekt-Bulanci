#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "engine.h"
#include "ui.h"
#include "sceneManager.h"
#include "player.h"

#define FONT_ADDRESS "./assets/fonts/Jersey_Pixelated/Jersey10-Regular.ttf"

scoreCounter scoreCounters[MAX_NUMBER_OF_PLAYERS];

button loaded_buttons_in_scene[MAX_BUTTONS_PER_SCENE];

TTF_Font *font = NULL;
TTF_Font *font_smaller = NULL;

void init_ui()
{
    font = TTF_OpenFont(FONT_ADDRESS, 72);
    font_smaller = TTF_OpenFont(FONT_ADDRESS, 24);
    if (!font || !font_smaller)
    {
        printf("Font load error: %s\n", TTF_GetError());
        return;
    }
}

void load_ui()
{
    load_buttons();
}

void render_ui()
{
    render_buttons();
    render_scoreCounter();
}

void clear_ui()
{
    clear_buttons();
    clear_scoreCounter();
}

void load_buttons()
{
    for (size_t i = 0; i < MAX_BUTTONS_PER_SCENE; i++)
    {
        if (scm_get_scm()->current_Scene->buttons[i].isActive == 1)
        {
            loaded_buttons_in_scene[i] = scm_get_scm()->current_Scene->buttons[i];

            create_button_text_texture(&loaded_buttons_in_scene[i]);
        }
    }
}

void render_buttons()
{
    SDL_Renderer *renderer = eng_get()->renderer;

    for (size_t i = 0; i < MAX_BUTTONS_PER_SCENE; i++)
    {
        button *btn = &loaded_buttons_in_scene[i];

        if (!btn->isActive)
        {
            continue;
        }

        SDL_Color color = btn->button_color;

        if (btn->state == BUTTON_HOVER)
        {
            color = btn->hover_color;
        }
        else if (btn->state == BUTTON_PRESSED)
        {
            color = btn->pressed_color;
        }

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &btn->button);

        if (btn->text_texture)
        {
            SDL_RenderCopy(renderer, btn->text_texture, NULL, &btn->text_size);
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

void create_button_text_texture(button *btn)
{
    SDL_Renderer *renderer = eng_get()->renderer;

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

int is_mouse_over_button(button *btn, int mx, int my)
{
    return (mx >= btn->button.x &&
            mx <= btn->button.x + btn->button.w &&
            my >= btn->button.y &&
            my <= btn->button.y + btn->button.h);
}

void update_buttons(SDL_Event *event)
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    for (size_t i = 0; i < MAX_BUTTONS_PER_SCENE; i++)
    {
        button *btn = &loaded_buttons_in_scene[i];

        if (!btn->isActive)
        {
            continue;
        }

        int hover = is_mouse_over_button(btn, mx, my);

        if (!hover)
        {
            btn->state = BUTTON_NORMAL;
            continue;
        }

        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
        {
            btn->state = BUTTON_PRESSED;
        }

        else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
        {
            if (btn->state == BUTTON_PRESSED)
            {
                if (btn->onClick)
                {
                    btn->onClick();
                }

                btn->state = BUTTON_HOVER;
            }
        }
        else
        {
            if (btn->state != BUTTON_PRESSED)
            {
                btn->state = BUTTON_NORMAL;
            }
        }
    }
}

void init_scoreCounter()
{
    int w, h = 0;
    SDL_Color text_color = {255, 255, 255, 255};
    SDL_GetWindowSize(eng_get()->window, &w, &h);

    SDL_Rect scoreCounterPosition[MAX_NUMBER_OF_PLAYERS] = {{.h = 50, .w = 125, .x = 0, .y = 0},
                                                            {.h = 50, .w = 125, .x = (w - 125), .y = 0},
                                                            {.h = 50, .w = 125, .x = 0, .y = (h - 50)},
                                                            {.h = 50, .w = 125, .x = (w - 125), .y = (h - 50)}};

    for (size_t i = 0; i < get_Num_Of_Players(); i++)
    {
        player *players = get_Players();

        scoreCounters[i].playerID = i;
        scoreCounters[i].playerName = players[i].playerName;
        scoreCounters[i].score = 0;
        scoreCounters[i].bg_pos = scoreCounterPosition[i];
        scoreCounters[i].text_color = text_color;
        scoreCounters[i].score_text_texture = NULL;
    }
}

void render_scoreCounter()
{
    SDL_Renderer *renderer = eng_get()->renderer;
    for (size_t i = 0; i < get_Num_Of_Players(); i++)
    {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &scoreCounters[i].bg_pos);
        if (scoreCounters[i].score_text_texture)
        {
            SDL_RenderCopy(renderer, scoreCounters[i].score_text_texture, NULL, &scoreCounters[i].score_text_pos);
        }
        if (scoreCounters[i].weapon_text_texture)
        {
            SDL_RenderCopy(renderer, scoreCounters[i].weapon_text_texture, NULL, &scoreCounters[i].weapon_text_pos);
        }
        if (scoreCounters[i].ammo_text_texture)
        {
            SDL_RenderCopy(renderer, scoreCounters[i].ammo_text_texture, NULL, &scoreCounters[i].ammo_text_pos);
        }
    }
}

void update_scoreCounter()
{
    player *players = get_Players();
    for (size_t i = 0; i < get_Num_Of_Players(); i++)
    {
        if (players[i].score != scoreCounters[i].score || scoreCounters[i].score_text_texture == NULL)
        {
            scoreCounters[i].score = players[i].score;
            create_scoreCounter_score_text_texture(&scoreCounters[i]);
        }
        if (players[i].current_weapon->weaponID != scoreCounters[i].weapon || scoreCounters[i].weapon_text_texture == NULL)
        {
            scoreCounters[i].weapon = players[i].current_weapon->weaponID;
            create_scoreCounter_weapon_text_texture(&scoreCounters[i]);
        }
        if (players[i].current_ammo_in_weapon != scoreCounters[i].ammo || scoreCounters[i].ammo_text_texture == NULL)
        {
            scoreCounters[i].ammo = players[i].current_ammo_in_weapon;
            create_scoreCounter_ammo_text_texture(&scoreCounters[i]);
        }
    }
}

void clear_scoreCounter()
{
    for (size_t i = 0; i < MAX_NUMBER_OF_PLAYERS; i++)
    {
        SDL_DestroyTexture(scoreCounters[i].score_text_texture);
        SDL_DestroyTexture(scoreCounters[i].weapon_text_texture);
        SDL_DestroyTexture(scoreCounters[i].ammo_text_texture);

        memset(&scoreCounters[i], 0, sizeof(scoreCounter));
    }
}

void create_scoreCounter_score_text_texture(scoreCounter *sc)
{
    SDL_Renderer *renderer = eng_get()->renderer;

    if (sc->score_text_texture)
        SDL_DestroyTexture(sc->score_text_texture);

    char score[16];
    snprintf(score, sizeof(score), "%d", sc->score);

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, score, sc->text_color);
    if (!surface)
        return;

    sc->score_text_texture = SDL_CreateTextureFromSurface(renderer, surface);

    sc->score_text_pos.w = surface->w;
    sc->score_text_pos.h = surface->h;

    sc->score_text_pos.x = sc->bg_pos.x + (sc->bg_pos.w - sc->score_text_pos.w) / 2 - 30;
    sc->score_text_pos.y = sc->bg_pos.y + (sc->bg_pos.h - sc->score_text_pos.h) / 2;

    SDL_FreeSurface(surface);
}

void create_scoreCounter_weapon_text_texture(scoreCounter *sc)
{
    SDL_Renderer *renderer = eng_get()->renderer;
    player *players = get_Players();

    if (sc->weapon_text_texture)
        SDL_DestroyTexture(sc->weapon_text_texture);

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font_smaller, players[sc->playerID].current_weapon->weapon_name, sc->text_color);
    if (!surface)
        return;

    sc->weapon_text_texture = SDL_CreateTextureFromSurface(renderer, surface);

    sc->weapon_text_pos.w = surface->w;
    sc->weapon_text_pos.h = surface->h;

    sc->weapon_text_pos.x = sc->bg_pos.x + (sc->bg_pos.w - sc->weapon_text_pos.w) / 2 + 30;
    sc->weapon_text_pos.y = sc->bg_pos.y + (sc->bg_pos.h - sc->weapon_text_pos.h) / 2 - 10;

    SDL_FreeSurface(surface);
}

void create_scoreCounter_ammo_text_texture(scoreCounter *sc)
{
    SDL_Renderer *renderer = eng_get()->renderer;
    player *players = get_Players();

    if (sc->ammo_text_texture)
        SDL_DestroyTexture(sc->ammo_text_texture);

    char ammo[16];
    if (players[sc->playerID].current_ammo_in_weapon == -1)
    {
        strcpy(ammo, "Infinite");
    }
    else
    {
        snprintf(ammo, sizeof(ammo), "%d", players[sc->playerID].current_ammo_in_weapon);
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font_smaller, ammo, sc->text_color);
    if (!surface)
        return;

    sc->ammo_text_texture = SDL_CreateTextureFromSurface(renderer, surface);

    sc->ammo_text_pos.w = surface->w;
    sc->ammo_text_pos.h = surface->h;

    sc->ammo_text_pos.x = sc->bg_pos.x + (sc->bg_pos.w - sc->ammo_text_pos.w) / 2 + 30;
    sc->ammo_text_pos.y = sc->bg_pos.y + (sc->bg_pos.h - sc->ammo_text_pos.h) / 2 + 10;

    SDL_FreeSurface(surface);
}

void on_start_game()
{
    printf("Game started\n");
    scm_load_scene(2);
}

void on_quit()
{
    eng_quit();
}
