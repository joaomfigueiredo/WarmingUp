#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"
#include "lists.h"
#include "graphics.h"
#include "data_treatment.h"

//ALEX estes consts chars não servem para nada pois não?
const char myName[] = "Alex";
const char myNumber[] = "Golo";
const int colors[3][MAX_COLORS] = {{246, 52, 255, 186, 124},{255, 211, 0, 0, 39},{37, 21, 0, 93, 137}};

int main(int argc, char *argv[]){
    int i=0;

    int aux_ms = 0, auxth = 0, aux_df = 0, aux_ma=0; //store user selections in menus
    int mode = 0;
    int T = 0; //periodo de analise para historico de temperaturas
    int ano = 0;
    int months_MA;

    int starting_yearmonth[2] = {0}; //vetor que guarda, nesta ordem, o ano e o mês em que o estudo começa
    int months_interval[2] = {0};
    char files[2][FILENAME_SIZE]={{0}};


    int board_pos_x = 0;
    int board_pos_y = 0;
    int square_size_px = 0;
    int board_size_px[2] = {0};
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};
    int moves[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}}; // guarda os moves
    int delay = 300;
    int pt_x = 0;
    int pt_y = 0;


    list_t* extremes_countries = NULL;
    list_t* extremes_cities = NULL;
    int number_of_cities=0, *pixel_coord_cities[2];
    char **cities_names;
    int extremes_dates[4]={0};//stores in 0 and 1 coutires minimum_date and maximum date and in 2 and 3 about CITIES
    float *temp_cities[1];
    char place_in_analysis[BUFFER_SIZE]={0};
    //Declarações para os graphics// initialize graphics
    int quit = 0;
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    TTF_Font *sans = NULL; //nova fonte
    SDL_Surface *imgs[6];
    SDL_Event event;


 //------------------------INITIALIZATIONS-------------------------------------------------//
      mode = ParamReading(argc, argv, files);

      extremes_countries = (list_t*)malloc(sizeof(list_t));
            if (extremes_countries == NULL){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
                  exit(EXIT_FAILURE);
            }

      extremes_cities = (list_t*)malloc(sizeof(list_t));
            if (extremes_cities == NULL){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
                  exit(EXIT_FAILURE);
            }

      fprintf(stderr,ANSI_COLOR_WARNINGS "LOADING AND SORTING COUNTRIES" ANSI_COLOR_RESET);
     	LoadTempCountries(files[0], extremes_countries, extremes_dates, mode);
      fprintf(stderr,ANSI_COLOR_BOLD_GREEN "\rCOUNTRIES ARE NOW LOADED AND SORTED\n" ANSI_COLOR_RESET);

      fprintf(stderr,"LOADING AND SORTING CITIES");
      LoadTempCities(files[1], extremes_cities, extremes_dates, mode);
      fprintf(stderr,ANSI_COLOR_BOLD_GREEN "\rCITIES ARE NOW LOADED AND SORTED\n" ANSI_COLOR_RESET);

      number_of_cities=CountCities(extremes_cities->head);
      pixel_coord_cities[0]=calloc(number_of_cities,sizeof(int));
      pixel_coord_cities[1]=calloc(number_of_cities,sizeof(int));
      temp_cities[0]=calloc(number_of_cities,sizeof(float));

      cities_names=calloc(number_of_cities,sizeof(char*));
      for ( i=0; i<number_of_cities; i++){
          cities_names[i]=calloc(BUFFER_SIZE,sizeof(char));
          if (cities_names[i]== NULL){
                printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of 'cities_names'!");
                exit(EXIT_FAILURE);
          }
      }

      CityCoordinateCalculator(extremes_cities->head, &pt_x, &pt_y,pixel_coord_cities, number_of_cities, temp_cities,cities_names);
