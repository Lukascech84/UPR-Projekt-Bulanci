#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "ui.h"
#include "engine.h"
#include "sceneManager.h"
#include "player.h"

#define FONT_ADDRESS "./assets/fonts/Jersey_Pixelated/Jersey10-Regular.ttf"

static scoreCounter scoreCounters[MAX_NUMBER_OF_PLAYERS];

static button loaded_buttons_in_scene[MAX_BUTTONS_PER_SCENE];

static textField loaded_textfields_in_scene[MAX_TEXTFIELDS_PER_SCENE];

static char player_name_buffers[MAX_NUMBER_OF_PLAYERS][MAX_PLAYER_NAME_LEN + 1];

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
    if (MAX_NUMBER_OF_PLAYERS > 0)
    {
        init_playerNames();
    }
}

void load_ui()
{
    load_textfields();
    load_buttons();
}

void render_ui()
{
    int scene_index = scm_get_scm()->current_Scene->scene_index;

    render_buttons();
    render_textfields();

    if (scene_index == SCENE_START_SCREEN)
    {
        render_players_in_start_screen();
    }

    if (scene_index >= PLAYABLE_SCENES_START_INDEX)
    {
        render_scoreCounter();
    }
}

void clear_ui()
{
    clear_buttons();
    clear_scoreCounter();
    clear_textfields();
}

