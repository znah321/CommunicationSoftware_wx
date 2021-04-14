/* Copyright 2018 The MathWorks, Inc. */

/**
 * @file
 * Utility functions for all autonomous functionality in codegen..
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_UTIL_HPP_
#define AUTONOMOUSCODEGEN_UTIL_HPP_

#if defined(BUILDING_LIBMWAUTONOMOUSCODEGEN)
/* For DLL_EXPORT_SYM and EXTERN_C */
#include "package.h"
/* For uint32_T, boolean_T, etc */
#include "tmwtypes.h"

#define AUTONOMOUS_CODEGEN_API DLL_EXPORT_SYM

#else

/* For uint32_T, boolean_T, etc */
#if defined(MATLAB_MEX_FILE) || defined(BUILDING_UNITTEST)
#include "tmwtypes.h"
#else
#include "rtwtypes.h"
#endif

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

#ifndef AUTONOMOUS_CODEGEN_API
#define AUTONOMOUS_CODEGEN_API
#endif

#endif /* else */

#endif /* AUTONOMOUSCODEGEN_UTIL_HPP_ */
