/*=================================================================
 * findnz.c 
 * Example for illustrating how to handle N-dimensional arrays in a 
 * MEX-file.  NOTE: MATLAB uses 1 based indexing, C uses 0 based indexing.
 *
 * Takes a N-dimensional array of doubles and returns the indices for
 * the non-zero elements in the array. Findnz works differently than
 * the FIND command in MATLAB in that it returns all the indices in
 * one output variable, where the column element contains the index
 * for that dimension.
 *
 * This is a MEX-file for MATLAB.  
 * Copyright 1984-2017 The MathWorks, Inc.
 *============================================================*/


#include "mex.h"

#define IsNonZero(d) ((d)!=0.0)


void
mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    /* Declare variables */ 
    size_t elements;
    mwSize j,cmplx;
    mwSize number_of_dims;
    mwSize nnz=0, count=0; 
    double *pr, *pi, *pind;
    const mwSize *dim_array;         
    #if MX_HAS_INTERLEAVED_COMPLEX
        mxComplexDouble * pc;
    #endif
    
    /* Check for proper number of input and output arguments */    
    if (nrhs != 1) {
        mexErrMsgIdAndTxt( "MATLAB:findnz:invalidNumInputs",
                "One input argument required.");
    }
    if (nlhs > 1){
        mexErrMsgIdAndTxt( "MATLAB:findnz:maxlhs",
                "Too many output arguments.");
    }

    /* Check data type of input argument */
    if (!(mxIsDouble(prhs[0]))) {
        mexErrMsgIdAndTxt( "MATLAB:findnz:invalidInputType",
                "Input array must be of type double.");
    }
    
    /* Get the number of elements in the input argument */
    elements=mxGetNumberOfElements(prhs[0]);
    #if MX_HAS_INTERLEAVED_COMPLEX
        cmplx = mxIsComplex(prhs[0]);

        /* Count the number of non-zero elements to be able to allocate
         * the correct size for output variable */
        if(cmplx) {
            pc = mxGetComplexDoubles(prhs[0]);
            for(j=0;j<elements;j++){
                if(IsNonZero(pc[j].real) || IsNonZero(pc[j].imag)) {
                    nnz++;
                }
            }
        }
        else{
            pr=mxGetDoubles(prhs[0]);
            for(j=0;j<elements;j++){
                if(IsNonZero(pr[j])) {
                    nnz++;
                }
            }
        }
    #else
        /* Get the data */
        pr=(double *)mxGetPr(prhs[0]);
        pi=(double *)mxGetPi(prhs[0]);
        cmplx = ((pi==NULL) ? 0 : 1);

        /* Count the number of non-zero elements to be able to allocate
         * the correct size for output variable */
        for(j=0;j<elements;j++){
            if(IsNonZero(pr[j]) || (cmplx && IsNonZero(pi[j]))) {
                nnz++;
            }
        }
    #endif
	
    /* Get the number of dimensions in the input argument. Allocate the
       space for the return argument */
    number_of_dims=mxGetNumberOfDimensions(prhs[0]);
    plhs[0]=mxCreateDoubleMatrix(nnz,number_of_dims,mxREAL);
    #if MX_HAS_INTERLEAVED_COMPLEX
        pind=mxGetDoubles(plhs[0]);
    #else
        pind=mxGetPr(plhs[0]);
    #endif
	
    /* Get the number of dimensions in the input argument. */
    dim_array=mxGetDimensions(prhs[0]);

    /* Fill in the indices to return to MATLAB. This loops through the
     * elements and checks for non-zero values. If it finds a non-zero
     * value, it then calculates the corresponding MATLAB index and
     * assigns them into the output array.  The 1 is added to the
     * calculated index because MATLAB is 1 based and C is zero
     * based. */
    #if MX_HAS_INTERLEAVED_COMPLEX
        if(cmplx) {
            for(j=0;j<elements;j++) {
                if(IsNonZero(pc[j].real) || IsNonZero(pc[j].imag)) {
                    mwSize temp=j;
                    mwSize k;
                    for (k=0;k<number_of_dims;k++){
                        pind[nnz*k+count]=(double)((temp % (dim_array[k])) +1);
                        temp/=dim_array[k];
                    }
                    count++;
                }
            }
        }
        else {
            for(j=0;j<elements;j++) {
                if(IsNonZero(pr[j])) {
                    mwSize temp=j;
                    mwSize k;
                    for (k=0;k<number_of_dims;k++){
                        pind[nnz*k+count]=(double)((temp % (dim_array[k])) +1);
                        temp/=dim_array[k];
                    }
                    count++;
                }
            }
        }
    #else
        for(j=0;j<elements;j++) {
            if(IsNonZero(pr[j]) || (cmplx && IsNonZero(pi[j]))) {
                mwSize temp=j;
                mwSize k;
                for (k=0;k<number_of_dims;k++){
                    pind[nnz*k+count]=(double)((temp % (dim_array[k])) +1);
                    temp/=dim_array[k];
                }
                count++;
            }
        }
    #endif
}
