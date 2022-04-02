#include "incs.h"

const int objWidth = 100;

const Uint8 *keys;

const int pipes = 5;

// 100 = pipeBottom width
int honecker = (WIDTH + objWidth) * -1;

int speed = 4;

SDL_Window *win;
SDL_Renderer *rend;

SDL_Rect rectBottom = {0, HEIGHT - 100, WIDTH + WIDTH + 100, 100};

SDL_Rect pipeBottom[5];
SDL_Rect pipeTop[5];

SDL_Rect brezhnev = {0, 0, 0, 0};

SDL_Rect rectPlayer = {(WIDTH / 3) - 100, HEIGHT / 2, 20, 20};

SDL_Texture *textureGround;

// stolen from phrl42/pong

int randRange(int min, int max)
{
    int diff = max - min;
    // stackoverflow magic
    return (int)(((double)(diff + 1) / RAND_MAX) * rand() + min); // math is powerful / useful(!!)
}

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

    if (!rend)
    {
        SDL_Log("initializing renderer failed: %s\n", SDL_GetError());
    }

    IMG_Init(IMG_INIT_PNG);

    textureGround = IMG_LoadTexture(rend, "src/img/ground.png");

    for (int i = 0; i < pipes; i++)
    {
        pipeBottom[i].w = objWidth;
        pipeTop[i].w = objWidth;
        pipeBottom[i].h = randRange(100, (HEIGHT / 2) - rectBottom.h);
        pipeTop[i].h = randRange(100, HEIGHT / 2);

        pipeTop[i].x = WIDTH - (i * 242);
        pipeBottom[i].x = WIDTH - (i * 242);
    }
}

void drawLanes()
{
    SDL_RenderCopy(rend, textureGround, NULL, &rectBottom);

    SDL_SetRenderDrawColor(rend, 0, 0, 255, 0);

    for (int i = 0; i < pipes; i++)
    {
        SDL_RenderFillRect(rend, &pipeBottom[i]);
        SDL_RenderFillRect(rend, &pipeTop[i]);
    }

    SDL_SetRenderDrawColor(rend, 255, 0, 0, 0);

    SDL_RenderFillRect(rend, &rectPlayer);

    SDL_SetRenderDrawColor(rend, 173, 216, 230, 1);
}

void playerMovement()
{
    keys = SDL_GetKeyboardState(NULL);

    // ground / top collision
    if (rectPlayer.y + rectPlayer.h >= rectBottom.y)
    {
        speed = 0;
        return;
    }
    else if (rectPlayer.y < 0)
    {
        rectPlayer.y += speed;
        return;
    }

    // bottom collision brezhnev == some pseudo rect
    if (SDL_IntersectRect(&rectPlayer, &pipeBottom[0], &brezhnev) == SDL_TRUE || SDL_IntersectRect(&rectPlayer, &pipeTop[0], &brezhnev) == SDL_TRUE)
    {
        speed = 0;
        return;
    }

    rectPlayer.y += speed; // ,,physics''

    if (keys[SDL_SCANCODE_SPACE] == 1)
    {
        rectPlayer.y -= 10;
    }
}

void generatePipes()
{
    // spawn location / resetting
    for (int i = 0; i < pipes; i++)
    {
        if (pipeBottom[i].x < 0)
        {
            pipeBottom[i].x = WIDTH;
            pipeTop[i].x = WIDTH;
        }
    }
}

void moveLanes()
{
    // "resetting"
    if (rectBottom.x <= honecker)
    {
        rectBottom.x = 0;
        for (int i = 0; i < pipes; i++)
        {
            pipeBottom[i].h = randRange(100, (HEIGHT / 2) - rectBottom.h);
            pipeTop[i].h = randRange(100, HEIGHT / 2);
        }
    }

    // (de)crement (stage movement)
    rectBottom.x -= speed;
    // make bottom pipe's y coordinate depend on height
    for (int i = 0; i < pipes; i++)
    {
        pipeBottom[i].y = (HEIGHT - pipeBottom[i].h) - rectBottom.h;

        // and move the pipe mate
        pipeBottom[i].x -= speed;
        pipeTop[i].x -= speed;
    }

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