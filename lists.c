#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "lists.h"


void LoadTempCountries(char file_countries[FILENAME_SIZE]){
      char buffer[BUFFER_SIZE]={0};
      data_temp_t* aux = NULL;
      FILE* csv_countries = fopen(file_countries, "r");

      if (csv_countries==0){
            fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "OPENING FILE\n");
            exit (0);
      }

      while (EOF != fscanf(csv_countries, "%1000[^\n]\n", buffer)) {
            aux = CountriesCsvToStruct(buffer);


      }
      PrintNode(aux->payload);
      fclose(csv_countries);
}


node_t* GetNewNode(data_temp_t _aux){
      node_t *newNode=NULL;struct node_t* head;
      struct node_t* tail;


      newNode = (node_t*)malloc(sizeof(node_t));
      if (newNode == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
            exit(EXIT_FAILURE);
      }
      newNode-> payload=_aux;
      newNode-> next = NULL;
      newNode-> prev = NULL;
      return newNode;
}


data_temp_t* CountriesCsvToStruct(char *_buffer){
      char *aux=NULL;
      data_temp_t *aux_1=NULL;

              aux_1= (node_t*)malloc(sizeof(node_t));

              aux = strtok(_buffer, ",");
              sscanf(aux, "%d-%d-%d", &aux_1->dt.year,&aux_1->dt.month,&aux_1->dt.day);
              aux = strtok(NULL, ",");
              if (atof(aux)==0)   return 0;
              aux_1->temperature = atof(aux);
              aux = strtok(NULL, ",");
              if (atof(aux)==0)   return 0;
	        aux_1->uncertainty = atof(aux);
              aux = strtok(NULL, "\n");
              strcpy(aux_1->country, aux);

      return aux_1;
}

void PrintNode(data_temp_t aux){
      printf("%d-%d-%d\n",aux.dt.year, aux.dt.month, aux.dt.day );
}
