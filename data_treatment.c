#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "data_treatment.h"
#include "lists.h"

/**
 * Scans inputs to filter list of countries/cities
 */
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

/**
 * Prints temeprature history data (and calculates average while printing)
 */
void PrintTH(list_th_t *extreme,char place_in_analysis[BUFFER_SIZE]){
      node_th_t *aux = extreme->head;
      int i=0;
      char c[10];

      if(extreme->head->payload.num_of_val==0 && extreme->tail->payload.num_of_val==0){
		      printf("Não foram encontrados dados sobre %s que permitissem a análise requerida", place_in_analysis);
		      return;
	}

	printf("_____________________%s_____________________\n\nInicio periodo, Fim periodo, temp max, temp min, media\n",place_in_analysis );

      while(aux!= NULL){

            printf("[%d,%d[",aux->payload.begin_period/10000, aux->payload.end_period/10000);
            if (aux->payload.average==0){
                  printf("      NO DATA AVAILABLE\n");
            }
            printf(", %.3f, %.3f, %.3f\n", (aux->payload.maximum_temp),(aux->payload.minimum_temp), (aux->payload.average/aux->payload.num_of_val));

            aux=aux->next;
            i++;
            if (i==20){
			while(1){
				printf("Introduz 'a' para ver mais 20 ou 'q' para sair: ");

                        fgets(c,10,stdin);

				if (c[0]=='q'){
					return;
				}
				else if(c[0]=='a') {
					i=0;
					break;
				}
				else{}
			}

		}
      }
}

/**
   gather usefull nodes to display temperature history on countries
 */
void TempHistGLOBAL_COUNTRIES(list_t *extremes_countries,int  T,int auxth,char place_in_analysis[BUFFER_SIZE],int extremes_dates[4]){
      list_th_t *temp_hist_list=NULL;
      node_t *aux = extremes_countries->head;
      node_th_t *newNode=NULL, *delayed_aux=NULL, *th_list_iterator=NULL, *aux1=NULL, *tmp=NULL;
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

      aux1 = temp_hist_list->head;
      while(aux1!=NULL){
            tmp=aux1;
            aux1=aux1->next;
            free(tmp);
      }
      free(temp_hist_list);

}

/**
   gather usefull nodes to display temperature history on cities
 */
void TempHistCITIES(list_t *extremes_cities,int  T,int auxth,char place_in_analysis[BUFFER_SIZE],int extremes_dates[4]){
      list_th_t *temp_hist_list=NULL;
      node_t *aux = extremes_cities->head;
      node_th_t *newNode=NULL, *delayed_aux=NULL, *th_list_iterator=NULL, *aux1=NULL, *tmp=NULL;
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


      aux1 = temp_hist_list->head;
      while(aux1!=NULL){
            tmp=aux1;
            aux1=aux1->next;
            free(tmp);
      }
      free(temp_hist_list);

}

/**
 * Creates a node with a payload specific but common both to temperature history funtionality and year analysis
 */
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

/**
 * same as above but with different initializations
 */
node_th_t* NewTHListNode_YA(){
      node_th_t *newNode=NULL;

      newNode = (node_th_t*)malloc(sizeof(node_th_t));

      if (newNode == NULL){
            printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of a TH node.");
            exit(EXIT_FAILURE);
      }

	newNode-> payload.average=0;
	newNode-> payload.num_of_val=0;
	newNode-> payload.maximum_temp = 0;
	newNode-> payload.minimum_temp = 0;
      newNode-> next = NULL;
      newNode-> prev = NULL;

      return newNode;
}

/**
 * [THNodeCloner description]
 * @param  InNode [description]
 * @return        [description]
 */
