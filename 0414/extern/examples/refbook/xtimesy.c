#include "mex.h"

/*
 * xtimesy.c
 * Multiplies a real input matrix y by a given real input scalar x.
 *
 * The calling syntax is:
 *
 *		[result] = xtimesy(x, y)
 *
 * This is a MEX-file for MATLAB.
 * Copyright 1984-2018 The MathWorks, Inc.
 */

void xtimesy(double x, double* y, double* z, size_t m, size_t n) {
    mwSize i, j, count = 0;

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            *(z + count) = x * *(y + count);
            count++;
        }
    }
}

/* the gateway function */
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
#if MX_HAS_INTERLEAVED_COMPLEX
    mxDouble *y, *z, x;
#else
    double *y, *z, x;
#endif
    size_t mrows, ncols;

    /*  check for proper number of arguments */
    /* NOTE: You do not need an else statement when using mexErrMsgIdAndTxt
       within an if statement, because it will never get to the else
       statement if mexErrMsgIdAndTxt is executed. (mexErrMsgIdAndTxt breaks you out of
       the MEX-file) */
    if (nrhs != 2)
        mexErrMsgIdAndTxt("MATLAB:xtimesy:invalidNumInputs", "Two inputs required.");
    if (nlhs != 1)
        mexErrMsgIdAndTxt("MATLAB:xtimesy:invalidNumOutputs", "One output required.");

    /* check to make sure the first input argument is a scalar */
    if (!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0]) || mxGetN(prhs[0]) * mxGetM(prhs[0]) != 1) {
        mexErrMsgIdAndTxt("MATLAB:xtimesy:fieldNotScalar",
                          "First input argument must be a real scalar value.");
    }

    /* check to make sure the second input argument is a real, double matrix */
    if (!mxIsDouble(prhs[1]) || mxIsComplex(prhs[1])) {
        mexErrMsgIdAndTxt("MATLAB:xtimesy:fieldNotRealMatrix",
                          "Second input argument must be a real, double matrix.");
    }

    /*  get the scalar input x */
    x = mxGetScalar(prhs[0]);

    /*  create a pointer to the input matrix y */
#if MX_HAS_INTERLEAVED_COMPLEX
    y = mxGetDoubles(prhs[1]);
#else
    y = mxGetPr(prhs[1]);
#endif

    /*  get the dimensions of the matrix input y */
    mrows = mxGetM(prhs[1]);
    ncols = mxGetN(prhs[1]);

    /*  set the output pointer to the output matrix */
    plhs[0] = mxCreateDoubleMatrix((mwSize)mrows, (mwSize)ncols, mxREAL);

    /*  create a C pointer to a copy of the output matrix */
#if MX_HAS_INTERLEAVED_COMPLEX
    z = mxGetDoubles(plhs[0]);
#else
    z = mxGetPr(plhs[0]);
#endif

    /*  call the C subroutine */
    xtimesy(x, y, z, mrows, ncols);
}
