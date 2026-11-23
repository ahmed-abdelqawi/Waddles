#include "render.h"

#include <stdio.h>

#define SDL_INIT_FLAGS SDL_INIT_VIDEO
#define SDL_WINDOW_FLAGS 0

/* 32bit RGBA color values */
#define COLOR_BLACK 0x95533bff
#define COLOR_WHITE 0xfbc273ff

/* extract color channels */
#define RGBA_R(rgba) ((uint8_t)(((uint32_t)(rgba) >> 24) & 0xFFU))
#define RGBA_G(rgba) ((uint8_t)(((uint32_t)(rgba) >> 16) & 0xFFU))
#define RGBA_B(rgba) ((uint8_t)(((uint32_t)(rgba) >> 8) & 0xFFU))
#define RGBA_A(rgba) ((uint8_t)(((uint32_t)(rgba) >> 0) & 0xFFU))

static PieceGraphic pg = (PieceGraphic){0};

#define BOARD_SIZE 8
#define SQUARE_SIZE 100
static void draw_chess_board(SDL_Renderer *renderer)
{
    SDL_FRect square = {.x = 0.0f, .y = 0.0f, .h = SQUARE_SIZE, .w = SQUARE_SIZE};

    for (size_t file = 0; file < BOARD_SIZE; ++file)
    {
        square.y = SQUARE_SIZE * file; /* cell's current rank (row) */

        for (size_t rank = 0; rank < BOARD_SIZE; ++rank)
        {
            square.x = SQUARE_SIZE * rank; /* cell's current file (column) */

            uint32_t square_color = ((rank + file) % 2 == 0) ? COLOR_WHITE : COLOR_BLACK;

            SDL_SetRenderDrawColor(renderer, RGBA_R(square_color), RGBA_G(square_color), RGBA_B(square_color),
                                   RGBA_A(square_color));
            if (!SDL_RenderFillRect(renderer, &square))
            {
                fprintf(stderr, "failed to render square: %s\n", SDL_GetError());
                return;
            }
        }
    }
}

/* return the path for the sprit "name" form chess-pieces directory */
static char *get_sprite_path(const char *name)
{
    const char *exe_path = SDL_GetBasePath();
    if (exe_path == NULL)
    {
        fprintf(stderr, "faild to get executable path: %s\n", SDL_GetError());
        return NULL;
    }

    char *piece_path = NULL;
    SDL_asprintf(&piece_path, "%s../chess-pieces/%s.png", exe_path, name);
    if (piece_path == NULL)
    {
        fprintf(stderr, "Faild to load sprite. Could NOT find %s.png", name);
        return NULL;
    }

    return piece_path;
}

static bool load_pieces(PieceGraphic *pieces, SDL_Renderer *renderer)
{
    const char *piece_names[PIECE_TYPE_COUNT] = {"w_king", "w_queen", "w_bishop", "w_knight", "w_rook", "w_pawn",
                                                 "b_king", "b_queen", "b_bishop", "b_knight", "b_rook", "b_pawn"};

    for (size_t i = 0; i < PIECE_TYPE_COUNT; ++i)
    {
        char *path = get_sprite_path(piece_names[i]);
        if (!path)
        {
            fprintf(stderr, "faild to create path for %s\n", piece_names[i]);
            return false;
        }

        pieces->pieces[i] = IMG_LoadTexture(renderer, path);
        if (pieces->pieces[i] == NULL)
        {
            fprintf(stderr, "could not load sprite (%s): %s\n", piece_names[i], SDL_GetError());
            return false;
        }

        fprintf(stdout, "piece loaded: %s\n", piece_names[i]);
    }

    fprintf(stdout, "successfully loaded all chess pieces!\n");
    return true;
}

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

    if (!load_pieces(&pg, state->app_renderer))
    {
        fprintf(stderr, "Could NOT load pieces!\n");
    }

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
        draw_chess_board(state->app_renderer);
        SDL_RenderPresent(state->app_renderer);

        state->end = SDL_GetPerformanceCounter();
        state->deltatime = (double)(state->end - state->start) / (double)SDL_GetPerformanceFrequency();

        if (state->deltatime < fps)
            SDL_Delay((fps - state->deltatime) * 1000.0f);
    }
}

static bool piece_textures_housekeeping()
{
    for (size_t i = 0; i < PIECE_TYPE_COUNT; ++i)
    {
        if (pg.pieces[i] != NULL)
            SDL_DestroyTexture(pg.pieces[i]);
        else
            fprintf(stderr, "wtf?????\n");
    }

    fprintf(stdout, "piece textures terminated!\n");

    return true;
}

/* clean-up before exiting the app */
bool app_housekeeping(AppState *state)
{
    if (!state)
    {
        fprintf(stderr, "app_housekeeping(): AppState is NULL!\n");
        return false;
    }

    if (!piece_textures_housekeeping())
    {
        fprintf(stderr, "piece_textures_housekeeping() failed!\n");
    }

    if (state->app_renderer)
    {
        SDL_DestroyRenderer(state->app_renderer);
        fprintf(stdout, "state->app_renderer has been TERMINATED!\n");
    }

    if (state->app_window)
    {
        SDL_DestroyWindow(state->app_window);
        fprintf(stdout, "state->app_window has been TERMINATED!\n");
    }

    SDL_Quit();

    return true;
}
