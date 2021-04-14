/*=================================================================
* mexgetarray.cpp
*
* This example demonstrates how to use getVariable,setVariable,
* and matlab::mex::Function::mexFunctionName. 
* This function takes no input arguments. It counts
* the number of times mexgetarray.cpp is called.  It has an internal
* counter in the MEX-file and a counter in the MATLAB global
* workspace.  Even if the MEX-file is cleared, it will not lose count
* of the number of times the MEX-file has been called.
*
* To use this function make sure mexgetarray.hpp and mexgetarray.cpp are in
* same directory.
*
* Build : from MATLAB
*         >> mex(['-I' pwd],'mexgetarray.cpp')
*
* Usage : from MATLAB
*         >> mexgetarray
*
* This is a MEX-file for MATLAB.
* Copyright 2017 The MathWorks, Inc.
* All rights reserved.
*=================================================================*/

#include "mex.hpp"
#include "mexAdapter.hpp"
#include "mexgetarray.hpp"

  int MexFunction::mex_count = 0;  
  void 
    MexFunction::operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
    
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
    matlab::data::ArrayFactory factory;

    /* Check for proper number of input and output arguments */
    if(inputs.size()!=0) {
      matlabPtr->feval(u"error", 0,
              std::vector<matlab::data::Array>({factory.createScalar("No input arguments required.")}));
    }
    
    if(outputs.size()>1 ) {
      matlabPtr->feval(u"error", 0,
              std::vector<matlab::data::Array>({factory.createScalar("Too many output arguments.")}));
    }
    
    /* Make copy of MEX-file name, then create variable for MATLAB
    workspace from MEX-file name. */
    std::string function_name = 
            matlab::engine::convertUTF16StringToUTF8String(getFunctionName());
    
    std::u16string variable_name = getFunctionName() + u"_called";
    
    /* Get variable that keeps count of how many times MEX-file has
     * been called from MATLAB global workspace. */
    matlab::data::Array matlabDataArray;
    try {
        matlabDataArray = matlabPtr->getVariable(variable_name, matlab::engine::WorkspaceType::GLOBAL);
    }

    /* If there was an issue while accessing the variable or 
     * variable does not exist then code will enter catch block.*/
    catch(...)
    {
        if (mex_count != 0) {
            mex_count = 0;
            matlabPtr->feval(u"fprintf", 0, 
                    std::vector<matlab::data::Array>(
                                         {factory.createScalar("Variable %s\n"),
                                          factory.createScalar(function_name)}));
                                         
            matlabPtr->feval(u"error", 0, 
                    std::vector<matlab::data::Array>
                    ({ factory.createScalar("Global variable was cleared from the MATLAB global workspace."
                                     "\nResetting count.\n") }));
        }

        /* Since variable does not yet exist in MATLAB workspace,
        * create it.*/
        matlabDataArray = factory.createScalar<int>(0);
    }

    /* Increment both MATLAB and MEX counters by 1 */
    matlab::data::TypedArray<int> matlabIntArray = matlabDataArray;
    matlabIntArray[0]=matlabIntArray[0]+1;
    mex_count = matlabIntArray[0];
    
    matlabPtr->feval(u"fprintf", 0,
             std::vector<matlab::data::Array>(
               {factory.createScalar("%s has been called %i time(s)\n"),
                factory.createScalar(function_name),
                factory.createScalar(mex_count)}));

    /* Put variable in MATLAB global workspace */
    try {
        matlabPtr->setVariable(variable_name, matlabIntArray, matlab::engine::WorkspaceType::GLOBAL);
    }
    catch (...)
    {
        matlabPtr->feval(u"fprintf", 0,
                std::vector<matlab::data::Array>(
                                         {factory.createScalar("Variable %s\n"),
                                          factory.createScalar(function_name)}));
        matlabPtr->feval(u"error", 0,
                std::vector<matlab::data::Array>
                ({ factory.createScalar("Could not put variable in global workspace.\n") }));
    }
  }