#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "math.h"
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


void InitEverything(int width, int height, TTF_Font **_font, TTF_Font **_font2, TTF_Font **_font3, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer){
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width + width, height, *_window);	//the width was doubled because we wanted the image to have double
																	//the width in relation to the height
    // load the table texture
    _img[0] = IMG_Load("./img/table_texture.png");	//inicialization of the map
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = IMG_Load("./img/setup.gif");		//inicialization of the menu image
    if (_img[1] == NULL)
    {
        printf("Unable to load gif: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    _img[2] = IMG_Load("./img/simtemp1.png");	//inicialization of the image with the opcion to do the temporal simulation
    if (_img[2] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    _img[3] = IMG_Load("./img/sair9.png");	//inicialization of the image with the opcion quit
    if (_img[3] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError()); 
        exit(EXIT_FAILURE);
    }


    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 17);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    *_font2 = TTF_OpenFont("OpenSans.ttf", 21);		//inicialization of a font (sans)
    if(!*_font2)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    *_font3 = TTF_OpenFont("7segdisplay.ttf", 45); ////inicialization of a font (7segmentdisplay type to cronometer in graphical)
    if(!*_font3)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer){
    SDL_Texture *text_IST;
    SDL_Rect boardPos;					//copied code from the teacher in ISTDots game that renders the logo and it will serve
										//to render more 2 images!
    // space occupied by the logo
    boardPos.x = x+650;
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


void RenderTable( int _board_size_px[], TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer){
    SDL_Color dark = { 255, 255, 255 };
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, board;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE + TABLE_SIZE; 
    tableDest.h = TABLE_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

  
    RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);  //renders the image of the menu

    RenderLogo(TABLE_SIZE, 150, _img[2], _renderer);  //renders the image of the time simulation

    RenderLogo(TABLE_SIZE, 500, _img[3], _renderer); //renders the quit img

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
}

int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer){
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;			//copied function from the ISTDots game made by the professor and it is used
    SDL_Rect solidRect;					//everytime we want to write something in the graphical mode

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

	//this function defines the quadrant that a city is in and returns a integer to every different case (4 cases)

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

	//This simple function calculate the position of a country in the map (pixel coordinates), and with the information
	//of the previous function it defines is quadrant

     int aux = 0;

     aux = QuadrantDefiner(_aux);

     *pt_x = (int)((_aux->payload.longit.angle) * (10.0/3));
     *pt_y = (int)((_aux->payload.lat.angle) * (10.0/3));

     if ( aux == 1){
		 //this means we are in the place 1st quadrant of a cartesian referencial and for the pixel referencial
		 // as y is pointed down we have to put negative its value
          *pt_x = *pt_x;
          *pt_y = -*pt_y;
     }

     else if (aux == 2){
		 //this means we are in the place 2nd quadrant of a cartesian referencial and for the pixel referencial
		 // as y is pointed down we have to put negative its value and x is negative aswell
          *pt_x =  -*pt_x;
          *pt_y = -*pt_y;
     }
     else if (aux == 3){
		 //this means we are in the place 3rd quadrant of a cartesian referencial and for the pixel referencial
		 // as y is pointed down we let positive and ptx have the oppositive value
          *pt_x = -*pt_x;
          *pt_y = *pt_y;
     }
     else if (aux == 4) {
		 //this means we are in the place 4th quadrant of a cartesian referencial and for the pixel referencial
		 // as y is pointed down we have to put negative its value
          *pt_x = *pt_x;
          *pt_y = *pt_y;
     }
    /*else { perguntar ao joao
           printf(ANSI_COLOR_WARNINGS "WARNING:" ANSI_COLOR_RESET "interpreting hemisphere failed(2_[%d]).\n", aux);
           exit(EXIT_FAILURE);
     }*/

}


