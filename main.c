// MANUÁLNĚ !NEFUNGUJÍ TEXTURY(ŠPATNÁ ADRESA) | gcc -g main.c player.c sceneManager.c -omain -lSDL2 -lSDL2_ttf -lSDL2_image -fsanitize=address
// PŘES CMAKE | ./build/make
// A PAK ./Bulanci

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "sceneManager.h"
#include "weapon.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 576

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Hra", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_DestroyWindow(window);
        printf("Error SDL2_image Initialization");
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    Uint64 last = SDL_GetPerformanceCounter();

    // Proměnné
    int target_scene = 0;

    // Načítání textur
    scm_load_textures(renderer);

    // Init bullet
    init_bullet();

    // Resize okna
    int resized = 0;
    int new_w, new_h;

    // Eventy
    SDL_Event event;
    int running = 1;

    while (running == 1)
    {
        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                resized = 1;
                new_w = event.window.data1;
                new_h = event.window.data2;
            }
            // Výměna scén
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                if (scm_get_scm()->current_Scene->scene_index == 0)
                {
                    target_scene = 1;
                }
                else
                {
                    target_scene = 0;
                }
            }
        }

        Uint64 now = SDL_GetPerformanceCounter();
        double deltaTime = (now - last) / (double)SDL_GetPerformanceFrequency();
        SDL_RenderClear(renderer);

        // Kód
        // Loading scén
        if (scm_get_scm()->current_Scene->scene_index != target_scene)
        {
            scm_load_scene(target_scene);
            printf("Loading scene: %s\n", scm_get_scene(target_scene)->scene_name);
        }

        // Vykreslení pozadí
        SDL_RenderCopy(renderer, scm_get_scm()->current_Scene->bg_texture, NULL, NULL);

        // Logika hráče
        if (scm_get_scm()->current_Scene->have_players && !scm_get_scm()->players_spawned)
        {
            init_Players();
            scm_get_scm()->players_spawned = 1;
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
            move_Players(deltaTime);
            update_bullet();
            render_Players(renderer);
            render_bullet(renderer);
            input_Players(renderer);
        }

        // Debug

        // printf("Current scene: %s\n", sceneManager.current_Scene.scene_name);
        // printf("%dx%d\n", SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h);
        // printf("x1: %d, y1: %d, x2: %d, y2: %d\n", players[0].hitbox.x, players[0].hitbox.y, players[1].hitbox.x, players[1].hitbox.y);

        last = now;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer); // Prezentace kreslítka
    }

    clear_Players();
    scm_destroy_textures();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}