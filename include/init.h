#ifndef __init__
#define __init__

#include "incs.h"

extern SDL_Renderer *rend;
extern SDL_Window *win;

void randRange();
void init();
void drawLanes();
void moveLanes();
void generatePipes();
void playerMovement();
void errorSolution();

#endif