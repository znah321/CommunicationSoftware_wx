/*=================================================================
 * arrayFillSetPr.c - example used to illustrate how to fill an mxArray
 *
 * Create an empty mxArray. Create a dynamic data array and
 * copy your data into it. Use mxSetDoubles (or mxSetPr for separate complex
 * MEX files) to dynamically allocate memory
 * as you fill mxArray with the contents of "dynamicData".
 *
 * Input:   none
 * Output:  mxArray
 *
 * Copyright 2008-2018 The MathWorks, Inc.
 *
 *=================================================================*/
#include "mex.h"

/* The mxArray in this example is 2x2 */
#define ROWS 2
#define COLUMNS 2
#define ELEMENTS 4

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
#if MX_HAS_INTERLEAVED_COMPLEX
    mxComplexDouble* dynamicData; /* pointer to dynamic data */
    const mxComplexDouble data[] = {{2.1, 2.1}, {3.4, 3.4}, {2.3, 2.3}, {2.45, 2.45}};
#else
    mxDouble* dynamicRealData;                         /* pointer to dynamic data */
    mxDouble* dynamicImagData;                         /* pointer to dynamic data */
    const mxDouble realData[] = {2.1, 3.4, 2.3, 2.45}; /* existing data */
    const mxDouble imagData[] = {2.1, 3.4, 2.3, 2.45}; /* existing data */
#endif
    mwSize index;
    mwSize elem_size;

    /* Check for proper number of arguments. */
    if (nrhs != 0) {
        mexErrMsgIdAndTxt("MATLAB:arrayFillSetPr:rhs", "This function takes no input arguments.");
    }

    /* Create a 0-by-0 mxArray; you will allocate the memory dynamically */
    plhs[0] = mxCreateNumericMatrix(0, 0, mxDOUBLE_CLASS, mxCOMPLEX);
    mxSetM(plhs[0], ROWS);
    mxSetN(plhs[0], COLUMNS);
    elem_size = mxGetElementSize(plhs[0]);

#if MX_HAS_INTERLEAVED_COMPLEX
    /* Create a local array and load data */
    dynamicData = mxMalloc(ELEMENTS * elem_size);
    for (index = 0; index < ELEMENTS; index++) {
        dynamicData[index] = data[index];
    }
    /* Put the C array into the mxArray and define its dimensions */
    mxSetComplexDoubles(plhs[0], dynamicData);
#else
    /* Create a local array and load data */
    dynamicRealData = mxMalloc(ELEMENTS * elem_size);
    dynamicImagData = mxMalloc(ELEMENTS * elem_size);
    for (index = 0; index < ELEMENTS; index++) {
        dynamicRealData[index] = realData[index];
        dynamicImagData[index] = imagData[index];
    }
    mxSetPr(plhs[0], dynamicRealData);
    mxSetPi(plhs[0], dynamicImagData);
#endif
    /* Do not call mxFree(dynamicData) because plhs[0] points to dynamicData */

    return;
}
