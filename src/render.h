#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

typedef struct
{
    /* SDL3 stuff */
    SDL_Window *app_window;
    SDL_Renderer *app_renderer;
    SDL_Event app_event;

    /* for calculating Deltatime */
    uint64_t start, end;
    double deltatime;

    bool is_running;
} AppState;

bool app_init(AppState *state, char *window_title, int window_width, int window_height);
void app_loop(AppState *state, const double target_fps);
bool app_housekeeping(AppState *state);

#endif /* RENDER_H */
