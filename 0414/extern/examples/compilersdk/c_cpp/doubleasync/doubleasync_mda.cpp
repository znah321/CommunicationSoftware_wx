/*==============================================================
 *
 * DOUBLEASYNC_MDA.CPP
 * Sample driver code that uses the MATLAB Data API interface 
 * (introduced in R2018a) and MATLAB Data API to call a C++ 
 * shared library created using the MATLAB Compiler SDK.
 * Demonstrates the usage of asynchronous calls.
 * Refer to the MATLAB Compiler SDK documentation for more 
 * information.
 *
 * Copyright 2017-2019 The MathWorks, Inc.
 *
 *============================================================*/
#include <MatlabCppSharedLib.hpp>
#include <chrono>
#include <functional>
#include <future>
#include <exception>
#include <iostream>
#include <string>

namespace mc = matlab::cpplib;
namespace md = matlab::data;

void usage(std::ostream & ostr, const int argc, const char * argv[]);
void parseIntArg(const int index, const int argc, const char * argv[], int & value);
int mainFunc(std::shared_ptr<mc::MATLABApplication> matlabApplication, const int argc,
    const char *argv[]);

int main(const int argc, const char * argv[])
{
    int ret = 0;
    try {
        std::vector<std::u16string> options = {mc::convertUTF8StringToUTF16String("-nojvm")};
        auto matlabApplication = mc::initMATLABApplication(
            mc::MATLABApplicationMode::IN_PROCESS,options);
        ret = mc::runMain(mainFunc, std::move(matlabApplication), argc, argv);
    } catch(const std::exception & exc) {
        std::cerr << exc.what() << std::endl;
        return -1;
    }
    return ret;
}
    
int mainFunc(std::shared_ptr<mc::MATLABApplication> matlabApplication, const int argc, 
    const char *argv[])
{
    try {
        // The path to the CTF (library archive file) passed to 
        // initMATLABLibrary or initMATLABLibraryAsync may be either absolute
        // or relative. If it is relative, the following will be prepended
        // to it, in turn, in order to find the CTF:
        // - the directory named by the environment variable 
        // CPPSHARED_BASE_CTF_PATH, if defined
        // - the working directory
        // - the directory where the executable is located
        // - on Mac, the directory three levels above the directory
        // where the executable is located
        
        // If the CTF is not in one of these locations, do one of the following:
        // - copy the CTF
        // - move the CTF
        // - change the working directory ("cd") to the location of the CTF
        // - set the environment variable to the location of the CTF
        // - edit the code to change the path
        auto future = mc::initMATLABLibraryAsync(matlabApplication, 
            mc::convertUTF8StringToUTF16String("libdoubleasync.ctf"));
        if (!future.valid()) {
            throw std::future_error(std::future_errc::no_state);
        }
        std::future_status status;
        do {
            status = future.wait_for(std::chrono::milliseconds(200));
            if (status == std::future_status::timeout) {
                std::cout << "Library initialization is in progress.\n";
            } else if (status == std::future_status::ready) {
                std::cout << "Library initialization has completed.\n";
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (status != std::future_status::ready); 
            
        auto lib = future.get();
        md::ArrayFactory factory;
        std::vector<md::Array> numbersToDouble;
        
        // Call the function repeatdouble, which iteratively continues to
        // double a number, printing out results along the way.  The 
        // (optional) second and third parameters determine, respectively, how 
        // many iterations should be performed and how many seconds should 
        // elapse between operations. We call the function asynchronously, 
        // then call it again (also asynchronously) with a different base 
        // number before all the iterations from the first call have completed.

        // We pass the arguments to the function as C++ native types (namely 
        // doubles) rather than a md::Array. The return type is also a native 
        // type (a double), so fevalAsync and the FutureResult need to take 
        // "double" as a template parameter.
        mc::FutureResult<double> futureA = lib->fevalAsync<double>(
            "repeatdouble", 10.0, 3.0, 0.5);
        mc::FutureResult<double> futureB = lib->fevalAsync<double>(
            "repeatdouble", 11.0, 3.0, 0.5);
            
        // Get the result once it's ready.
        double resultA = futureA.get();
        double resultB = futureB.get();
        
        std::cout << "resultA: " << resultA << std::endl;
        std::cout << "resultB: " << resultB << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cout << "error occurred: " << ex.what() << std::endl;
        return -1;
    }
    return 0;
}

