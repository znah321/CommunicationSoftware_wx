#include "mex.h"

/*
 * timestwo.c
 * Computational function that takes a scalar and doubles it.
 *
 * This is a MEX-file for MATLAB.
 * Copyright 1984-2018 The MathWorks, Inc.
 */

void timestwo(double y[], double x[]) {
    y[0] = 2.0 * x[0];
}

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
#if MX_HAS_INTERLEAVED_COMPLEX
    mxDouble *x, *y;
#else
    double *x, *y;
#endif
    size_t mrows, ncols;

    /* Check for proper number of arguments. */
    if (nrhs != 1) {
        mexErrMsgIdAndTxt("MATLAB:timestwo:invalidNumInputs", "One input required.");
    } else if (nlhs > 1) {
        mexErrMsgIdAndTxt("MATLAB:timestwo:maxlhs", "Too many output arguments.");
    }

    /* The input must be a noncomplex scalar double.*/
    mrows = mxGetM(prhs[0]);
    ncols = mxGetN(prhs[0]);
    if (!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0]) || !(mrows == 1 && ncols == 1)) {
        mexErrMsgIdAndTxt("MATLAB:timestwo:inputNotRealScalarDouble",
                          "Input must be a noncomplex scalar double.");
    }

    /* Create matrix for the return argument. */
    plhs[0] = mxCreateDoubleMatrix((mwSize)mrows, (mwSize)ncols, mxREAL);

    /* Assign pointers to each input and output. */
#if MX_HAS_INTERLEAVED_COMPLEX
    x = mxGetDoubles(prhs[0]);
    y = mxGetDoubles(plhs[0]);
#else
    x = mxGetPr(prhs[0]);
    y = mxGetPr(plhs[0]);
#endif
    /* Call the timestwo subroutine. */
    timestwo(y, x);
}
