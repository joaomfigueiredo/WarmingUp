#ifndef DATATRMT_H
#define DATATRMT_H

#include "data.h"

void TreatmentDataFilter(int*, int[], int[]);
void TreatmentTemperatureHistory(list_t *,int ,int ,char [BUFFER_SIZE], int [4]);
node_th_t* NewTHListNode(int, int);


#endif
