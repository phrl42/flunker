#ifndef __init__
#define __init__

#include "incs.h"

extern SDL_Renderer *rend;
extern SDL_Window *win;

extern SDL_Rect rectBottom;
extern SDL_Rect rectTop;

extern SDL_Rect rectPlayer;

void init();
void drawLanes();
void moveLanes();
void errorSolution();

#endif