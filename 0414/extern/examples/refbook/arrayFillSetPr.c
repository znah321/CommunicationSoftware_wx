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

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
#if MX_HAS_INTERLEAVED_COMPLEX    
    mxDouble  *dynamicData;        /* pointer to dynamic data */
    const mxDouble data[] = {2.1, 3.4, 2.3, 2.45};  /* existing data */
#else
    double  *dynamicData;          /* pointer to dynamic data */
    const double data[] = {2.1, 3.4, 2.3, 2.45};  /* existing data */
#endif
    mwSize index;

	/* Check for proper number of arguments. */
    if ( nrhs != 0 ) {
        mexErrMsgIdAndTxt("MATLAB:arrayFillSetPr:rhs","This function takes no input arguments.");
    }

    /* Create a local array and load data */
    dynamicData = mxMalloc(ELEMENTS * sizeof(double));
    for ( index = 0; index < ELEMENTS; index++ ) {
        dynamicData[index] = data[index];
    }

    /* Create a 0-by-0 mxArray; you will allocate the memory dynamically */
    plhs[0] = mxCreateNumericMatrix(0, 0, mxDOUBLE_CLASS, mxREAL);

    /* Put the C array into the mxArray and define its dimensions */
#if MX_HAS_INTERLEAVED_COMPLEX
    mxSetDoubles(plhs[0], dynamicData);
#else
    mxSetPr(plhs[0], dynamicData);
#endif
    mxSetM(plhs[0], ROWS);
    mxSetN(plhs[0], COLUMNS);

    /* Do not call mxFree(dynamicData) because plhs[0] points to dynamicData */
  
    return;
}
