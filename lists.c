#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "lists.h"

void LoadTempCountries(char file_countries[FILENAME_SIZE]){
      list_t* extremes_countries = NULL;
      char buffer[BUFFER_SIZE]={0};
      data_temp_t *aux = NULL;
      data_temp_t *aux_csv_line=NULL;

      FILE* csv_countries = fopen(file_countries, "r");

      if (csv_countries==0){
            fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "OPENING FILE\n");
            exit (0);
      }

      extremes_countries = (list_t*)malloc(sizeof(list_t));
      extremes_countries->head = NULL;
      extremes_countries->tail = NULL;
      extremes_countries->root = NULL;
      aux_csv_line = (data_temp_t*)malloc(sizeof(data_temp_t));

      while (NULL != fgets(buffer,BUFFER_SIZE,csv_countries)) {
            aux = CountriesCsvToStruct(buffer, aux_csv_line);
            if (aux->temperature==ERRORCODE) continue;
            TreeLoader(*aux, extremes_countries);
       }

       TreetoList(extremes_countries->root, extremes_countries);

       PrintList(extremes_countries->head);

       free(aux_csv_line);
       free(extremes_countries);
       fclose(csv_countries);
}

data_temp_t* CountriesCsvToStruct(char *_buffer, data_temp_t* _aux_csv_line){
      char *aux=NULL;

              aux = strtok(_buffer, ",");
              if (atof(aux)==0){
                  _aux_csv_line->temperature = ERRORCODE;
                  return _aux_csv_line;
              }
              sscanf(aux, "%d-%d-%d", &_aux_csv_line->dt.year,&_aux_csv_line->dt.month,&_aux_csv_line->dt.day);
              _aux_csv_line->concatenated_date=10000*_aux_csv_line->dt.year+100*_aux_csv_line->dt.month+_aux_csv_line->dt.day;
              aux = strtok(NULL, ",");
              if (atof(aux)==0){
                  _aux_csv_line->temperature = ERRORCODE;
                  return _aux_csv_line;
              }
              _aux_csv_line->temperature = atof(aux);
              aux = strtok(NULL, ",");
              if (atof(aux)==0){
                  _aux_csv_line->temperature = ERRORCODE;
                  return _aux_csv_line;
              }
	            _aux_csv_line->uncertainty = atof(aux);
              aux = strtok(NULL, "\n");
              strcpy(_aux_csv_line->country, aux);

      return _aux_csv_line;

}

void PrintNode(data_temp_t aux){
      printf("%d-%d-%d",aux.dt.year, aux.dt.month, aux.dt.day );
      printf("_______TEMP - %f", aux.temperature );
      printf("_______UNC - %f", aux.uncertainty );
      printf("_____%s\n", aux.country);
}

void PrintCompleteNode(node_t aux){
      printf("%d-%d-%d",aux.payload.dt.year, aux.payload.dt.month, aux.payload.dt.day );
      printf("_______TEMP - %f", aux.payload.temperature );
      printf("_______UNC - %f", aux.payload.uncertainty );
      printf("_____%s\n", aux.payload.country);
}

tree_node_t* NewTreeNode(data_temp_t _aux){
      tree_node_t *newNode=NULL;

      newNode = (tree_node_t*)malloc(sizeof(tree_node_t));

      if (newNode == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
            exit(EXIT_FAILURE);
      }

      newNode-> payload = _aux;
      newNode-> right = NULL;
      newNode-> left= NULL;

      return newNode;
}

node_t* NewListNode(data_temp_t _aux){
      node_t *newNode=NULL;

      newNode = (node_t*)malloc(sizeof(node_t));

      if (newNode == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
            exit(EXIT_FAILURE);
      }

      newNode-> payload =_aux;
      newNode-> next = NULL;
      newNode-> prev = NULL;

      return newNode;
}

void TreeBuilder(list_t *extremes_countries, tree_node_t *_newNode){
      tree_node_t *curr = NULL;

      curr = extremes_countries->root;

      if(curr == NULL){
        extremes_countries->root = _newNode;
        return;
      }

      while (curr != NULL){
            if (_newNode->payload.concatenated_date < curr->payload.concatenated_date){
                  if(curr->left==NULL){
                       curr->left= _newNode;
                       return;
                  }
                  curr = curr->left;
            }
            else{
                  if(curr->right==NULL){
                        curr->right = _newNode;
                        return;
                  }
                  curr = curr->right;
            }
      }
}

void TreeLoader(data_temp_t newNodeDATA, list_t *extremes_countries){
      tree_node_t *newNode = NULL;

      newNode=NewTreeNode(newNodeDATA);
      TreeBuilder(extremes_countries, newNode);
}

void TreetoList(tree_node_t* root, list_t* _extremes_countries){
      if(root->left!=NULL)    TreetoList(root->left,_extremes_countries);

      insertListTail_fromtree(root, _extremes_countries);

      if(root->right!=NULL) TreetoList(root->right,_extremes_countries);
      free(root);
}

void insertListTail_fromtree(tree_node_t* node_to_insert, list_t* _extremes_countries){
      node_t *newNode = NULL;

      newNode=NewListNode(node_to_insert->payload);

      if (_extremes_countries->head==NULL){
            _extremes_countries->head=newNode;
            _extremes_countries->tail=newNode;
      }
      else{
            _extremes_countries->tail->next=newNode;
            newNode->prev=_extremes_countries->tail;
            _extremes_countries->tail=newNode;
      }
}

void PrintList(node_t *_head){
      node_t *aux = _head;

      while(aux->next != NULL){
            PrintCompleteNode(*aux);
            aux=aux->next;
            free(aux->prev);
      }

      free(aux);
}
