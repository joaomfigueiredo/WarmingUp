#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"
#include "lists.h"

int main(int argc, char *argv[]){

      int mode = 0;
      int T = 0;
      int ano = 0;
      int months = 0;
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

      if (mode==TEXTUAL) MenuSurfer(&T, &ano, &months);

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
