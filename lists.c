#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "lists.h"


void LoadTempCountries(){
      int num_lines=0, ch, a=0;
      char buffer[BUFFER_SIZE]={0};
      data_temp *country = NULL;
      FILE* csv_countries = fopen("tempcountries.csv", "r");

      if (csv_countries==0){
            fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "OPENING FILE\n");
            exit (0);
      }

      while (EOF != fscanf(csv_countries, "%1000[^\n]\n", buffer)) {
            CountriesCsvToStruct(buffer, country, &a);
            //a++;
            if (a>100) exit (0);
      }

      fclose(csv_countries);
}

/*
node_t* GetNewNode(char *_buffer){
      node_t *newNode=NULL;

      newNode = (node_t*)malloc(sizeof(node_t));
      if (newNode == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
            exit(EXIT_FAILURE);
      }
      newNode-> payload=CountriesCsvToStruct(*_buffer);
      newNode-> next = NULL;
      newNode-> prev = NULL;
      return newNode;
}
*/

void CountriesCsvToStruct(char *_buffer, data_temp *country, int *_a){
      char *aux=NULL;

              aux = strtok(_buffer, "-");
              if (atoi(aux)==0) return;
              //printf("  %d-", atoi(aux));
              aux = strtok(NULL, "-");
              //printf("%d-", atoi(aux));
              aux = strtok(NULL, ",");
             // printf("%d -----", atoi(aux));
              aux = strtok(NULL, ",");
                    if (atof(aux)==0) {
                          *_a=*_a+1;
                          printf("EMPTY _ %d\n",*_a);



            //  printf("temp %f  ", atof(aux));
              aux = strtok(NULL, ",");
            //  printf("unc %f  ", atof(aux));
              aux = strtok(NULL, "\n");
              printf(" %s \n", (aux));

      }
      printf(" %s \n", (aux));

//      return country[0];
}
