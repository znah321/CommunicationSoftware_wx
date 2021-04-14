#include "mex.h"

/*
 * mexeval.c : Example MEX-file code emulating the functionality of the
 *             MATLAB command EVAL
 *
 *              
 * Copyright 1984-2011 The MathWorks, Inc.
 */

void
mexFunction( int nlhs, mxArray *plhs[],
	     int nrhs, const mxArray *prhs[] )
{
  (void) nlhs;     /* unused parameters */
  (void) plhs;

  if(nrhs==0)
    mexErrMsgIdAndTxt( "MATLAB:mexeval:minrhs",
            "Function 'mexeval' not defined for variables of class 'double'.\n");

  else if(!mxIsChar(prhs[0])) {
    const char str[]="Function 'mexeval' not defined for variables of class";
    char errMsg[100];
    
    sprintf(errMsg,"%s '%s'\n",str,mxGetClassName(prhs[0]));
    mexErrMsgIdAndTxt( "MATLAB:mexeval:UndefinedFunction", errMsg);
  }
  
  else {
    /* causes MATLAB to execute the string as an expression 
     * or statement the biggest limitation is that it won't 
     * evaluate any left-hand arguments except 'ans'
     */
    char  *fcn;
    int    status;
    size_t buflen=mxGetN(prhs[0])+1;
    
    fcn=(char *)mxCalloc(buflen,sizeof(char));
    status=mxGetString(prhs[0],fcn,(mwSize)buflen);
    
    status=mexEvalString(fcn);
    
    if((nrhs==2)&&(status)) {
      /* provides the ability to catch errors.  It
       * executes string s1 and returns if the operation was
       * successful. If the operation generates an error,
       * string s2 is evaluated before returning.
       */
      char *cmd;
      
      buflen=mxGetN(prhs[1])+1;
      cmd=(char *)mxCalloc(buflen,sizeof(char));
      
      mxGetString(prhs[1],cmd,(mwSize)buflen);
      mexEvalString(cmd);
      
      mxFree(cmd);
    }
    
    mxFree(fcn);
  }
}
