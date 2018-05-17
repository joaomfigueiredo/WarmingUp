#ifndef LISTS_H
#define LISTS_H

#include "data.h"

node_t* NewCountryNode(int);
void LoadTempCountries(char [50]);
tree_node_t* GenNewNode(data_temp_t);
data_temp_t* CountriesCsvToStruct(char *);
void PrintNode(data_temp_t);
void PrintCompleteNode(node_t);

void sortedInsert(list_t *, tree_node_t *);
void insertionSort(data_temp_t , list_t *);

void TreetoList(tree_node_t *, list_t *);
void insertListTail(tree_node_t*, list_t*);
void PrintList(node_t*);


#endif
