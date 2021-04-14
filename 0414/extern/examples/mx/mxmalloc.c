/*=================================================================
 * mxmalloc.c
 * 
 * This function takes a MATLAB string as an argument and copies it in 
 * NULL terminated ANSI C string. Use this function only with 
 * MATLAB strings represented in single-byte encoding schemes.
 *
 * This is a MEX-file for MATLAB.  
 * Copyright 1984-2017 The MathWorks, Inc.
 *=================================================================*/

#include "mex.h"
   
void
mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[])
{
    char *buf;
    size_t buflen;
    int status;

    (void) plhs;    /* unused parameters */
    
    /* Check for proper number of input and output arguments */
    if (nrhs != 1) { 
	    mexErrMsgIdAndTxt( "MATLAB:mxmalloc:invalidNumInputs", 
                "One input argument required.");
    } 
    if (nlhs > 1) {
	    mexErrMsgIdAndTxt( "MATLAB:MXMALLOC:maxlhs",
                "Too many output arguments.");
    }
    
    /* Check for proper input type */
    if (!mxIsChar(prhs[0]) || (mxGetM(prhs[0]) != 1 ) )  {
	    mexErrMsgIdAndTxt( "MATLAB:mxmalloc:invalidInput", 
                "Input argument must be a string.");
    }
    
    /* Get number of characters in the input string.  Allocate enough
       memory to hold the converted string. */
    
    buflen = mxGetN(prhs[0]) + 1;
    buf = mxMalloc(buflen);
    
    /* Copy the string data into buf. */ 
    status = mxGetString(prhs[0], buf, (mwSize) buflen);

    if (status != 0) {
        mexErrMsgIdAndTxt( "MATLAB:mxmalloc:mxGetString", 
                           "Failed to copy input string into allocated memory.");
    }

    mexPrintf("The input string is:  %s\n", buf);
    /* NOTE: You could add your own code here to manipulate 
       the string */
    
    /* When finished using the string, deallocate it. */
    mxFree(buf);
    
}
