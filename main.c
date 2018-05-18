#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"
#include "lists.h"
#include "data_treatment.h"

int main(int argc, char *argv[]){

	  int stoppingone = 0; //variavel de confirmaçao para o menu textual
      int aux_ms = 0; // 0- parametrizar as funçoes||||| 1-executando e voltando
      int mode = 0;
      int T = 0;
      int ano = 0;
      int months = 0;
      int criteria = 0; //variavel que criteria se existe algum criterio ou nao xD (so pa quando for exec)
      int aux_df = 0; //aux do data filter ( ta ativo a 1 2 ou 3 se estiver no menu datafilter)
      int startingpoint[2] = {0}; //vetor que guarda, nesta ordem, o ano e o mês em que o estudo começa
      int monthspan[2] = {0};
      char files[2][FILENAME_SIZE]={{0}};

      list_t* extremes_countries = NULL;
      list_t* extremes_cities = NULL;

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
      LoadTempCountries(files[0], extremes_countries);
      fprintf(stderr,ANSI_COLOR_BOLD_GREEN "\rCOUNTRIES ARE NOW LOADED AND SORTED\n" ANSI_COLOR_RESET);




      fprintf(stderr,"LOADING AND SORTING CITIES");
      LoadTempCities(files[1], extremes_cities);
      fprintf(stderr,ANSI_COLOR_BOLD_GREEN "\rCITIES ARE NOW LOADED AND SORTED" ANSI_COLOR_RESET);

      if (mode==TEXTUAL){
	   while(1){
                if ( (aux_ms == 1) && (stoppingone == 1)) break;
                stoppingone = 1;
                MenuSurfer(&T, &ano, &months, &aux_df, &aux_ms);
                if ( (aux_df == 1) || (aux_df == 2) || (aux_df == 3)){
                     TreatmentDataFilter(&criteria, &aux_df, &startingpoint[2], &monthspan[2]);
                }
          }

	  }

      printf("Temp %d\n\n", T);
      printf("ano %d\n\n", ano);
      printf("meses %d\n\n", months);

      PrintList(extremes_countries->tail, COUNTRIES);
      PrintList(extremes_cities->tail, CITIES);
      CountCities(extremes_cities->tail);

      freeList(extremes_cities->head);
      freeList(extremes_countries->head);
      free(extremes_cities);
      free(extremes_countries);
      return EXIT_SUCCESS;
}
