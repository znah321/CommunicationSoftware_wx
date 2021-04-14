function [m,t] = houdini();
% Doc example, chapter 3.

% Copyright 1997 The MathWorks, Inc.

m = magic(2); % Assign 2x2 matrix to m. 
t = m .^ 3;   % Cube each element of m. 
disp(t);      % Display the value of t.

