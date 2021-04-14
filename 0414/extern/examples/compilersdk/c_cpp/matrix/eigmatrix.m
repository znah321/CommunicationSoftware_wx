function e = eigmatrix(a1)
%EIGMATRIX Returns the eigenvalues of the given matrix
%    This function returns the eigenvalues of the input matrix. This
%    function is used to demonstrate the functionality of MATLAB Compiler SDK.
%    Refer to the MATLAB Compiler SDK C/C++ shared library documentation for more
%    information.

% Copyright 2003-2018 The MathWorks, Inc.

    try
        %Tries to calculate the eigenvalues and return them.
        e = eig(a1);
    catch
        %Returns a -1 on error.
        e = -1;
end