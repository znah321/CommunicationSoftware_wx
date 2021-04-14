/* Copyright 2005-2019 The MathWorks, Inc. */

// catcherror_mwarray.cpp
// MATLAB Compiler SDK Example
//
// This example demonstrates how to use print and error handling functions
// to control the output from a MATLAB Compiler SDK-generated shared library.
// 
// This example relies on the MATLAB functions realacos and reveal, found in
// realacos.m and reveal.m, respectively. It calls realacos twice, once in a 
// way that should not fail, and once in a way that should.
//
// To build this example, see Makefile in this directory.
//

#include <stdio.h>

// The MCC command above will generate an include file, libcatcherror.h, which
// contains a complete definition of the public interface of the libracos
// shared library. Include this generated header file first.

#include "libcatcherror.h"

// Print and error handling functions. The print handler will be called for
// all non-error-related output, and the error handler will be called to 
// process error message strings.
// 
// The print handler wraps a banner around the printed output, like so:
//
// ******** Start MATLAB output *******
// *
// * <Output message>
// *
// ******** End MATLAB output *******
//
// The error handler stores the most recent error in a static buffer 
// for processing after the function returns, much like MATLAB's LASTERROR
// function.

static bool emitPrefix = false;

static int PrintHandler(const char *s)
{
    // Declare and initialize all variables 
    size_t len = 0;
    const char * prefix = "* ";
    size_t written = 0;

    if (s == nullptr)
    {
        return 0;
    }

    len = strlen(s);

    /* DISP adds two carriage returns. Suppress the last one. */
    if (len >= 2 && s[len-1] == '\n' && s[len-2] == '\n')
    {
        len = len-1;
    }
    
    if (emitPrefix)
    {
        fwrite(prefix, sizeof(char), strlen(prefix), stdout);
    }

    written = fwrite(s, sizeof(char), len, stdout);

    if (s[len-1] == '\n')
    {
        emitPrefix = true;
    }
    else
    {
        emitPrefix = false;
    }
    
    return static_cast<int>(written);
}

static void StartMatlabOutput()
{
    const char *startBanner = "******** Start MATLAB output *******\n";
    emitPrefix = false;
    PrintHandler(startBanner);
}

static void EndMatlabOutput()
{
    const char *endBanner = "********* End MATLAB output ********\n";
    emitPrefix = false;
    PrintHandler(endBanner);
}

static char LastError[2048];

static int ErrorHandler(const char *s)
{
    size_t len = strlen(s);
    LastError[0] = '\0';
    strcpy(LastError, s);
    return static_cast<int>(len+1);
}

// The primary function, called by main().

int run_main(int argc, const char ** argv)
{
    double in1val = 0.1625;
    double in2val = 17;

    // Declare input variables. Note that any attempt to manipulate
    // mwArrays before calling mclInitializeApplication will generate
    // a segmentation fault, since the function pointers in the 
    // MCLMCRRT library will not be initialized.
    mwArray in1(in1val);
    mwArray in2(in2val);

    // Declare a variable to hold the output
    mwArray out;

    // Initialize the shared library. Pass in
    // pointers to our print and error handling functions.
    if (!libcatcherrorInitializeWithHandlers(ErrorHandler, PrintHandler))
    {
        fprintf(stderr,"Could not initialize the library.\n");
        return -2;
    }
    else
    {
        // Call the library function. This call should succeed.
        try
        {
            realacos(1, out, in1);
            std::cout << "realacos(" << in1val << ") = " << std::endl;
            StartMatlabOutput();
            reveal(out);
            EndMatlabOutput();
        }
        catch(const mwException& e)
        {
            std::cout << "Disaster! An unexpected error occurred."
                      << std::endl;
            std::cout << "Error:" << std::endl << LastError << std::endl;
            std::cerr << e.what() << std::endl;
        }

        // Call the library function. This call should fail, because 
        // the input value is greater than 1.0.
        try
        {
            realacos(1, out, in2);
            // Display the return value of the library function.
            // We expect that this code will never execute.
            std::cout << "Error! realacos should have thrown an exception."
                      << std::endl;
            std::cout << "realacos(" << in2val << ") = " << std::endl;
            StartMatlabOutput();
            reveal(out);
            EndMatlabOutput();
        }
        catch(mwException &e)
        {
            const char *errStartBanner = "\n######## Start MATLAB error ########\n";
            const char *errEndBanner = "\n######### End MATLAB error #########\n";
            std::cerr << errStartBanner << LastError;
            std::cerr << "Exception: " << std::endl << e.what() << std::endl;
            std::cerr << errEndBanner << std::endl;
        }
        catch (...)
        {
          std::cerr << "Unexpected error thrown" << std::endl;
          return -3;
        }     

        // Call the library termination routine
        libcatcherrorTerminate();
    }

    // mclTerminateApplication shuts down the MATLAB Runtime.
    // You cannot restart it by calling mclInitializeApplication.
    // Call mclTerminateApplication once and only once in your application.
    mclTerminateApplication();
    return 0;
}

// The main routine. On the Mac, the main thread runs the system code, and
// user code must be processed by a secondary thread. On other platforms, 
// the main thread runs both the system code and the user code.
int main(int argc, const char ** argv)
{
    // Call the mclInitializeApplication routine once and only once. 
    // This initializes the MATLAB Runtime; if it fails, the application 
    // cannot run. Make sure that the application was initialized properly 
    // by checking the return value. This initialization must be done 
    // before calling any MATLAB APIs or MATLAB Compiler SDK generated 
    // shared library functions.
    if( !mclInitializeApplication(nullptr, 0) )
    {
        fprintf(stderr, "Could not initialize the application.\n");
        return -1;
    }
    return mclRunMain(static_cast<mclMainFcnType>(run_main), argc, argv);
}
