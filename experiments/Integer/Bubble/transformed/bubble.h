
#ifndef _BUBBLE_FUNC_H_
#define _BUBBLE_FUNC_H_

#define K 400
#include "ap_int.h"
extern bool guard_error_sort; 
void bubbleSort(ap_uint<4> num[]);
void swap(ap_uint<4> arr[], ap_uint<9> i, ap_uint<9> j);
#endif
