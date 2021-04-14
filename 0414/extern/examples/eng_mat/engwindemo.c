/*
 *	engwindemo.c
 *
 *	This program illustrates how to call MATLAB
 *	Engine functions from a C program on Windows.
 *
 * Copyright 1984-2017 The MathWorks, Inc.
 */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#include <matrix.h>
#define BUFSIZE 256

static double Areal[6] = { 1, 2, 3, 4, 5, 6 };

int PASCAL WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpszCmdLine,
                    int       nCmdShow)

{
	Engine *ep;
	mxArray *T = NULL, *a = NULL, *d = NULL;
	char buffer[BUFSIZE+1];
	double *Dreal, *Dimag;
	double time[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	/*
	 * Start MATLAB engine
	 */
	if (!(ep = engOpen(NULL))) {
		MessageBox ((HWND)NULL, (LPSTR)"Can't start MATLAB engine",
			(LPSTR) "Engwindemo.c", MB_OK);
		exit(-1);
	}

	/*
	 * PART I
	 *
	 * For the first half of this demonstration, we will send data
	 * to MATLAB, analyze the data, and plot the result.
	 */

	/*
	 * Create a variable from our data
	 */
	T = mxCreateDoubleMatrix(1, 10, mxREAL);
	/*
	 * Copy data from time to matrix T
	 */
	#if MX_HAS_INTERLEAVED_COMPLEX
		memcpy(mxGetDoubles(T), time, 10*sizeof(double));
	#else
		memcpy(mxGetPr(T), time, 10*sizeof(double));
	#endif
	/*
	 * Place the variable T into the MATLAB workspace
	 */
	engPutVariable(ep, "T", T);

	/*
	 * Evaluate a function of time, distance = (1/2)g.*t.^2
	 * (g is the acceleration due to gravity)
	 */
	engEvalString(ep, "D = .5.*(-9.8).*T.^2;");

	/*
	 * Plot the result
	 */
	engEvalString(ep, "plot(T,D);");
	engEvalString(ep, "title('Position vs. Time for a falling object');");
	engEvalString(ep, "xlabel('Time (seconds)');");
	engEvalString(ep, "ylabel('Position (meters)');");

	/*
	 * PART II
	 *
	 * Create another mxArray, put it into MATLAB,
	 * and calculate its eigen values.
	 * 
	 */

	a = mxCreateDoubleMatrix(3, 2, mxREAL);
	/*
	 * Copy data from Areal to matrix a
	 */
	#if MX_HAS_INTERLEAVED_COMPLEX
		memcpy(mxGetDoubles(a), Areal, 6*sizeof(double));
	#else
		memcpy(mxGetPr(a), Areal, 6*sizeof(double));
	#endif
	engPutVariable(ep, "A", a);

	/*
	 * Calculate the eigen value
	 */
	engEvalString(ep, "d = eig(A*A')");

	/*
	 * Use engOutputBuffer to capture MATLAB output. Ensure first that
	 * the buffer is always NULL terminated.
	 */
	buffer[BUFSIZE] = '\0';
	engOutputBuffer(ep, buffer, BUFSIZE);

	/*
	 * the evaluate string returns the result into the
	 * output buffer.
	 */
	engEvalString(ep, "whos");
	MessageBox ((HWND)NULL, (LPSTR)buffer, (LPSTR) "MATLAB - whos", MB_OK);

	/*
	 * Get the eigen value mxArray
	 */
	d = engGetVariable(ep, "d");
	engClose(ep);

	if (d == NULL) {
		MessageBox ((HWND)NULL, (LPSTR)"Get Array Failed", (LPSTR)"Engwindemo.c", MB_OK);
	}
	else {
		/*
		 * Using interleaved complex representation we can access complex number
		 * with a single pointer instead of using two separate pointers.
		 */
		#if MX_HAS_INTERLEAVED_COMPLEX
			if (mxIsComplex(d)) {
				sprintf(buffer,"Eigenval 2: %g+%gi",mxGetComplexDoubles(d)[1].real,mxGetComplexDoubles(d)[1].imag);
			}
			else {
				sprintf(buffer,"Eigenval 2: %g",mxGetDoubles(d)[1]);
			}
		#else
			Dreal = mxGetPr(d);
			Dimag = mxGetPi(d);
			if (Dimag) {
				sprintf(buffer,"Eigenval 2: %g+%gi",Dreal[1],Dimag[1]);
			}
			else {
				sprintf(buffer,"Eigenval 2: %g",Dreal[1]);
			}
		#endif
		MessageBox ((HWND)NULL, (LPSTR)buffer, (LPSTR)"Engwindemo.c", MB_OK);
		mxDestroyArray(d);
	} 

	/*
	 * We're done! Free memory, close MATLAB engine and exit.
	 */
	mxDestroyArray(T);
	mxDestroyArray(a);

	return(0);
}

/* LocalWords:  eigen eng Eigenval gi
 */