void CityCoordinateCalculator(node_t *_head, int *pt_x, int *pt_y,int *pixel_coord_cities[2], int number_of_cities, char *cities_names[BUFFER_SIZE]){

	//this function uses the information in the list of the cities
	//if inside the while is to make sure its not the last position and the city is different than the next, to keep record of
	//its name and its coordinates
	//after he execute the function clculate pos to calculate is pixel coordinates to be ready to draw it in the map
	
      node_t *aux = _head;
      int i=0;

      while(aux->next != NULL){
            if (strcmp(aux->payload.city, aux->next->payload.city)!=0){
                  CalculatePos(aux, pt_x, pt_y);
                  if ((*pt_x==0)&&(*pt_y==0)) printf(ANSI_COLOR_WARNINGS "WARNING:" ANSI_COLOR_RESET "CalculatePos returning zeros.");
                  pixel_coord_cities[0][i]=*pt_x;
                  pixel_coord_cities[1][i]=*pt_y;
                  strcpy(cities_names[i], aux->payload.city);
                  i++;
            }
            strcpy(cities_names[i],aux->payload.city);


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

void RenderPoints(TTF_Font *segment,list_t* extremes_cities, int *pixel_coord_cities[2], int _board_size_px[], int _square_size_px, SDL_Renderer *_renderer, int number_of_cities, char *cities_names[BUFFER_SIZE], int extremes_dates[4] ){

	//function that renders the points in the map and paint them with the respective color
	//it uses the information that citycoordinatecalculator calculate 

	node_t *aux = extremes_cities->head;
	 int delay = 300;
     int circleX = 0;
     int circleY = 0;
     int circleR = 0;
     int blue = 0;
     int red = 0;
     int green = 0;
     float temp = 0;
     int i = 0;
     int k = 0;
     int f = 0;
     int min = 0;			//variable to calculate the min_year of the file
     int max = 0;			//variable to calculate the max_year of the file

	 min = extremes_dates[2]/10000; 
	 max = extremes_dates[3]/10000;
	
	 for ( k = min; k < max; k++){
		 for ( f = 01; f < 13; f++){

			 RenderCounterOnScreen(_renderer, segment, k, f); //Render the counter to show the date displayed

			 for ( i = 0; i < number_of_cities; i++){

				circleX = pixel_coord_cities[0][i]+600;		//calculates the center of the circle
				circleY = pixel_coord_cities[1][i]+300;
				circleR = 5;
				
				while (aux->next != NULL){
					if ((aux->payload.dt.year == k) && (aux->payload.dt.month == f) && strcmp(aux->payload.city,cities_names[i]) == 0){
						//if this conditions are all true it means we need to fill the circle with the right rgb color
						//calculated by the function colorTemperatureToRGB
					temp = aux->payload.temperature;
					colorTemperatureToRGB (temp, &red, &green, &blue );
					filledCircleRGBA(_renderer, circleX, circleY, circleR, red, green, blue);
					break;
					}
					aux = aux->next;
				}
				aux = extremes_cities->head;
			}
			SDL_RenderPresent(_renderer);
			//SDL_Delay(delay);
		}
	}
}

/**
 * filledCircleRGBA: renders a filled circle
 */

void filledCircleRGBA(SDL_Renderer * _renderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b){
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    float step = M_PI / (_circleR*8);

	//copied functions of the professor that fill a circle with the color predefined before

    SDL_SetRenderDrawColor(_renderer, _r, _g, _b, 255);

    while (_circleR > 0)
    {
        for (degree = 0.0; degree < M_PI/2; degree+=step)
        {
            off_x = (int)(_circleR * cos(degree));
            off_y = (int)(_circleR * sin(degree));

            SDL_RenderDrawPoint(_renderer, _circleX+off_x, _circleY+off_y);
            SDL_RenderDrawPoint(_renderer, _circleX-off_y, _circleY+off_x);
            SDL_RenderDrawPoint(_renderer, _circleX-off_x, _circleY-off_y);
            SDL_RenderDrawPoint(_renderer, _circleX+off_y, _circleY-off_x);
        }
        _circleR--;
    }
}



void colorTemperatureToRGB (float _temp ,int* red, int* green, int* blue){

    //this function is based on linear calculus. the measures where made bases in a -15 to a 40 degrees scale, putting blue as cold
    //and red as hot, it has several conditions cause the formula to pass temperature to rgb is not linear so it was needed several
    //linear regressions to obtain somo good aproximate scale

     if( _temp < 41 && _temp >= 23 ){

        *red = 255;
        *blue = 0;						//colors between a "weak" orange and a red
        *green = (566 - (14.15*_temp));
     }

     if( _temp < 23 && _temp >= 18 ){

        *red = ((17*_temp) - 136);
        *blue = 0;						//colors from a yellow to a "weak" orange
        *green = 255;
     }

     if( _temp < 18 && _temp >= 17 ){

        *red = ((170*_temp) - 2890);
        *blue = 0;
        *green = 255;
     }

     if( _temp < 17 && _temp >= 16 ){

        *red = 0;							//this 3 ifs are the passage grom the blue scale in rgb to the yellow one,
        *blue = 255;						//without giving a big jump in the scale
        *green = (2720 - (170*_temp));
     }

     if( _temp < 16 && _temp >= 10 ){

        *red = 0;
        *blue = 255;
        *green = (396.5 - (14.15*_temp));
     }

     if( _temp < 10 && _temp > -15 ){

        *red = 0;
        *blue = 10.2*_temp + 153;	//colors to a blue scale
        *green = 255;
     }
}



void RenderCounterOnScreen( SDL_Renderer *_renderer, TTF_Font *_font3, int _year, int _month){

	//this function renders the counter in the screen when we want to see the temporal simulator
     SDL_Color red = {255 , 0, 0};                      //defines color red
     SDL_Rect rect = {10, 500, 250, 80};				//defines the rectangele where it will show up

     char str[STRING_SIZE] = {0};						//used to save the year and month in type yyyy:mm

     SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 50);

     SDL_RenderFillRect(_renderer, &rect);

     sprintf(str, "%d:%d", _year, _month);				//put the year and month in analisis in a string to show it with rendertext

     RenderText(rect.x+ 5, rect.y+15, str, _font3, &red, _renderer );		//renders the executions before
}


/*
void ChoosingYear( SDL_Renderer *_renderer, TTF_Font *sans, int data_year_graphical[], TTF_Font *segment){


	int i = 0;
	int j = 0;
	int k = 1;
     SDL_Color light = {255, 255, 255};
     SDL_Color red = {255 , 0, 0};
     char str[STRING_SIZE] = "Qual o ano/mes que deseja introduzir? ";
     char str2[STRING_SIZE] = "Introduza no tipo AAAAMM no teclado!! ";
     char str3[STRING_SIZE] = {0};
     SDL_Rect rect = {400, 100, 400, 400};

     SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(_renderer, &rect);
	RenderText(rect.x+ 5, rect.y, str, sans, &light, _renderer );
	RenderText(rect.x+ 5, rect.y+22, str2, sans, &light, _renderer );

	for (i = 0; i < 3; i++){
		for( j = 0; j < 3; j++){
			sprintf(str3, "%d",k);
			//SDL_Rect rect2 = {rect.x + 25, rect.y+60,100,100};
			//SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 50);
			//SDL_RenderFillRect(_renderer, &rect2);
			rect.x = rect.x + 115;
			RenderText(rect.x-55, rect.y+55, str3, segment, &red, _renderer );
			k++;
		}
		rect.y = rect.y + 105;
		rect.x = rect.x - 345;
	}
	k = 0;
	sprintf(str3, "%d",k);
	
	RenderText(rect.x+50, rect.y+30, str3, segment, &red, _renderer );
	
	k = 1;
	SDL_RenderPresent(_renderer);
	
	//acabou aqui esta parte. agora é por uma variavel a chamar la no main pa outra funcao que vai executar a esxcrita, renderizar um quadrad
	//de alguma cor e por la os digitos a serem digitados no momento, nao deve ser uma funçao com mais de 50 linhas ate as 10 da noite.
	//depois é fazer o da funçao atras percorrendo o ficheiro e desenhando as bolas para os dados desse mes e ano, e implementar quando se la clica.

}
*/
