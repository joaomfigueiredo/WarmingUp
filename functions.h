#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data.h"

void myscanint ( int*, int, int);
void myscanstring(char [BUFFER_SIZE]);
int ParamReading(int, char *[], char [2][50]);
void MenuSurfer(int*, int*, int*, int*, int*, int*, char [BUFFER_SIZE]);
int MainMenu();
int DataFilter();
void TemperatureHistory(int*, int*, char [BUFFER_SIZE]);
int YearTempAnalise(int*);
int GlobalTempAnalise(int*);

#endif
