#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "data_treatment.h"



void TreatmentDataFilter(int* aux_df, int starting_yearmonth[2], int monthspan[2]){ //função que trata a filtragem de dados
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
          printf("Os dados a analisar estão agora a ser reduzidos.\n");
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

      *aux_df=0;

}


void PrintTH(node_th_t *extreme){
      node_th_t *aux = extreme;
      int i=0;
      //prev-tail-decreasing / next-head-increasing
      while(aux->next!= NULL){
            printf("%d--%d--tempMAX__%f\n",aux->payload.begin_period, aux->payload.end_period, aux->payload.maximum_temp );
            aux=aux->next;
            free(aux->prev);
            i++;
      }
      printf("TOTAL_____%d\n", i );
}




void TreatmentTemperatureHistory(list_t *extremes_countries,int  T,int auxth,char place_in_analysis[BUFFER_SIZE],int extremes_dates[4]){
      list_th_t *temp_hist_list=NULL;
      node_t *aux = extremes_countries->head;
      node_th_t *newNode=NULL, *delayed_aux=NULL, *th_list_iterator=NULL;
      int i=0, num_of_periods=0;

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
      PrintTH(temp_hist_list->head);



      //while(aux->next != NULL){
      //th_list_iterator=temp_hist_list->head;
      //      while(th_list_iterator != NULL){
      //            if(aux->payload.ordering_identifier>=th_list_iterator->payload.begin_period && aux->payload.ordering_identifier<=th_list_iterator->payload.end_period){
      //                  if(aux->payload.temperature>th_list_iterator->payload.maximum_temp){
      //                        th_list_iterator->payload.maximum_temp=aux->payload.temperature;
      //                        printf("%f\n", aux->payload.temperature );
      //                        break;
      //                  }
      //            }
      //            th_list_iterator=th_list_iterator->next;
      //      }
      //      aux=aux->next;
      //}
      //PrintTH(temp_hist_list->head);



      // while(aux!= NULL){
      //
      //
      //
      //
      //
      //
      //
      //
      //
      //
      //
      //
      //
      //       aux=aux->next;
      // }
      //
      //
      //free(aux);
//      free(delayed_aux);
     free(temp_hist_list);

}

// node_t* NewListNode(temp_hist_t _aux){
      // node_t *newNode=NULL;
//
      // newNode = (node_t*)malloc(sizeof(node_t));
//
      // if (newNode == NULL){
            // printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation");
            // exit(EXIT_FAILURE);
      // }
//
      // newNode-> payload =_aux;
      // newNode-> next = NULL;
      // newNode-> prev = NULL;
//
      // return newNode;
// }

node_th_t* NewTHListNode(int i,int T){
      node_th_t *newNode=NULL;

      newNode = (node_th_t*)malloc(sizeof(node_th_t));

      if (newNode == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of a TH node.");
            exit(EXIT_FAILURE);
      }

      newNode-> payload.begin_period= i;
      newNode-> payload.end_period=i+(T*10000);
      newNode-> next = NULL;
      newNode-> prev = NULL;

      return newNode;
}
/*
typedef struct temp_hist{
	int begin_period;
	int end_period;
	int average;
	int num_of_val;
	int maximum;
	int minimum;
} temp_hist_t;
typedef struct node_th{
	temp_hist_t payload;
	struct node *next;
	struct node *prev;
}node_th_t;
*/
