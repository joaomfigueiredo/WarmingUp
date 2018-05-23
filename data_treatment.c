#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "data_treatment.h"
#include "lists.h"


void TreatmentDataFilter(int* aux_df, int starting_yearmonth[2], int monthspan[2]){
     int i = 0;
     char buffer[BUFFER_SIZE] = {0};
     char *request_df[2] = {"ano", "mês"};

     if (*aux_df == 1){
          for (i = 0; i < 2; i++){
               starting_yearmonth[i] = 0;
               monthspan[i] = 0;
          }
           printf("Os critérios foram limpos.");
     }

     if (*aux_df == 2){
          for (i = 0; i < 2; i++){
               printf("A partir de que %s quer analisar os dados: ", request_df[i]);
               while(1){
                     if(fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

                     if (i==0){
                           if((sscanf(buffer,"%d",&starting_yearmonth[i])==1)&&(1600<starting_yearmonth[i]&&starting_yearmonth[i]<2014))
                              break;
                           else
                              printf("Introduza um valor entre 1600 e 2014: ");
                     }
                     else if (i==1){
                           if((sscanf(buffer,"%d",&starting_yearmonth[i])==1)&&(0<starting_yearmonth[i]&&starting_yearmonth[i]<13))
                              break;
                           else
                              printf("Introduza um valor entre 1 e 12: ");
                     }
               }
          }
     }

     if (*aux_df == 3){

               printf("Que intervalo de meses pertende analisar em cada ano( [mes1][espaço][mes2] ): ");
               while (1) {
                    if (fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

                    if ((sscanf(buffer," %d %d",&monthspan[0], &monthspan[1])==2)&&(monthspan[0]<=monthspan[1]))
                       break;
                    else
                       (printf("Verifique o formato dos dados introduzidos: "));
               }
      }

}

void PrintTH(list_th_t *extreme,char place_in_analysis[BUFFER_SIZE]){
      node_th_t *aux = extreme->head;
      int i=0;
      char c=0;

      if(extreme->head->payload.num_of_val==0 && extreme->tail->payload.num_of_val==0){
		printf("Não foram encontrados dados sobre %s que permitissem a análise requerida", place_in_analysis);
		return;
	}

	printf("_____________________%s_____________________\nInicio periodo, Fim periodo, temp max, temp min, media\n",place_in_analysis );

      while(aux->next!= NULL){

            printf("[%d,%d[",aux->payload.begin_period/10000, aux->payload.end_period/10000);
            if (aux->payload.average==0){
                  printf("      NO DATA AVAILABLE\n");
            }
            else printf(",%f,%f,%f\n", (aux->payload.maximum_temp),(aux->payload.minimum_temp), (aux->payload.average/aux->payload.num_of_val));
            // RevertConcatenation(aux->payload.begin_period);
            aux=aux->next;
            i++;
            if (i==20){
			while(1){
				printf("Introduz 'a' para ver mais 20 ou 'q' para sair: ");
				scanf("%c", &c);

				if (c=='q'){
					return;
				}
				else if(c=='a') {
					i=0;
					break;
				}
				else{}
			}

		}
      }
}

void TempHistGLOBAL_COUNTRIES(list_t *extremes_countries,int  T,int auxth,char place_in_analysis[BUFFER_SIZE],int extremes_dates[4]){
      list_th_t *temp_hist_list=NULL;
      node_t *aux = extremes_countries->head;
      node_th_t *newNode=NULL, *delayed_aux=NULL, *th_list_iterator=NULL;
      int i=0, num_of_periods=0;

      //PrintCompleteNode(*aux, COUNTRIES);

      temp_hist_list = (list_th_t*)malloc(sizeof(list_th_t));
            if (temp_hist_list== NULL){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of temp_hist_list!");
                  exit(EXIT_FAILURE);
            }
      temp_hist_list->head=NULL;

      for(i=extremes_dates[0]; i<extremes_dates[1]; i=i+(T*10000)){
            num_of_periods++;
            newNode=NewTHListNode(i, T);

            if (temp_hist_list->head==NULL){
                  temp_hist_list->head=newNode;
                  delayed_aux=newNode;
            }
            else{
                  newNode->prev=delayed_aux;
                  delayed_aux->next=newNode;
                  delayed_aux=newNode;
            }
      }
      temp_hist_list->tail=newNode;

     // PrintCompleteNode(*aux, COUNTRIES);

      while(aux->next != NULL){
		th_list_iterator=temp_hist_list->head;
		if(auxth==PER_COUNTRY){
			if (strcmp(place_in_analysis, aux->payload.country)!=0){
				aux=aux->next;
				continue;
			}

		}

		//printf("%s___%s___\n", place_in_analysis, aux->payload.country);

            while(th_list_iterator != NULL){
			//test if the value of  countries list belongs to the selected period, if not triy to fit it in another period
                  if(aux->payload.ordering_identifier>=th_list_iterator->payload.begin_period
				&& aux->payload.ordering_identifier<th_list_iterator->payload.end_period){
				//checks for a new max_temperature
				if((aux->payload.temperature > th_list_iterator->payload.maximum_temp) ||(th_list_iterator->payload.maximum_temp==0)){
                              th_list_iterator->payload.maximum_temp=aux->payload.temperature;
                        }
                        //checks for a new min_temperature
                        if((aux->payload.temperature < th_list_iterator->payload.minimum_temp) ||(th_list_iterator->payload.minimum_temp==0)){
                              th_list_iterator->payload.minimum_temp=aux->payload.temperature;
				}

				th_list_iterator->payload.average =th_list_iterator->payload.average+ aux->payload.temperature;
				th_list_iterator->payload.num_of_val++;

                        //if it fits here doesn't fit anywhere else
                        break;
                  }
                  th_list_iterator=th_list_iterator->next;
            }
            aux=aux->next;
      }

      PrintTH(temp_hist_list, place_in_analysis);

      free(temp_hist_list);

}

void TempHistCITIES(list_t *extremes_cities,int  T,int auxth,char place_in_analysis[BUFFER_SIZE],int extremes_dates[4]){
      list_th_t *temp_hist_list=NULL;
      node_t *aux = extremes_cities->head;
      node_th_t *newNode=NULL, *delayed_aux=NULL, *th_list_iterator=NULL;
      int i=0, num_of_periods=0, concatenateddate=0;

      //PrintCompleteNode(*aux, COUNTRIES);

      temp_hist_list = (list_th_t*)malloc(sizeof(list_th_t));
            if (temp_hist_list== NULL){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of temp_hist_list!");
                  exit(EXIT_FAILURE);
            }
      temp_hist_list->head=NULL;

      for(i=extremes_dates[2]; i<extremes_dates[3]; i=i+(T*10000)){
            num_of_periods++;
            newNode=NewTHListNode(i, T);

            if (temp_hist_list->head==NULL){
                  temp_hist_list->head=newNode;
                  delayed_aux=newNode;
            }
            else{
                  newNode->prev=delayed_aux;
                  delayed_aux->next=newNode;
                  delayed_aux=newNode;
            }
      }
      temp_hist_list->tail=newNode;

     // PrintCompleteNode(*aux, COUNTRIES);

      while(aux->next != NULL){
		th_list_iterator=temp_hist_list->head;
            concatenateddate=aux->payload.dt.year*10000+aux->payload.dt.month*100+aux->payload.dt.day;

            if (strcmp(place_in_analysis, aux->payload.city)!=0){
                  aux=aux->next;
			continue;
		}

		//printf("%d_%s___%d_%s___\n",strlen(place_in_analysis), place_in_analysis, strlen(aux->payload.city),aux->payload.city);

            while(th_list_iterator != NULL){
			//test if the value of  countries list belongs to the selected period, if not triy to fit it in another period
                if(concatenateddate>=th_list_iterator->payload.begin_period
				&& concatenateddate<th_list_iterator->payload.end_period){
                    //printf("%d\n", concatenateddate);
				    //checks for a new max_temperature
				    if((aux->payload.temperature > th_list_iterator->payload.maximum_temp) ||(th_list_iterator->payload.maximum_temp==0)){
                              th_list_iterator->payload.maximum_temp=aux->payload.temperature;
                        }
                        //checks for a new min_temperature
                        if((aux->payload.temperature < th_list_iterator->payload.minimum_temp) ||(th_list_iterator->payload.minimum_temp==0)){
                              th_list_iterator->payload.minimum_temp=aux->payload.temperature;
				}

				th_list_iterator->payload.average =th_list_iterator->payload.average+ aux->payload.temperature;
				th_list_iterator->payload.num_of_val++;

                        //if it fits here doesn't fit anywhere else
                        break;
                  }
                  th_list_iterator=th_list_iterator->next;
            }
            aux=aux->next;
      }

      PrintTH(temp_hist_list, place_in_analysis);

      free(temp_hist_list);

}

node_th_t* NewTHListNode(int i,int T){
      node_th_t *newNode=NULL;

      newNode = (node_th_t*)malloc(sizeof(node_th_t));

      if (newNode == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of a TH node.");
            exit(EXIT_FAILURE);
      }

	newNode-> payload.average=0;
	newNode-> payload.num_of_val=0;
	newNode-> payload.maximum_temp = 0; // usefull to understand if any of the values was updated later
	newNode-> payload.minimum_temp = 0;
	newNode-> payload.begin_period= i;
      newNode-> payload.end_period=i+(T*10000);
      newNode-> next = NULL;
      newNode-> prev = NULL;

      return newNode;
}

void MovingAverage(){

}
