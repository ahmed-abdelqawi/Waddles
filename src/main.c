#include <stdio.h>
#include <stdlib.h>

#include "render.h"
#include <SDL3/SDL_main.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 800

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    printf("Hi, I'm Waddles!\n");

    AppState app_state = (AppState){0};

    if (!app_init(&app_state, "Waddles - A Chess Engine", WIN_WIDTH, WIN_HEIGHT))
        exit(EXIT_FAILURE);

    app_loop(&app_state, 60.0f);

    if (!app_housekeeping(&app_state))
        exit(EXIT_FAILURE);

    return EXIT_SUCCESS;
}
