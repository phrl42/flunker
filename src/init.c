#include "incs.h"

SDL_Window *win;
SDL_Renderer *rend;

SDL_Rect rectBottom = {0, HEIGHT - 101, WIDTH, 100};
SDL_Rect rectTop = {0, 0, WIDTH, 100};

SDL_Rect rectPlayer = {(WIDTH / 2) - 100, HEIGHT / 2, 20, 20};

void init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("initializing sdl2 failed: %s\n", SDL_GetError());
        exit(0);
        SDL_Quit();
    }
    win = SDL_CreateWindow("flunker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(!rend)
    {
        SDL_Log("initializing renderer failed: %s\n", SDL_GetError());
    }
}

void drawLanes()
{
    SDL_SetRenderDrawColor(rend, 0, 255, 10, 0);

    SDL_RenderDrawRect(rend, &rectBottom);
    SDL_RenderDrawRect(rend, &rectTop);

    SDL_SetRenderDrawColor(rend, 255, 0, 0, 0);

    SDL_RenderFillRect(rend, &rectPlayer);

    SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
}

void moveLanes()
{
    if(rectBottom.x + WIDTH < WIDTH)
    {
        rectBottom.w += 3;
    }
    rectBottom.x -= 3;

    if(rectTop.x + WIDTH < WIDTH)
    {
        rectTop.w += 3;
    }
    rectTop.x -= 3;
}

void errorSolution()
{
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}