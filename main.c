//WARMING UP
//INSTITUTO SUPERIOR TECNICO
//PROGRAMAÇÃO
//professor: João Ascenso
//realizado por:
//Alexandre Silva     90004
//João Figueiredo     90108
//
//Breves notas sobre o projeto:
//O nosso projeto realiza, excluindo as funcionalidades gráficas avançadas, as operações
//requisitadas sobre os dados fornecidos. Para carregarmos o ficheiro do paises ordenadamente
//(em tempo util) recorremos a uma organização dos valores numa árvore binária. Esta é,
//imediatemente após servir como ferramenta de sort,  "transformada" numa lista ordenada
//Atendemos assim ao requerido no enunciado e facilitamos as posteriores operaçoes.
//O ficheiro das cidades, para garantir que os valores de cada cidade ficam agrupados
//também é carregado com recurso á mesma ferramenta
//Para fazer comparações entre datas, optamos por concatenar ano/mes/dia. À partida
//parecia simplificar e acelerar as comparações as ao longo do projeto concluimos que o
//benificio não foi grande.
//


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


int main(int argc, char *argv[]){
 //------------------------INITIALIZATIONS-----------------------------------//
      int i=0;
      char c[10];

      char files[2][FILENAME_SIZE]={{0}};//name off csv files
      int mode = 0;//#define TEXTUAL 1 <----OR---> #define GRAPHICAL 2

      int aux_quit = 0, auxth = 0, aux_df = 0, aux_ma=0, auxyta=0; //store user selections in menus
      int T = 0; //period for temperature
      int year_in_analysis = 0;//year being studied in 3.
      int months_MA=0;//num of months used in moving average 4.
      char place_in_analysis[BUFFER_SIZE]={0};

      int starting_yearmonth[2] = {0}; //data_filtering
      int months_interval[2] = {0}; //data_filtering

      list_t* extremes_countries = NULL;//list of COUNTRIES loaded from csv
      list_t* extremes_cities = NULL;//list of CITIES loaded from csv
      int extremes_dates[4]={0};//stores in 0 and 1 coutires minimum_date and maximum date and in 2 and 3 about CITIES


      int square_size_px = 0;
      int board_size_px[2] = {0};
      int pt_x = 0;
      int pt_y = 0;
      int number_of_cities=0, *pixel_coord_cities[2];
      char **cities_names;
      float *temp_cities[1];

      int quit = 0;
      int width = (TABLE_SIZE + LEFT_BAR_SIZE);
      int height = TABLE_SIZE;
      SDL_Window *window = NULL;
      SDL_Renderer *renderer = NULL;
      TTF_Font *serif = NULL;
      TTF_Font *sans = NULL;
      TTF_Font *segment = NULL;
      SDL_Surface *imgs[4];
      SDL_Event event;

//----------------END OF INITIALIZATIONS------------------//

      mode = ParamReading(argc, argv, files);

//-----------------ALLOCS-----------------------//
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

//-----------------END OF ALLOCS-----------------------//

      if (mode==TEXTUAL){
	      while(aux_quit == 0){

                  MenuSurfer(&T, &year_in_analysis, &aux_df, &aux_quit, &auxth, &aux_ma, &auxyta, &months_MA, place_in_analysis);

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
                        MovingAverage(aux_ma, place_in_analysis, extremes_dates, extremes_cities, extremes_countries, months_MA);
                        aux_ma=0;
                  }

                  while(auxyta!=0){
                        YearAnalysis(auxyta, year_in_analysis,extremes_dates,extremes_cities,extremes_countries);
                        auxyta=0;
                  }

                  while(1){
				printf("Continue(Y/n):");
                        fgets(c,10,stdin);
				if (c[0]=='Y'||c[0]=='y'){
	                        break;
				}
				else if(c[0]=='n'||c[0]=='N') {
					aux_quit=1;
					break;
				}
				else{}
			}

	      }
      }
      else if (mode == GRAPHICAL){
            CityCoordinateCalculator(extremes_cities->head, &pt_x, &pt_y,pixel_coord_cities, number_of_cities,cities_names);
            //initialize graphics
            InitEverything(width, height, &serif, &sans, &segment, imgs, &window, &renderer);

		RenderTable( board_size_px, serif, imgs, renderer);
		SDL_RenderPresent(renderer);
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
                              case SDLK_1:
                              RenderPoints(segment,extremes_cities, pixel_coord_cities, board_size_px, square_size_px, renderer, number_of_cities, cities_names, extremes_dates);
                              SDL_RenderClear(renderer);
                              RenderTable( board_size_px, serif, imgs, renderer);
                              SDL_RenderPresent(renderer);
                              break;

                              case SDLK_9:
                                    quit = 1;

                                    break;

                              default:
                                    break;
                }
                  }
            }

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
