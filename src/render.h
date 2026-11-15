#ifndef __RENDER_H_
#define __RENDER_H_

#include <SDL3/SDL.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 800

#define SQUARE_SIZE 100

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    int is_running;
} Game;

/* SDL3 Stuff */
int initialize_graphics(Game *game);
void render_loop(Game *game);
int housekeeping(Game *game);

/* Drawing functions for Waddles */
int draw_chess_board(SDL_Renderer *rendering_target);

#endif // __RENDER_H_