void load_buttons()
{
    for (size_t i = 0; i < MAX_BUTTONS_PER_SCENE; i++)
    {
        if (scm_get_scm()->current_Scene->buttons[i].isActive == 1)
        {
            loaded_buttons_in_scene[i] = scm_get_scm()->current_Scene->buttons[i];

            create_textfield_texture(&loaded_buttons_in_scene[i].textField);
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

        if (btn->textField.text_texture)
        {
            SDL_RenderCopy(renderer, btn->textField.text_texture, NULL, &btn->textField.text_pos);
        }
    }
}

void clear_buttons()
{
    for (size_t i = 0; i < MAX_BUTTONS_PER_SCENE; i++)
    {
        if (loaded_buttons_in_scene[i].textField.text_texture)
        {
            SDL_DestroyTexture(loaded_buttons_in_scene[i].textField.text_texture);
            loaded_buttons_in_scene[i].textField.text_texture = NULL;
        }

        memset(&loaded_buttons_in_scene[i], 0, sizeof(button));
    }
}

void load_textfields()
{
    for (size_t i = 0; i < MAX_TEXTFIELDS_PER_SCENE; i++)
    {
        if (scm_get_scm()->current_Scene->textFields[i].isActive == 1)
        {
            loaded_textfields_in_scene[i] = scm_get_scm()->current_Scene->textFields[i];

            create_textfield_texture(&loaded_textfields_in_scene[i]);
        }
    }
}

void render_textfields()
{
    SDL_Renderer *renderer = eng_get()->renderer;

    for (size_t i = 0; i < MAX_TEXTFIELDS_PER_SCENE; i++)
    {
        textField *tf = &loaded_textfields_in_scene[i];

        if (!tf->isActive)
        {
            continue;
        }

        SDL_Color box_color = tf->text_box_color;
        if (tf->isFocused)
        {
            box_color = tf->text_box_focused_color;
        }
        SDL_SetRenderDrawColor(renderer, box_color.r, box_color.g, box_color.b, box_color.a);
        SDL_RenderFillRect(renderer, &tf->text_box);

        if (tf->text_texture)
        {
            SDL_RenderCopy(renderer, tf->text_texture, NULL, &tf->text_pos);
        }
    }
}

void clear_textfields()
{
    for (size_t i = 0; i < MAX_TEXTFIELDS_PER_SCENE; i++)
    {
        if (loaded_textfields_in_scene[i].text_texture)
        {
            SDL_DestroyTexture(loaded_textfields_in_scene[i].text_texture);
            loaded_textfields_in_scene[i].text_texture = NULL;
        }

        memset(&loaded_textfields_in_scene[i], 0, sizeof(textField));
    }
}

void update_textfields(SDL_Event *event)
{
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    for (size_t i = 0; i < MAX_TEXTFIELDS_PER_SCENE; i++)
    {
        textField *tf = &loaded_textfields_in_scene[i];

        if (tf->isActive && tf->isEditable && tf->isFocused)
        {
            writable_textfield_input(event, tf);
        }
        else if (!tf->isActive || !tf->isEditable)
        {
            continue;
        }

        int hover = is_mouse_over_rect(&tf->text_box, mx, my);

        if (hover && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
        {
            tf->isFocused = 1;
        }
        else if (!hover && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
        {
            tf->isFocused = 0;
        }
    }
}

void writable_textfield_input(SDL_Event *event, textField *tf)
{
    if (!tf->isActive || !tf->isEditable || !tf->isFocused)
    {
        return;
    }

    if (event->type == SDL_TEXTINPUT)
    {
        if (strlen(tf->text) + strlen(event->text.text) < tf->max_length)
        {
            strcat(tf->text, event->text.text);
            create_textfield_texture(tf);
        }
    }
    else if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.sym == SDLK_BACKSPACE && strlen(tf->text) > 0)
        {
            tf->text[strlen(tf->text) - 1] = '\0';
            create_textfield_texture(tf);
        }
    }
}

void create_textfield_texture(textField *tf)
{
    SDL_Renderer *renderer = eng_get()->renderer;

    if (tf->text_texture)
    {
        SDL_DestroyTexture(tf->text_texture);
        tf->text_texture = NULL;
    }

    if (tf->text[0] == '\0' && tf->number_value == NULL)
    {
        return;
    }

    char *text = tf->text;
    if (tf->number_value != NULL)
    {
        static char number_text[20];
        snprintf(number_text, sizeof(number_text), "%d", *(tf->number_value));
        text = number_text;
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, tf->text_color);
    if (!surface)
    {
        printf("Error creating surface for text");
        return;
    }

    tf->text_texture = SDL_CreateTextureFromSurface(renderer, surface);

    tf->text_pos.w = surface->w;
    tf->text_pos.h = surface->h;

    SDL_FreeSurface(surface);

    if (!tf->text_texture)
    {
        printf("Error creating texture for text");
        return;
    }

    tf->text_pos.x = tf->text_box.x + (tf->text_box.w - tf->text_pos.w) / 2;
    tf->text_pos.y = tf->text_box.y + (tf->text_box.h - tf->text_pos.h) / 2;
}

void init_playerNames()
{
    for (size_t i = 0; i < MAX_NUMBER_OF_PLAYERS; i++)
    {
        snprintf(player_name_buffers[i], sizeof(player_name_buffers[i]), "Player %d", (int)(i + 1));
    }
}

int is_mouse_over_rect(SDL_Rect *rect, int mx, int my)
{
    return (mx >= rect->x &&
            mx <= rect->x + rect->w &&
            my >= rect->y &&
            my <= rect->y + rect->h);
}

void render_players_in_start_screen()
{
    sceneManager *scm = scm_get_scm();

    if (scm->current_Scene->scene_index != SCENE_START_SCREEN)
    {
        return;
    }

    for (size_t i = 0; i < get_Num_Of_Players(); i++)
    {
        SDL_Renderer *renderer = eng_get()->renderer;
        SDL_Color *color = get_player_color_pointer(i);

        SDL_Rect player_rect = {.x = 475 + (i * 325), .y = 100, .w = 100, .h = 100};
        if (player_rect.x + player_rect.w > 1024)
        {
            player_rect.x = 475 + ((i - 2) * 325);
            player_rect.y += 200;
        }

        SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
        SDL_RenderFillRect(renderer, &player_rect);
    }
}

void update_player_name_textfields()
{
    int player_count = (size_t)get_Num_Of_Players();
    int start_index = PLAYER_NAME_TEXTFIELD_START;

    for (size_t i = 0; i < MAX_NUMBER_OF_PLAYERS; i++)
    {
        textField *tf = &loaded_textfields_in_scene[start_index + i];

        if (tf->isActive)
        {
            strcpy(player_name_buffers[i], tf->text);
        }
    }

    for (size_t i = 0; i < MAX_NUMBER_OF_PLAYERS; i++)
    {
        textField *tf = &loaded_textfields_in_scene[i + start_index];

        if (i < player_count)
        {

            tf->isActive = 1;
            tf->isEditable = 1;
            tf->max_length = MAX_PLAYER_NAME_LEN;

            snprintf(tf->text, sizeof(tf->text), "%s", player_name_buffers[i]);

            tf->text_box = (SDL_Rect){.x = 400 + (i * 325), .y = 225, .w = 275, .h = 50};
            if (tf->text_box.x + tf->text_box.w > 1024)
            {
                tf->text_box.x = 400 + ((i - 2) * 325);
                tf->text_box.y += 200;
            }

            tf->text_box_color = (SDL_Color){200, 200, 200, 255};
            tf->text_box_focused_color = (SDL_Color){255, 255, 255, 255};
            tf->text_color = (SDL_Color){0, 0, 0, 255};

            create_textfield_texture(tf);
        }
        else
        {
            tf->isActive = 0;
            tf->isEditable = 0;
            tf->text[0] = '\0';

            if (tf->text_texture)
            {
                SDL_DestroyTexture(tf->text_texture);
                tf->text_texture = NULL;
            }
        }
    }
}

char *get_player_names(int i)
{
    return player_name_buffers[i];
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

        int hover = is_mouse_over_rect(&btn->button, mx, my);

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
                btn->state = BUTTON_HOVER;
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
    player *players = get_Players();
    SDL_Renderer *renderer = eng_get()->renderer;
    for (size_t i = 0; i < get_Num_Of_Players(); i++)
    {
        SDL_SetRenderDrawColor(renderer, players[i].player_color->r, players[i].player_color->g, players[i].player_color->b, players[i].player_color->a);
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

void on_start()
{
    scm_load_scene(SCENE_START_SCREEN);
    update_player_name_textfields();
}

void on_settings()
{
    scm_load_scene(SCENE_SETTINGS);
}

void on_leaderboard()
{
    scm_load_scene(SCENE_LEADERBOARD);
}

void on_quit()
{
    eng_quit();
}

void on_start_game()
{
    start_game();
}

void on_decrease_player_count()
{
    if (get_Num_Of_Players() > 1)
    {
        set_Num_Of_Players(get_Num_Of_Players() - 1);
        create_textfield_texture(&loaded_textfields_in_scene[SCENE_START_SCREEN]);
        update_player_name_textfields();
    }
}

void on_increase_player_count()
{
    if (get_Num_Of_Players() < MAX_NUMBER_OF_PLAYERS)
    {
        set_Num_Of_Players(get_Num_Of_Players() + 1);
        create_textfield_texture(&loaded_textfields_in_scene[SCENE_START_SCREEN]);
        update_player_name_textfields();
    }
}

void on_decrease_map_index()
{
    if (scm_get_scm()->selected_map_index > 1)
    {
        scm_get_scm()->selected_map_index--;
        create_textfield_texture(&loaded_textfields_in_scene[2]);
    }
}

void on_increase_max_index()
{
    if (scm_get_scm()->selected_map_index < MAX_SCENES - PLAYABLE_SCENES_START_INDEX)
    {
        scm_get_scm()->selected_map_index++;
        create_textfield_texture(&loaded_textfields_in_scene[2]);
    }
}

void on_back()
{
    scm_load_scene(scm_get_scm()->last_scene_index);
}
