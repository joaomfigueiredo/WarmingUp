#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "lists.h"


void LoadTempCountries(char file_countries[FILENAME_SIZE],list_t* extremes_countries, int extremes_dates[4], int mode ){
      int filetype=COUNTRIES;
      char buffer[BUFFER_SIZE]={0};
      data_temp_t *aux = NULL;
      data_temp_t *aux_csv_line=NULL;

      FILE* csv_countries = fopen(file_countries, "r");

      if (csv_countries==0){
            fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "OPENING COUNTRIES FILE\n");
            exit (0);
      }

      extremes_countries->head = NULL;
      extremes_countries->tail = NULL;
      extremes_countries->root = NULL;

      aux_csv_line = (data_temp_t*)malloc(sizeof(data_temp_t));
      if (aux_csv_line == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
            exit(EXIT_FAILURE);
      }

      while (NULL != fgets(buffer,BUFFER_SIZE,csv_countries)) {
            aux = CsvToStruct(buffer, aux_csv_line, filetype, mode);
            if (aux->temperature==ERRORCODE) continue;
            TreeLoader(*aux, extremes_countries);
       }

       TreetoList(extremes_countries->root, extremes_countries);

       extremes_dates[0]=extremes_countries->head->payload.ordering_identifier;
       extremes_dates[1]=extremes_countries->tail->payload.ordering_identifier;

       free(aux_csv_line);

       fclose(csv_countries);
}

void LoadTempCities(char file_cities[FILENAME_SIZE],list_t* extremes_cities, int extremes_dates[4],int mode){
      int filetype=CITIES;
      char buffer[BUFFER_SIZE]={0};
      data_temp_t *aux = NULL;
      data_temp_t *aux_csv_line=NULL;

      FILE* csv_cities = fopen(file_cities, "r");

      if (csv_cities==0){
            fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "OPENING CITIES FILE\n");
            exit (0);
      }

      extremes_cities->head = NULL;
      extremes_cities->tail = NULL;
      extremes_cities->root = NULL;

      aux_csv_line = (data_temp_t*)malloc(sizeof(data_temp_t));
      if (aux_csv_line == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
            exit(EXIT_FAILURE);
      }

      while (NULL != fgets(buffer,BUFFER_SIZE,csv_cities)) {
            aux = CsvToStruct(buffer, aux_csv_line, filetype, mode);
            if (aux->temperature==ERRORCODE) continue;
            TreeLoader(*aux, extremes_cities);
            if ( ((aux->dt.year*10000)+(aux->dt.month*100)+(aux->dt.day)) > extremes_dates[3] )
                  extremes_dates[3]=((aux->dt.year*10000)+(aux->dt.month*100)+(aux->dt.day));
            if (( ((aux->dt.year*10000)+(aux->dt.month*100)+(aux->dt.day)) < extremes_dates[2] ) || extremes_dates[2]==0)
                  extremes_dates[2]=((aux->dt.year*10000)+(aux->dt.month*100)+(aux->dt.day));
      }

       TreetoList(extremes_cities->root, extremes_cities);

       free(aux_csv_line);
       fclose(csv_cities);
}

data_temp_t* CsvToStruct(char *_buffer, data_temp_t* _aux_csv_line, int _filetype, int mode){
      char *aux=NULL;
      int aux_01=0;
            aux = strtok(_buffer, ",");

              if (atof(aux)==0){
                  _aux_csv_line->temperature = ERRORCODE;
                  return _aux_csv_line;
              }

              sscanf(aux, "%d-%d-%d", &_aux_csv_line->dt.year,&_aux_csv_line->dt.month,&_aux_csv_line->dt.day);
              _aux_csv_line->ordering_identifier=10000*_aux_csv_line->dt.year+100*_aux_csv_line->dt.month+_aux_csv_line->dt.day;

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

              if(_filetype==COUNTRIES){
                    aux = strtok(NULL, "\n");
                    strcpy(_aux_csv_line->country, aux);
                    return _aux_csv_line;
              }
              else{
                    aux = strtok(NULL, ",");
                    strcpy(_aux_csv_line->city, aux);

                    aux = strtok(NULL, ",");
                    strcpy(_aux_csv_line->country, aux);

                    aux = strtok(NULL, ",");
                    sscanf(aux,"%f%c", &_aux_csv_line->lat.angle,  &_aux_csv_line->lat.hemisphere);
                    aux = strtok(NULL, "\n");

                    aux_01=strlen(aux);
                    switch (aux_01) {
                          case 6:
                              sscanf(aux,"%4f%c", &_aux_csv_line->longit.angle,  &_aux_csv_line->longit.hemisphere);
                              break;
                          case 7:
                              sscanf(aux,"%5f%c",  &_aux_csv_line->longit.angle,  &_aux_csv_line->longit.hemisphere);
                              break;
                          case 8:
                              sscanf(aux,"%6f%c", &_aux_csv_line->longit.angle,  &_aux_csv_line->longit.hemisphere);
                              break;
                          default:
                                  break;
                    }

                   if (_aux_csv_line->lat.hemisphere=='N') {
                        if(_aux_csv_line->longit.hemisphere=='W')
                              _aux_csv_line->ordering_identifier=(-_aux_csv_line->longit.angle+_aux_csv_line->lat.angle);
                        else
                              _aux_csv_line->ordering_identifier=(_aux_csv_line->longit.angle+_aux_csv_line->lat.angle);
                   }
                   else{
                        if(_aux_csv_line->longit.hemisphere=='W')
                              _aux_csv_line->ordering_identifier=(-_aux_csv_line->longit.angle-_aux_csv_line->lat.angle);
                        else
                              _aux_csv_line->ordering_identifier=(_aux_csv_line->longit.angle-_aux_csv_line->lat.angle);
                  }
               }



      return _aux_csv_line;
}

