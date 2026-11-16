#include "render.h"

#include <stdio.h>
#include <stdint.h>

#define SDL_INIT_FLAGS SDL_INIT_VIDEO
#define WINDOW_FALGS 0

static int draw_chess_board(SDL_Renderer *rendering_target)
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

static int load_pieces(Piece_Render *pieces, Game *game)
{
    /* black textures setup */
    pieces->b_king = IMG_LoadTexture(game->renderer, "../assets/chess-set/b_king.png");
    if (!pieces->b_king)
    {
        fprintf(stderr, "Failed to load BLAKC KING texture.\n");
        return 0;
    }

    pieces->b_queen = IMG_LoadTexture(game->renderer, "../assets/chess-set/b_queen.png");
    if (!pieces->b_queen)
    {
        fprintf(stderr, "Failed to load BLAKC QUEEN texture.\n");
        return 0;
    }

    pieces->b_knight = IMG_LoadTexture(game->renderer, "../assets/chess-set/b_knight.png");
    if (!pieces->b_knight)
    {
        fprintf(stderr, "Failed to load BLAKC KNIGHT texture.\n");
        return 0;
    }

    pieces->b_rook = IMG_LoadTexture(game->renderer, "../assets/chess-set/b_rook.png");
    if (!pieces->b_rook)
    {
        fprintf(stderr, "Failed to load BLAKC ROOK texture.\n");
        return 0;
    }

    pieces->b_bishop = IMG_LoadTexture(game->renderer, "../assets/chess-set/b_bishop.png");
    if (!pieces->b_bishop)
    {
        fprintf(stderr, "Failed to load BLAKC BISHOP texture.\n");
        return 0;
    }

    pieces->b_pawn = IMG_LoadTexture(game->renderer, "../assets/chess-set/b_pawn.png");
    if (!pieces->b_pawn)
    {
        fprintf(stderr, "Failed to load BLAKC PAWN texture.\n");
        return 0;
    }

    /* white pieces */
    pieces->w_king = IMG_LoadTexture(game->renderer, "../assets/chess-set/w_king.png");
    if (!pieces->w_king)
    {
        fprintf(stderr, "Failed to load WHITE KING texture.\n");
        return 0;
    }

    pieces->w_queen = IMG_LoadTexture(game->renderer, "../assets/chess-set/w_queen.png");
    if (!pieces->w_queen)
    {
        fprintf(stderr, "Failed to load WHITE QUEEN texture.\n");
        return 0;
    }

    pieces->w_knight = IMG_LoadTexture(game->renderer, "../assets/chess-set/w_knight.png");
    if (!pieces->w_knight)
    {
        fprintf(stderr, "Failed to load WHITE KNIGHT texture.\n");
        return 0;
    }

    pieces->w_rook = IMG_LoadTexture(game->renderer, "../assets/chess-set/w_rook.png");
    if (!pieces->b_rook)
    {
        fprintf(stderr, "Failed to load WHITE ROOK texture.\n");
        return 0;
    }

    pieces->w_bishop = IMG_LoadTexture(game->renderer, "../assets/chess-set/w_bishop.png");
    if (!pieces->w_bishop)
    {
        fprintf(stderr, "Failed to load WHITE BISHOP texture.\n");
        return 0;
    }

    pieces->w_pawn = IMG_LoadTexture(game->renderer, "../assets/chess-set/w_pawn.png");
    if (!pieces->w_pawn)
    {
        fprintf(stderr, "Failed to load WHITE PAWN texture.\n");
        return 0;
    }

    return 1;
}

static void draw_pieces(Piece_Render *pieces, Game *game)
{
    SDL_FRect black_rect = {.x = 0, .y = SQUARE_SIZE * 1, .w = 100, .h = 100};

    // drawing the black pawns
    for (int i = 0; i < 8; ++i)
    {
        black_rect.x = SQUARE_SIZE * i;

        SDL_RenderTexture(game->renderer, pieces->b_pawn, NULL, &black_rect);
    }

    SDL_FRect white_rect = {.x = 0, .y = SQUARE_SIZE * 6, .w = 100, .h = 100};

    for (int i = 0; i < 8; ++i)
    {
        white_rect.x = SQUARE_SIZE * i;

        SDL_RenderTexture(game->renderer, pieces->w_pawn, NULL, &white_rect);
    }
}

int initialize_graphics(Game *game, Piece_Render *pieces)
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
        housekeeping(game, pieces);
        return 0;
    }

    game->renderer = SDL_CreateRenderer(game->window, NULL);
    if (game->renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer() Failed! %s\n", SDL_GetError());
        housekeeping(game, pieces);
        return 0;
    }

    load_pieces(pieces, game);

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

void render_loop(Game *game, Piece_Render *pieces)
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
        draw_pieces(pieces, game);

        SDL_RenderPresent(game->renderer);

        /* time snyc */
        end = SDL_GetPerformanceCounter();
        delta_time = (double)(end - start) / (double)SDL_GetPerformanceFrequency();
        if (delta_time < target_fps)
            SDL_Delay((target_fps - delta_time) * 1000.0f);
    }
}

static int terminate_textures(Piece_Render *pieces)
{
    if (pieces == NULL)
    {
        fprintf(stderr, "terminate_textures(), pieces is not initialized!\n");
        return 0;
    }

    /* black */
    SDL_DestroyTexture(pieces->b_king);
    SDL_DestroyTexture(pieces->b_queen);
    SDL_DestroyTexture(pieces->b_rook);
    SDL_DestroyTexture(pieces->b_knight);
    SDL_DestroyTexture(pieces->b_bishop);
    SDL_DestroyTexture(pieces->b_pawn);

    /* white */
    SDL_DestroyTexture(pieces->w_king);
    SDL_DestroyTexture(pieces->w_queen);
    SDL_DestroyTexture(pieces->w_rook);
    SDL_DestroyTexture(pieces->w_knight);
    SDL_DestroyTexture(pieces->w_bishop);
    SDL_DestroyTexture(pieces->w_pawn);

    fprintf(stdout, "All Textures Cleared!\n");

    return 1;
}

int housekeeping(Game *game, Piece_Render *pieces)
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

    if (!terminate_textures(pieces))
        return 0;

    SDL_Quit();
    return 1;
}
