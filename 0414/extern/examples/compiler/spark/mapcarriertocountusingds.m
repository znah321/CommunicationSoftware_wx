%
% Copyright 2016 MathWorks, Inc
%
function results = mapcarriertocountusingds(input)
    tbl = input{1};
    intermKeys = tbl.UniqueCarrier;
    [intermKeys, ~, idx] = unique(intermKeys);
    intermValues = num2cell(accumarray(idx, ones(size(idx))));
    results = cellfun( @(x,y) {x,y} , intermKeys, intermValues,'UniformOutput',false);
end