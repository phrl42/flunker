#include "incs.h"

SDL_Window *win;
SDL_Renderer *rend;

SDL_Rect rectBottom = {0, HEIGHT - 100, WIDTH + WIDTH + 100, 100};
SDL_Rect pipeBottom = {WIDTH + 100, HEIGHT - 200, 100, 100};
SDL_Rect pipeTop = {0, 0, 100, 100};

SDL_Rect brezhnev = {0, 0, 0, 0};

SDL_Rect rectPlayer = {(WIDTH / 2) - 100, HEIGHT / 2, 20, 20};

SDL_Texture *textureGround;

const Uint8* keys;

int honecker = -1400;

int speed = 4;

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
    
    IMG_Init(IMG_INIT_PNG);
    
    textureGround = IMG_LoadTexture(rend, "src/img/ground.png");
}

void drawLanes()
{
    SDL_RenderCopy(rend, textureGround, NULL, &rectBottom);

    SDL_SetRenderDrawColor(rend, 0, 0, 255, 0);
    SDL_RenderFillRect(rend, &pipeBottom);
    SDL_RenderFillRect(rend, &pipeTop);

    SDL_SetRenderDrawColor(rend, 255, 0, 0, 0);

    SDL_RenderFillRect(rend, &rectPlayer);

    SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
}

void playerMovement()
{
    // ground collision
    if(rectPlayer.y + rectPlayer.h >= rectBottom.y)
    {
        speed = 0;
        return;
    }

    // bottom collision brezhnev == some pseudo rect
    if(SDL_IntersectRect(&rectPlayer, &pipeBottom, &brezhnev) == SDL_TRUE || SDL_IntersectRect(&rectPlayer, &pipeTop, &brezhnev) == SDL_TRUE)
    {
        speed = 0;
        return;
    }

    rectPlayer.y += speed; // physics

    keys = SDL_GetKeyboardState(NULL);

    if(keys[SDL_SCANCODE_SPACE] == 1)
    {
        rectPlayer.y -= 10;
    }
}

void generatePipes()
{
    pipeBottom.x = rectBottom.x + WIDTH;
    pipeTop.x = rectBottom.x + WIDTH;
}

// stolen from phrl42/pong

int randRange(int min, int max)
{
    int diff = max - min;
    // stackoverflow magic
    return (int) (((double)(diff+1)/RAND_MAX) * rand() + min); // math is powerful / useful(!!)
}

void moveLanes()
{
    // "resetting"
    if(rectBottom.x <= honecker)
    {
        rectBottom.x = 0;
        pipeBottom.h = randRange(100, (HEIGHT / 2) - rectBottom.h);
        pipeTop.h = randRange(100, HEIGHT / 2);
    }

    // (de)crement (stage movement)
    rectBottom.x -= speed;
    // make bottom pipe's y coordinate depend on height
    pipeBottom.y = (HEIGHT - pipeBottom.h) - rectBottom.h;

    generatePipes();
}

void errorSolution()
{
    SDL_DestroyTexture(textureGround);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}