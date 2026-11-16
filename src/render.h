#ifndef __RENDER_H_
#define __RENDER_H_

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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

typedef struct
{
    /* black pieces */
    SDL_Texture *b_king, *b_queen, *b_knight, *b_rook, *b_bishop, *b_pawn;
    /* white pieces */
    SDL_Texture *w_king, *w_queen, *w_knight, *w_rook, *w_bishop, *w_pawn;
} Piece_Render;

/* SDL3 Stuff */
int initialize_graphics(Game *game, Piece_Render *pieces);
void render_loop(Game *game, Piece_Render *pieces);
int housekeeping(Game *game, Piece_Render *pieces);

#endif // __RENDER_H_
