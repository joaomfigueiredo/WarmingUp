#ifndef DATATRMT_H
#define DATATRMT_H

#include "data.h"

void PrintTH(list_th_t *,char [BUFFER_SIZE]);
void TreatmentDataFilter(int*, int[], int[]);
void TempHistGLOBAL_COUNTRIES(list_t *,int ,int ,char [BUFFER_SIZE], int [4]);
void TempHistCITIES(list_t *,int ,int ,char [BUFFER_SIZE], int [4]);
node_th_t* NewTHListNode(int, int);

void MovingAverage(int , char [BUFFER_SIZE],int [4], list_t*, list_t*, int);
void Fill_MAarray(float *[13], int , int [4],int ,list_t* , list_t* , char [BUFFER_SIZE]);
void ComputeMA_permonth(float *[13], float *, int , int );
void Compute_DisplayWarming(float *[13], int [5], int [4], int); 

#endif
