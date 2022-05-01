#include "init.h"

int main()
{
    bool gameLoop = false;
    bool menu = true;
    bool pause = false;
    bool restart = false;
    SDL_Event ev;

    init();

    while (menu)
    {
        // clear the renderer
        SDL_RenderClear(rend);

        // render stuff here
        drawBackground();

        if (pause)
        {
            drawLanes();
        }

        menuStuff(restart);
        // buffer magic
        SDL_RenderPresent(rend);
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_KEYDOWN:
                switch (ev.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    menu = false;
                    break;

                case SDLK_RETURN:
                    if(restart)
                    {
                        reset();
                        restart = false;
                        failure = false;
                    }
                    gameLoop = true;
                    pause = true;
                    break;
                }
                break;

            default:
                break;
            }
        }

        while (gameLoop)
        {
            // clear the renderer
            SDL_RenderClear(rend);

            // render stuff here
            drawBackground();
            drawLanes();
            moveLanes();
            playerMovement();
            points();
            displayPoints();
            changeSeconds();
            // switch front buffer with back buffer or vice versa
            SDL_RenderPresent(rend);

            if(failure)
            {
                restart = true;
                gameLoop = false;
                SDL_Log("highscore: %d\n", highscore);
            }

            while (SDL_PollEvent(&ev))
            {
                switch (ev.type)
                {
                case SDL_KEYDOWN:
                    switch (ev.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        gameLoop = false;
                    }
                    break;

                default:
                    break;
                }
            }
        }
    }
    errorSolution();
}