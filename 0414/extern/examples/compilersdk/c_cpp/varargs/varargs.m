function varargout = varargs(varargin)
    disp("Number of provided inputs: " + length(varargin))
    disp("Number of requested outputs: " + nargout)
    
    for k = 1:nargout
        varargout{k} = k;
    end
end