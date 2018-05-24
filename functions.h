#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "data.h"

//auxiliar functions to read inputs and avoid errors
void myscanint ( int*, int, int);
void myscanstring(char [BUFFER_SIZE]);
//input parameters processing
int ParamReading(int, char *[], char [2][50]);
//textual menu
void MenuSurfer(int*, int*, int*, int*, int*, int*, int*, int*, char [BUFFER_SIZE]);
int MainMenu();
//textual sub-menus
int DataFilter();
void TemperatureHistory(int*, int*, char [BUFFER_SIZE]);
void YearTempAnalise(int*, int*);
void GlobalTempAnalise(int*, int*, char [BUFFER_SIZE]);

#endif
