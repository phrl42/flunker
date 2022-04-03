#include "init.h"

int main()
{
    bool gameLoop = false;
    bool menu = true;
    SDL_Event ev;

    init();

    while (menu)
    {
        // clear the renderer
        SDL_RenderClear(rend);

        // render stuff here
        drawLanes();
        menuStuff();
        // buffer magic
        SDL_RenderPresent(rend);
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_QUIT:
                menu = false;
                break;

            case SDL_KEYDOWN:
                switch (ev.key.keysym.sym)
                {
                case SDLK_RETURN:
                    gameLoop = true;
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
            drawLanes();
            moveLanes();
            playerMovement();
            points();
            // switch front buffer with back buffer or vice versa
            SDL_RenderPresent(rend);
            while (SDL_PollEvent(&ev))
            {
                switch (ev.type)
                {
                case SDL_QUIT:
                    gameLoop = false;
                    break;

                default:
                    break;
                }
            }
        }
    }
    errorSolution();
}