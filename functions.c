#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"
#include "lists.h"


/*int myscannoerrorsforintvecs (int _vector[MAX_STR], int strsize, int _min, int _max ){

     int i = 0;
     char buffer[BUFFER_SIZE] = {0};

     for (i = 0; i < strsize; i++){
          printf("Introduza a partir de que %s quer analisar os dados: ", request_df[i]);
          while(1){
                if(fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

                if((sscanf(buffer,"%d",&_vector[i])==1)&&(_min<_vector[i]&&vector[i]<_max)) break;
                else (printf("Introduz um número entre <gama>! : "));
           }
     }
}
*/


int ParamReading(int argc, char *argv[], char files[2][50]){
      int i=0, _mode=0;
      char *aux = NULL;
      if (argc!=6){
            fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters number of parameters.\n");
            exit (-1);
      }
      for(i=1; i<6; i++){

            if (strcmp(argv[i], "-g")==0){
                  if (_mode!=0){
                        fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (repeated definition -g).\n");
                        exit (-1);
                  }
                  _mode = GRAPHICAL;
                  continue;
            }

            else if (strcmp(argv[i], "-t")==0){
                  if (_mode!=0){
                        fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (repeated definition -t).\n");
                        exit (-1);
                  }
                  _mode = TEXTUAL;
                  continue;
            }

            else if(i==5) break;

            else if ( (strcmp(argv[i], "-f1")) == 0 ){
                  if (files[0][0]!=0){
                        fprintf(stderr,ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (repeated definition -f1).\n");
                        exit (-1);
                  }

                  if( (aux = strrchr(argv[i+1],'.')) != NULL ) {
                        if(strcmp(aux,".csv") == 0)   strcpy(files[0], argv[i+1]);
                        else  fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (f1 isn't csv).\n");
                  }
                  else fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (f1 is missing file extension).\n");

                  i++;
                  continue;
            }

            else if ( (strcmp(argv[i], "-f2")) == 0){
                  if (files[1][0]!=0){
                        fprintf(stderr,ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (repeated definition -f2).\n");
                        exit (-1);
                  }

                  if( (aux = strrchr(argv[i+1],'.')) != NULL ) {
                        if(strcmp(aux,".csv") == 0)   strcpy(files[1], argv[i+1]);
                        else  fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (f2 isn't csv).\n");
                  }
                        else fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (f2 is missing file extension).\n");

                  i++;
                  continue;
            }

            else{
                  printf ( ANSI_COLOR_WARNINGS "WARNING:" ANSI_COLOR_RESET "trouble with '%s'\n", argv[i]);
            }

      }

      if ( (_mode==0) || files[0][0]==0 || files[1][0]==0){
            if (_mode==0){
                  fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (missing mode).\n");
            }
            if (files[0][0]==0){
                  fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (missing f1).\n");
            }
            if (files[1][0]==0){
                  fprintf(stderr, ANSI_COLOR_ERRORS "ERROR:" ANSI_COLOR_RESET "Check parameters (missing f2).\n");
            }
            exit (-1);
      }

      return _mode;
}

void MenuSurfer(int* T, int* ano, int* aux_df, int* aux_ms, int* _auxth,int* aux_ma, int* months_MA, char place_in_analysis[BUFFER_SIZE]){

      switch (MainMenu()) {
                        case 0:
                            break;
                        case 1:
                            *aux_ms = 0;
                            *aux_df = DataFilter();
                            break;
                        case 2:
                            *aux_ms = 1;
                            TemperatureHistory(T,_auxth, place_in_analysis);
                            break;
                        case 3:
                            *aux_ms = 1;
                            YearTempAnalise(ano);
                            break;
                        case 4:
                            *aux_ms = 1;
                            GlobalTempAnalise(months_MA, aux_ma, place_in_analysis);
                            break;
                        default:
                            break;
      }

}

int MainMenu(){
      int _aux=0;
      char buffer[BUFFER_SIZE];

      printf(ANSI_COLOR_BOLD_MAGENTA "\n\nMENU PRINCIPAL: \n\n" ANSI_COLOR_RESET);
      printf(ANSI_COLOR_BOLD_WHITE "1 - Seleção de dados \n" ANSI_COLOR_RESET);
      printf(ANSI_COLOR_BOLD_BLUE "2 - Histórico de Temperaturas \n" ANSI_COLOR_RESET);
      printf(ANSI_COLOR_BOLD_GREEN "3 - Análise de temperatura por ano \n" ANSI_COLOR_RESET);
      printf(ANSI_COLOR_BOLD_CYAN "4 - Análise de temperatura\n" ANSI_COLOR_RESET);
      printf("\n Escolha a opção: ");
      while(1){
            if(fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

            if((sscanf(buffer,"%d",&_aux)==1)&&(0<_aux&&_aux<5)) break;
            else (printf("Introduz um número associado a uma opção! : "));
      }

      return _aux;
}

int DataFilter(){

     int _auxdf = 0;

     printf(ANSI_COLOR_BOLD_MAGENTA "\n\nMENU FILTRAGEM DE DADOS: \n\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_WHITE "1 - Limpar critérios atuais? \n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_BLUE "2 - Escolher intervalo de tempo para análise? \n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_GREEN "3 - Escolher mês para análise? \n" ANSI_COLOR_RESET);

     printf("\n Escolha a opção: ");
     myscanint (&_auxdf, 0, 4);

     return _auxdf;

}

void TemperatureHistory(int* T, int* _auxth, char place_in_analysis[BUFFER_SIZE]){


     printf("Qual é o período, em anos, que quer definir para cada análise:  ");
     myscanint (T, 0, 500);

     printf(ANSI_COLOR_BOLD_MAGENTA "\n\nMENU HISTÓRICO DE TEMPERATURAS: \n\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_WHITE "1 - Análise global?\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_GREEN "2 - Análise por país? \n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_CYAN "3 - Análise por cidade? \n" ANSI_COLOR_RESET);

     printf("\n Escolha a opção: ");
     myscanint (_auxth, 0, 4);

     switch (*_auxth) {
           case 1:
            break;
           case 2:
            printf("Introduza o nome do país a analisar: ");
            myscanstring(place_in_analysis);
            break;
           case 3:
            printf("Introduza a cidade a analisar: " );
            myscanstring(place_in_analysis);
            break;
           default:
            break;
     }


}

int YearTempAnalise(int* ano){

     int _auxyta=0;

     printf("Qual o ano que pretende analisar? ");
     myscanint ( ano, 1700, 2018);


     printf(ANSI_COLOR_BOLD_MAGENTA "\n\nMENU ANÁLISE DA TEMPERATURA POR ANO: \n\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_BLUE "1 - Análise por país? \n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_BLUE "2 - Análise por cidade? \n" ANSI_COLOR_RESET);

     printf("\n Escolha a opção: ");
     myscanint ( &_auxyta, 0, 3);

     return _auxyta;
}

void GlobalTempAnalise(int* months_MA, int* _aux_ma, char place_in_analysis[BUFFER_SIZE]){

     printf("Quantos meses pretende utilizar no cálculo do MA? ");
     myscanint (months_MA, 0, 99);

     printf(ANSI_COLOR_BOLD_MAGENTA "\n\nMENU ANALISE GLOBAL DE TEMPERATURA:\n\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_WHITE "1 - Aumento da temperatura global do planeta?\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_GREEN "2 - Aumento da temperatura global de um pais?\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_CYAN "3 - Aumento da temperatura global de uma cidade?\n" ANSI_COLOR_RESET);

     printf("\n Escolha a opção: ");
     myscanint (_aux_ma, 0, 4);

     switch (*_aux_ma) {
           case 1:
            break;
           case 2:
            printf("Introduza o nome do país a analisar: ");
            myscanstring(place_in_analysis);
            break;
           case 3:
            printf("Introduza a cidade a analisar: " );
            myscanstring(place_in_analysis);
            break;
           default:
            break;
     }




}

void myscanint ( int* _aux, int _min, int _max){
      char buffer[BUFFER_SIZE] = {0};

      while(1){
            if(fgets(buffer, BUFFER_SIZE, stdin)==NULL)
                  exit(-1);

            if((sscanf(buffer,"%d",_aux)==1)&&((_min<*_aux)&&(*_aux<_max)))
                  break;
            else
                  (printf("Introduz um número entre %d e %d! : ", _min,_max));
     }
}

void myscanstring(char place_in_analysis[BUFFER_SIZE]){

      while(1){
            if(fgets(place_in_analysis, BUFFER_SIZE, stdin)==NULL)
                  exit(-1);
            else{
                  place_in_analysis[strlen(place_in_analysis)-1]=0;
                  break;
            }
     }
}
