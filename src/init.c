#include "incs.h"

const int objWidth = 100;

const Uint8 *keys;

const int pipes = 5;

// 100 = pipeBottom width
int honecker = (WIDTH + objWidth) * -1;

int speed = 4;

int score = 0;
int highscore = 0;

SDL_Window *win;
SDL_Renderer *rend;

SDL_Rect rectBottom = {0, HEIGHT - 100, WIDTH + WIDTH + 100, 100};

SDL_Rect pipeBottom[5];
SDL_Rect pipeTop[5];

SDL_Rect pipeBottomHead[5];
SDL_Rect pipeTopHead[5];

SDL_Rect brezhnev = {0, 0, 0, 0};

SDL_Rect rectPlayer = {1 + (WIDTH / 3) - 100, HEIGHT / 2, 20, 20};

SDL_Texture *textureGround;

SDL_Texture *texturePipe;
SDL_Texture *texturePipeBottom;
SDL_Texture *texturePipeTop;

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

    texturePipe = IMG_LoadTexture(rend, "src/img/pipe.png");
    texturePipeBottom = IMG_LoadTexture(rend, "src/img/pipeHeadUp.png");
    texturePipeTop = IMG_LoadTexture(rend, "src/img/pipeHeadDown.png");


    for (int i = 0; i < pipes; i++)
    {
        pipeBottom[i].w = objWidth;
        pipeTop[i].w = objWidth;
        pipeBottom[i].h = randRange(pipeBottom[i].h, (HEIGHT / 2) - rectBottom.h);
        pipeTop[i].h = randRange(pipeTop[i].h, HEIGHT / 2);

        // 264 is the nearest possible value for the distance between the pipes (one could calculate this)
        pipeTop[i].x = WIDTH - (i * 264);
        pipeBottom[i].x = WIDTH - (i * 264);

        pipeBottomHead[i].h = 18;
        pipeTopHead[i].h = 18;
        pipeBottomHead[i].w = objWidth;
        pipeTopHead[i].w = objWidth;
        pipeBottomHead[i].x = pipeBottom[i].x;
        pipeTopHead[i].x = pipeTop[i].x;
    }
}

void drawLanes()
{
    SDL_RenderCopy(rend, textureGround, NULL, &rectBottom);

    SDL_SetRenderDrawColor(rend, 0, 0, 255, 0);

    for (int i = 0; i < pipes; i++)
    {
        SDL_RenderCopy(rend, texturePipe, NULL, &pipeBottom[i]);
        SDL_RenderCopy(rend, texturePipe, NULL, &pipeTop[i]);

        SDL_RenderCopy(rend, texturePipeBottom, NULL, &pipeBottomHead[i]);
        SDL_RenderCopy(rend, texturePipeTop, NULL, &pipeTopHead[i]);
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

    // pipe collision (brezhnev == some pseudo rect)
    for (int i = 0; i < pipes; i++)
    {
        if (SDL_IntersectRect(&rectPlayer, &pipeBottomHead[i], &brezhnev) == SDL_TRUE || SDL_IntersectRect(&rectPlayer, &pipeTopHead[i], &brezhnev) == SDL_TRUE)
        {
            speed = 0;
            return;
        }
    }

    rectPlayer.y += speed; // ,,physics''

    if (keys[SDL_SCANCODE_SPACE] == 1)
    {
        rectPlayer.y -= 10;
    }
}

void generatePipes()
{
    // spawn location / resetting pipes
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
    // resetting ground
    if (rectBottom.x <= honecker)
    {
        rectBottom.x = 0;
    }

    // stage movement
    rectBottom.x -= speed;

    for (int i = 0; i < pipes; i++)
    {
        // make bottom pipe's y coordinate depend on its height
        pipeBottom[i].y = (HEIGHT - pipeBottom[i].h) - rectBottom.h;
        pipeBottomHead[i].y = pipeBottom[i].y - pipeBottomHead[i].h;


        // make pipeTopHead relative to pipeTop's height
        pipeTopHead[i].y = pipeTop[i].h;

        // and move the pipes mate
        pipeBottom[i].x -= speed;
        pipeBottomHead[i].x = pipeBottom[i].x;
        
        pipeTop[i].x -= speed;
        pipeTopHead[i].x = pipeTop[i].x;
    }
    generatePipes();
}

void points()
{
    for(int i = 0; i < pipes; i++)
    {
        if(pipeBottom[i].x + objWidth == rectPlayer.x)
        {
            score++;
        }
    }
}

void menuStuff()
{
    
}

void errorSolution()
{
    SDL_DestroyTexture(texturePipeTop);
    SDL_DestroyTexture(texturePipeBottom);
    SDL_DestroyTexture(textureGround);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}