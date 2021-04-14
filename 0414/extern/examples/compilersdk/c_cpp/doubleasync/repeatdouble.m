function result = repeatdouble(originalValue, varargin)
% REPEATDOUBLE Repeatedly double an input value and return the final result.
%   RESULT = REPEATDOUBLE(ORIGINALVALUE) doubles ORIGINALVALUE 10 times, waiting 1 second between operations.
%   RESULT = REPEATDOUBLE(ORIGINALVALUE, NUMITERATIONS) specifies the number of iterations.
%   RESULT = REPEATDOUBLE(ORIGINALVALUE, NUMITERATIONS, PAUSELEN) pauses for PAUSELEN seconds between operations.

% Copyright 2017-2018 The MathWorks, Inc.

% default values    
numberOfIterations = 10;
pauseLength = 1;
% set values if passed in
lenv = length(varargin);
if (lenv > 0)
    numberOfIterations = varargin{1};
    if (lenv > 1)
        pauseLength = varargin{2};
    end
end
a = originalValue;
for i=1:numberOfIterations
    fprintf('Doubling the value of %d\n', a);
    a = 2*a;
    pause(pauseLength);
end
fprintf('Return value = %d\n', a);
result = a;
end
