#include <stdio.h>
#include <stdlib.h>

#include "render.h"
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{
    Game waddles = {0};

    if (!initialize_graphics(&waddles))
    {
        fprintf(stderr, "Something Went Wrong in initialize_graphics()\n");
        exit(EXIT_FAILURE);
    }

    render_loop(&waddles);

    if (!housekeeping(&waddles))
    {
        fprintf(stderr, "Something Went Wrong in housekeeping()\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
