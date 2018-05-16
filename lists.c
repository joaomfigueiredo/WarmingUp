#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "lists.h"

void LoadTempCountries(char file_countries[FILENAME_SIZE]){ //carrega do ficheiro começando numa linha, depois diz q o aux = ao que a funçao
      list_t* extremes_countries = NULL;
      char buffer[BUFFER_SIZE]={0};                         //countriescsvto truck retorna que é a linha num tipo de estrutura e vai continuan
      data_temp_t* aux = NULL;     // ate fazer as linhas todas

      FILE* csv_countries = fopen(file_countries, "r");

      extremes_countries = (list_t*)malloc(sizeof(list_t));
      extremes_countries->head = NULL;


      if (csv_countries==0){
            fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "OPENING FILE\n");
            exit (0);
      }

      while (NULL != fgets(buffer,BUFFER_SIZE,csv_countries)) {
            aux = CountriesCsvToStruct(buffer);
            if (aux->temperature==ERRORCODE) continue;
            //GenNewNode(*aux);
            insertionSort(*aux, extremes_countries);
      //      PrintNode(*aux);
       }

       TreetoList(extremes_countries->head);

      fclose(csv_countries);
}

data_temp_t* CountriesCsvToStruct(char *_buffer){ //define cada linha do txt no tipo de estrutura data_temp_t
      char *aux=NULL;
      data_temp_t *aux_1=NULL;

              aux_1 = (data_temp_t*)malloc(sizeof(data_temp_t));

              aux = strtok(_buffer, ",");
              if (atof(aux)==0){
                  aux_1->temperature = ERRORCODE;
                  return aux_1;
              }
              sscanf(aux, "%d-%d-%d", &aux_1->dt.year,&aux_1->dt.month,&aux_1->dt.day);
              aux_1->concatenated_date=10000*aux_1->dt.year+100*aux_1->dt.month+aux_1->dt.day;
              aux = strtok(NULL, ",");
              if (atof(aux)==0){
                  aux_1->temperature = ERRORCODE;
                  return aux_1;
              }
              aux_1->temperature = atof(aux);
              aux = strtok(NULL, ",");
              if (atof(aux)==0){
                  aux_1->temperature = ERRORCODE;
                  return aux_1;
              }
	            aux_1->uncertainty = atof(aux);
              aux = strtok(NULL, "\n");
              strcpy(aux_1->country, aux);
            //DEVE SER LIBERTADA?? AIND ANAO FI GUARDADO O VALOR
            //  free(aux_1);

      return aux_1;



}

void PrintNode(data_temp_t aux){ //so pa imprimir cada um por data
      printf("%d-%d-%d",aux.dt.year, aux.dt.month, aux.dt.day );
      printf("_______TEMP - %f", aux.temperature );
      printf("_______UNC - %f", aux.uncertainty );
      printf("_____%s\n", aux.country);
}

void PrintCompleteNode(node_t aux){ //so pa imprimir cada um por data
      printf("%d-%d-%d",aux.payload.dt.year, aux.payload.dt.month, aux.payload.dt.day );
      printf("_______TEMP - %f", aux.payload.temperature );
      printf("_______UNC - %f", aux.payload.uncertainty );
      printf("_____%s\n", aux.payload.country);
}

node_t* GenNewNode(data_temp_t _aux){
      node_t *newNode=NULL;
      //struct node_t* head;
      //struct node_t* tail;

      newNode = (node_t*)malloc(sizeof(node_t));

      if (newNode == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
            exit(EXIT_FAILURE);
      }

      newNode-> payload = _aux;
      newNode-> next = NULL;
      newNode-> prev = NULL;

      return newNode;
}


void sortedInsert(list_t *extremes_countries, node_t *_newNode){
      node_t *curr = NULL;
      node_t *prev = NULL;
      //se a lista estiver vazia
      // ou se for necessário inserir antes do 1º elemento da lista
      //printf("________%d__________\n", ((extremes_countries)->head==NULL  ));
      //PrintCompleteNode(*_newNode);

      curr = extremes_countries->head;

      if(curr == NULL){
        extremes_countries->head = _newNode;
        return;
      }


      while (curr != NULL){

            if (_newNode->payload.concatenated_date < curr->payload.concatenated_date){
              prev = curr;
              curr = curr->prev;
            }else{
              prev = curr;
              curr = curr->next;
            }
      }

      if (_newNode->payload.concatenated_date < prev->payload.concatenated_date){
          prev->prev = _newNode;
      }else{
        prev->next = _newNode;
      }


      _newNode->next = curr;
}


void insertionSort(data_temp_t newNodeDATA, list_t *extremes_countries){
      node_t *newNode = NULL;

      newNode=GenNewNode(newNodeDATA);

      sortedInsert(extremes_countries, newNode);


}

void TreetoList(node_t* root){
      if(root->prev!=NULL)    TreetoList(root->prev);

      PrintCompleteNode(*root);


      if(root->next!=NULL) TreetoList(root->next);



}
