#ifndef REGRESS_H
#define REGRESS_H
#include "array.h"

Array1D *RegressMatrix2D(Array2D *x, Array2D *y);
Array2D *CIBeta(Array2D *x, Array2D *b, Array2D *y, double alpha);
#endif

