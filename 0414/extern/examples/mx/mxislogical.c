/*=================================================================
 * mxislogical.c 
 * This example demonstrates how to use mexGetVariablePtr and
 * mxIsLogical.  You pass in the name of a variable in the caller
 * workspace.  It then gets the pointer to that variable and returns
 * an logical array which indicates whether the named variable is a 
 * logical.
 *
 * This is a MEX-file for MATLAB.  
 * Copyright 1984-2011 The MathWorks, Inc.
 *=================================================================*/

#include "mex.h"

void
mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[])
{
    const mxArray  *array_ptr;
    char     *variable;
    mxLogical *pl;
    
    /* Check for proper number of input and output arguments */
    if (nrhs != 1) {
        mexErrMsgIdAndTxt( "MATLAB:mxislogical:invalidNumInputs",
                "One input argument required.");
    }
    if(nlhs > 1){
        mexErrMsgIdAndTxt( "MATLAB:mxislogical:maxlhs",
                "Too many output arguments.");
    }
    
    /* Check to be sure input argument is a string. */
    if (!(mxIsChar(prhs[0]))){
        mexErrMsgIdAndTxt( "MATLAB:mxislogical:invalidInputType",
                "Input must be a character vector.");
    }
    
    /* Get input variable */
    variable= mxArrayToString(prhs[0]);
    array_ptr = mexGetVariablePtr("caller", variable);
    if (array_ptr == NULL){
       mxFree(variable);
       mexErrMsgIdAndTxt( "MATLAB:mxislogical:getVariableFailed",
                "Could not get variable.\n");
    }
    
    plhs[0] = mxCreateLogicalMatrix(1,1);
    pl = mxGetLogicals(plhs[0]);
    
    pl[0] = mxIsLogical(array_ptr);
    
    mxFree(variable);
}
