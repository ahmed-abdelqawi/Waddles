#include "render.h"

#include <stdio.h>
#include <stdint.h>

#define SDL_INIT_FLAGS SDL_INIT_VIDEO
#define WINDOW_FALGS 0

int draw_chess_board(SDL_Renderer *rendering_target)
{
    int board_size = 8;

    SDL_FRect cell = {.x = 0, .y = 0, .w = SQUARE_SIZE, .h = SQUARE_SIZE};

    /* drawing the chess board */
    for (int row = 0; row < board_size; ++row)
    {
        cell.y = row * SQUARE_SIZE;

        for (int col = 0; col < board_size; ++col)
        {
            cell.x = col * SQUARE_SIZE;

            if ((row + col) % 2 == 0)
            {
                SDL_SetRenderDrawColor(rendering_target, 0xee, 0xee, 0xee, 0xff);
            }
            else
            {
                SDL_SetRenderDrawColor(rendering_target, 0x58, 0x64, 0x6c, 0xff);
            }

            SDL_RenderFillRect(rendering_target, &cell);
        }
    }

    return 1;
}

int initialize_graphics(Game *game)
{
    if (!SDL_Init(SDL_INIT_FLAGS))
    {
        fprintf(stderr, "SDL_Init() Failed!\n");
        return 0;
    }

    game->window = SDL_CreateWindow("Waddles - Chess Engine", WIN_WIDTH, WIN_HEIGHT, WINDOW_FALGS);
    if (game->window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow() Failed! %s\n", SDL_GetError());
        housekeeping(game);
        return 0;
    }

    game->renderer = SDL_CreateRenderer(game->window, NULL);
    if (game->renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer() Failed! %s\n", SDL_GetError());
        housekeeping(game);
        return 0;
    }

    if (!SDL_SetRenderVSync(game->renderer, 1))
    {
        fprintf(stderr, "SDL_SetRenderVSync() Failed! %s\n", SDL_GetError());
    }

    game->is_running = 1;

    return 1;
}

static void handle_events(Game *game)
{
    while (SDL_PollEvent(&game->event))
    {
        switch (game->event.type)
        {
        case SDL_EVENT_QUIT:
            game->is_running = 0;
            break;

        default:
            break;
        }
    }
}

void render_loop(Game *game)
{
    /* fps stuff */
    const double target_fps = 1.0 / 60.0;
    uint64_t start = 0, end = 0;
    double delta_time = 0.0;

    while (game->is_running)
    {
        start = SDL_GetPerformanceCounter();

        /* window events */
        handle_events(game);

        /* drawing starts here! */
        SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 255); /* if anything goes wrong, it will display Magenta! */

        SDL_RenderClear(game->renderer);

        draw_chess_board(game->renderer);

        SDL_RenderPresent(game->renderer);

        /* time snyc */
        end = SDL_GetPerformanceCounter();
        delta_time = (double)(end - start) / (double)SDL_GetPerformanceFrequency();
        if (delta_time < target_fps)
            SDL_Delay((target_fps - delta_time) * 1000.0f);
    }
}

int housekeeping(Game *game)
{
    if (game == NULL)
    {
        return 0;
    }

    if (game->renderer)
    {
        SDL_DestroyRenderer(game->renderer);
        fprintf(stdout, "game->renderer cleared!\n");
    }

    if (game->window)
    {
        SDL_DestroyWindow(game->window);
        fprintf(stdout, "game->window cleared!\n");
    }

    SDL_Quit();
    return 1;
}
