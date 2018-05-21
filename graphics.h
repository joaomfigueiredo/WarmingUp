#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "data.h"

void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
void InitEverything(int , int , TTF_Font **, TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
int RenderTable(int, int, int [], TTF_Font *, SDL_Surface **, SDL_Renderer *, int [][MAX_BOARD_POS], int [][MAX_BOARD_POS]);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);


void CityCoordinateCalculator(node_t *, int *, int *, int*[], int);
int QuadrantDefiner (node_t* );
void CalculatePos(node_t* ,int* , int*);

#endif
