/* This is a C++ MEX file for MATLAB.
 * This function demonstrates how to use evalAsync API in MEX function.
 * This function will create 2 new thread one to generate data in MATLAB
 * and another to plot that data into MATLAB.
 * 
 * This function does not accept or return any arguments.
 * Usage:
 * >> mex plotAsync.cpp
 * >> plotAsync;
 * To unlock the mex file for rebuilding or deletion:
 * >> plotAsync(1);
 * Copyright 2018 The MathWorks, Inc.
 *
 */

#include "mex.hpp"
#include "mexAdapter.hpp"
#include <thread>
#include <future>   

// Function to plot "myData" into MATLAB, assuming "myData" is already present in MATLAB.
void plotData(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr) {
    matlabPtr->evalAsync(u"figure('Name','Rand(2,5)','NumberTitle','off')");
    for (int i = 0; i < 20; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        matlabPtr->evalAsync(u"plot(myData);");
    }
}

// Function to generate "myData" as random matrix of size 2x5.
void generateData(std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr) {
    for (int i = 0; i < 20; i++) {
        matlabPtr->evalAsync(u"myData = (rand(2,5));");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


class MexFunction : public matlab::mex::Function {
private:
    std::future<void> generateDataFuture;
    std::future<void> plotDataFuture;
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr;
public:
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {
        matlab::data::ArrayFactory factory;
        matlabPtr = getEngine();
        /* This code locks the mex file and avoid being deleted while an async task
         * is still running in the MEX file. Once the async task is completed the file
         * can be unlocked again.
         */
        if(inputs.empty()) {          
            mexLock();
            if (outputs.size() != 0) {
                matlabPtr->feval(u"error", 0, 
                    std::vector<matlab::data::Array>({ factory.createScalar("No output required.") }));
            }

            /* Run function to generate data asynchronously.
               This will not block matlab to finish the execution of current function and
               main thread will continue execution. */
            generateDataFuture = std::async(std::launch::async, &generateData, matlabPtr);

            /* Run function to plot data asynchronously.
               This will not block matlab to finish the execution of current function and
               main thread will continue execution. */
            plotDataFuture = std::async(std::launch::async, &plotData, matlabPtr);
        } else {
            mexUnlock();
            matlabPtr->feval(u"fprintf", 0, 
                std::vector<matlab::data::Array>({ factory.createScalar("Mex file 'PlotAsync' is Unlocked.\n") }));
        }
    }
};