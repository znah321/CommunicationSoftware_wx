/* Copyright 2019 The MathWorks, Inc. */

/* cppshrhelp.hpp
 * Shared library helper include file for cross platform portability
 * define EXPORT before including this file in source files that build the library
 * no defines are needed to use the library. */

#ifndef CPPSHRHELP
#define CPPSHRHELP

#ifdef _WIN32
#ifdef EXPORT
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT __attribute__((visibility ("default")))
#endif

#endif

