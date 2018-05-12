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
