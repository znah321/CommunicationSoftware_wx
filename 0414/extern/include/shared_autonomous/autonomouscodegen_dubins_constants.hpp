/* Copyright 2018 The MathWorks, Inc. */

/**
 * @file
 * Constants used for Dubins motion primitive calculations.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_DUBINS_CONSTANTS_H_
#define AUTONOMOUSCODEGEN_DUBINS_CONSTANTS_H_

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_util.hpp"
#else
#include "autonomouscodegen_util.hpp"
#endif

namespace autonomous {
namespace dubins {

static const uint32_T TotalNumPaths = 6; ///< Total number of possible Dubins paths

} // namespace dubins
} // namespace autonomous

#endif // AUTONOMOUSCODEGEN_DUBINS_CONSTANTS_H_
