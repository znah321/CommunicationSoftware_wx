/*=================================================================
 * mxcreatecellmatrix.c
 *
 * mxcreatecellmatrix takes the input arguments and places them in a
 * cell array. This cell array is returned back to MATLAB as the result.
 *
 *
 * This is a MEX-file for MATLAB.
 * Copyright 1984-2018 The MathWorks, Inc.
 * All rights reserved.
 *=================================================================*/

#include "mex.h"

void
mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    mxArray *cell_array_ptr;
    mwIndex i;

    /* Check for proper number of input and output arguments */
    if (nrhs < 1) {
        mexErrMsgIdAndTxt( "MATLAB:mxcreatecellmatrix:minrhs",
                "At least one input argument required.");
    }

    if(nlhs > 1){
        mexErrMsgIdAndTxt( "MATLAB:mxcreatecellmatrix:maxlhs",
                "Too many output arguments.");
    }

    /* Create a nrhs x 1 cell mxArray. */
    cell_array_ptr = mxCreateCellMatrix((mwSize)nrhs,1);

    /* Fill cell matrix with input arguments */
    for( i=0; i<(mwIndex)nrhs; i++){
        mxSetCell(cell_array_ptr,i,mxDuplicateArray(prhs[i]));
    }

    plhs[0] = cell_array_ptr;
}