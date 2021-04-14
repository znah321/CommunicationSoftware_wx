/*=================================================================
 * mxcreateuninitnumericmatrix.c
 * Create an uninitialized mxArray, fill it with the contents
 * of "data", and return it in plhs[0].
 *
 * Input:   none
 * Output:  mxArray
 *
 * Copyright 2015-2018 The MathWorks, Inc.
 *
 *=================================================================*/
#include "mex.h"

/* The mxArray in this example is 2x2 */
#define ROWS 2
#define COLUMNS 2
#define ELEMENTS 4

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
    mwSize index;
#if MX_HAS_INTERLEAVED_COMPLEX
    mxDouble* pointer;                             /* pointer to real data in new array */
    const mxDouble data[] = {2.1, 3.4, 2.3, 2.45}; /* existing data */
#else
    double* pointer;                             /* pointer to real data in new array */
    const double data[] = {2.1, 3.4, 2.3, 2.45}; /* existing data */
#endif

    /* Check for proper number of arguments. */
    if (nrhs != 0) {
        mexErrMsgIdAndTxt("MATLAB:mxcreateuninitnumericmatrix:rhs",
                          "This function takes no input arguments.");
    }

    /* Create an m-by-n mxArray. */
    plhs[0] = mxCreateUninitNumericMatrix(ROWS, COLUMNS, mxDOUBLE_CLASS, mxREAL);

    /* Copy existing data. */
#if MX_HAS_INTERLEAVED_COMPLEX
    pointer = mxGetDoubles(plhs[0]);
#else
    pointer = mxGetPr(plhs[0]);
#endif
    for (index = 0; index < ELEMENTS; index++) {
        pointer[index] = data[index];
    }
    return;
}
