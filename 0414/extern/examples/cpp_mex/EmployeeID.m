classdef EmployeeID
   properties
      Name char
      Picture (1000,800) uint8      
   end  
   methods
      function obj = EmployeeID(n, p)
         if nargin > 0
            obj.Name = n;
            obj.Picture = p;
         end
      end
   end
end