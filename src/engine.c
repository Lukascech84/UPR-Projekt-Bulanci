#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "engine.h"
#include "ui.h"
#include "player.h"
#include "sceneManager.h"
#include "weaponPickup.h"
#include "weapon.h"
#include "time.h"

static engine eng;

engine *eng_get(void)
{
    return &eng;
}

int eng_init(char *title, int w, int h)
{
    eng.vsync_enabled = 1;

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    eng.window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!eng.window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    eng.renderer = SDL_CreateRenderer(eng.window, -1, SDL_RENDERER_ACCELERATED | (eng.vsync_enabled ? SDL_RENDERER_PRESENTVSYNC : 0));
    if (!eng.renderer)
    {
        SDL_DestroyWindow(eng.window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_DestroyWindow(eng.window);
        printf("Error SDL2_image Initialization");
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1)
    {
        SDL_DestroyWindow(eng.window);
        printf("TTF_Init error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawBlendMode(eng.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // Všechny inits
    init_scenes();
    scm_load_textures(eng.renderer);
    init_ui();
    load_ui(eng.renderer);

    srand(time(NULL));

    eng.running = 1;

    return 0;
}

void update()
{
    update_Players();
    update_bullet();
    update_weapon_pickup_respawn_timer();
    update_scoreCounter();
    scm_update_timer();
    render_Players();
    render_bullet();
    render_weapon_pickup();
    input_Players();
}

void start_game()
{
    sceneManager *scm = scm_get_scm();

    printf("Game started\n");
    scm_load_scene(scm->selected_map_index + PLAYABLE_SCENES_START_INDEX - 1);
    scm->weapon_pickup.isActive = 0;
    scm->weapon_pickup.weaponID = -1;
    scm->weapon_pickup.respawn_timer_elapsed = 0.0f;
    scm->weapon_pickup.respawn_timer = WEAPON_PICKUP_RESPAWN_TIME;
    scm_start_timer();
    scm_init_after_game_stats_array();
}

void stop_game()
{
    sceneManager *scm = scm_get_scm();
    printf("Game stopped\n");
    scm_save_stats_after_game();
    scm_load_scene(SCENE_END_SCREEN);
    scm->isTimerRunning = 0;
    scm->players_spawned = 0;
    scm->weapon_pickup.isActive = 0;
    scm->timer_elapsed = 0.0f;
    scm->timer = 180;
    update_playerNames_in_end_screen();
}

void eng_run()
{
    // Resize okna
    int resized = 0;
    int new_w, new_h;

    // Eventy
    SDL_Event event;

    while (eng.running)
    {
        while (SDL_PollEvent(&event))
        {
            update_buttons(&event);
            update_textfields(&event);

            if (event.type == SDL_QUIT)
            {
                eng.running = 0;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                resized = 1;
                new_w = event.window.data1;
                new_h = event.window.data2;
            }
        }

        Uint64 now = SDL_GetPerformanceCounter();
        eng.deltaTime = (now - eng.lastTicks) / (double)SDL_GetPerformanceFrequency();
        SDL_RenderClear(eng.renderer);

        // Kód

        // Vykreslení pozadí
        SDL_RenderCopy(eng.renderer, scm_get_scm()->current_Scene->bg_texture, NULL, NULL);

        // Logika hráče
        if (scm_get_scm()->current_Scene->have_players && !scm_get_scm()->players_spawned)
        {
            init_Players();
            init_bullet();
            scm_get_scm()->players_spawned = 1;
            init_scoreCounter();
        }
        else if (!scm_get_scm()->current_Scene->have_players && scm_get_scm()->players_spawned)
        {
            clear_Players();
            scm_get_scm()->players_spawned = 0;
        }
        if (scm_get_scm()->players_spawned)
        {
            if (resized)
            {
                resize_Players(new_h / 10, new_w / 10);
                resized = 0;
            }

            update();
        }

        // scm_render_collisionMap(); // Debug render kolizí
        render_ui();

        // Debug
        // printf("Current selected scene: %d\n", scm_get_scm()->selected_map_index);
        // printf("Current scene: %d\n", scm_get_scm()->current_Scene->scene_index);
        //  printf("%dx%d\n", SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h);
        //  printf("%d\n", get_Num_Of_Players());
        //  printf("FPS: %.2f\r", 1.0 / eng.deltaTime);

        eng.lastTicks = now;
        // SDL_SetRenderDrawColor(eng.renderer, 0, 0, 0, 255);
        SDL_RenderPresent(eng.renderer); // Prezentace kreslítka
    }
}

void eng_quit()
{
    clear_ui();
    clear_Players();

    scm_destroy_textures();

    if (TTF_WasInit())
    {
        TTF_Quit();
    }

    if (IMG_Init(0))
    {
        IMG_Quit();
    }

    engine *eng = eng_get();

    if (eng->renderer)
    {
        SDL_DestroyRenderer(eng->renderer);
    }

    if (eng->window)
    {
        SDL_DestroyWindow(eng->window);
    }

    eng->running = 0;

    SDL_Quit();
}
