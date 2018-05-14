#ifndef LISTS_H
#define LISTS_H

#include "data.h"

node_t* NewCountryNode(int);
void LoadTempCountries(char [50]);
node_t* GetNewNode(char *);
node_t* CountriesCsvToStruct(char *, data_temp_t *, int *);
void PrintNode(data_temp_t);


#endif
