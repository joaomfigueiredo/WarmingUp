#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "data_treatment.h"


void TreatmentDataFilter(int* criteria, int* aux_df, int startingpoint[2], int monthspan[2]){ //função que trata a filtragem de dados

     int i = 0;
     char buffer[BUFFER_SIZE] = {0};
     char *request_df[2] = {"ano", "mês"};


     if (*aux_df == 1){

          *criteria = 0;
          printf("Os critérios foram limpos. ");

          for (i = 0; i < 2; i++){
               startingpoint[i] = 0;
               monthspan[i] = 0;
          }

     }

     if (*aux_df == 2){

          *criteria = 1;
          for (i = 0; i < 2; i++){
               printf("Introduza a partir de que %s quer analisar os dados: ", request_df[i]);
               while(1){
                     if(fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

                     if((sscanf(buffer,"%d",&startingpoint[i])==1)&&(0<startingpoint[i]&&startingpoint[i]<3)) break;
                     else (printf("Introduz um número entre <gama>! : "));
                }
          }

          printf("Os critérios para esta opção foram alterados!\n");
     }


     if (*aux_df == 3){

          *criteria = 1;
          for (i = 0; i < 2; i++){
               printf("Introduza de que mês a que mês é pretendido analisar. ");
               while(1){
                     if(fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

                     if((sscanf(buffer,"%d",&monthspan[i])==1)&&(0<monthspan[i]&&monthspan[i]<3)) break;
                     else (printf("Introduz um número entre <gama>! : "));
                }
          }

          printf("Os critérios para esta opção foram alterados!\n");
     }
}
