#ifndef DATATRMT_H
#define DATATRMT_H

#include "data.h"

void PrintTH(list_th_t *,char [BUFFER_SIZE]);
void TreatmentDataFilter(int*, int[], int[]);
void TempHistGLOBAL_COUNTRIES(list_t *,int ,int ,char [BUFFER_SIZE], int [4]);
void TempHistCITIES(list_t *,int ,int ,char [BUFFER_SIZE], int [4]);
node_th_t* NewTHListNode(int, int);


#endif
