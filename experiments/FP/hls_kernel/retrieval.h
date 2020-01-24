#ifndef _RETRIEVAL_H_
#define _RETRIEVAL_H_

#include <math.h>
//#include "inputdata.h"

#include "thls/tops/fp_flopoco.hpp"
#include "thls/tops/policy_flopoco.hpp"

const float eps = 0.5;

float l2norm(float query[3], float data[3], int dim);

float l1norm(float query[3], float data[3], int dim);

float l0norm(float query[3], float data[3], int dim);

#endif

