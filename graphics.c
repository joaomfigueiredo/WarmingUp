#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "graphics.h"

extern const char myName[];
extern const char myNumber[];
extern const int colors[3][MAX_COLORS];


void InitSDL(){
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
* InitFont: Initializes the SDL2_ttf font library
*/
void InitFont(){
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height){

    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "Warming Up", WINDOW_POSX, WINDOW_POSY, width + width -200, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;

}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window){
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}


void InitEverything(int width, int height, TTF_Font **_font, TTF_Font **_font2, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer){
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width + width, height, *_window); // ao duplicar a width no create renderer bsicamente ponho a imagem de aarecer quadrada para a su aresoluçao

    // load the table texture
    _img[0] = IMG_Load("./img/table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = IMG_Load("./img/setup.gif");
    if (_img[1] == NULL)
    {
        printf("Unable to load gif: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    _img[2] = IMG_Load("./img/simtemp1.png");
    if (_img[2] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    _img[3] = IMG_Load("./img/ano2.png");
    if (_img[3] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    _img[4] = IMG_Load("./img/infopais3.png");
    if (_img[4] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    _img[5] = IMG_Load("./img/sair9.png");
    if (_img[5] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }


    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    *_font2 = TTF_OpenFont("OpenSans.ttf", 35); //inicializaçao da outra fonte
    if(!*_font2)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer){
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x+750;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}


int RenderTable( int _board_pos_x, int _board_pos_y, int _board_size_px[],
        TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, int _board[][MAX_BOARD_POS], int moves[][MAX_BOARD_POS]){
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color light = { 255, 255, 255 };
    SDL_Color dark = { 255, 255, 255 };
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, board, board_square;
    int height, board_size, square_size_px, max_pos /*,clr=0*/;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE + TABLE_SIZE; // table est é a estrutura que define o tamanho da imagem!
    tableDest.h = TABLE_SIZE; //define a altura. como tenho table size x2 em cima fcia com o dobrodo tamanho e visto termos uma img 2048/1024 facil

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    height = RenderLogo(TABLE_SIZE, 150, _img[2], _renderer);

    height = RenderLogo(TABLE_SIZE, 265, _img[3], _renderer);

    height = RenderLogo(TABLE_SIZE, 380, _img[4], _renderer);

    height = RenderLogo(TABLE_SIZE, 500, _img[5], _renderer);

    // render the student name
    //height += RenderText(TABLE_SIZE+3*MARGIN, height, myName, _font, &black, _renderer);

    // this renders the student number
    //RenderText(TABLE_SIZE+3*MARGIN, height, myNumber, _font, &black, _renderer);

    // compute and adjust the size of the table and squares
   // max_pos = MAX(_board_pos_x, _board_pos_y);
   // board_size = (int)(BOARD_SIZE_PER*TABLE_SIZE);
   // square_size_px = (board_size - (max_pos+1)*SQUARE_SEPARATOR) / max_pos;
    //_board_size_px[0] = _board_pos_x+5*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;
    //_board_size_px[1] = _board_pos_y+5*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;

    // renders the entire board background
    SDL_SetRenderDrawColor(_renderer, dark.r, dark.g, dark.b, dark.a );
      board.x = (TABLE_SIZE - _board_size_px[0]) >> 1;
      board.y = (TABLE_SIZE - _board_size_px[1] - 15);
      board.w = _board_size_px[0];
      board.h = _board_size_px[1];
      SDL_RenderFillRect(_renderer, &board);

    // destroy everything
    SDL_DestroyTexture(table_texture);
    // return for later use
    return square_size_px;
}

int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer){
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

int QuadrantDefiner (node_t* _aux){

     if ((_aux->payload.lat.hemisphere == 'N') && (_aux->payload.longit.hemisphere == 'E')) return 1;
     else if ((_aux->payload.lat.hemisphere == 'N') && (_aux->payload.longit.hemisphere == 'W')) return 2;
     else if ((_aux->payload.lat.hemisphere == 'S') && (_aux->payload.longit.hemisphere == 'W')) return 3;
     else if ((_aux->payload.lat.hemisphere == 'S') && (_aux->payload.longit.hemisphere == 'E')) return 4;
     else{
           printf(ANSI_COLOR_WARNINGS "WARNING:" ANSI_COLOR_RESET "interpreting hemisphere failed.");
           exit(EXIT_FAILURE);
     }
}

void CalculatePos (node_t* _aux ,int* pt_x, int* pt_y  ){

     int aux = 0;

     aux = QuadrantDefiner(_aux);

     *pt_x = (int)((_aux->payload.longit.angle) * (5.0/3));
     *pt_y = (int)((_aux->payload.lat.angle) * (5.0/3));


     if (aux == 2){
       *pt_x =  *pt_x;
     }
     else if (aux == 3){
          *pt_x = -*pt_x;
          *pt_y = -*pt_y;
     }
     else {
       *pt_y = -*pt_y;
     }
    /*else {
           printf(ANSI_COLOR_WARNINGS "WARNING:" ANSI_COLOR_RESET "interpreting hemisphere failed(2_[%d]).\n", aux);
           exit(EXIT_FAILURE);
     }*/


}


void CityCoordinateCalculator(node_t *_head, int *pt_x, int *pt_y,int  *pixel_coord_cities[2], int number_of_cities){
      node_t *aux = _head;
      int i=0;

      while(aux->next != NULL){
            if (strcmp(aux->payload.city, aux->next->payload.city)!=0){
                  CalculatePos(aux, pt_x, pt_y);
                  if ((*pt_x==0)&&(*pt_y==0)) printf(ANSI_COLOR_WARNINGS "WARNING:" ANSI_COLOR_RESET "CalculatePos returning zeros.");
                  pixel_coord_cities[0][i]=*pt_x;
                  pixel_coord_cities[1][i]=*pt_y;
                  //printf("%d  %d\n", pixel_coord_cities[0][i], pixel_coord_cities[1][i]  );
                  //printf("%s\n", aux->payload.city);
                  i++;
            }


            if (number_of_cities<=i){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Exceeded number of expected cities (%d)", number_of_cities);
                  exit(EXIT_FAILURE);
                  break;
            }

            aux=aux->next;
      }
      //for the last one
      CalculatePos(aux, pt_x, pt_y);
      if ((*pt_x==0)&&(*pt_y==0)) printf(ANSI_COLOR_WARNINGS "WARNING:" ANSI_COLOR_RESET "CalculatePos returning zeros.");
      pixel_coord_cities[0][i]=*pt_x;
      pixel_coord_cities[1][i]=*pt_y;
}
