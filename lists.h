#ifndef LISTS_H
#define LISTS_H

#include "data.h"

node_t* NewCountryNode(int);
void LoadTempCountries();
node_t* GetNewNode(char *);
void CountriesCsvToStruct(char *, data_temp *, int *);


#endif
