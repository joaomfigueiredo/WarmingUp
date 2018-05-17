#ifndef LISTS_H
#define LISTS_H

#include "data.h"


void LoadTempCountries(char [50]);

data_temp_t* CountriesCsvToStruct(char *, data_temp_t *);
tree_node_t* NewTreeNode(data_temp_t);
node_t* NewListNode(data_temp_t);

void PrintNode(data_temp_t);
void PrintCompleteNode(node_t);
void TreeBuilder(list_t *, tree_node_t *);
void TreeLoader(data_temp_t , list_t *);
void TreetoList(tree_node_t *, list_t *);
void insertListTail_fromtree(tree_node_t*, list_t*);
void PrintList(node_t*);


#endif
