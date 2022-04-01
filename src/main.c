#include "init.h"

int main()
{
    bool gameLoop = true;
    SDL_Event ev;

    init();

    while (gameLoop)
    {
        // clear the renderer
        SDL_RenderClear(rend);

        // render stuff here
        drawLanes();
        moveLanes();
        playerMovement();
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

    errorSolution();
}