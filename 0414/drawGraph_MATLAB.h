//
// MATLAB Compiler: 7.0.1 (R2019a)
// Date: Wed Apr 14 14:12:29 2021
// Arguments:
// "-B""macro_default""¨CW""cpplib:drawGraph_MATLAB""drawGraph.m""¨CT""link:lib"
//

#ifndef __drawGraph_MATLAB_h
#define __drawGraph_MATLAB_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_drawGraph_MATLAB_C_API 
#define LIB_drawGraph_MATLAB_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_drawGraph_MATLAB_C_API 
bool MW_CALL_CONV drawGraph_MATLABInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_drawGraph_MATLAB_C_API 
bool MW_CALL_CONV drawGraph_MATLABInitialize(void);

extern LIB_drawGraph_MATLAB_C_API 
void MW_CALL_CONV drawGraph_MATLABTerminate(void);

extern LIB_drawGraph_MATLAB_C_API 
void MW_CALL_CONV drawGraph_MATLABPrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_drawGraph_MATLAB_C_API 
bool MW_CALL_CONV mlxDrawGraph(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_drawGraph_MATLAB
#define PUBLIC_drawGraph_MATLAB_CPP_API __declspec(dllexport)
#else
#define PUBLIC_drawGraph_MATLAB_CPP_API __declspec(dllimport)
#endif

#define LIB_drawGraph_MATLAB_CPP_API PUBLIC_drawGraph_MATLAB_CPP_API

#else

#if !defined(LIB_drawGraph_MATLAB_CPP_API)
#if defined(LIB_drawGraph_MATLAB_C_API)
#define LIB_drawGraph_MATLAB_CPP_API LIB_drawGraph_MATLAB_C_API
#else
#define LIB_drawGraph_MATLAB_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_drawGraph_MATLAB_CPP_API void MW_CALL_CONV drawGraph(const mwArray& s, const mwArray& t, const mwArray& w);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
