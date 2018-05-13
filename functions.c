#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "functions.h"
#include "data.h"

int ParamReading(int argc, char *argv[], char files[2][50]){
      int i=0, _mode=0;
      char *aux = NULL;
      if (argc!=6){
            fprintf(stderr, "ERROR: Check parameters number of parameters.\n");
            exit (-1);
      }
      for(i=1; i<6; i++){

            if (strcmp(argv[i], "-g")==0){
                  if (_mode!=0){
                        fprintf(stderr, "ERROR: Check parameters (repeated definition -g).\n");
                        exit (-1);
                  }
                  _mode = GRAPHICAL;
                  continue;
            }
            else if (strcmp(argv[i], "-t")==0){
                  if (_mode!=0){
                        fprintf(stderr, "ERROR: Check parameters (repeated definition -t).\n");
                        exit (-1);
                  }
                  _mode = TEXTUAL;
                  continue;
            }
            else if(i==5) break;
            else if ( (strcmp(argv[i], "-f1")) == 0 ){
                  if (files[0][0]!=0){
                        fprintf(stderr, "ERROR: Check parameters (repeated definition -f1).\n");
                        exit (-1);
                  }
                  if( (aux = strrchr(argv[i+1],'.')) != NULL ) {
                       if(strcmp(aux,".csv") == 0) {
                             strcpy(files[0], argv[i+1]);
                        }
                    }
                  else{
                        fprintf(stderr, "ERROR: Check parameters (f1 isn't csv).\n");
                        exit (-1);
                  }
                  i++;
                  continue;
            }
            else if ( (strcmp(argv[i], "-f2")) == 0){
                  if (files[1][0]!=0){
                        fprintf(stderr, "ERROR: Check parameters (repeated definition -f2).\n");
                        exit (-1);
                  }
                  if( (aux = strrchr(argv[i+1],'.')) != NULL ) {
                       if(strcmp(aux,".csv") == 0) {
                             strcpy(files[1], argv[i+1]);
                        }
                    }
                  else{
                        fprintf(stderr, "ERROR: Check parameters (f2 isn't csv).\n");
                        exit (-1);
                  }
                  i++;
                  continue;
            }

            else{
                  printf ("WARNING: trouble with '%s'\n", argv[i]);
            }

      }

      if ( (_mode==0) || files[0][0]==0 || files[1][0]==0){
            if (_mode==0){
                  fprintf(stderr, "ERROR: Check parameters (missing mode).\n");
            }
            if (files[0][0]==0){
                  fprintf(stderr, "ERROR: Check parameters (missing f1).\n");
            }
            if (files[1][0]==0){
                  fprintf(stderr, "ERROR: Check parameters (missing f2).\n");
            }
            exit (-1);
      }

      return _mode;
}


void MenuSurfer(int* T, int* ano, int* months){

      if ( MainMenu () == 1){

           DataFilter();
           return;
      }


      if ( MainMenu () == 2){

           TemperatureHistory(T);
           return;
      }

      if ( MainMenu () == 3){

           YearTempAnalise(ano);
           return;
      }

      if ( MainMenu() == 4){

           GlobalTempAnalise(months);
           return;
      }

}

int MainMenu(){
      int _aux=0;
      char buffer[BUFFER_SIZE];

      printf(ANSI_COLOR_BOLD_MAGENTA "\n\nMENU PRINCIPAL: \n\n" ANSI_COLOR_RESET);
      printf(ANSI_COLOR_BOLD_RED "1 - Seleção de dados \n" ANSI_COLOR_RESET);
      printf(ANSI_COLOR_BOLD_BLUE "2 - Histórico de Temperaturas \n" ANSI_COLOR_RESET);
      printf(ANSI_COLOR_BOLD_YELLOW "3 - Análise de temperatura por ano \n" ANSI_COLOR_RESET);
      printf(ANSI_COLOR_BOLD_GREEN "4 - Análise de temperatura\n" ANSI_COLOR_RESET);
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
     char buffer[BUFFER_SIZE];

     printf(ANSI_COLOR_BOLD_MAGENTA "\n\nMENU FILTRAGEM DE DADOS: \n\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_RED "1 - Limpar critérios atuais? \n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_BLUE "2 - Escolher intervalo de tempo para análise? \n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_YELLOW "3 - Escolher mês para análise? \n" ANSI_COLOR_RESET);

     printf("\n Escolha a opção: ");
     while(1){
           if(fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

           if((sscanf(buffer,"%d",&_auxdf)==1)&&(0<_auxdf&&_auxdf<4)) break;
           else (printf("Introduz um número associado a uma opção! : "));
     }

     return _auxdf;

}

int TemperatureHistory(int* T){
     int _auxth=0;
     char buffer[BUFFER_SIZE];
     char buffer2[BUFFER_SIZE];

     printf("Qual o valor de tempo (em anos) relativo ao período de amostragem que quer analisar? ");
     while(1){
           if(fgets(buffer2, BUFFER_SIZE, stdin)==NULL) exit(-1);

           if((sscanf(buffer2,"%d",T)==1)&&(1800 < *T && *T< 2018)) break;
           else (printf("Introduz um valor na gama permitida! : "));
     }

     printf(ANSI_COLOR_BOLD_MAGENTA "\n\nMENU HISTÓRICO DE TEMPERATURAS: \n\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_RED "1 - Análise global?\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_GREEN "2 - Análise por país? \n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_CYAN "3 - Análise por cidade? \n" ANSI_COLOR_RESET);

     printf("\n Escolha a opção: ");
     while(1){
           if(fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

           if((sscanf(buffer,"%d",&_auxth)==1)&&(0<_auxth&&_auxth<4)) break;
           else (printf("Introduz um número associado a uma opção! : "));
     }

     return _auxth;
}

int YearTempAnalise(int* ano){

     int _auxyta=0;
     char buffer[BUFFER_SIZE];
     char buffer2[BUFFER_SIZE];

     printf("Qual o ano que pretende analisar? ");
     while(1){
           if(fgets(buffer2, BUFFER_SIZE, stdin)==NULL) exit(-1);

           if((sscanf(buffer2,"%d",ano)==1)&&(1800 < *ano && *ano< 2018)) break; //valor tmb n mt certo digo eu
           else (printf("Introduz um valor na gama permitida! : "));
     }


     printf(ANSI_COLOR_BOLD_MAGENTA "\n\nMENU ANÁLISE DA TEMPERATURA POR ANO: \n\n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_YELLOW "1 - Análise por país? \n" ANSI_COLOR_RESET);
     printf(ANSI_COLOR_BOLD_BLUE "2 - Análise por cidade? \n" ANSI_COLOR_RESET);

     printf("\n Escolha a opção: ");
     while(1){
           if(fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

           if((sscanf(buffer,"%d",&_auxyta)==1)&&(0 <_auxyta&&_auxyta< 3)) break;
           else (printf("Introduz um número associado a uma opção! : "));
     }

     return _auxyta;
}

int GlobalTempAnalise(int* months){

     char buffer[BUFFER_SIZE];  //ESTA FUNÇÃO PODERÁ SER VOID DEPENDENDO DO RUMO DO PROGRAMA MAS VEREMOS DEPOIS

     printf("Quantos meses pretende utilizar no cálculo do MA? ");
     while(1){
           if(fgets(buffer, BUFFER_SIZE, stdin)==NULL) exit(-1);

           if((sscanf(buffer,"%d",months)==1)&&(0 < *months && *months < 1000)) break; //valor random deve ter que se alocar dinamicly?
           else (printf("Introduz um valor na gama permitida! : "));
     }

     return EXIT_SUCCESS;

}