node_th_t* THNodeCloner(node_th_t* InNode){
    node_th_t *newNode=NULL;

    newNode = (node_th_t*)malloc(sizeof(node_th_t));

    if (newNode == NULL){
          printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of a CLONE node.");
          exit(EXIT_FAILURE);
    }

    strcpy(newNode->payload.place,InNode->payload.place);
    newNode->payload.average=InNode->payload.average;
    newNode->payload.num_of_val=InNode->payload.num_of_val;
    newNode->payload.maximum_temp =InNode->payload.maximum_temp;
    newNode->payload.minimum_temp =InNode->payload.minimum_temp;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

void Fill_YA_Nodes(node_th_t* aux_newlist, node_t* aux){
    aux_newlist->payload.average+=aux->payload.temperature;
    aux_newlist->payload.num_of_val++;
    if (aux_newlist->payload.maximum_temp < aux->payload.temperature || aux_newlist->payload.maximum_temp==0){
            aux_newlist->payload.maximum_temp=aux->payload.temperature;
    }
    if (aux_newlist->payload.minimum_temp > aux->payload.temperature || aux_newlist->payload.minimum_temp==0){
            aux_newlist->payload.minimum_temp=aux->payload.temperature;
    }
}

/**
 * bubble sorted highest temp on top
 */
void YASort_hotter(list_th_t *target_list){
      node_th_t *aux1=NULL, *aux2=NULL;

      aux1=target_list->head;
      aux2=aux1->next;

      while(aux2!=NULL){
            if (aux1->payload.maximum_temp < aux2->payload.maximum_temp){
                  if(aux1==target_list->head){
                        target_list->head=aux2;
                        aux1->next=aux2->next;
                        aux2->next->prev=aux1;
                        aux2->next=aux1;
                        aux1->prev=aux2;
                        target_list->head->prev=NULL;
                  }
                  else{
                        aux1->prev->next=aux2;
                        aux2->prev=aux1->prev;
                        aux1->prev=aux2;
                        aux1->next=aux2->next;
                        if (aux2->next!=NULL) aux2->next->prev=aux1;
                        aux2->next=aux1;
                  }
                  aux1=target_list->head;
                  aux2=aux1->next;
            }
            else{
                  aux1=aux2;
                  aux2=aux2->next;
            }
      }
      target_list->tail=aux1;
}

/**
 * bubble sorted lowest temp on top
 */
void YASort_colder(list_th_t *target_list){
      node_th_t *aux1=NULL, *aux2=NULL;

      aux1=target_list->head;
      aux2=aux1->next;

      while(aux2!=NULL){
            if (aux1->payload.minimum_temp > aux2->payload.minimum_temp){
                  if(aux1==target_list->head){
                        target_list->head=aux2;
                        aux1->next=aux2->next;
                        aux2->next->prev=aux1;
                        aux2->next=aux1;
                        aux1->prev=aux2;
                        target_list->head->prev=NULL;
                  }
                  else{
                        aux1->prev->next=aux2;
                        aux2->prev=aux1->prev;
                        aux1->prev=aux2;
                        aux1->next=aux2->next;
                        if (aux2->next!=NULL) aux2->next->prev=aux1;
                        aux2->next=aux1;
                  }
                  aux1=target_list->head;
                  aux2=aux1->next;
            }
            else{
                  aux1=aux2;
                  aux2=aux2->next;
            }
      }
      target_list->tail=aux1;
}

/**
 * bubble sorted highest temp amplitude on top
 */
void YASort_amplitude(list_th_t *target_list){
      node_th_t *aux1=NULL, *aux2=NULL;

      aux1=target_list->head;
      aux2=aux1->next;

      while(aux2!=NULL){
            if ( (aux1->payload.maximum_temp - aux1->payload.minimum_temp) < (aux2->payload.maximum_temp - aux2->payload.minimum_temp)){
                  if(aux1==target_list->head){
                        target_list->head=aux2;
                        aux1->next=aux2->next;
                        aux2->next->prev=aux1;
                        aux2->next=aux1;
                        aux1->prev=aux2;
                        target_list->head->prev=NULL;
                  }
                  else{
                        aux1->prev->next=aux2;
                        aux2->prev=aux1->prev;
                        aux1->prev=aux2;
                        aux1->next=aux2->next;
                        if (aux2->next!=NULL) aux2->next->prev=aux1;
                        aux2->next=aux1;
                  }
                  aux1=target_list->head;
                  aux2=aux1->next;
            }
            else{
                  aux1=aux2;
                  aux2=aux2->next;
            }
      }
      target_list->tail=aux1;
}

/**
 * print Year Analysis in the require format
 */
void PrintYA(list_th_t *list_places){
      list_th_t *Hot=NULL, *Cold=NULL, *Amp=NULL; //lists with the same nodes of list_places but sorted
      node_th_t *newNode=NULL, *aux=NULL;
      node_th_t *H=NULL, *C=NULL, *A=NULL, *aux1=NULL, *aux2=NULL, *aux3=NULL;
      int i=0, j=0, k=0;

      Hot = (list_th_t*)malloc(sizeof(list_th_t));
            if (Hot == NULL){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of Hot!");
                  exit(EXIT_FAILURE);
            }
      Hot->head=NULL;

      Cold = (list_th_t*)malloc(sizeof(list_th_t));
            if (Cold == NULL){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of Cold!");
                  exit(EXIT_FAILURE);
            }
      Cold->head=NULL;

      Amp = (list_th_t*)malloc(sizeof(list_th_t));
            if (Amp == NULL){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of Amp!");
                  exit(EXIT_FAILURE);
            }
      Amp->head=NULL;

      aux=(list_places->head);
      //clones original list to them
      while(aux!=NULL){
            if (aux==(list_places->head)){
            Hot->head=THNodeCloner(aux);
            Cold->head=THNodeCloner(aux);
            Amp->head=THNodeCloner(aux);
            aux=aux->next;
            continue;
            }

            newNode=THNodeCloner(aux);
            Hot->head->prev=newNode;
            newNode->next = Hot->head;
	      Hot->head= newNode;

            newNode=THNodeCloner(aux);
            Cold->head->prev=newNode;
            newNode->next = Cold->head;
	      Cold->head= newNode;

            newNode=THNodeCloner(aux);
            Amp->head->prev=newNode;
            newNode->next = Amp->head;
	      Amp->head= newNode;

            aux=aux->next;
      }

      H=Hot->head;
      C=Cold->head;
      A=Amp->head;

      YASort_hotter(Hot);
      YASort_colder(Cold);
      YASort_amplitude(Amp);

      H=Hot->head;
      C=Cold->head;
      A=Amp->head;

      printf("Quantas linhas quer visualizar: ");
      myscanint(&k,0, 21);

      printf("\nMAIS QUENTE         MAIS FRIO           MAIS EXTREMO\n\n");
      while(H!=NULL && j<k){
            printf ("%s", H->payload.place);
            for(i=0; i<(20-strlen(H->payload.place)); i++){
                  printf(" ");
            }
            printf ("%s",C->payload.place);
            for(i=0; i<(20-strlen(C->payload.place)); i++){
                  printf(" ");
            }
            printf("%s\n", A->payload.place );


            H=H->next;
            C=C->next;
            A=A->next;
            j++;
      }

      H=Hot->head;
      C=Cold->head;
      A=Amp->head;

      while(H!=NULL && C!=NULL && A!=NULL){
            aux1=H;
            aux2=C;
            aux3=A;
            H=H->next;
            C=C->next;
            A=A->next;
            free(aux1);
            free(aux2);
            free(aux3);
      }

      free(Hot);
      free(Cold);
      free(Amp);

}

/**
 * Mainly this function calls those above.
 * Together they gather the info needed in a list
 *triplicates the list and order each one as required
 *for the previous printing
 */
void YearAnalysis(int auxyta, int year_in_analysis,int extremes_dates[4], list_t* extremes_cities, list_t* extremes_countries){
      //char warmest[20][BUFFER_SIZE], coolest[20][BUFFER_SIZE], greatest_amp[20][BUFFER_SIZE];
      list_th_t *list_places=NULL;
      node_t *aux = NULL;
      node_th_t *aux_newlist=NULL, *aux_to_link=NULL;

      auxyta++;//to correspond to define vars for per country and per city;

      list_places = (list_th_t*)malloc(sizeof(list_th_t));
            if (list_places == NULL){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of list_places!");
                  exit(EXIT_FAILURE);
            }
        list_places->head=NULL;
      //FILLS A LIST IN WICH EACH NODE COLLECTS ALL THE INFO OF A COUNTRU IN THE SPECIFIED YEAR
      if(auxyta==PER_COUNTRY){
	    aux=extremes_countries->head;
		while(aux!=NULL){
                  if(aux->payload.dt.year!=year_in_analysis){
                        aux=aux->next;
                        continue;
                  }
			if(list_places->head==NULL){
				list_places->head=NewTHListNode_YA();
				strcpy(list_places->head->payload.place, aux->payload.country);
                        aux_newlist=list_places->head;
                        Fill_YA_Nodes(aux_newlist,aux);
                        aux=aux->next;
      			continue;
			}

			aux_newlist=list_places->head;
			while(aux_newlist!=NULL){
				if (strcmp(aux_newlist->payload.place,aux->payload.country)==0){
					Fill_YA_Nodes(aux_newlist,aux);
                  	break;
				}
				aux_newlist=aux_newlist->next;
                        if (aux_newlist==NULL){
                              aux_to_link=list_places->head;

                              list_places->head=NewTHListNode_YA();
                              list_places->head->next=aux_to_link;
                              strcpy(list_places->head->payload.place, aux->payload.country);
                              aux_newlist=list_places->head;
                              Fill_YA_Nodes(aux_newlist,aux);
                        }
			}

			aux=aux->next;
		}
	}
      //FILLS A LIST IN WICH EACH NODE COLLECTS ALL THE INFO OF A CITY IN THE SPECIFIED YEAR
	else if(auxyta==PER_CITY){
        aux=extremes_cities->head;
		while(aux!=NULL){
                  if(aux->payload.dt.year!=year_in_analysis){
                        aux=aux->next;
                        continue;
                  }
			if(list_places->head==NULL){
				list_places->head=NewTHListNode_YA();
				strcpy(list_places->head->payload.place, aux->payload.city);
                        aux_newlist=list_places->head;
            	Fill_YA_Nodes(aux_newlist,aux);
      			continue;
			}

			aux_newlist=list_places->head;
			while(aux_newlist!=NULL){
				if (strcmp(aux_newlist->payload.place,aux->payload.city)==0){
					Fill_YA_Nodes(aux_newlist,aux);
                  	break;
				}
				aux_newlist=aux_newlist->next;
                        if (aux_newlist==NULL){
                              aux_to_link=list_places->head;
                              list_places->head=NewTHListNode_YA();
                              list_places->head->next=aux_to_link;
                              strcpy(list_places->head->payload.place, aux->payload.city);
                              aux_newlist=list_places->head;
                              Fill_YA_Nodes(aux_newlist,aux);
                        }
			}

			aux=aux->next;
		}
	}

      PrintYA(list_places);

      free(list_places);
}
/**
*calculates moving average with the number of months
*indicated by the user. To do so fills a table with
*months temperatures and dimension [13][number of years]
*then takes the months values to a unidmensional [12*number
*of years] array to ease moving average computing. As it is
*calculated, the output values replace the ones in the table.
*In the end of this iteration, calculates a anual arithmetic
*average. at this point, the "WARMINGup" allong the years is
*calculated and displayed
 */
void MovingAverage(int aux_ma, char place_in_analysis[BUFFER_SIZE],int extremes_dates[4], list_t* extremes_cities, list_t* extremes_countries, int months_MA){
      float *ma_array[13], *ma_unidim_array;
      int  num_years=0, i=0;
      int checkpoints_ma[5]={1860, 1910, 1960, 1990, 2013};

      if(aux_ma==PER_COUNTRY || aux_ma==GLOBAL){
            num_years=(extremes_dates[1]/10000)-(extremes_dates[0]/10000)+1;
      }
      else if (aux_ma==PER_CITY){
            num_years=(extremes_dates[3]/10000)-(extremes_dates[2]/10000)+1;
      }


      for (i=0; i<13; i++){
            ma_array[i] = calloc(num_years,sizeof(float));
            if (ma_array[i]== NULL){
                  printf(ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "in memory allocation of 'ma_array'!");
                  exit(EXIT_FAILURE);
            }
      }
      ma_unidim_array=calloc((num_years*12), sizeof(float));

      Fill_MAarray(ma_array, num_years, extremes_dates, aux_ma, extremes_cities, extremes_countries, place_in_analysis);
      ComputeMA_permonth(ma_array, ma_unidim_array, num_years, months_MA);
      Compute_DisplayWarming(ma_array, checkpoints_ma, extremes_dates, aux_ma);

      for (i=0; i<13; i++){
            free(ma_array[i]);
      }
      free(ma_unidim_array);

}
/**
 *fills the bidimentiona, above mentioned, array
 */
void Fill_MAarray(float *ma_array[13], int num_years, int extremes_dates[4], int aux_ma,
                  list_t* extremes_cities, list_t* extremes_countries,  char place_in_analysis[BUFFER_SIZE]){

      node_t* aux=NULL;
      int num_measures=0;
      if(aux_ma==GLOBAL || aux_ma==PER_COUNTRY){
            aux = extremes_countries->head;
      }
      else if(aux_ma==PER_CITY){
            aux =extremes_cities->head;
      }
      while(aux->next!=NULL){
            if (aux_ma!=GLOBAL){
                  if (aux_ma==PER_COUNTRY){
                        if (strcmp(place_in_analysis, aux->payload.country)!=0){
                              aux=aux->next;

                              continue;
                        }
                        else{

                              ma_array[aux->payload.dt.month][aux->payload.dt.year-(extremes_dates[0]/10000)]=aux->payload.temperature;
                              aux=aux->next;

                              continue;
                        }
                  }
                  else if (aux_ma==PER_CITY){
                        if (strcmp(place_in_analysis, aux->payload.city)!=0){
                              aux=aux->next;

                              continue;
                        }
                        else{
                              ma_array[aux->payload.dt.month][aux->payload.dt.year-(extremes_dates[2]/10000)]=aux->payload.temperature;
                              aux=aux->next;

                              continue;
                        }
                  }
            }

            ma_array[aux->payload.dt.month][aux->payload.dt.year-(extremes_dates[0]/10000)]+=aux->payload.temperature;

            num_measures++;

            if (aux->next->payload.ordering_identifier!=aux->payload.ordering_identifier){

                  ma_array[aux->payload.dt.month][aux->payload.dt.year-(extremes_dates[0]/10000)]=
                        ma_array[aux->payload.dt.month][aux->payload.dt.year-(extremes_dates[0]/10000)]/num_measures;

                  num_measures=0;
            }
            aux=aux->next;

      }


}
/**
 * Computes moving average iterating to a, also here filled, uni dim array
 */
void ComputeMA_permonth(float *ma_array[13], float *ma_unidim_array, int num_years, int months_MA){
      int i=0, j=0, k=0;
      int a1=0, a2=0;
      float sum=0;

      for(i=0;i<num_years;i++){
            for(j=1;j<13;j++){
                  ma_unidim_array[k]=ma_array[j][i];
                  k++;
            }
      }

      a1=months_MA-1;


      while (a1<(num_years*12)){
            if (a1==(months_MA-1)){
                  for(i=0; i<=a1;i++){
                        sum+=ma_unidim_array[i];
                  }
                  if (((a1+1)%12)==0)      ma_array[12][a1/12]=sum/months_MA;
                  else            ma_array[((a1+1)%12)][a1/12]=sum/months_MA;
                  a1++;
                  continue;
            }
            sum=sum-ma_unidim_array[a2]+ma_unidim_array[a1];
            if (((a1+1)%12)==0)      ma_array[12][a1/12]=sum/months_MA;
            else            ma_array[((a1+1)%12)][a1/12]=sum/months_MA;

            a2++;
            a1++;
      }

      for(i=0;i<num_years;i++){
            for(j=1;j<13;j++){
                  ma_array[0][i]+=ma_array[j][i];
            }
            ma_array[0][i]=ma_array[0][i]/12;
      }

}
/**
 * Performs the subtractions
 */
void Compute_DisplayWarming(float *ma_array[13], int checkpoints_ma[5], int extremes_dates[4], int aux_ma){
      int i=0, j=0, a=0;
      float maximum_temp=0, minimum_temp=50;

      if(aux_ma==GLOBAL || aux_ma==PER_COUNTRY){
            i=extremes_dates[0]/10000;
            a=extremes_dates[0]/10000;
      }
      else if (aux_ma==PER_CITY){
            i=extremes_dates[2]/10000;
            a=extremes_dates[2]/10000;
      }

      while(i<=2013){
            if (maximum_temp<ma_array[0][i-a]) maximum_temp=ma_array[0][i-a];
            if (minimum_temp>ma_array[0][i-a]) minimum_temp=ma_array[0][i-a];
            for(j=0;j<5;j++){
                  if (i==checkpoints_ma[j]){
                        if(maximum_temp-minimum_temp==0){
                            printf("      NO DATA AVAILABLE\n");
                            return;
                        }
                        printf("Até %d a temperatura subiu %.2fº graus!\n",i, (maximum_temp-minimum_temp));
                  }

            }
            i++;
      }

}
