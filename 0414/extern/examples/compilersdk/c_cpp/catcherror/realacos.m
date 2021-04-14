function r = realacos(x)
% REALACOS Arc-cosine, output guaranteed to be non-complex
%
% This function generates an error if the input is complex or has absolute value  
% greater than 1.0, because only those values between -1.0 and 1.0 have non-complex
% arc cosine.

% Copyright 2013-2018 The MathWorks, Inc.

    if ( any(imag(x(:))) || any(abs(x) > 1.0) )
        error(['Input X must be real, between -1.0 and 1.0 inclusive. (Was: ' num2str(x) ')']);
    end

    r = acos(x);