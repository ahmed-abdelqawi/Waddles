#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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

typedef enum
{
    /* white piece types */
    W_KNIG,
    W_QUEEN,
    W_BISHOP,
    W_KNIGHT,
    W_ROOK,
    W_PAWN,
    /* black piece types */
    B_KING,
    B_QUEEN,
    B_BISHOP,
    B_KNIGHT,
    B_ROOK,
    B_PAWN,

    PIECE_TYPE_COUNT // 12
} peices;

typedef struct
{
    SDL_Texture *pieces[PIECE_TYPE_COUNT]; /* 12 --> 6 piece types for each */
} PieceGraphic;

bool app_init(AppState *state, char *window_title, int window_width, int window_height);
void app_loop(AppState *state, const double target_fps);
bool app_housekeeping(AppState *state);

#endif /* RENDER_H */
