#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "functions.h"

int main(int argc, char *argv[]){

      int mode=0;
      char files[2][50]={{0}};
      int T=0;

      mode = ParamReading(argc, argv, files);

      if (mode==TEXTUAL) MenuSurfer(&T);

      printf("Temp %d", T);

      return EXIT_SUCCESS;
}
