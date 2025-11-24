// gcc -g main.c -omain -lSDL2 -lSDL2_ttf -lSDL2_image -fsanitize=address
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "sceneManager.h"

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
    int num_of_players = 3;
    player *players = NULL;
    players = init_Players(players, num_of_players);
    float speed = 200;

    int num_of_scenes = 2;
    scene scenes[2] = {{.scene_index = 0, .scene_name = "Menu", .bg_texture_address = "./assets/maps/00_Menu/menu_test.png"},
                       {.scene_index = 1, .scene_name = "Mapa1", .bg_texture_address = "./assets/maps/01_Grass/map1_scaled.png"}};

    sceneManager sceneManager = {.current_Scene = &scenes[0]};

    // Načítání textur
    load_textures(scenes, num_of_scenes, renderer);

    SDL_Event event;
    int running = 1;

    // Eventy
    while (running == 1)
    {
        while (SDL_PollEvent(&event))
        {

            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            input_Players(players, num_of_players, event);
        }

        Uint64 now = SDL_GetPerformanceCounter();
        double deltaTime = (now - last) / (double)SDL_GetPerformanceFrequency();
        SDL_RenderClear(renderer);

        // Kód
        int target_scene = 1;
        if (sceneManager.current_Scene->scene_index != target_scene)
        {
            load_scene(&sceneManager, target_scene, scenes);
            printf("Loading scene: %s\n", scenes[target_scene].scene_name);
        }

        SDL_RenderCopy(renderer, sceneManager.current_Scene->bg_texture, NULL, NULL);

        move_Players(players, num_of_players, speed, deltaTime);
        render_Players(players, num_of_players, renderer);

        // Debug

        // printf("Current scene: %s\n", sceneManager.current_Scene.scene_name);
        // printf("%dx%d\n", SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h);
        // printf("x1: %d, y1: %d, x2: %d, y2: %d\n", players[0].hitbox.x, players[0].hitbox.y, players[1].hitbox.x, players[1].hitbox.y);

        last = now;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer); // Prezentace kreslítka
    }

    clear_Players(players);
    destroy_textures(scenes, num_of_scenes);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}