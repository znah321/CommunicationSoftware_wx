/*=================================================================
 * mexgetproperty.cpp 
 *
 * This example demonstrates how to use setProperty and getProperty. The input
 * to this function is a graphics handle. mexgetproperty.cpp gets the
 * Color property of the handle that was passed into the function. It
 * then changes the colors, and uses setProperty to set the Color property
 * of the handle to the new color values. Since the object passed is handle,
 * example demonstrates that handles are passed by reference and changed
 * value is reflected in MATLAB.
 *
 * Build : from MATLAB
 *         >> mex mexgetproperty.cpp
 *
 * Usage : from MATLAB
 *         >> l = line([1:10],[1:10])
 *         >> mexgetproperty(l)
 *
 * This is a MEX-file for MATLAB.  
 * Copyright 2017 The MathWorks, Inc.
 * All rights reserved.
 *=================================================================*/

#include "mex.hpp"
#include "mexAdapter.hpp"

#define RED   0
#define GREEN 1
#define BLUE  2

class MexFunction : public matlab::mex::Function {
public:
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
      std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
       matlab::data::ArrayFactory factory;
        checkArguments(outputs, inputs);
        matlab::data::ObjectArray object(inputs[0]);
        
        /* Get the "Color" property associated with this handle. */
        matlab::data::Array color = matlabPtr->getProperty(object, u"Color");
        double red = color[RED];
        double green = color[GREEN];
        double blue = color[BLUE];
        
        /* Reset the color values. */
        red = (1+red)/2;
        green = green/2;
        blue = blue/2;
        
        matlab::data::Array colorArray = 
                factory.createArray<double>({ 1,3 }, { red, green, blue});
                
        /* Reset the "Color" property to use the new color. */    
        matlabPtr->setProperty(object, u"Color", colorArray);
    }

    void checkArguments(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
        std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
        matlab::data::ArrayFactory factory;
        
        /* Check for the correct number of inputs. */
        if (inputs.size() != 1) {
            matlabPtr->feval(u"error", 
                0, std::vector<matlab::data::Array>({ factory.createScalar("One input required.") }));
        }
        
        /* Check for the correct number of outputs. */
        if (outputs.size() > 1) {
            matlabPtr->feval(u"error", 
                0, std::vector<matlab::data::Array>({ factory.createScalar("Too many output arguments specified.") }));
       
          }

        std::vector<matlab::data::Array> args{inputs[0]};
        
        matlab::data::CharArray result = matlabPtr->
            feval(u"class", args);
        std::string className = result.toAscii();
        
        /* Check to make sure passed object is of matlab graphics handle. */
        if(className.compare(0,15,"matlab.graphics"))
        {
            matlabPtr->feval(u"error",
                0, 
                std::vector<matlab::data::Array>({ factory.createScalar("Input must be an MATLAB graphics object") }));
        }
    }
};