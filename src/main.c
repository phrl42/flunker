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

        // switch front buffer with back buffer or vice versa
        SDL_RenderPresent(rend);
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_QUIT:
                gameLoop = false;
                break;

            case SDL_KEYDOWN:
                switch (ev.key.keysym.sym)
                {
                case SDLK_SPACE:
                    SDL_Log("aa\n");
                    break;
                }
                break;

            default:
                break;
            }
        }
    }

    return 0;
}