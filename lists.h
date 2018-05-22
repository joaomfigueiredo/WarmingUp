#ifndef LISTS_H
#define LISTS_H

#include "data.h"


void LoadTempCountries(char [50], list_t*, int[4], int);
void LoadTempCities(char [50],list_t*, int[4], int);

data_temp_t* CsvToStruct(char *, data_temp_t *, int, int);
tree_node_t* NewTreeNode(data_temp_t);
node_t* NewListNode(data_temp_t);

void PrintNode(data_temp_t);
void PrintCompleteNode(node_t, int);
void TreeBuilder(list_t *, tree_node_t *);
void TreeLoader(data_temp_t , list_t *);
void TreetoList(tree_node_t *, list_t *);
void insertListTail_fromtree(tree_node_t*, list_t*);
void PrintList(node_t*, int);
void ConditionalNodeDeleter(list_t *, int, int[2], int [2], int [4]);


int CountCities(node_t *);

void freeList(node_t*);

#endif
