#include "incs.h"

bool failure = false;

const int objWidth = 100;

const Uint8 *keys;

const int pipes = 5;

// 100 = pipeBottom width
int honecker = (WIDTH + objWidth) * -1;

int speed = 4;

int score = 0;
int highscore = 0;

char scoreChar[4];

Uint32 temp0 = 0;
Uint32 temp1;

int seconds = 1;

SDL_Window *win;
SDL_Renderer *rend;

SDL_Rect rectBottom = {0, HEIGHT - 100, WIDTH + WIDTH + 100, 100};

SDL_Rect pipeBottom[5];
SDL_Rect pipeTop[5];

SDL_Rect pipeBottomHead[5];
SDL_Rect pipeTopHead[5];

SDL_Rect brezhnev = {0, 0, 0, 0};

SDL_Rect rectPlayer = {3 + (WIDTH / 3) - 100, HEIGHT / 2, 20, 20};

SDL_Texture *textureGround;

SDL_Texture *texturePipe;
SDL_Texture *texturePipeBottom;
SDL_Texture *texturePipeTop;

SDL_Texture *startTexture;
SDL_Texture *restartTexture;

SDL_Rect startRect = {WIDTH / 3, HEIGHT / 2, 350, 150};

TTF_Font *scoreDisplayFont;
SDL_Surface *scoreDisplay;
SDL_Texture *scoreTexture;
SDL_Color black = {0, 0, 0, 0};
SDL_Rect scoreRect = {(WIDTH / 2) - 30, 0, 30, 50};

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
        pipeBottom[i].w = 82;
        pipeTop[i].w = 82;
        pipeBottom[i].h = randRange(pipeBottom[i].h, (HEIGHT / 2) - rectBottom.h);
        pipeTop[i].h = randRange(pipeTop[i].h, HEIGHT / 2);

        // 264 is the nearest possible value for the distance between the pipes (one could calculate this)
        pipeTop[i].x = WIDTH - (i * 264);
        pipeBottom[i].x = WIDTH - (i * 264);

        pipeBottomHead[i].h = 18;
        pipeTopHead[i].h = 18;
        pipeBottomHead[i].w = objWidth;
        pipeTopHead[i].w = objWidth;
        pipeBottomHead[i].x = pipeBottom[i].x - 18;
        pipeTopHead[i].x = pipeTop[i].x - 18;
    }

    startTexture = IMG_LoadTexture(rend, "src/img/start.png");
    restartTexture = IMG_LoadTexture(rend, "src/img/restart.png");

    if(TTF_Init() == -1)
    {
        SDL_Log("initializing sdl2 ttf failed: %s\n", SDL_GetError());
    }
    scoreDisplayFont = TTF_OpenFont("src/ttf/mononoki-Regular.ttf", 18);
}

void drawBackground()
{
    sprintf(scoreChar, "%d", score);
    scoreDisplay = TTF_RenderText_Solid(scoreDisplayFont, (const char*)&scoreChar, black);
    scoreTexture = SDL_CreateTextureFromSurface(rend, scoreDisplay);
    
    SDL_RenderCopy(rend, textureGround, NULL, &rectBottom);

    SDL_SetRenderDrawColor(rend, 0, 0, 255, 0);

    SDL_SetRenderDrawColor(rend, 255, 0, 0, 0);

    SDL_RenderFillRect(rend, &rectPlayer);

    SDL_SetRenderDrawColor(rend, 173, 216, 230, 1);
}

void drawLanes()
{
    for (int i = 0; i < pipes; i++)
    {
        SDL_RenderCopy(rend, texturePipe, NULL, &pipeBottom[i]);
        SDL_RenderCopy(rend, texturePipe, NULL, &pipeTop[i]);

        SDL_RenderCopy(rend, texturePipeBottom, NULL, &pipeBottomHead[i]);
        SDL_RenderCopy(rend, texturePipeTop, NULL, &pipeTopHead[i]);
    }
}

int limiter = 0;

void playerMovement()
{
    keys = SDL_GetKeyboardState(NULL);

    // ground / top collision
    if (rectPlayer.y + rectPlayer.h >= rectBottom.y)
    {
        speed = 0;
        failure = true;
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
        if (SDL_IntersectRect(&rectPlayer, &pipeBottomHead[i], &brezhnev) == SDL_TRUE || SDL_IntersectRect(&rectPlayer, &pipeTopHead[i], &brezhnev) == SDL_TRUE || SDL_IntersectRect(&rectPlayer, &pipeTop[i], &brezhnev) == SDL_TRUE || SDL_IntersectRect(&rectPlayer, &pipeBottom[i], &brezhnev) == SDL_TRUE)
        {
            speed = 0;
            failure = true;
            return;
        }
    }

    rectPlayer.y += speed * seconds; // ,,physics''

    if (keys[SDL_SCANCODE_SPACE] == 1)
    {
        if(limiter == 0)
        {
            seconds = 1;
        }
        rectPlayer.y -= (speed * seconds) + speed;
        limiter++;
    }
    else 
    {
        // so the timer gets resetted once per jump (hinders constant pressing 'cheat') and long holded jumps are being penalized
        limiter = 0;
    }
}

void changeSeconds()
{
    temp1 = SDL_GetTicks();
    if(!temp0)
    {
        temp0 = temp1;
    }
    // SDL_TICKS_PASSED() exists
    if(temp0 + 1000 == temp1)
    {
        temp0 = temp1;
        seconds++;
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
        pipeBottomHead[i].x = pipeBottom[i].x - (pipeBottomHead[i].h / 2);
        
        pipeTop[i].x -= speed;
        pipeTopHead[i].x = pipeTop[i].x - (pipeBottomHead[i].h / 2);
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
            SDL_Log("score: %d\n", score);
            if(score >= highscore)
            {
                highscore = score;
            }
        }
    }
}

void menuStuff(bool restart)
{
    if(restart)
    {
        SDL_RenderCopy(rend, restartTexture, NULL, &startRect);
    }
    else
    {
        SDL_RenderCopy(rend, startTexture, NULL, &startRect);
    }
}

void reset()
{
    score = 0;
    speed = 4;
    limiter = 0;
    temp0 = 0;

    rectPlayer.x = 3 + (WIDTH / 3) - 100;
    rectPlayer.y = HEIGHT / 2;

    for (int i = 0; i < pipes; i++)
    {
        // 264 is the nearest possible value for the distance between the pipes (one could calculate this)
        pipeTop[i].x = WIDTH - (i * 264);
        pipeBottom[i].x = WIDTH - (i * 264);

        pipeBottomHead[i].x = pipeBottom[i].x - 18;
        pipeTopHead[i].x = pipeTop[i].x - 18;
    }
}

void displayPoints()
{
    SDL_RenderCopy(rend, scoreTexture, NULL, &scoreRect);
}

void errorSolution()
{
    SDL_DestroyTexture(texturePipeTop);
    SDL_DestroyTexture(texturePipeBottom);
    SDL_DestroyTexture(textureGround);
    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(restartTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    TTF_CloseFont(scoreDisplayFont);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
