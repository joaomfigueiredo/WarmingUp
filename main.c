#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"



int main(int argc, char *argv[]){
      int mode=0;
      char files[2][50]={{0}};
      mode = ParamReading(argc, argv, files);
      printf("%d\n", mode);
      printf("-----%c------\n", files[1][0] );
      printf("1- %s\n2- %s",files[0], files[1] );
      return EXIT_SUCCESS;
}