void PrintNode(data_temp_t aux){
      printf("%d-%d-%d",aux.dt.year, aux.dt.month, aux.dt.day );
      printf("_______TEMP - %f", aux.temperature );
      printf("_______UNC - %f", aux.uncertainty );
      printf("_____%s\n", aux.country);
}

void PrintCompleteNode(node_t aux, int filetype){
      printf("OI %d,", aux.payload.ordering_identifier);
      printf("%d-%d-%d,",aux.payload.dt.year, aux.payload.dt.month, aux.payload.dt.day );
      printf("%f,", aux.payload.temperature );
      printf("%f,", aux.payload.uncertainty );
      printf("%s", aux.payload.country);

      if(filetype==CITIES){
            printf(",%s,", aux.payload.city);
            printf("%f,%c,", aux.payload.lat.angle,  aux.payload.lat.hemisphere);
            printf("LONGIT,%f,%c", aux.payload.longit.angle,  aux.payload.longit.hemisphere);
      }

      printf("\n");

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
            if (_newNode->payload.ordering_identifier < curr->payload.ordering_identifier){
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

void PrintList(node_t *extreme, int filetype){
      node_t *aux = extreme;
      int i=0;
      //prev-tail-decreasing / next-head-increasing
      while(aux != NULL){
            PrintCompleteNode(*aux, filetype);
            aux=aux->prev;
            i++;
      }

      printf("TOTAL_____%d\n", i );
}

int CountCities(node_t *_head){
      node_t *aux = _head;
      int i=0;
      while(aux->next != NULL){
            if (strcmp(aux->payload.city, aux->next->payload.city)!=0){
                  i++;
            }
            aux=aux->next;
      }
      i++; // for the last one
      return i;
}

void freeList(node_t *_head){
      node_t *aux = _head;

      while(aux->next!= NULL){
            aux=aux->next;
            free(aux->prev);
      }
      free(aux);
}

void ConditionalNodeDeleter(list_t *extreme, int filetype, int months_interval[2], int starting_yearmonth[2] , int extremes_dates[4]){
      node_t *aux=NULL, *next_aux=NULL;
      int minimum_date = starting_yearmonth[0]*10000+starting_yearmonth[1]*100;

      switch (filetype) {
            case COUNTRIES:
                  //to limit lowest date
                  aux=extreme->head;
                  while(aux->payload.ordering_identifier<minimum_date){
                        aux=aux->next;
                        extreme->head=aux;
                        free(aux->prev);
                        extreme->head->prev=NULL;
                  }
                  extremes_dates[0]=extreme->head->payload.ordering_identifier;
                  extremes_dates[1]=extreme->tail->payload.ordering_identifier;
                  //date interval
                  aux=extreme->head;
                  if(months_interval[1]==0) break;
                  while(aux->next!=NULL){
                        if ((months_interval[0]-aux->payload.dt.month)<=0 && (months_interval[1]-aux->payload.dt.month)>=0){
                              aux=aux->next;
                        }
                        else{
                              if(aux==extreme->head){
                                    aux->next->prev=NULL;
                                    extreme->head=aux->next;
                                    extreme->head->prev=NULL;
                              }
                              else if(aux==extreme->tail){
                                    aux->prev->next=NULL;
                                    extreme->tail=aux->prev;
                                    printf("TAIL");
                              }
                              else{
                                    aux->prev->next=aux->next;
                                    aux->next->prev=aux->prev;
                              }
                              next_aux=aux->next;
                              free(aux);
                              aux=next_aux;
                        }
                  }
                  extremes_dates[0]=extreme->head->payload.ordering_identifier;
                  extremes_dates[1]=extreme->tail->payload.ordering_identifier;
                  break;
            case CITIES:
                  aux=extreme->head;
                  while(aux!=NULL){
                              if (((aux->payload.dt.year*10000)+(aux->payload.dt.month*100)+(aux->payload.dt.day))>=minimum_date){
                                    aux=aux->next;
                              }
                              else{
                                    if(aux==extreme->head){
                                          aux->next->prev=NULL;
                                          extreme->head=aux->next;
                                          extreme->head->prev=NULL;
                                    }
                                    else if(aux==extreme->tail){
                                          aux->prev->next=NULL;
                                          extreme->tail=aux->prev;
                                          printf("TAIL");
                                    }
                                    else{
                                          aux->prev->next=aux->next;
                                          aux->next->prev=aux->prev;
                                    }
                                    next_aux=aux->next;
                                    free(aux);
                                    aux=next_aux;
                              }
                  }
                  extremes_dates[2]=minimum_date;
                  if(months_interval[1]==0) break;
                  aux=extreme->head;
                  while(aux!=NULL){
                              if (((months_interval[0]-aux->payload.dt.month)<=0 && (months_interval[1]-aux->payload.dt.month)>=0)){
                                    aux=aux->next;
                              }
                              else{
                                    if(aux==extreme->head){
                                          aux->next->prev=NULL;
                                          extreme->head=aux->next;
                                          extreme->head->prev=NULL;
                                    }
                                    else if(aux==extreme->tail){
                                          aux->prev->next=NULL;
                                          extreme->tail=aux->prev;
                                    }
                                    else{
                                          aux->prev->next=aux->next;
                                          aux->next->prev=aux->prev;
                                    }
                                    next_aux=aux->next;
                                    free(aux);
                                    aux=next_aux;
                              }
                  }
                  break;
            default:
                  break;
      }

}
