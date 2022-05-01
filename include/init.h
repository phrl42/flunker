#ifndef __init__
#define __init__

#include "incs.h"

extern bool failure;
extern int highscore;
extern SDL_Renderer *rend;
extern SDL_Window *win;

void randRange();
void init();
void drawBackground();
void drawLanes();
void moveLanes();
void generatePipes();
void playerMovement();
void points();
void menuStuff(bool restart);
void changeSeconds();
void reset();
void displayPoints();
void errorSolution();

#endif