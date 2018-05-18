#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data.h"

void myscanint ( int*, int, int);
int ParamReading(int, char *[], char [2][50]);
void MenuSurfer(int*, int*, int*, int*, int*);
int MainMenu();
int DataFilter();
int TemperatureHistory(int*);
int YearTempAnalise(int*);
int GlobalTempAnalise(int*);

#endif
