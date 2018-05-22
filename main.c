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
      int i=0, j=0;

      int aux_ms = 0; // 0- parametrizar as funçoes||||| 1-executando e voltando
      int mode = 0;
	int T = 0;
      int auxth = 0;
      int ano = 0;
	int months = 0;
      int aux_df = 0; //aux do data filter ( ta ativo a 1 2 ou 3 se estiver no menu datafilter)

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
      int number_of_cities=NULL, *pixel_coord_cities[2];
      char *cities_names[BUFFER_SIZE];
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

    for ( i=0; i<number_of_cities; i++){
          cities_names[i]=calloc(number_of_cities,BUFFER_SIZE*sizeof(char));
    }


      CityCoordinateCalculator(extremes_cities->head, &pt_x, &pt_y,pixel_coord_cities, number_of_cities, temp_cities,cities_names);
   /*   for(i=0; i<number_of_cities; i++){
            printf("%s\n", cities_names[i] );
            printf("%d____%d\n", pixel_coord_cities[0][i], pixel_coord_cities[1][i] );
      }
	*/

    printf("%d  %d   %d   %d", extremes_dates[0],extremes_dates[1],extremes_dates[2],extremes_dates[3]);

      if (mode==TEXTUAL){
	      while(1){
                  if ( (aux_ms == 1)) break;

                  MenuSurfer(&T, &ano, &months, &aux_df, &aux_ms, &auxth, place_in_analysis);
                  printf("T___%d__auxth____%d____%s__\n",T, auxth, place_in_analysis );

                  while (aux_df != 0){
                        TreatmentDataFilter(&aux_df, starting_yearmonth, months_interval);
                        ConditionalNodeDeleter(extremes_countries, COUNTRIES,months_interval,starting_yearmonth, extremes_dates);
                        ConditionalNodeDeleter(extremes_cities, CITIES,months_interval,starting_yearmonth, extremes_dates);
                        printf("%s\n", "DELETEEEEED");
                        PrintCompleteNode(*extremes_countries->head, COUNTRIES);
                        aux_df=0;
                  }
                  while (auxth!=0){

                        TreatmentTemperatureHistory(extremes_countries, T, auxth, place_in_analysis, extremes_dates);
                        auxth=0;
                        printf("SAIUUUU\n" );
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
                }
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

	printf("Temp %d\n\n", T);
	printf("ano %d\n\n", ano);
	printf("meses %d\n\n", months);
	
	
		//PrintList(extremes_countries->tail, COUNTRIES);
	//PrintList(extremes_cities->tail, CITIES);
	CountCities(extremes_cities->tail);
	
	
	freeList(extremes_cities->head);
	printf("%s\n","cidades freed" );
	freeList(extremes_countries->head);
	printf("%s\n","paises freed" );
		
	//free(cities_names);
	// for (i=0; i<number_of_cities; i++){
		// free(++cities_names[i]);
	// }


	printf("%s\n", "nomes das cidades freed" );
	free(pixel_coord_cities[0]);
	free(pixel_coord_cities[1]);
	free(temp_cities[0]);
	free(extremes_cities);
	free(extremes_countries);
	return EXIT_SUCCESS;
}