//----------------END OF INITIALIZATIONS-------------------------------------------------//



      /*for(i=0; i<number_of_cities; i++){
            printf("%s\n", cities_names[i] );
            printf("%d____%d\n", pixel_coord_cities[0][i], pixel_coord_cities[1][i] );
      }
	*/

      printf("%d  %d   %d   %d", extremes_dates[0],extremes_dates[1],extremes_dates[2],extremes_dates[3]);

      if (mode==TEXTUAL){
	      while(1){
                  if ( (aux_ms == 1)) break;

                  MenuSurfer(&T, &ano, &aux_df, &aux_ms, &auxth, &aux_ma, &months_MA, place_in_analysis);

                  while (aux_df != 0){
                        if (aux_df==3 && months_interval[0]!=0){
                              ReLoadFiles(files,extremes_countries, extremes_cities, extremes_dates);
                        }
                        TreatmentDataFilter(&aux_df, starting_yearmonth, months_interval);
                        if (aux_df==1){
                              ReLoadFiles(files,extremes_countries, extremes_cities, extremes_dates);
                              aux_df=0;
                              break;
                        }

                        if (aux_df==2 && starting_yearmonth[0]*10000+starting_yearmonth[1]*100<extremes_dates[0]){
                              ReLoadFiles(files,extremes_countries, extremes_cities, extremes_dates);
                        }

                        ConditionalNodeDeleter(extremes_countries, COUNTRIES,months_interval,starting_yearmonth, extremes_dates);

                        ConditionalNodeDeleter(extremes_cities, CITIES,months_interval,starting_yearmonth, extremes_dates);

                        printf("%d  %d   %d   %d", extremes_dates[0],extremes_dates[1],extremes_dates[2],extremes_dates[3]);

                        aux_df=0;
                  }
                  while (auxth!=0){
                        switch (auxth) {
                              case GLOBAL:
                                    TempHistGLOBAL_COUNTRIES(extremes_countries, T, auxth, place_in_analysis, extremes_dates);
                                    break;
                              case PER_COUNTRY:
                                    TempHistGLOBAL_COUNTRIES(extremes_countries, T, auxth, place_in_analysis, extremes_dates);
                                    break;
                              case PER_CITY:
                                    TempHistCITIES(extremes_cities, T, auxth, place_in_analysis, extremes_dates);
                                    break;
                              default:
                                    break;
                        }
                        auxth=0;

                  }

                  while(aux_ma!=0){
                        printf("%d  %s  %d  --------------TOBEDONE----------\n\n\n", aux_ma, place_in_analysis, months_MA);

                        MovingAverage(aux_ma, place_in_analysis, extremes_dates, extremes_cities, extremes_countries, months_MA);

                        aux_ma=0;

                  }


	      }
      }
      else if (mode == GRAPHICAL){
            //initialize graphics
            InitEverything(width, height, &serif, &sans, imgs, &window, &renderer);

            while( quit == 0 ){
            // while there's events to handle
            while( SDL_PollEvent( &event ) )
            {
                  if( event.type == SDL_QUIT )
                  {
                        quit = 1;
                  }
                  else if ( event.type == SDL_KEYDOWN )
                  {
                        switch ( event.key.keysym.sym ){
                              case SDLK_n:
                              // todo
                              case SDLK_q:
                                    quit = 1;
                              case SDLK_u:
                              // todo
                              default:
                                    break;
                }      i++; // for the last one
                  }
            }
            // render game table
           square_size_px = RenderTable( board_pos_x, board_pos_y, board_size_px, serif, imgs, renderer, board, moves);

           RenderPoints(board, pixel_coord_cities, temp_cities, board_size_px, square_size_px, renderer, number_of_cities);
           //render in the screen all changes above
           SDL_RenderPresent(renderer);

           // add a delay
           SDL_Delay( delay );
           }
      }
      else{
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Even after input check, fell in a undefined mode.");
            exit(EXIT_FAILURE);
      }

	//PrintList(extremes_countries->tail, COUNTRIES);
	//PrintList(extremes_cities->tail, CITIES);
	//CountCities(extremes_cities->tail);

	freeList(extremes_cities->head);
	freeList(extremes_countries->head);
	for (i=0; i<number_of_cities; i++){
	     free(cities_names[i]);
	}
      free(cities_names);
	free(pixel_coord_cities[0]);
      free(pixel_coord_cities[1]);
	free(temp_cities[0]);
	free(extremes_cities);
	free(extremes_countries);


      printf("Reached EOP\n");
	return EXIT_SUCCESS;
}
