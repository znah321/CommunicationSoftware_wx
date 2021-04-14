#include "mex.hpp"
#include "mexAdapter.hpp"
#include <thread>
#include <future>    

class MexFunction : public matlab::mex::Function {
private:
    std::future<void> voidStdFuture;
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
    bool isRunning = false;
public:

    void DisplayDateTime( ) {
        // Create factory for MATLAB Data API
        matlab::data::ArrayFactory factory;

        // Use asynchronous Engine API to display date-time string
        // Uppdated onece per second
        // If MATLAB text object handle is invalid (window closed by user) stop updates
        matlabPtr->evalAsync(u"dateTimeWindow;");
 
        while (isRunning) {
            matlabPtr->evalAsync(u"set(findall(0,'Tag','date_time'),'String',datestr(now));");
            std::vector<matlab::data::Array> args({
                factory.createScalar<double>(0),
                factory.createCharArray("Tag"),
                factory.createCharArray("date_time"),
            });
            matlab::engine::FutureResult<matlab::data::Array> fresult;
            fresult = matlabPtr->fevalAsync(u"findall", args);
            matlab::data::Array result = fresult.get();
            isRunning = !result.isEmpty();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {

        // Get a pointer to a MATLAB engine
        // Call functions asynchronously on separate thread
        if (inputs.size() == 0) {
            mexLock();
            if(!isRunning) {
                isRunning = true;
                voidStdFuture = std::async(std::launch::async, &MexFunction::DisplayDateTime, this);
            }
        }
        else {
            isRunning = false;
            matlabPtr->eval(u"close(findall(0,'Tag','mexDateTimeHandle'))");
            mexUnlock();
        }
    }
};