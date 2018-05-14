#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"
#include "lists.h"

int main(int argc, char *argv[]){

      int mode = 0;
      int T = 0;
      int ano = 0;
      int months = 0;
      char files[2][FILENAME_SIZE]={{0}};

      mode = ParamReading(argc, argv, files);
      LoadTempCountries(files[0]);
      printf("%s7end", files[1]);
      if (mode==TEXTUAL) MenuSurfer(&T, &ano, &months);



      printf("Temp %d\n\n", T);
      printf("ano %d\n\n", ano);
      printf("meses %d\n\n", months);

      return EXIT_SUCCESS;
}
