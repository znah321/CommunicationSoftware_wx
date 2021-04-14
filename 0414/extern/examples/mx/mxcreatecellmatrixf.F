C===================================================================
#include "fintrf.h"
C     mxcreatecellmatrixf.f
C
C     mxcreatecellmatrix takes the input arguments and places them in a
C     cell array. This cell array is returned back to MATLAB as the result.
C
C     This is a MEX-file for MATLAB.
C     Copyright 1984-2018 The MathWorks, Inc.
C     All rights reserved.
C
C===================================================================

      subroutine mexFunction(nlhs, plhs, nrhs, prhs)

C     Declarations
      implicit none

      mwPointer plhs(*), prhs(*)
      integer nlhs, nrhs
      mwPointer mxCreateCellMatrix, mxDuplicateArray
      mwPointer cell_array_ptr
      mwSize i, m, n
      mwPointer NULL

C     Check for proper number of input and output arguments   
      if (nrhs .lt. 1) then
         call mexErrMsgIdAndTxt( 'MATLAB:mxcreatecellmatrixf:minrhs',
     +    'One input argument required.')
      end if

      if (nlhs .gt. 1) then
         call mexErrMsgIdAndTxt( 'MATLAB:mxcreatecellmatrixf:maxlhs',
     +     'Too many output arguments.')
      end if

C     Create a nrhs x 1 cell mxArray.
      m = nrhs
      n = 1
      cell_array_ptr = mxCreateCellMatrix(m, n)

C     Fill cell matrix with input arguments
      do 10 i=1,m
        call mxSetCell(cell_array_ptr,i,
     +                     mxDuplicateArray(prhs(i)))
   10 continue

      plhs(1) = cell_array_ptr

      return
      end

