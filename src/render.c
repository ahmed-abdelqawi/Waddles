#include "render.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define SDL_INIT_FLAGS SDL_INIT_VIDEO
#define SDL_WINDOW_FLAGS 0

/* start a new SDL3 (with VSync) application, with window(window_title, window_width, window_height) */
bool app_init(AppState *state, char *window_title, int window_width, int window_height)
{
    if (!SDL_Init(SDL_INIT_FLAGS))
    {
        fprintf(stderr, "SDL_Init() Failed!\n");
        return false;
    }

    state->app_window = SDL_CreateWindow(window_title, window_width, window_height, SDL_WINDOW_FLAGS);
    if (!state->app_window)
    {
        fprintf(stderr, "SDL_CreateWindow() Failed: %s\n", SDL_GetError());
        app_housekeeping(state);
        return false;
    }

    state->app_renderer = SDL_CreateRenderer(state->app_window, NULL);
    if (!state->app_renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer() Failed: %s\n", SDL_GetError());
        app_housekeeping(state);
        return false;
    }

    if (!SDL_SetRenderVSync(state->app_renderer, 1))
    {
        fprintf(stderr, "Could NOT enable VSync on \"app_renderer\": %s\n", SDL_GetError());
    }

    state->is_running = true;

    return true;
}

static void app_event_handling(AppState *state)
{
    while (SDL_PollEvent(&state->app_event))
    {
        switch (state->app_event.type)
        {
        case SDL_EVENT_QUIT:
            state->is_running = false;
            break;

        default:
            break;
        }
    }
}

/* the main gameloop that will run with target_fps/s */
void app_loop(AppState *state, const double target_fps)
{
    double fps = 1 / target_fps;

    while (state->is_running)
    {
        state->start = SDL_GetPerformanceCounter();

        app_event_handling(state);

        /* setting the background to Magenta so if any graphics issue happened I'll see it :D */
        SDL_SetRenderDrawColor(state->app_renderer, 0xff, 0x00, 0xff, 0xff);

        SDL_RenderClear(state->app_renderer);
        SDL_RenderPresent(state->app_renderer);

        state->end = SDL_GetPerformanceCounter();
        state->deltatime = (double)(state->end - state->start) / (double)SDL_GetPerformanceFrequency();

        if (state->deltatime < fps)
            SDL_Delay((fps - state->deltatime) * 1000.0f);
    }
}

/* clean-up before exiting the app */
bool app_housekeeping(AppState *state)
{
    if (!state)
    {
        fprintf(stderr, "app_housekeeping(): AppState is NULL!\n");
        return false;
    }

    if (state->app_renderer)
    {
        SDL_DestroyRenderer(state->app_renderer);
        fprintf(stderr, "state->app_renderer has been TERMINATED!\n");
    }

    if (state->app_window)
    {
        SDL_DestroyWindow(state->app_window);
        fprintf(stderr, "state->app_window has been TERMINATED!\n");
    }

    SDL_Quit();

    return true;
}
