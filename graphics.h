#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "data.h"

void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
void InitEverything(int , int , TTF_Font **, TTF_Font **,TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
void RenderTable( int [], TTF_Font *, SDL_Surface **, SDL_Renderer *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
void RenderPoints(TTF_Font *, list_t*, int*[], int [], int, SDL_Renderer *, int, char*[BUFFER_SIZE], int []);
void filledCircleRGBA(SDL_Renderer * , int, int , int , int , int , int );


void CityCoordinateCalculator(node_t *, int *, int *, int*[], int, char*[BUFFER_SIZE]);
int QuadrantDefiner (node_t* );
void CalculatePos(node_t* ,int* , int*);
void colorTemperatureToRGB (float, int*, int*, int*);
void RenderCounterOnScreen( SDL_Renderer *,TTF_Font *, int , int);
void ChoosingYear( SDL_Renderer *, TTF_Font *, int[], TTF_Font *);

#endif
