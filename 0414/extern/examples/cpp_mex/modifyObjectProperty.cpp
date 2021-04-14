/* C++ MEX file for MATLAB
*  obj = EmployeeID('My Name',randi([1,255],1000,800,'uint8'));
*  returnObj = modifyObjectProperty(obj, 'New Name') modify object property.
*
*  Copyright 20017 MathWorks, Inc.
*/

#include "mex.hpp"
#include "mexAdapter.hpp"

using matlab::mex::ArgumentList;
using namespace matlab::data;

class MexFunction : public matlab::mex::Function {
public:
    void operator()(ArgumentList outputs, ArgumentList inputs) {
        checkArguments(outputs, inputs);
        Array object(inputs[0]);
        assignProperty(object, inputs);

        // Return modified object
        outputs[0] = object;
    }
     
    void assignProperty(Array& obj, ArgumentList in) {
        std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
        ArrayFactory factory;
        std::string newPropertyValue;

        // Determine if input is MATLAB char or string
        if (in[1].getType() == ArrayType::CHAR) {
            CharArray newName(in[1]);
            newPropertyValue = newName.toAscii();
        }
        else if (in[1].getType() == ArrayType::MATLAB_STRING) {
            StringArray newName(in[1]);
            newPropertyValue = (std::string)newName[0];
        }
        else {
            matlabPtr->feval(u"error", 0,
                std::vector<Array>({ factory.createScalar("Name must be a char or string") }));
        }

        // If new value is different from new value, set new value
        CharArray currentName = matlabPtr->getProperty(obj, u"Name");
        if (currentName.toAscii() != newPropertyValue) {
            matlabPtr->setProperty(obj, u"Name", factory.createCharArray(newPropertyValue));
        }
    }

    void checkArguments(ArgumentList out, ArgumentList in) {
        std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
        ArrayFactory factory;
        if (in.size() != 2) {
            matlabPtr->feval(u"error", 0, 
                std::vector<Array>({ factory.createScalar("Two inputs required") }));
        }

        std::vector<Array> args{ in[0], factory.createCharArray("EmployeeID") };
        TypedArray<bool> result = matlabPtr->feval(u"isa", args);
        if (result[0] != true) {
            matlabPtr->feval(u"error", 0, 
                std::vector<Array>({ factory.createScalar("Input must be an EmployeeID object") }));
        }     
    }
};