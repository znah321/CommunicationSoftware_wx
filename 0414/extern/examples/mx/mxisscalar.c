/*=================================================================
 * mxisscalar.c 
 * This example tests an input variable. 
 * If the input is a scalar, returns the scalar value. 
 * Otherwise, displays an error message.
 *
 * This is a MEX-file for MATLAB.  
 * Copyright 2015 The MathWorks, Inc.
 *=================================================================*/

#include "mex.h"

void mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[])
{
    double variable;
    
    /* Check for proper number of input and output arguments */
    if (nrhs != 1) {
        mexErrMsgIdAndTxt( "MATLAB:mxIsScalar:invalidNumInputs",
                "One input argument required.");
    }
    if(nlhs > 1){
        mexErrMsgIdAndTxt( "MATLAB:mxIsScalar:maxlhs",
                "Too many output arguments.");
    }
    
    /* Check to be sure input argument is a scalar */
    if (!(mxIsScalar(prhs[0]))){
        mexErrMsgIdAndTxt( "MATLAB:mxIsScalar:invalidInputType",
                "Input must be a scalar.");
    }
    
    /* Get input variable */
    variable = mxGetScalar(prhs[0]);

    /* Initialize a scalar double precision array */
    plhs[0] = mxCreateDoubleScalar(variable);
    
}
